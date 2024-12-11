> [!NOTE]
Comandos de Compilación: 
```
nasm -f elf Proyecto2Arqui.asm
gcc -m32 -o MegaGato Proyecto2Arqui.o SuperGatoInterfaz.c $(pkg-config --cflags --libs gtk+-3.0) -rdynamic -z noexecstack
./MegaGato
```
