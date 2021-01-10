
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>
#include "mftp.h"

int DEBUG = 0;

#define MAX_REQUEST_QUEUE_SIZE (4)
#define MAX_BUF (256)

int read_command( int socketfd, char * cmd, char * arg, int max_arg_size) {
    unsigned char c;
    int arg_size = 0;
    int invalid_arg = 0;

    // Read one byte
    int bytes_read = read(socketfd, &c, 1);

    if( bytes_read < 0 ) {
        // Something bad happened
        perror("Error: ");
        exit(1);
    } else if( bytes_read == 0 ) {
        return 0;
    }
    *cmd = (char)c;

    while( 1 ) {

        // Read one byte
        int bytes_read = read(socketfd, &c, 1);

        if( bytes_read < 0 ) {
            // Something bad happened
            perror("Error: ");
            exit(1);
        } else if( bytes_read == 0 ) {
            // Nothing left to read, quit
            break;
        }

        // Linefeed terminates a command
        if( c == '\n' ) {
            break;
        }
        
        if( arg_size >= (max_arg_size) ) {
            if( !invalid_arg ) {
                fprintf(stderr, "Server argument exceeded max size of %d\n", max_arg_size);
            }
            invalid_arg = 1;
        } else {
            arg[arg_size++] = (char)c;
        }
    }
    arg[arg_size] = 0;

    // Total characters read
    arg_size += 1;

    // If we had an error, return negative number
    if( invalid_arg ) {
        arg_size *= -1;
    }
    return arg_size;
}

void send_binary_data(int socketfd, char * response, int bytes_to_send) {
    int total_bytes_sent = 0;

    while( bytes_to_send ) {
        int bytes_sent = write(socketfd, &(response[total_bytes_sent]), bytes_to_send);

        if( bytes_sent < 0 ) {
            perror("Error: ");
            exit(1);
        } else if( bytes_sent == 0 ) {
            fprintf(stderr, "Error: Unexpected 0-bytes-sent error for a socket\n");
            exit(1);
        }
        bytes_to_send -= bytes_sent;
        total_bytes_sent += bytes_sent;
    }
}

void send_response(char cmd, char * arg_buf, int socketfd, char * response) {
    int bytes_to_send = strlen(response);
    int total_bytes_sent = 0;

    while( bytes_to_send ) {
        int bytes_sent = write(socketfd, &(response[total_bytes_sent]), bytes_to_send);

        if( bytes_sent < 0 ) {
            perror("Error: ");
            exit(1);
        } else if( bytes_sent == 0 ) {
            fprintf(stderr, "Error: Unexpected 0-bytes-sent error for a socket\n");
            exit(1);
        }
        bytes_to_send -= bytes_sent;
        total_bytes_sent += bytes_sent;
    }

    if( response[0] == 'A' ) {
        if( strlen(arg_buf) == 0 ) {
            printf("SUCCESS : %c\n", cmd);
        } else {
            printf("SUCCESS : %c %s\n", cmd, arg_buf);
        }
    } else {
        if( strlen(arg_buf) == 0 ) {
            printf("  FAIL  : %c\n", cmd);
        } else {
            printf("  FAIL  : %c %s\n", cmd, arg_buf);
        }
    }
}

int create_listen_data_socket( int * listen_data_socket) {
    if( *listen_data_socket != 0 ) {
        fprintf(stderr, "Error: Already created listen data socket \n");
        return 0;
    }

    *listen_data_socket = socket(AF_INET, SOCK_STREAM, 0);

    if( *listen_data_socket < 0 ) {
        perror("Error listen data socket create");
        exit(1);
    }

    struct sockaddr_in servAddr;

    memset( &servAddr, 0, sizeof(servAddr) );
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(0);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind( *listen_data_socket, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        perror("Error listen data socket bind");
        exit(1);
    } 

    struct sockaddr_in data_addr;
    socklen_t data_addr_len = sizeof(data_addr);
    memset( &data_addr, 0, sizeof(data_addr) );

    if( getsockname( *listen_data_socket, (struct sockaddr *)&data_addr, &data_addr_len) < 0 ) {
        perror("Error listen data socket getsockname");
        exit(1);
    }

    int port = ntohs(data_addr.sin_port);
    // This is the ephemeral port assigned to the listener

    // Setup connection queue one level deep
    if( listen(*listen_data_socket, 1) < 0 ) {
        perror("Error listen data socket: ");
        exit(1);
    }

    if( DEBUG ) printf("create_listen_data_socket: port=%d\n", port);
    return port;

}

int wait_for_data_connection(int * listen_data_socket) {
    int data_socket = 0;
    socklen_t length = sizeof(struct sockaddr_in);
    struct sockaddr_in clientAddr;

    if( DEBUG ) printf("\nCalling accept() on listen data socket\n");
    data_socket = accept(*listen_data_socket, (struct sockaddr *) &clientAddr, &length);

    if( data_socket < 0 ) {
        perror("Error: ");
        exit(1);
    }

    return data_socket;
}

