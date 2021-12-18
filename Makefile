TARGET = kfs.iso

RM = rm -rf
MKDIR = mkdir -pv
CFLAGS = -m32 -fno-builtin -fno-exceptions -fno-stack-protector -nostdlib -nodefaultlibs

BIN = kernel
CFG = grub.cfg
SRC_PATH = src
ISO_PATH = iso
BOOT_PATH = $(ISO_PATH)/boot
GRUB_PATH = $(BOOT_PATH)/grub
SRC = $(wildcard $(SRC_PATH)/*.c)
OBJ = $(SRC:.c=.o)
OBJ += $(SRC_PATH)/boot.o

all: bootloader linker iso
	@echo Finish!!!

bootloader: $(SRC_PATH)/boot.s
	nasm -f elf32 $(SRC_PATH)/boot.s

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

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
	/mnt/c/Program\ Files/qemu/qemu-system-x86_64.exe -cdrom "C:\Users\umber\Desktop\kfs.iso"

.PHONY: all clean fclean re