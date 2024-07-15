# George Yacoubian
# Test project for MCD Rev 2

LINKER := linker.ld
#FILES := main crt0 morgio conversions morglib srec_parser commands registers mem
FILES := main crt0 morgio morglib conversions mem registers commands
#FILES := test crt0 morgio morglib
OUTPUT_FILES := $(foreach file, $(FILES), $(file).o)
CMD_SEPERATOR := &


all: gcc link clean done
# add -g to gcc for debugging
gcc:
	$(foreach file, $(FILES), m68k-elf-gcc -Wall $(file).c -m68000 -c -o $(file).o -ffreestanding;)
link:
	m68k-elf-ld -o morg.S68 -T $(LINKER) $(OUTPUT_FILES)
clean:
	$(foreach file, $(FILES), rm $(file).o $(CMD_SEPERATOR))
done:
	echo "ALL DONE :)"
run: 
	cmd.exe /c morg.bat
linkelf:
	m68k-elf-ld -o morg.elf -T linker.ld $(OUTPUT_FILES)
	echo "ALL DONE :)"
dbug: gcc
	m68k-elf-ld -o morg.elf -T elf_linker.ld $(OUTPUT_FILES)
	echo "ALL DONE :)"
	m68k-elf-gdb morg.elf
deploy:
	cmd.exe /c deploy.bat morg
qemubin:
	m68k-elf-ld -oformat=binary -o qmorg.bin -T linker.ld $(OUTPUT_FILES)

# To start Qemu Server
# qemu-system-m68k -M none -s -S -cpu m68000 -cdrom ./test/morg.iso

# To start gdbgui
# gdbgui -g m68k-elf-gdb

# Start Qemu Server
# Start gdbgui
# target remote localhost:1234

