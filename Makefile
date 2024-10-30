# Output
SRC_OUTPUT 		:= src
BIN_OUTPUT 		:= bin

# Files
KERNEL_SRC 		:= ${SRC_OUTPUT}/kernel.c
KERNEL_ASM 		:= ${SRC_OUTPUT}/kernel_entry.asm
LINKER_SCRIPT 	:= ${SRC_OUTPUT}/linker.ld

# Toolchain
CC				:= i686-elf-gcc
LD				:= i686-elf-ld
OBJCOPY			:= i686-elf-objcopy
NASM			:= nasm

# Output Files
KERNEL_ELF 		:= ${BIN_OUTPUT}/kernel.elf
KERNEL_BIN 		:= ${BIN_OUTPUT}/kernel.bin

clean:
	rm -rf ${BIN_OUTPUT}/*.bin ${BIN_OUTPUT}/*.elf boot.iso

bootloader:
	mkdir -p ${BIN_OUTPUT}
	nasm ${SRC_OUTPUT}/bootloader.asm -f bin -o ${BIN_OUTPUT}/bootloader.bin

kernel: # 1. Compile kernel C 2. Link kernel 3. Convert to binary
	mkdir -p ${BIN_OUTPUT}
	${CC} -m32 -ffreestanding -c ${KERNEL_SRC} -o ${BIN_OUTPUT}/kernel.o
	${NASM} -f elf32 ${KERNEL_ASM} -o ${BIN_OUTPUT}/entry.o

	${LD} -m elf_i386 -o ${KERNEL_ELF} -wl,-e,_start -Ttext 0x1000 ${BIN_OUTPUT}/entry.o ${BIN_OUTPUT}/kernel.o

	${OBJCOPY} -O binary ${KERNEL_ELF} ${KERNEL_BIN}

iso:
	dd if=/dev/zero of=boot.iso bs=512 count=16
	dd if=${BIN_OUTPUT}/bootloader.bin of=boot.iso bs=512 count=15 conv=notrunc
	dd if=${BIN_OUTPUT}/kernel.bin of=boot.iso bs=512 seek=15 conv=notrunc	

qemu:
	make bootloader
	make kernel
	make iso
	qemu-system-i386 -drive format=raw,file=boot.iso -d int,pcall,cpu_reset