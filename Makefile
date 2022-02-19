TARGET = kfs.iso

RM = rm -rf
MKDIR = mkdir -pv
CFLAGS = -m32 -ffreestanding -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs
CFLAGS += -Werror -Wextra -Wall

BIN = kernel
CFG = grub.cfg
SRC_PATH = src
ISO_PATH = iso
BOOT_PATH = $(ISO_PATH)/boot
GRUB_PATH = $(BOOT_PATH)/grub
SRC = $(wildcard */*.c) $(wildcard */*/*.c)
SRC_ASM = $(wildcard */*.s) $(wildcard */*/*.s)
OBJ = $(SRC:.c=.o)
OBJ += $(SRC_ASM:.s=.o)

all: linker iso
	@echo Finish!!!

%.o: %.c
	gcc $(CFLAGS) -g -c $< -o $@

%.o: %.s
	nasm -f elf32 -g $<

linker: $(SRC_PATH)/linker.ld $(OBJ)
	ld -m elf_i386 -T $(SRC_PATH)/linker.ld -o kernel $(OBJ)

iso: kernel
	$(MKDIR) $(GRUB_PATH)
	cp $(BIN) $(BOOT_PATH)
	cp $(SRC_PATH)/$(CFG) $(GRUB_PATH)
	grub-file --is-x86-multiboot $(BOOT_PATH)/$(BIN)
	grub-mkrescue -o $(TARGET) $(ISO_PATH)

clean:
	$(RM) $(OBJ) $(BIN) iso

fclean: clean
	$(RM) $(TARGET)

re: fclean all

run:
	yes | cp -iv kfs.iso /mnt/c/Users/umber/Desktop/.
	/mnt/c/Program\ Files/qemu/qemu-system-i386.exe -cdrom "C:\Users\umber\Desktop\kfs.iso"

debug:
	yes | cp -iv kfs.iso /mnt/c/Users/umber/Desktop/.
	gdb -q -x debug.gdb

.PHONY: all clean fclean re
