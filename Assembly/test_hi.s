.rodata	@dusan
msg:
        .asciz  "Hi\n"
		.char 'H', 'e', 'l', 'l', 'o', 0x2c, 0x20, 'w', 'o', 'r', 'l', 'd', '.', '\n'	# niz za ispis
		.skip 32
		.align 4
.text
.global  main
main:
        push   ebp
        mov       ebp, esp	# enter 0, 0
        call   getchar
        cmp    eax, 'A'
        jne    skip
        push   offset msg
        call   printf
        add    esp, 4
skip:
        mov    eax, 0
        mov    esp, ebp
        pop    ebp
        ret
.end