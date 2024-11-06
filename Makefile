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

C_KERNEL_SRCS	:= $(wildcard $(SRC_DIR)/**/*.c)
C_KERNEL_OBJS	:= $(patsubst $(SRC_DIR)/%,$(BIN_DIR)/%, $(C_KERNEL_SRCS:.c=.o))

# Output Files
KERNEL_ELF 		:= ${BIN_DIR}/kernel.elf
KERNEL_OBJ		:= $(BIN_DIR)/kernel.o
KERNEL_BIN 		:= ${BIN_DIR}/kernel.bin
KERNEL_LIB		:= $(BIN_DIR)/kernel.a

all: compile qemu

compile: clean dirs bootloader kernel iso

clean:
	rm -rf bin boot.iso

dirs:
	mkdir -p ${BIN_DIR}

test:
	@echo $(C_KERNEL_OBJS)

bootloader: dirs
	nasm ${SRC_DIR}/bootloader.asm -f bin -o ${BIN_DIR}/bootloader.bin

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

stdlib: $(C_KERNEL_OBJS)
	$(CC) $(CFLAGS) -c $(KERNEL_SRC) -o $(KERNEL_OBJ)
	$(AR) $(ARFLAGS) $(KERNEL_LIB) $(KERNEL_OBJ) $(C_KERNEL_OBJS)

kernel: dirs
	${CC} ${CFLAGS} -c $(KERNEL_SRC) -o $(BIN_DIR)/kernel.o
	${NASM} ${NASM_FLAGS} ${KERNEL_ENTRY} -o $(BIN_DIR)/entry.o
	${LD} -T ${LINKER_SCRIPT} -m elf_i386 -o ${KERNEL_ELF} ${BIN_DIR}/entry.o ${BIN_DIR}/kernel.o
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_BIN)
	rm bin/*.o bin/*.elf

iso: bootloader kernel
	dd if=/dev/zero of=boot.iso bs=512 count=55
	dd if=${BIN_DIR}/bootloader.bin of=boot.iso conv=notrunc seek=0 count=1
	dd if=${BIN_DIR}/kernel.bin of=boot.iso conv=notrunc seek=1 count=54

qemu:
	qemu-system-i386 -drive format=raw,file=boot.iso -d int,pcall,cpu_reset --no-reboot --no-shutdown