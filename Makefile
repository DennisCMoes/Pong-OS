SRC_OUTPUT 		:= src
BIN_OUTPUT 		:= bin

clean:
	rm -rf ${BIN_OUTPUT}/*.bin boot.iso

bootloader:
	mkdir -p ${BIN_OUTPUT}
	nasm ${SRC_OUTPUT}/bootloader.asm -f bin -o ${BIN_OUTPUT}/bootloader.bin

iso:
	dd if=/dev/zero of=boot.iso bs=512 count=11
	dd if=${BIN_OUTPUT}/bootloader.bin of=boot.iso bs=512 count=10 conv=notrunc

qemu:
	make bootloader
	make iso
	qemu-system-i386 -drive format=raw,file=boot.iso -d int,pcall,cpu_reset