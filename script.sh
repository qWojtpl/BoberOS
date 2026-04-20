#!/bin/sh
rm os.vdi
echo "Compiling bootloader..."
nasm -f bin boot.asm -o boot.bin

echo "Compiling kernel..."

for file in drivers/*.c kernel/*.c; do
    filename=$(basename "$file" .c)
    gcc -m32 -nostdlib -fno-pie -fno-stack-protector -ffreestanding \
        -fno-asynchronous-unwind-tables -Iinclude -c "$file" -o "build/$filename.o"
done

ld -m elf_i386 -o kernel.bin -T linker.ld --oformat binary ./build/*.o -no-pie

echo "Creating image..."
dd if=/dev/zero of=os.img bs=512 count=2880
dd if=boot.bin of=os.img conv=notrunc
dd if=kernel.bin of=os.img bs=512 seek=1 conv=notrunc

echo "Creating VDI file..."
VBoxManage convertfromraw os.img os.vdi --format VDI
echo "Changing VDI UUID..."
VBoxManage internalcommands sethduuid os.vdi 798bdaa1-7906-4052-9cc6-b796e6a20651

echo "Cleaning up..."
rm os.img
rm kernel.bin
rm boot.bin