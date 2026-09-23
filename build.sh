nasm -f elf32 kernel/kernel.asm -o build/kasm.o
gcc -m32 -c kernel/kernel.c -o build/kc.o --freestanding

ld -m elf_i386 -T kernel/link.ld -o build/kernel build/kasm.o build/kc.o

qemu-system-i386 -kernel build/kernel -d int,cpu_reset -D qemu.log