void send_ls(int data_socket) {
    if( DEBUG ) printf("Sending ls -l output\n");

    int fd[2];

    if( pipe(fd) < 0 ) {
        perror("Error pipe");
        return;
    }

    int child_pid = (int)fork();

    if( child_pid < 0 ) {
        perror("Error fork");
        return;
    }

    if (child_pid) {
        //Parent Process
        close (fd[1]);

        // read bytes from socket and forward to child process
        char read_buf[MAX_BUF];
        int total_bytes_read = 0;
        while( 1 ) {
            // Read one byte
            int bytes_read = read(fd[0], read_buf, MAX_BUF);

            if( bytes_read < 0 ) {
                // Something bad happened
                perror("Error");
                exit(1);
            } else if( bytes_read == 0 ) {
                // Nothing left to read, quit
                break;
            }

            total_bytes_read += bytes_read;

            send_binary_data(data_socket, read_buf, bytes_read);
        }

        close(fd[0]);
        wait(NULL);
    } else {
        //Child Process
        close (fd[0]);
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("Error dup2");
            printf("Calling exit: error with dup2\n");
            exit(errno);
        }
        close(fd[1]);

        int result = system("ls -l");
        if( result < 0 ) {
            perror("Error 'ls -l' system");
        }
        exit(0);
    }

}

#define MAX_ARG (256)

char arg_buf[MAX_ARG+1];

int process_command( int socketfd, int * listen_data_socket ) {
    char cmd;
    // If result is > 0, this indicates a successful server argument
    int result = read_command( socketfd, &cmd, arg_buf, MAX_ARG );
    char response_str[128];
    if( result <= 0 ) {
        return result;
    }

    switch( cmd ) {
        int data_port;
        case 'D':
            data_port = create_listen_data_socket( listen_data_socket );
            sprintf(response_str, "A%d\n", data_port);
            send_response(cmd, arg_buf, socketfd, response_str);
            break;

        case 'C':
            if( strlen(arg_buf) == 0 ) {
                send_response(cmd, arg_buf, socketfd, "ENo command argument\n");
            } else {
                char buf[MAX_BUF];
                if( DEBUG ) printf("Before cd: %s\n", getcwd(buf, MAX_BUF-1));
                if( chdir(arg_buf) != 0 ) {
                    perror("Error");
                    sprintf(buf, "E%s\n", strerror(errno));
                    send_response(cmd, arg_buf, socketfd, buf);
                } else {
                    if( DEBUG ) printf("After cd: %s\n", getcwd(buf, MAX_BUF-1));
                    send_response(cmd, arg_buf, socketfd, "A\n");
                }

            }
            break;

        case 'L':
            if( *listen_data_socket == 0 ) {
                send_response(cmd, arg_buf, socketfd, "ENo Data Socket Created\n" );
            } else {
                int data_socket = wait_for_data_connection(listen_data_socket);

                send_ls(data_socket);

                close(data_socket);
                close(*listen_data_socket);
                *listen_data_socket = 0;

                send_response(cmd, arg_buf, socketfd, "A\n");
            }
            break;

        case 'G':
            if( *listen_data_socket == 0 ) {
                send_response(cmd, arg_buf, socketfd, "ENo Data Socket Created\n" );
            } else {
                int data_socket = wait_for_data_connection(listen_data_socket);

                FILE * fptr = fopen(arg_buf, "rb");
                if( fptr == NULL ) {
                    perror("Error");
                    close(data_socket);
                    close(*listen_data_socket);
                    *listen_data_socket = 0;
                    send_response(cmd, arg_buf, socketfd, "ECould not open file\n");
                    if( DEBUG ) printf("Error response sent over control socket\n");
                } else {
                    char file_read_buf[MAX_BUF];

                    while( 1 ) {
                        size_t file_bytes_read = fread(file_read_buf, 1, MAX_BUF, fptr);
                        if( file_bytes_read <= 0 )  {
                            break;
                        } else {
                            send_binary_data(data_socket, file_read_buf, (int)file_bytes_read);
                        }
                    }
                    close(data_socket);
                    close(*listen_data_socket);
                    *listen_data_socket = 0;
                    if( DEBUG ) printf("Data sent\n");
                    send_response(cmd, arg_buf, socketfd, "A\n");
                    if( DEBUG ) printf("Response sent over control socket\n");
                    fclose(fptr);
                }
            }
            break;

        case 'P':
            if( *listen_data_socket == 0 ) {
                send_response(cmd, arg_buf, socketfd, "ENo Data Socket Created\n" );
            } else {
                int data_socket = wait_for_data_connection(listen_data_socket);

                FILE * fptr = fopen(arg_buf, "wb");
                if( fptr == NULL ) {
                    perror("Error");
                    close(data_socket);
                    close(*listen_data_socket);
                    *listen_data_socket = 0;
                    send_response(cmd, arg_buf, socketfd, "ECould not open file\n");
                    if( DEBUG ) printf("Error response sent over control socket\n");
                } else {
                    char read_buf[MAX_BUF];
                    int error = 0;

                    while( 1 ) {
                        size_t bytes_read = read(data_socket, read_buf, MAX_BUF);
                        if( bytes_read <= 0 )  {
                            break;
                        } else {
                            int bytes_written = fwrite(read_buf, 1, bytes_read, fptr);
                            if( bytes_written != bytes_read ) {
                                error = 1;
                                break;
                            }
                        }
                    }
                    close(data_socket);
                    close(*listen_data_socket);
                    *listen_data_socket = 0;
                    if( error ) {
                        send_response(cmd, arg_buf, socketfd, "EError writing bytes to file\n");
                    } else {
                        send_response(cmd, arg_buf, socketfd, "A\n");
                    }
                    if( DEBUG ) printf("Response sent over control socket\n");
                    fclose(fptr);
                }
            }
            break;

        case 'Q':
            send_response(cmd, arg_buf, socketfd, "A\n");

            // Returning result == 0 means done accepting commands
            result = 0;
            break;

        default:
            fprintf(stderr, "Error: Unknown command '%c'\n", cmd);
            sprintf(response_str, "EUnknown command '%c'\n", cmd);
            send_response(cmd, arg_buf, socketfd, response_str);
            break;
    }
    return result;
}

