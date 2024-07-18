# George Yacoubian
# Morg Monitor Program

FILES := main crt0 morgio conversions morglib commands registers mem srec_parser

OUTPUT_FILES := $(foreach file, $(FILES), $(file).o)
CMD_SEPERATOR := &


all: gcc link clean done
# add -g to gcc for debugging
gcc:
	$(foreach file, $(FILES), m68k-elf-gcc -Wall src/$(file).c -m68000 -c -o $(file).o -ffreestanding;)
link:
	m68k-elf-ld -o bin/morg.S68 -T etc/linker.ld $(OUTPUT_FILES)
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
	cd scripts; cmd.exe /c deploy.bat morg; cd ..