
SECTION	.data

badprompt:	db	"Bad Number.",10
plenbad:	equ	$-badprompt
enternumber:	db	"Enter a positive integer: "
plennumber:	equ	$-enternumber
gcdequals:	db	"Greatest common divisor = "
gcdlength:	equ	$-gcdequals

SECTION	.bss
	
maxchars:	equ	30
inbuf:		resb	maxchars + 2

SECTION	.text

global	_start

;---------------------------------------------------------------
;   int get_int(char * s);
;---------------------------------------------------------------
get_int:
		push	ebp		; save registers and set ebp
		mov	ebp, esp
		push	edx
		push    esi
		push	ebx

		mov     esi, [ebp+8]	; Get address of string
		mov     edx, 0		; Clear edx
		mov     eax, 0		; Clear eax
		mov     ebx, 0		; Clear ebx
get_int_loop:
                mov     bl, [esi+edx]	; Grab next character from string 1
                inc     edx		; increment counter counting characters compared
                cmp     bl, 10 		; Check if newline
                je      get_int_done	; If they are not equal, jump out of loop
                cmp     bl, 0		; Check for null character, end of string
                je      get_int_done	; if al == bl == 0, strings are same
		cmp	bl, '0'		; Check if less than '0'
                jl	get_int_bad	;
		cmp 	bl, '9'		;
		jg	get_int_bad
		sub	bl, '0'
                imul	eax, 10		; eax = eax * 10a
		add	eax, ebx
                jmp     get_int_loop	; letters equal, not at end, keep going
get_int_bad:
		mov	eax, 4		; write
		mov	ebx, 1		; to standard output
		mov	ecx, badprompt	; the prompt string
		mov	edx, plenbad	; of length string
		int 	80H		; interrupt with the syscall
		jmp	exit
get_int_done:
		pop	ebx		; restore registers and return
		pop	esi
		pop	edx
		mov	esp, ebp
                pop	ebp
                ret

;---------------------------------------------------------------
;    unsigned int gcd(unsigned int n, unsigned int m)
;---------------------------------------------------------------
gcd:
		push	ebp		; push values and set ebp
		mov	ebp, esp
		push	ebx

		mov     eax, [ebp+8]	; Get n
		mov     ebx, [ebp+12]	; Get m
		cmp	eax, ebx	; compare n & m
		jg	gcd_greater	; jump if n > m
		jl	gcd_less	; jump if n < m
		jmp	gcd_done	; jump if n == m
gcd_greater:
		sub	eax, ebx	; New Values: n-m, m
		jmp 	gcd_recurse

gcd_less:
		sub	ebx, eax	; New Values: n, m-n
gcd_recurse:
		push	ebx		; push 2nd param of gcd()
		push	eax		; push 1rst param of gcd()
		call 	gcd		; aex = gcd( New Values )
		add	esp, 0x08	; recover the stack for 2 pushed args
gcd_done:
		pop	ebx		; pop values, and return
		mov	esp, ebp
                pop	ebp
                ret

;---------------------------------------------------------------
;    void make_decimal(unsigned int n);
;---------------------------------------------------------------
make_decimal:				
		push	ebp		; push values and set ebp
		mov	ebp, esp
		push	ebx
		push	ecx
		push	edx

		mov     eax, [ebp+8]	; Get n

		mov	edx, 0		; clear edx
		mov	ecx, 10		; Get ready for eax /= 10
		div	ecx		; EAX = EAX / 10,  EDX = EAX % 10
		cmp	eax, 0		; If n was < 10, at last digit
		je	make_decimal_last

		push	eax		; n >= 10, deal with higher 
		call 	make_decimal	; significant digit first
		add	esp, 0x04	; recover the stack for 1 pushed args

make_decimal_last:
		add	edx, '0'	; Make digit ASCII
		push	edx		; Will print character at top of stack
		mov	eax, 4		; write
		mov	ebx, 1		; to standard output
		mov	ecx, esp	; character to print is at top of stack
		mov	edx, 1		; Print a single character
		int 	80H		; interrupt with the syscall
		add	esp, 4		; restore stack

make_decimal_done:
		pop	edx		; recover values and return
		pop	ecx
		pop	ebx
		mov	esp, ebp
                pop	ebp
                ret

;---------------------------------------------------------------
;     int read_num();
;---------------------------------------------------------------

read_num:
		push	ebp		; push values and set ebp
		mov	ebp, esp
		push	ebx
		push	ecx
		push	edx

		mov	eax, 4		; write
		mov	ebx, 1		; to standard output
		mov	ecx, enternumber	; the prompt string
		mov	edx, plennumber	; with correct length
		int 	80H		; interrupt with the syscall

		mov	eax, 3		; read
		mov	ebx, 0		; from standard input
		mov	ecx, inbuf	; into the input buffer
		mov	edx, maxchars+1	; upto maxchars + 1 bytes
		int 	80H		; interrupt with the syscall

		mov	eax, inbuf	; get address where to put digits
		push	eax		; push address as only argument
		call 	get_int		; aex = get_int( &inbuf )
		add	esp, 0x04	; recover the stack for 1 pushed args

					; result from get_int is in eax, which
					; will be returned

read_num_done:
		pop	edx		; pop values and prepare to return
		pop	ecx
		pop	ebx
		mov	esp, ebp
                pop	ebp
                ret

_start:
		push	ecx		; save registers
		push	edx		;

		call 	read_num	; eax = read_num()
		push	eax
		call 	read_num	; eax = read_num()
		push	eax
		call 	gcd		; aex = gcd( two read values )
		add	esp, 0x08	; recover the stack for 2 pushed args

		push	eax		; push argument to prep for 
					; calling make_decimal
		
		mov	eax, 4		; write
		mov	ebx, 1		; to standard output
		mov	ecx, gcdequals	; the prompt string
		mov	edx, gcdlength	; with correct length
		int 	80H		; interrupt with the syscall

		call	make_decimal	; pushed argument already
		add	esp, 0x04	; recover the stack pushed arg

		mov	edx, 10		; Move new-line into edx
		push	edx		; Will print character at top of stack
		mov	eax, 4		; write
		mov	ebx, 1		; to standard output
		mov	ecx, esp	; character to print is at top of stack
		mov	edx, 1		; Print a single character
		int 	80H		; interrupt with the syscall
		add	esp, 4		; restore stack

		pop	edx		; restore registers
		pop	ecx		; 
exit:
		mov eax, 1		; set up process exit
		mov ebx, 0		; and
		int	80H		; terminate

