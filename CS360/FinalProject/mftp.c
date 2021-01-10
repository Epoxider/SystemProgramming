#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ctype.h>

#include <unistd.h>
#include "mftp.h"

int DEBUG = 0;

#define MAX_BUF (256)

void send_command(int socketfd, char * server_cmd) {
    int bytes_to_send = strlen(server_cmd);
    int total_bytes_sent = 0;

    while( bytes_to_send ) {
        if( DEBUG ) printf("\tTryin to send %d bytes\n", bytes_to_send);
        int bytes_sent = write(socketfd, &(server_cmd[total_bytes_sent]), bytes_to_send);

        if( bytes_sent < 0 ) {
            perror("Error");
            exit(1);
        } else if( bytes_sent == 0 ) {
            fprintf(stderr, "Error: Unexpected bytes_sent==0 for a socket\n");
            exit(1);
        }
        if( DEBUG ) printf("\tBytes actually sent: %d\n", bytes_sent);
        bytes_to_send -= bytes_sent;
        total_bytes_sent += bytes_sent;
    }
}

int get_response(int socketfd) {
    int total_bytes_read = 0;
    char response_str[256];
    int server_error = 0;
    while( 1 ) {
        unsigned char c;

        // Read one byte
        int bytes_read = read(socketfd, &c, 1);

        if( bytes_read < 0 ) {
            // Something bad happened
            perror("Error");
            exit(1);
        } else if( bytes_read == 0 ) {
            // Nothing left to read, quit
            break;
        }

        if( total_bytes_read == 0 ) {
            if( c == 'E' ) {
                server_error = 1;
            }
            total_bytes_read++;
        } else {
            response_str[(total_bytes_read++)-1] = (char)c;
        }

        // Linefeed terminates a command
        if( c == '\n' ) {
            response_str[total_bytes_read-1] = 0;
            break;
        }
    }
    if( DEBUG ) printf("Server response: %s\n", response_str);
    if( !server_error && total_bytes_read > 1 ) {
        return atoi(response_str);
    } else {
        if( server_error ) {
            fprintf(stderr, "Server error: %s\n", response_str);
            return -1;
        } else {
            return 0;
        }
    }
}

void get_filename_from_path( char * arg, char * filename, int filename_size) {
    int arg_len = strlen(arg);

    // Find the right-most slash
    int pos = arg_len-1;
    while( (pos >= 0) && (arg[pos] != '/') ) {
        pos--;
    }
    // move to first character to right of slash
    pos++;
    int filename_pos = 0;
    while( pos < arg_len ) {
        // If we filled up filename, quit
        if( filename_pos > (filename_size-1) ) {
            break;
        }
        filename[filename_pos++] = arg[pos++];
    }
    filename[filename_pos] = 0;
}

int connect_socket( const char * server, int data_port ) {
    struct addrinfo hints, *actualdata;
    memset( &hints, 0, sizeof(hints));
    char portstr[10];
    sprintf(portstr, "%d", data_port);

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;

    if( getaddrinfo(server, portstr, &hints, &actualdata) != 0 ) {
        fprintf(stderr, "Error: Could not get address info for '%s:%d'\n", server, data_port);
        exit(1);
    }

    int data_socket_fd = socket(actualdata->ai_family, actualdata->ai_socktype, 0);
    if( data_socket_fd < 0 ) {
        perror("Error");
        exit(1);
    }

    if( connect(data_socket_fd, actualdata->ai_addr, actualdata->ai_addrlen) < 0 ) {
        perror("Error");
        exit(1);
    }

    return data_socket_fd;
}

void display_data_socket_response( const char * server, int data_port ) {
    int fd[2];

    if( pipe (fd) < 0 ) {
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
        close (fd[0]);

        int data_socket_fd = connect_socket( server, data_port );
        if( DEBUG ) printf("\nSuccessfully connected to data socket '%s:%d'\n", server, data_port);

        // read bytes from socket and forward to child process
        char read_buf[MAX_BUF];
        while( 1 ) {
            // Read one byte
            int bytes_read = read(data_socket_fd, read_buf, MAX_BUF);

            if( bytes_read < 0 ) {
                // Something bad happened
                perror("Error");
                exit(1);
            } else if( bytes_read == 0 ) {
                // Nothing left to read, quit
                break;
            }

            // Now, write bytes to pipe
            int bytes_written = write( fd[1], read_buf, bytes_read);
            if( bytes_written < 0 ) {
                perror("Error data pipe write");
                exit(1);
            }

            if( bytes_written != bytes_read ) {
                fprintf(stderr, "Error: written bytes=%d != bytes_read=%d\n", bytes_written, bytes_read );
            } 
        }

        if( close( data_socket_fd ) < 0 ) {
            perror("Error");
            exit(1);
        }

        close(fd[1]);
        wait(NULL);
    } else {
        //Child Process
        close (fd[1]);
        if (dup2(fd[0], STDIN_FILENO) == -1) {
            perror("Error dup2");
            printf("Calling exit: error with dup2\n");
            exit(errno);
        }
        close(fd[0]);

        int result = system("more -20");
        if( result < 0 ) {
            perror("Error 'more -20' system");
        }
        exit(0);
    }
}

