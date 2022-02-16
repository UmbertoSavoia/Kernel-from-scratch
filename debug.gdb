add-symbol-file kernel
dashboard -layout assembly breakpoints source stack variables
set disassembly-flavor intel
target remote | /mnt/c/Program\ Files/qemu/qemu-system-i386.exe -cdrom "C:\Users\umber\Desktop\kfs.iso" -gdb stdio
