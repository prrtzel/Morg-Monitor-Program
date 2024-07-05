# George Yacoubian
# Test project for MCD Rev 2

LINKER := linker.ld
FILES := main crt0 morgio conversions morglib srec_parser
OUTPUT_FILES := $(foreach file, $(FILES), $(file).o)
CMD_SEPERATOR := &


all: gcc link clean done
gcc:
	$(foreach file, $(FILES), m68k-elf-gcc -Wall -g $(file).c -c -m68000 -o $(file).o -ffreestanding $(CMD_SEPERATOR))
link:
	m68k-elf-ld -o morg.S68 -T $(LINKER) $(OUTPUT_FILES)
clean:
	$(foreach file, $(FILES), rm $(file).o $(CMD_SEPERATOR))
done:
	echo "ALL DONE :)"
run: 
	cmd.exe /c morg.bat
dbug: gcc
	m68k-elf-ld -o morg.elf -T elf_linker.ld $(OUTPUT_FILES)
	echo "ALL DONE :)"
	m68k-elf-gdb morg.elf