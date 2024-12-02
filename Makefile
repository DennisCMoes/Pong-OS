NASM=nasm
CC=i686-elf-gcc
LD=i686-elf-ld

CCFLAGS= -m32 -std=c11 -O2 -g -Wall -Wextra -Wpedantic -Wstrict-aliasing
CCFLAGS+=-Wno-pointer-arith -Wno-unused-parameter
CCFLAGS+=-nostdlib -nostdinc -ffreestanding -fno-pie -fno-stack-protector
CCFLAGS+=-fno-builtin-function -fno-builtin

BOOTSECT_SRCS=src/stage0.asm
BOOTSECT_OBJS=$(BOOTSECT_SRCS:.asm=.o)

KERNEL_C_SRCS=	$(wildcard src/*.c)
KERNEL_ASM_SRCS=$(filter-out $(BOOTSECT_SRCS), $(wildcard src/*.asm))
KERNEL_OBJS=	$(KERNEL_ASM_SRCS:.asm=.o) $(KERNEL_C_SRCS:.c=.o)

BOOTLOADER=	bootloader.bin
KERNEL=		kernel.bin
ISO=		boot.iso

all: clean

clean:
	rm -f ./**/*.o
	rm -f ./**/*.elf
	rm -f ./**/*.bin

dirs:
	mkdir -p bin

%.o: %.asm
	$(NASM) -f elf32 -o $@ $<

%.o: %.c
	$(CC) -o $@ -c $< $(CCFLAGS)

bootloader: 
	nasm -f bin $(BOOTSECT_SRCS) -o ./bin/bootloader.bin

kernel: $(KERNEL_OBJS)
	$(LD) -o ./bin/$(KERNEL) $^ -Tsrc/linker.ld

iso: bootloader kernel
	dd if=/dev/zero of=boot.iso bs=512 count=2880
	dd if=./bin/$(BOOTLOADER) of=boot.iso conv=notrunc bs=512 seek=0 count=1
	dd if=./bin/$(KERNEL) of=boot.iso conv=notrunc bs=512 seek=1 count=2048

qemu: iso
	qemu-system-i386 -drive format=raw,file=boot.iso -d int,pcall,cpu_reset,guest_errors --no-reboot --no-shutdown -serial stdio
