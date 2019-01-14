        .section ".rodata"
msg:
        .asciz  "Hi\n"
        .section ".text"
        .globl  main
main:
        push   ebp
        mov       ebp, esp
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