#define CMD_LEN (40)
#define ARG_LEN (512)

void run_new_connection( int connectfd, struct sockaddr_in clientAddr) {
    if( DEBUG ) printf("\nEnter run_new_connection()\n");

    char hostName[NI_MAXHOST];
    int hostEntry = getnameinfo((struct sockaddr*)&clientAddr, sizeof(clientAddr),
                    hostName, sizeof(hostName), NULL, 0, NI_NUMERICSERV);
    if( hostEntry != 0 ) {
        if( DEBUG ) printf("run_new_connection: getnameinfo failed: %s\n", strerror(errno));
        fprintf(stderr, "Error: %s\n", gai_strerror(hostEntry));
        exit(1);
        return;
    }

    printf("New client connection: '%s'\n", hostName);
    fflush(stdout);

    char message[18];
    time_t rawtime;
    time( &rawtime );
    struct tm *timeinfo = localtime(&rawtime);
    int data_socket = 0;

    strftime( message, 18, "%a %b %e %H:%M:%S", timeinfo );

    // Keep executing commands until we see 'exit' command
    while( 1 ) {
        int result = process_command( connectfd, &data_socket );
        if( result == 0 ) {
            if( DEBUG ) printf("read_command: Quit or connection apparently closed\n");
            break;
        } else if( result < 0 ) {
            if( DEBUG ) printf("read_command: bad command, ignore and continue\n");
        }
    }


    if( close( connectfd ) < 0 ) {
        perror("Error: ");
        exit(1);
    }
    
}

void wait_for_child(int * active_children ) {
    int status;
    if( DEBUG ) printf("Waiting for child to exit\n");
    pid_t pid = waitpid(-1, &status, 0);
    if( WIFEXITED(status) ) {
        int exit_status = WEXITSTATUS(status);
        if( DEBUG ) printf("\nProcess %d exited normally with exit status=%d\n", pid, exit_status);
        if( active_children != NULL ) {
            (*active_children)--;
        }
    }
}

void run_server() {
    int active_children = 0;

    // Create server socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    if( listenfd < 0 ) {
        perror("Error: ");
        exit(1);
    }

    // This is useful to clear the socket and make it available if server is killed
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    struct sockaddr_in servAddr;

    memset( &servAddr, 0, sizeof(servAddr) );
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(MY_PORT_NUMBER);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if( bind( listenfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
        perror("Error: ");
        exit(1);
    } 

    // Setup connection queue one level deep
    if( listen(listenfd, MAX_REQUEST_QUEUE_SIZE) < 0 ) {
        perror("Error: ");
        exit(1);
    }

    while( 1 ) {
        // Wait for new connection
        int connectfd = 0;
        socklen_t length = sizeof(struct sockaddr_in);
        struct sockaddr_in clientAddr;

        if( DEBUG ) printf("\nCalling accept()\n");
        connectfd = accept(listenfd, (struct sockaddr *) &clientAddr, &length);

        if( connectfd < 0 ) {
            perror("Error: ");
            exit(1);
        }

        int pid = fork();

        if( pid < 0 ) {
            perror("Error: ");
        } else if( pid == 0 ) {
            // child
            run_new_connection( connectfd, clientAddr);
            exit(0);
        } else {
            // parent
            active_children++;
            if( DEBUG ) printf("\nCreated child pid=%d\n---------------------\n", pid);

            // If there are more 4 active children, block for one to finish
            if( active_children >= 4 ) {
                wait_for_child( &active_children );
            }

            close( connectfd );
        }

    }

    while( active_children > 0 ) {
        wait_for_child( &active_children );
    }

}

int main(int argc, char const *argv[]){

    if( (argc > 2) || ((argc == 2) && strcmp(argv[1], "-d") != 0) ) {
        printf("\nUsage: %s [-d]\n\n", argv[0]);
        exit(1);
    }

    if(argc == 2) {
        DEBUG = 1;
    }

    run_server();
    return 0;
}