int save_data_socket_to_file( const char * server, int data_port, char * filename ) {

    int data_socket_fd = connect_socket( server, data_port );
    int total_bytes_read = 0;


    FILE * fptr = NULL;

    // read bytes from socket and forward to child process
    char read_buf[MAX_BUF];
    while( 1 ) {
        // Read one byte
        int bytes_read = read(data_socket_fd, read_buf, MAX_BUF);

        if( bytes_read < 0 ) {
            // Something bad happened
            perror("Error");
            exit(1);
        } else if( bytes_read == 0 ) {
            // Nothing left to read, quit
            break;
        }

        total_bytes_read += bytes_read;

        // Only open the file after we got at least 1 byte
        if( fptr == NULL ) {
            if( (fptr = fopen(filename, "wb")) == NULL ) {
                perror("Error");
                close( data_socket_fd );
                return total_bytes_read;
            }
        }

        // Now, write bytes to pipe
        int bytes_written = fwrite( read_buf, 1, bytes_read, fptr);
        if( bytes_written < 0 ) {
            perror("Error data pipe write");
            exit(1);
        }

        if( bytes_written != bytes_read ) {
            fprintf(stderr, "Error: written bytes=%d != bytes_read=%d\n", bytes_written, bytes_read );
        } 
    }

    if( fptr != NULL ) {
        if( fclose(fptr) < 0 ) {
            perror("Error");
            exit(1);
        }
    }

    if( close( data_socket_fd ) < 0 ) {
        perror("Error");
        exit(1);
    }
    return total_bytes_read;
}

void send_file_to_data_socket( const char * server, int data_port, char * filename ) {
    int data_socket_fd = connect_socket( server, data_port );

    FILE * fptr;
    if( (fptr = fopen(filename, "rb")) == NULL ) {
        perror("Error");
        close( data_socket_fd );
        return;
    }

    // read bytes from socket and forward to child process
    char read_buf[MAX_BUF];
    while( 1 ) {
        // Read one byte
        int bytes_read = fread(read_buf, 1, MAX_BUF, fptr);

        if( bytes_read < 0 ) {
            // Something bad happened
            perror("Error");
            exit(1);
        } else if( bytes_read == 0 ) {
            // Nothing left to read, quit
            break;
        }

        // Now, write bytes to pipe
        int bytes_written = write( data_socket_fd, read_buf, bytes_read);
        if( bytes_written < 0 ) {
            perror("Error data pipe write");
            exit(1);
        }

        if( bytes_written != bytes_read ) {
            fprintf(stderr, "Error: written bytes=%d != bytes_read=%d\n", bytes_written, bytes_read );
        } 
    }

    if( fclose(fptr) < 0 ) {
        perror("Error");
        exit(1);
    }

    if( close( data_socket_fd ) < 0 ) {
        perror("Error");
        exit(1);
    }
}

