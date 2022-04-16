
C_SOURCES = $(wildcard kernel/*.c drivers/keyboard/*.c drivers/ports/*.c drivers/vga/*.c cpu/*.c )
HEADERS = $(wildcard kernel/*.h  drivers/keyboard/*.h drivers/ports/*.h drivers/vga/*.h  cpu/*.h )
OBJ_FILES = ${C_SOURCES:.c=.o cpu/interrupt.o}

ASM = $(wildcard kernel/asm/*.asm)
ASM_OBJ = ${ASM:.asm=.o kernel/asm/a.o}

all: run

restart:
	clean
kernel.bin:  boot/kernel_entry.o ${OBJ_FILES} kernel/asm/utils.o cpu/kernel.o
	i686-elf-ld -m elf_i386 -o $@ -Ttext 0x1000 $^  --oformat binary

os-image.bin: boot/mbr.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -fda $<

echo: os-image.bin
	xxd $<

kernel.elf: boot/kernel_entry.o ${OBJ_FILES}
	i686-elf-ld -m elf_i386 -o $@ -Ttext 0x1000 $^

debug: os-image.bin kernel.elf
	qemu-system-i386 -s -S -fda os-image.bin -d guest_errors,int &
	i686-elf-gdb -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"
	
dd:os-image.bin
	sudo dd if=os-image.bin of=/dev/sdb

%.o: %.c ${HEADERS}
	i686-elf-gcc -g -m32 -ffreestanding -c $< -o $@ # -g for debugging

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

clean:
	$(RM) *.bin *.o *.dis *.elf
	$(RM) kernel/*.o
	$(RM) boot/*.o boot/*.bin
	$(RM) drivers/*.o
	$(RM) drivers/vga/*.o
	$(RM) drivers/keyboard/*.o
	$(RM) drivers/ports/*.o
	$(RM) cpu/*.o
	$(RM) kernel/asm/*.o
