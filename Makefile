# Output
SRC_DIR 		:= src
BIN_DIR			:= bin
KERNEL_DIR		:= $(SRC_DIR)/kernel

# Flags
CFLAGS			:= -m32 -ffreestanding -nostdlib -nostdinc -Wall -I src/kernel -msoft-float
ARFLAGS			:= rcs
NASM_FLAGS		:= -f elf32

# Toolchain
CC				:= i686-elf-gcc
LD				:= i686-elf-ld
OBJCOPY			:= i686-elf-objcopy
AR				:= i686-elf-ar
NASM			:= nasm

# Files
KERNEL_SRC 		:= $(SRC_DIR)/kernel.c
KERNEL_ENTRY	:= $(KERNEL_DIR)/entry/kernel_entry.asm
LINKER_SCRIPT 	:= ${SRC_DIR}/linker.ld

C_KERNEL_SRCS	:= $(shell find src -name "*.c")
ASM_KERNEL_SRC 	:= $(filter-out src/bootloader.asm src/bootloader/%, $(shell find src -name "*.asm"))

C_KERNEL_OBJS	:= $(patsubst $(SRC_DIR)/%,$(BIN_DIR)/%, $(C_KERNEL_SRCS:.c=.o))
ASM_KERNEL_OBJS	:= $(patsubst $(SRC_DIR)/%,$(BIN_DIR)/%, $(ASM_KERNEL_SRC:.asm=.o))

# Output Files
KERNEL_ELF 		:= ${BIN_DIR}/kernel.elf
KERNEL_OBJ		:= $(BIN_DIR)/kernel.o
KERNEL_BIN 		:= ${BIN_DIR}/kernel.bin
KERNEL_LIB		:= $(BIN_DIR)/kernel.a

all: compile qemu

compile: clean dirs bootloader kernel iso

clean:
	rm -rf bin

dirs:
	mkdir -p ${BIN_DIR}

test:
	@echo $(C_KERNEL_SRCS)

bootloader: dirs
	nasm ${SRC_DIR}/bootloader.asm -f bin -o ${BIN_DIR}/bootloader.bin

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	$(NASM) $(NASM_FLAGS) $< -o $@

stdlib: $(ASM_KERNEL_OBJS) $(C_KERNEL_OBJS)
	$(AR) $(ARFLAGS) $(KERNEL_LIB) $(ASM_KERNEL_OBJS) $(C_KERNEL_OBJS)

kernel: dirs bootloader stdlib
	$(LD) -T $(LINKER_SCRIPT) -m elf_i386 -o $(KERNEL_ELF) $(KERNEL_LIB)
	$(OBJCOPY) -I elf32-i386 -O binary $(KERNEL_ELF) $(KERNEL_BIN)
	find bin -mindepth 1 ! -name 'bootloader.bin' ! -name 'kernel.bin' -exec rm -rf {} +

iso: bootloader kernel
	dd if=/dev/zero of=boot.iso bs=512 count=55
	dd if=${BIN_DIR}/bootloader.bin of=boot.iso conv=notrunc seek=0 count=1
	dd if=${BIN_DIR}/kernel.bin of=boot.iso conv=notrunc seek=1 count=54

qemu:
	qemu-system-i386 -drive format=raw,file=boot.iso -d int,pcall,cpu_reset --no-reboot --no-shutdown