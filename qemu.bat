@echo off
start "QEMU" "E:\Program Files\qemu\qemu-system-i386.exe" -S -s -drive file=boot.bin,index=0,if=floppy,format=raw