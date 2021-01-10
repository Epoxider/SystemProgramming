#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

int open_file( char * filename ) {
    int fileHandle = open(filename, O_RDONLY);
    if( fileHandle < 0 ) {
        printf("\nCould not open '%s' for reading\n\n", filename );
        exit(-1);
    }
    return fileHandle;
}

void verify_line_width( int fileHandle, int lineWidth ) {
    unsigned char c;
    lseek( fileHandle, lineWidth - 1, SEEK_SET );
    ssize_t read_bytes = read( fileHandle, (void *)&c, 1);
    assert( read_bytes == 1 );
    assert( c == '\n' );
}

int get_current_position( int fileHandle ) {
    // By seeking 0 bytes from current position, we
    // "move" to the current position.  Any move request
    // returns the position of where the move ended.
    // This has the effect of returning the current file position.
    return (int)lseek( fileHandle, 0, SEEK_CUR );
}

int get_file_size( int fileHandle ) {
    // Find current position so we can restore it after seeking to end
    int current_position = get_current_position( fileHandle );

    // Seek to end of file (position one past last byte in file)
    int seek_end_value = lseek( fileHandle, 0, SEEK_END );

    // Now, move back to where we were when entering this fucntion
    lseek( fileHandle, current_position, SEEK_SET );

    // Return the position of one past the last byte in the file, which is file size
    return (int)seek_end_value;
}

void get_line( int line, int fileHandle, char * buf, int lineWidth ) {
    lseek( fileHandle, line * lineWidth, SEEK_SET );
    ssize_t read_bytes = read( fileHandle, (void *)&(buf[0]), lineWidth);
    assert( (int)read_bytes == lineWidth );

    // Make sure we end with a null terminator
    buf[lineWidth] = 0;
}

int get_number_of_lines( int fileHandle, int lineWidth ) {
    int fileSize = get_file_size( fileHandle );

    // Make sure file size is an even number of lines
    assert( fileSize % lineWidth == 0 );

    return fileSize / lineWidth;
}

int main(int argc, char ** argv) {
    char buf[1024];

    if( argc != 3 ) {
        printf("\n\tUsage:\n\t\t%s input.txt 15\n\n", argv[0]);
        exit(-1);
    }

    int lineWidth = atoi(argv[2]);
    printf("lineWidth=%d\n", lineWidth);

    int fileHandle = open_file( argv[1] );

    // Verify linewidth by looking for '\n'
    verify_line_width(fileHandle, lineWidth);
    printf("lineWidth verified!\n");

    int fileSize = get_file_size(fileHandle);
    printf("File size=%d\n", fileSize);

    int numberOfLines = get_number_of_lines( fileHandle, lineWidth );
    printf("Number of lines: %d\n", numberOfLines);

    printf("\n\nLines in reverse order:\n");
    for( int i = numberOfLines - 1; i>= 0; i-- ) {
        get_line( i, fileHandle, buf, lineWidth );
        printf("line %d: %s\n", i, buf);
    }

    close(fileHandle);
    return 0;
}