int process_command(const char * server, int socketfd, char * cmd, char * arg) {
    int done = 0;
    char buf[MAX_BUF];

    if( DEBUG ) printf("Processing cmd=%s, arg=%s\n", cmd, arg);
    if( strcmp(cmd, "exit") == 0 ) {
        done = 1;
        send_command(socketfd, "Q\n");
        get_response(socketfd);
    } else if( strcmp(cmd, "cd") == 0 ) {
        if( arg == NULL ) {
            fprintf(stderr, "Error: No argument supplied for 'cd' command\n");
        } else {
            if( DEBUG ) printf("Before cd: %s\n", getcwd(buf, MAX_BUF-1)); 
            if( chdir(arg) != 0 ) {
                perror("Error");
            } else {
                if( DEBUG ) printf("After cd: %s\n", getcwd(buf, MAX_BUF-1)); 
            }
        }
    } else if( strcmp(cmd, "rcd") == 0 ) {
        if( arg == NULL ) {
            fprintf(stderr, "rcd command must have a parameter\n");
        } else {
            sprintf(buf, "C%s\n", arg);
            send_command(socketfd, buf);
            get_response(socketfd);
        }
    } else if( strcmp(cmd, "ls") == 0 ) {
        system("ls -l | more -20");
    } else if( strcmp(cmd, "rls") == 0 ) {
        if( arg != NULL ) {
            fprintf(stderr, "rls does not take any arguments\n");
        } else {
            send_command(socketfd, "D\n");
            int data_port = get_response(socketfd);

            send_command(socketfd, "L\n");

            display_data_socket_response(server, data_port);

            get_response(socketfd);
        }
    } else if( strcmp(cmd, "get") == 0 ) {
        send_command(socketfd, "D\n");
        int data_port = get_response(socketfd);

        sprintf(buf, "G%s\n", arg);
        send_command(socketfd, buf);
        char filename[MAX_BUF];
        get_filename_from_path(arg, filename, MAX_BUF);

        int bytes_read = save_data_socket_to_file(server, data_port, filename);

        if( (bytes_read == 0) && (get_response(socketfd) >= 0) ) {
            // Handle the case where we are copying a zero-length file
            // We just need to create the file and close it to make empty file
            FILE * fptr = NULL;
            if( (fptr = fopen(filename, "wb")) == NULL ) {
                perror("Error");
            } else {
                fclose(fptr);
            }
        }
    } else if( strcmp(cmd, "show") == 0 ) {
        send_command(socketfd, "D\n");
        int data_port = get_response(socketfd);

        sprintf(buf, "G%s\n", arg);
        send_command(socketfd, buf);

        display_data_socket_response(server, data_port);

        get_response(socketfd);
    } else if( strcmp(cmd, "put") == 0 ) {
        send_command(socketfd, "D\n");
        int data_port = get_response(socketfd);

        char filename[MAX_BUF];
        get_filename_from_path(arg, filename, MAX_BUF);
        sprintf(buf, "P%s\n", filename);
        send_command(socketfd, buf);

        send_file_to_data_socket(server, data_port, arg);

        get_response(socketfd);
    } else {
        fprintf(stderr, "Error: Unknown command '%s'\n", cmd);
    }

    return done;
}

void process_user_commands( const char * server, int socketfd ) {

    char buf[MAX_BUF];
    char * read_buf = NULL;
    int done = 0;

    char *cmd_str;
    char *arg_str;
    const char delims[3] = " \t";

    while( !done ) {
        // When successful, read_buf points to sam thing as buf.
        // When end of file, NULL is returned.
        printf("\nEnter Command: ");
        read_buf = fgets(buf, MAX_BUF, stdin);
        if( read_buf == NULL ) {
            break;
        }
        // Make sure the buf array always ends with null terminator
        buf[MAX_BUF-1] = 0;

        // get the first token
        cmd_str = strtok(buf, delims);
        if( cmd_str != NULL ) {
            // get argument, if any
            arg_str = strtok(NULL, delims);
            if( arg_str ) {
                int all_white = 1;
                for( int i = 0; i < strlen(arg_str); i++) {
                    if( !isspace(arg_str[i] ) ) {
                        all_white = 0;
                        break;
                    }
                }
                if( all_white ) {
                    arg_str = NULL;
                }
            }
        }

        if( cmd_str != NULL ) {
            int cmd_len = strlen(cmd_str); 
            if( cmd_str[cmd_len-1] == '\n' ) {
                cmd_str[cmd_len-1] = 0;
            }
        }
        if( arg_str != NULL ) {
            int arg_len = strlen(arg_str); 
            if( arg_str[arg_len-1] == '\n' ) {
                arg_str[arg_len-1] = 0;
            }
        }
        done = process_command(server, socketfd, cmd_str, arg_str);
    }
    if( DEBUG ) printf("\nExiting process_user_commands.  End of file reached.\n");

}

void run_client( const char * server ) {
    if( DEBUG ) printf("\nEntering mftp run_client( %s )\n\n", server);

    int socketfd = connect_socket( server, MY_PORT_NUMBER );
    printf("\nSuccessfully connected to '%s:%d'\n", server, MY_PORT_NUMBER);

    process_user_commands( server, socketfd );

    if( close( socketfd ) < 0 ) {
        perror("Error");
        exit(1);
    }
}

int main(int argc, char const *argv[]){

    // Make sure arguments to program are good
    if( (argc < 2) || (argc > 3) || 
         ((argc == 3) && (strcmp(argv[1], "-d") != 0) ) ) {
        printf("\nUsage: %s [-d] <server>\n\n", argv[0]);
        exit(1);
    }

    if( argc == 2 ) {
        run_client( argv[1] );
    } else {
        DEBUG = 1;
        run_client( argv[2] );
    }

    return 0;
}

