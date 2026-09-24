nasm -f elf32 kernel/kernel.asm -o build/kasm.o
gcc -m32 -c kernel/kernel.c -o build/kc.o --freestanding

ld -m elf_i386 -T kernel/link.ld -o build/kernel build/kasm.o build/kc.o

cp build/kernel iso/boot/kernel

grub-mkrescue -o build/mipaOS32.iso iso

qemu-system-i386 -cdrom build/mipaOS32.iso -d int,cpu_reset -D qemu.log