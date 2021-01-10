
SECTION	.text

global	stringgt

stringgt:
		push	ebp
		mov	ebp, esp
		push	edx
		push    esi
		push 	edi
		push	ebx

		mov     esi, [ebp+8]		; Get address to first string
		mov     edi, [ebp+12]		; Get address to second string
		mov     edx, 0			; Clear edx
loop:
                mov     al, [esi+edx]		; Grab next character from string 1
                mov     bl, [edi+edx]		; Grab next character from string 2
                inc     edx			; increment counter counting characters compared
                cmp     al, bl 			; Compare the two characters
                jne     notequal		; If they are not equal, jump out of loop
                cmp     al, 0			; Check for null character, end of string
                je      notgreater		; if al == bl == 0, strings are same
                jmp     loop			; letters equal, not at end, keep going

notequal:
                jc      notgreater		; from cmp, carry bit is set if al < bl
greater:
		mov	eax, 1			; write
                jmp     done
notgreater:
		mov	eax, 0			; write
done:
		pop	ebx
		pop 	edi
		pop	esi
		pop	edx
		mov	esp, ebp
                pop	ebp
                ret


