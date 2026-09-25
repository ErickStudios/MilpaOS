dd if=/dev/zero of=build/disk.img bs=1M count=10
mkfs.fat -F 16 build/disk.img -n MIPA

echo 'hola MIPA' > /tmp/HOLA.TXT

for src in progs/*.c; do
    name=$(basename $src)
    fname=$(echo $name | tr 'a-z' 'A-Z' | cut -c1-8)
    aname="${fname%.*}"
    echo " $src -> $aname.BIN"

    gcc -m32 -ffreestanding -nostdlib -fPIE -O2 -c $src -o build/$name.o
    ld -m elf_i386 -T progs/app.ld -o build/$name.elf build/$name.o
    objcopy -O binary build/$name.elf /tmp/$aname.BIN

    mcopy -i build/disk.img /tmp/$aname.BIN ::/
    cp /tmp/$aname.BIN outprg/$aname.BIN
done

mcopy -i build/disk.img /tmp/HOLA.TXT ::/
mdir -i build/disk.img ::/

nasm -f elf32 kernel/kernel.asm -o build/kasm.o
gcc -m32 -c kernel/kernel.c -o build/kc.o --freestanding -Os

ld -m elf_i386 -T kernel/link.ld -o build/kernel build/kasm.o build/kc.o

cp build/kernel iso/boot/kernel

grub-mkrescue -o build/mipaOS32.iso iso

qemu-system-i386 -cdrom build/mipaOS32.iso -hda build/disk.img -d int,cpu_reset -D qemu.log -boot d
