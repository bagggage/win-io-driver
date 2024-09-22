PUBLIC IsaReadDword
.code _text

IsaReadDword PROC PUBLIC
	mov dx, cx
	in eax, dx
	ret
IsaReadDword ENDP

IsaWriteDword PROC PUBLIC
	mov eax, edx
	mov dx, cx
	out dx, eax
	ret
IsaWriteDword ENDP

END