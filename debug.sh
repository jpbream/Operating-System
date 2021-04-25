#!/bin/sh

if [[ $# == 0 ]]; then
    objdump -S -EL -M i386,intel -D os.bin > debug.txt
elif [[ $# == 3 && $1 == "find" ]]; then
    objdump -S -EL -M i386,intel -D os.bin --start-address=$2 --stop-address=$3
fi