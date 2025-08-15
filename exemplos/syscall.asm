; comando - compilação: nasm -f elf64 -o syscall.o syscall.asm
; comando - link: ld -o syscall syscall.o
; execução - ./syscall

section .data
msg db "Olá Mundo!", 0xA    ; Define uma sequência de bytes chamada 'msg' - 
tamanho: equ 13             ; tamanho: constante que armazenada o valor 13

section .text

global _start

_start
    mov rax, 1              ; RAX = 1, código para sys_write
    mov rdi, 1              ; RDI = 1, descritor de arquivo (stdout)
    mov rsi, msg            ; RSI: Ponteiro para os dados a serem escritos
    mov rdx, tamanho        ; RDX: A quantidade de bytes a serem escritos
    syscall                 ; Executa a chamada de sistema

    mov rax, 60             ; RAX = 60, código para sys_exit
    mov rdi, 0              ; RDI = 0, código de status, significa sucesso
    syscall                 ; Executa a chamada de sistema
