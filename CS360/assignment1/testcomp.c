
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "dict.h"
#include "lzw.h"

#define FILE_BUFFER_SIZE (1024)
typedef struct _my_context {
    FILE * fptrRead;
    FILE * fptrWrite;
    unsigned char readBuffer[FILE_BUFFER_SIZE];
    unsigned char writeBuffer[FILE_BUFFER_SIZE];
    unsigned int bytesInReadBuffer;
    unsigned int readLoc;
    unsigned int writeLoc;
} MyContext;

int myReadFunc( void* voidContext ) {
    MyContext * context = (MyContext *)voidContext;

    assert( context->fptrRead != NULL );
    if( (context->bytesInReadBuffer - context->readLoc) <= 0 ) {
        if( feof( context->fptrRead ) ) {
            fclose( context->fptrRead );
            context->fptrRead = NULL;
            return -1;
        }
        context->bytesInReadBuffer = (unsigned int)fread(context->readBuffer, 1, FILE_BUFFER_SIZE, context->fptrRead );
        if( context->bytesInReadBuffer <= 0 ) {
            fclose( context->fptrRead );
            context->fptrRead = NULL;
            return -1;
        }
        context->readLoc = 0;
    }
    // printf("readFunction: returning %c\n", context->readBuffer[context->readLoc]);
    return context->readBuffer[context->readLoc++];
}

void myWriteFunc( unsigned char c, void *voidContext ) {
    // printf("writeFunction: Writing value = %d\n", (int)c );

    MyContext * context = (MyContext *)voidContext;

    assert( context->fptrWrite != NULL );

    if( context->writeLoc >= FILE_BUFFER_SIZE ) {
        size_t bytesWritten = fwrite(context->writeBuffer , 1 , FILE_BUFFER_SIZE , context->fptrWrite );
        assert( bytesWritten == FILE_BUFFER_SIZE );
        context->writeLoc = 0;
    }
    context->writeBuffer[context->writeLoc++] = c;
}

MyContext * setupContext( char * readFilename, char * writeFilename ) {
    MyContext * context = (MyContext *)malloc( sizeof(MyContext) );
    assert( context != NULL );

    context->fptrRead = fopen(readFilename,"rb");
    assert( context->fptrRead != NULL );

    context->fptrWrite = fopen(writeFilename,"wb");
    assert( context->fptrWrite != NULL );

    context->bytesInReadBuffer = 0;
    context->readLoc = 0;
    context->writeLoc = 0;

    return context;
}

void cleanupContext( MyContext * context ) {
    if( context->fptrRead != NULL ) {
        fclose( context->fptrRead );
        context->bytesInReadBuffer = 0;
        context->readLoc = 0;
        context->fptrRead = NULL;
    }

    if( context->fptrWrite != NULL ) {
        if( context->writeLoc > 0 ) {
            size_t bytesWritten = fwrite( context->writeBuffer, 1, context->writeLoc, context->fptrWrite );
            assert( bytesWritten == context->writeLoc );
            context->writeLoc = 0;
        }
        fclose( context->fptrWrite );
        context->fptrWrite = NULL;
    }
    free(context);
}

int main(int argc, char ** argv) {
    printf("\nCalling lzwEncode...\n");
    unsigned int bits = 9;
    unsigned int maxBits = 16;
    int (*readFunc)(void* context);
    void (*writeFunc)(unsigned char c, void* context);

    readFunc = myReadFunc;
    writeFunc = myWriteFunc;

    MyContext * context = setupContext("compInput", "compOutput");

    bool success = lzwEncode(bits, maxBits, readFunc, writeFunc, (void *)context );
    printf("\nSucces = %d\n", (int)success );

    cleanupContext( context );

    return 0;
}
