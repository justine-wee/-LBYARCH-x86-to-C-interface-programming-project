nasm -f win64 asmgrayscale.asm
gcc -c cgrayscale.c -o cgrayscale.obj -m64
gcc cgrayscale.obj asmgrayscale.obj -o cgrayscale.exe -m64
cgrayscale.exe
pause