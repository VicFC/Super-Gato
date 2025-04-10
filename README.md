# Super Gato
Una versión modificada del juego tradicional Gato. En lugar de jugar en una sola cuadrícula de 3x3, el Super Gato se compone de 9 tableros de Gato dispuestos en una gran cuadrícula de 3x3, formando un total tablero de 9x9.

## Detalles
La lógica del juego esta programada en lenguaje ensamblador NASM x32 y la interfaz gráfica esta hecha en Glade GTK. 

Se tienen las siguientes funcionalidades:
- Modo Jugador vs Jugador
- Modo Jugador vs Máquina (IA)
    * Utiliza un algoritmo no aleatorio y que se adapta a las jugadas del jugador
- Interfaz con Pixel Art


> [!NOTE]
Comandos de Compilación: 
```
nasm -f elf Proyecto2Arqui.asm
gcc -m32 -o MegaGato Proyecto2Arqui.o SuperGatoInterfaz.c $(pkg-config --cflags --libs gtk+-3.0) -rdynamic -z noexecstack
./MegaGato
```
