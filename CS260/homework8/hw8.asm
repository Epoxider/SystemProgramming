
SECTION	.data

prompt1:	db	"Enter first string (max 30 characters): "
plen1:		equ	$-prompt1
prompt2:	db	"Enter second string (max 30 characters): "
plen2:		equ	$-prompt2
trueprompt:	db	"True",10
truelen:	equ	$-trueprompt
falseprompt:	db	"False",10
falselen:	equ	$-falseprompt

SECTION	.bss
	
maxchars:	equ	30
inbuf1:		resb	maxchars + 2
inbuf2:		resb	maxchars + 2

SECTION	.text

global	_start

_start:
		nop
		mov		eax, 4		; write
		mov		ebx, 1		; to standard output
		mov		ecx, prompt1	; the prompt string
		mov		edx, plen1	; of length plen
		int 	80H			; interrupt with the syscall

		mov		eax, 3		; read
		mov		ebx, 0		; from standard input
		mov		ecx, inbuf1	; into the first input buffer
		mov		edx, maxchars+1	; upto maxchars + 1 bytes
		int 	80H			; interrupt with the syscall

		nop
		mov		eax, 4		; write
		mov		ebx, 1		; to standard output
		mov		ecx, prompt2	; the second prompt string
		mov		edx, plen2	; of length plen
		int 	80H			; interrupt with the syscall

		mov		eax, 3		; read
		mov		ebx, 0		; from standard input
		mov		ecx, inbuf2	; into the second input buffer
		mov		edx, maxchars+1	; upto maxchars + 1 bytes
		int 	80H			; interrupt with the syscall

		mov     esi, inbuf1		; Set esi to first input string
		mov     edi, inbuf2		; Set edi to second input string
		mov     edx, 0			; Clear edx
loop:
                mov     al, [esi+edx]		; Grab next character from string 1
                mov     bl, [edi+edx]		; Grab next character from string 2
                inc     edx			; increment counter counting characters compared
                cmp     al, bl 			; Compare the two characters
                jne     notequal		; If they are not equal, jump out of loop
                cmp     al, 0			; Check for null character, end of string
                je      notgreater		; if al == bl == 0, strings are same
                cmp     edx, maxchars		; if we got to end of string without 0,
                jge     notgreater		; then jump to notgreater
                jmp     loop			; letters equal, not at end, keep going

notequal:
                jc      notgreater		; from cmp, carry bit is set if al < bl
greater:
		mov	eax, 4			; write
		mov	ebx, 1			; to standard output
		mov	ecx, trueprompt		; the true prompt string
		mov	edx, truelen		; of length truelen
		int 	80H			; interrupt with the syscall
                jmp     done
                
notgreater:
		mov	eax, 4			; write
		mov	ebx, 1			; to standard output
		mov	ecx, falseprompt	; the false prompt string
		mov	edx, falselen		; of length falselen
		int 	80H			; interrupt with the syscall

done:
		mov eax, 1				; set up process exit
		mov ebx, 0				; and
		int	80H				; terminate


