#!/bin/bash



echo "padding u-boot-spl.binto 8192:"
dd ibs=1 count=8192 if=/dev/zero of=paddedFile.bin
dd if=spl/u-boot-spl.bin of=paddedFile.bin conv=notrunc


echo "merge the  padded u-boot-spl.bin and u-boot.bin"
cat paddedFile.bin > nand.image
cat u-boot.bin >> nand.image


rm paddedFile.bin
