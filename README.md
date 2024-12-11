Videojuego en NASM + C

Esto es, el tradicional juego de 3x3 celdas llamado Gato, pero es un Súper Gato formado por 3x3 juegos de Gato completos, es decir 9x9 celdas. En el juego tradicional de gato, gana el jugador que logre colocar 3 elementos iguales, tales como O o X, en línea vertical, horizontal o diagonal. En el súper gato, ganará el jugador que logre ganar 3 juegos de gato igualmente en línea vertical, horizontal o diagonal.

> [!NOTE]
Comandos de Compilación: 
```
nasm -f elf Proyecto2Arqui.asm
gcc -m32 -o MegaGato Proyecto2Arqui.o SuperGatoInterfaz.c $(pkg-config --cflags --libs gtk+-3.0) -rdynamic -z noexecstack
./MegaGato
```
