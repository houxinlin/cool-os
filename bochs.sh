#! /bin/sh
make clean os-image.bin
bochs -f bochsrc.txt
