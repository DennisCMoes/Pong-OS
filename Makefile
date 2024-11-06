# Output
SRC_OUTPUT 		:= src
BIN_OUTPUT 		:= bin

# Files
KERNEL_SRC 		:= ${SRC_OUTPUT}/kernel.c
KERNEL_ASM 		:= ${SRC_OUTPUT}/bootloader/kernel_entry.asm
LINKER_SCRIPT 	:= ${SRC_OUTPUT}/linker.ld

# Toolchain
CC				:= i686-elf-gcc
LD				:= i686-elf-ld
OBJCOPY			:= i686-elf-objcopy
NASM			:= nasm

# Output Files
KERNEL_ELF 		:= ${BIN_OUTPUT}/kernel.elf
KERNEL_BIN 		:= ${BIN_OUTPUT}/kernel.bin

all: compile qemu

compile: clean dirs bootloader kernel iso

clean:
	rm -rf ${BIN_OUTPUT}/*.bin ${BIN_OUTPUT}/*.elf boot.iso

dirs:
	mkdir -p ${BIN_OUTPUT}

bootloader: dirs
	nasm ${SRC_OUTPUT}/bootloader.asm -f bin -o ${BIN_OUTPUT}/bootloader.bin

kernel: dirs
	${CC} -m32 -ffreestanding -nostdlib -nostdinc -I src/kernel -msoft-float -c src/kernel.c -o ${BIN_OUTPUT}/kernel.o
	${NASM} -f elf32 ${KERNEL_ASM} -o ${BIN_OUTPUT}/entry.o
	${LD} -T ${LINKER_SCRIPT} -m elf_i386 -o ${KERNEL_ELF} ${BIN_OUTPUT}/entry.o ${BIN_OUTPUT}/kernel.o
	${OBJCOPY} -O binary ${KERNEL_ELF} ${KERNEL_BIN}
	rm bin/*.o bin/*.elf

iso:
	dd if=/dev/zero of=boot.iso bs=512 count=55
	dd if=${BIN_OUTPUT}/bootloader.bin of=boot.iso conv=notrunc seek=0 count=1
	dd if=${BIN_OUTPUT}/kernel.bin of=boot.iso conv=notrunc seek=1 count=54

qemu:
	qemu-system-i386 -drive format=raw,file=boot.iso -d int,pcall,cpu_reset --no-reboot --no-shutdown