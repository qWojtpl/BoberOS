; boot.asm
BITS 16
ORG 0x7C00

KERNEL_OFFSET equ 0x1000

start:
    mov ah, 0x0E
    mov al, 'A'
    int 0x10
    mov [BOOT_DRIVE], dl

    mov bp, 0x9000
    mov sp, bp

    mov bx, KERNEL_OFFSET
    mov dh, 20
    mov dl, [BOOT_DRIVE]
    call disk_load

    call switch_to_pm

    jmp $

disk_load:
    pusha
    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov cl, 0x02
    mov dh, 0x00
    int 0x13
    jc disk_error
    popa
    mov ah, 0x0E
    mov al, 'B'
    int 0x10
    ret

disk_error:
    jmp $

; --- GDT ---
gdt_start:
gdt_null: dq 0x0
gdt_code: dq 0x00CF9A000000FFFF
gdt_data: dq 0x00CF92000000FFFF
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

switch_to_pm:
    cli
    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp 0x08:init_pm

[bits 32]
init_pm:

    mov esp, 0x90000
    mov ax, 0x10
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call KERNEL_OFFSET

    jmp $

BOOT_DRIVE db 0

times 510 - ($ - $$) db 0
dw 0xAA55

