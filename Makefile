SRC_OUTPUT 		:= src
BIN_OUTPUT 		:= bin

clean:
	rm -rf ${BIN_OUTPUT}

bootloader:
	nasm ${SRC_OUTPUT}/bootloader.S -f bin -o ${BIN_OUTPUT}/bootloader.bin

iso:
	dd if=/dev/zero of=boot.iso bs=512 count=2880
	dd if=./${BIN_OUTPUT}/bootloader.bin of=boot.iso bs=512 count=1

qemu:
	make bootloader
	make iso
	qemu-system-i386 -drive format=raw,file=boot.iso