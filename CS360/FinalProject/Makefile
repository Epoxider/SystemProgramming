
CFLAGS=-std=gnu99 -Wall -pedantic -g

all: mftp mftpserve

mftp: mftp.c 
	gcc $(CFLAGS) mftp.c -o mftp -lc

mftpserve: mftpserve.c 
	gcc $(CFLAGS) mftpserve.c -o mftpserve -lc

clean:
	rm -f mftp mftpserve

