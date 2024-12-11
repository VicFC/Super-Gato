// Instituto Tecnologico de Costa Rica
// Campus Tecnologico Central Cartago
// Escuela de Ingenieria en Computacion
// IC3101 - Arquitectura de Computadoras
// Proyecto 2 - Super Gato
// Victor Fung y Giovanni Esquivel
// II-Semestre, 2024

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

// GTK Widgets
GtkWidget *window;
GtkWidget *window2;
GtkWidget *window3;
GtkWidget *fixed;
GtkWidget *fixed2;
GtkWidget *fixed3;

GtkBuilder *builder;
GtkGrid *grid;

GtkWidget *button1;
GtkWidget *button2;
GtkWidget *button3;
GtkWidget *button4;
GtkWidget *button5;
GtkWidget *button6;
GtkWidget *button7;
GtkWidget *button8;
GtkWidget *button9;
GtkWidget *button10;

GtkWidget *buttonJugador;
GtkWidget *buttonRobot;
GtkWidget *buttonInstrucciones;
GtkWidget *buttonRegresar;

GtkWidget *entry1;
GtkWidget *entry2;

GtkWidget *label;
GtkWidget *label2;
GtkWidget *label3;

// Variables Globales
const gchar* circulo = "O";
const gchar* equis = "X";
const gchar* espacio = "_";

char tmp1[1] = "O";
char tmp2[1] = "X";

const gchar* valUno = tmp1; // Para cambiar simbolos
const gchar* valDos = tmp2; // Para cambiar simbolos


int resultadoFinal = 0; // Quien gano? Nadie, J1, J2, Empate (0-1-2-3)

bool elegir;             // El primer jugador puede elegir donde empezar
int modo;                // Modo de juego, Jugador o Computadora (1-2)
int juego = -1;          // Area de juego, se elige al principio (0-8)
int casilla;             // En que de los 81 cuadrados se jugo (0-8)
int turno;               // Jugador1 o Jugador2/Maquina (1-2) 

int jugadaComputadora;   // Se guarda donde jugo la computadora (0-8)

// Funciones NASM
extern void resetGato();

extern int getterInd(int, int);
extern void setterInd(int, int, int);

extern int granGetter(int);

extern int checkGaneInd(int);
extern int granCheckGane();

extern int siguienteSubGato(int);

extern void turnoComputadora(int, int*);

extern int randNum();


// Funcion que imprime el Gato en consola (pruebas)
int printGato() {

    for (int i = 0; i <= 8; i++) {

        if (i == 3) {
            printf("\n");
        }
        if (i == 6) {
            printf("\n");
        }
        for (int j = 0; j <= 8; j++) {

            int value1 = getterInd(i, j);
            printf("%d", value1);
        }
        printf("|");
    }
    printf("\n\n");
    printf("ModoJuego: %d\n\n", modo);
    printf("Juego: %d\n\n", juego);
    printf("Casilla: %d\n\n", casilla);

}

int printGanes() { // Funciones que imprime los ganes (pruebas)

    for (int i = 0; i <= 8; i++) {

        if (i == 3) {
            printf("\n");
        }
        if (i == 6) {
            printf("\n");
        }
        int estado = checkGaneInd(i);
        printf("%i", estado); 
    } 
    printf("\n\n");
    resultadoFinal = granCheckGane();
    printf("RESULTADO FINAL:\n");
    printf("%d\n-----------------------------------------\n\n\n\n", resultadoFinal);

}



// Cambia el subgato en el que se esta jugando en Jugador vs Computadora
void cambiarJuego() {

    int gato = siguienteSubGato(casilla); 
    juego = gato;
    
}

// Cambia el turno, va alternado entre 1  y 2
void siguienteTurno() {

    if (turno == 1){ 
        turno = 2;
    }
    else { 
        turno = 1;
    }
}

// Revisar en que lugares se gano y los registra en memoria.
void revisarGanes() {

    for (int i = 0; i <= 8; i++) { // 0 - 8 para cada gato

        checkGaneInd(i);
    }
    resultadoFinal = granCheckGane();
}

// Muestra en pantalla el gato grande que es una GTK Grid
void iniciarGrid(GtkGrid *grid) {

    for (int gato = 0; gato < 9; ++gato) {
        for (int casilla = 0; casilla < 9; ++casilla) {
            int row = (gato / 3) * 3 + casilla / 3;
            int col = (gato % 3) * 3 + casilla % 3;

            int valor = getterInd(gato,casilla); // 0
            const gchar *text = g_strdup_printf("%d", valor); // convertir valor entero a const gchar*

            GtkWidget *label = gtk_label_new(text); // asignar texto

            gtk_widget_set_size_request(label, 33, 33); // tamano del label

            // anadir al grid
            gtk_grid_attach(grid, label, col, row, 1, 1);
        }
    }
}

// Movimiento de la computadora -> siguiente turno -> cambiar juego
void computadora() {

    turnoComputadora(juego, &jugadaComputadora);
    setterInd(juego, jugadaComputadora, 2);
    casilla = jugadaComputadora;
    siguienteTurno();
    cambiarJuego();
    printf("Computadora Jugo en: %d\n\n", jugadaComputadora);

}

// Se usa en el delay de 3 segundos para simular que la computadora esta pensando
gboolean delayComputadora() {
    printf("Pensando...\n\n");
    computadora();
    return FALSE; // Falso para que solo ocurra una vez por llamada
}

// Determinar si alguien ya gano
gboolean actualizarGanes(GtkWidget *) {

    if (resultadoFinal == 1) {
        gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Gano Jugador 1!");
    } 
    else if (resultadoFinal == 2) {
        gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Gano Jugador 2!");
    }
    else if (resultadoFinal == 3){
        gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Empate!");
    }
    else {
        return TRUE;
    }
    return TRUE;
}

// Actualiza el texto del gato segun lo que juegan los jugadores
gboolean actualizarGrid(GtkGrid *grid) {

    for (int gato = 0; gato < 9; ++gato) {
        for (int casilla = 0; casilla < 9; ++casilla) {
            int row = (gato / 3) * 3 + casilla / 3;
            int col = (gato % 3) * 3 + casilla % 3;

            // Valor de casilla
            int valor = getterInd(gato,casilla);

            // actual es un label
            GtkWidget *actual = gtk_grid_get_child_at(grid, col, row);

            if (valor == 1) {
                gtk_label_set_text(GTK_LABEL(actual), circulo); // Circulo o Equis valUno
            }
            else if (valor == 2) {
                gtk_label_set_text(GTK_LABEL(actual), equis); // Circulo o Equis valDos
            }
            else {
                gtk_label_set_text(GTK_LABEL(actual), espacio);
            }    
        }
    }
    return TRUE;
}

// Actualiza el texto diciendo que turnos es basado en la variable global turno
gboolean actualizarTurnos(GtkWidget *) {

    if (turno == 1) {
        gtk_label_set_text(GTK_LABEL(label2), (const gchar*) "Turno del Jugador 1");
    }
    else {
        gtk_label_set_text(GTK_LABEL(label2), (const gchar*) "Turno del Jugador 2");
    }
    return TRUE;

}

// Actualiza el label que dice si se esta eligiendo gato o casilla
gboolean actualizarElegir(GtkWidget *) {

    if (elegir) {
        gtk_label_set_text(GTK_LABEL(label3), (const gchar*) "Modo:\nElegir Gato");
    }   
    else {
        gtk_label_set_text(GTK_LABEL(label3), (const gchar*) "Modo:\nElegir Casilla");
    } 
    return true;
}

// Funcion para las jugadas del jugador
void movimientoJugador(int num) {
    printf("JUEGO ANTERIOR: %d\n\n", juego);
    int anterior = juego; // Lugar de juego anterior
    casilla = num; // Donde se esta jugando 0-8
    printf("CASILLA: %d\n\n", casilla);
    if (elegir) { // Eligiendo gato

        int estado = checkGaneInd(casilla); // Casilla valida?
        if (estado == 0) {
            juego = casilla; // cambia zona de juego

            const gchar *text = g_strdup_printf("Gato Seleccionado: %d", casilla);
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) text);

            elegir = false;
        }
        else {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Gato No Disponible");
        }

    }
    else {       // Eligiendo casilla

        int actual = getterInd(juego, num);
        if (anterior != casilla) { // No se esta jugando en casilla = gato
         
            if (actual == 0) { // Espacio vacio
                const gchar *text = g_strdup_printf("Casilla Seleccionada: %d", casilla);
                gtk_label_set_text(GTK_LABEL(label), (const gchar*) text);

                if (turno == 1) {
                    setterInd(juego, num, 1);
                }
                else {
                    setterInd(juego, num, 2);
                }
                siguienteTurno();

                int estado = checkGaneInd(casilla); // revisa si la casilla es valida o jugable
                if (estado == 0) {
                    juego = casilla;
                } else {
                    elegir = true;
                }
            }
            else {
                gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Ocupada");
            }
        }
        else {
            elegir = true;
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Mismo Gato");
        }

    }
    revisarGanes();
}

// Casilla 0
void on_button1_clicked(GtkButton *b) {

    if (modo == 1) {           // Jugador vs Jugador
        movimientoJugador(0);
    }
    else {     // Jugador vs Robot
        casilla = 0; 
        int actual = getterInd(juego, 0); 
        if (actual == 0) {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Elegida: 0");
            if (turno == 1) {
                setterInd(juego, 0, 1);
            }
            else {
                setterInd(juego, 0, 2);
            }
        cambiarJuego();
        siguienteTurno();
        }
        else {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Ocupada");
        }
        printf("DELAY...\n");
        g_timeout_add(2000, (GSourceFunc) delayComputadora, NULL);
        //computadora();
    }
    revisarGanes();
}   

// Casilla 1
void on_button2_clicked(GtkButton *b) {

    if (modo == 1) {           // Jugador vs Jugador
        movimientoJugador(1);
    }
    else {     // Jugador vs Robot
        casilla = 1; 
        int actual = getterInd(juego, 1); 
        if (actual == 0) {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Elegida: 1");
            if (turno == 1) {
                setterInd(juego, 1, 1);
            }
            else {
                setterInd(juego, 1, 2);
            }
        cambiarJuego();
        siguienteTurno();
        }
        else {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Ocupada");
        }
        printf("DELAY...\n");
        g_timeout_add(2000, (GSourceFunc) delayComputadora, NULL);
        //computadora();
    }
    revisarGanes();
} 

// Casilla 2
void on_button3_clicked(GtkButton *b) {

    if (modo == 1) {           // Jugador vs Jugador
        movimientoJugador(2);
    }
    else {     // Jugador vs Robot
        casilla = 2; 
        int actual = getterInd(juego, 2); 
        if (actual == 0) {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Elegida: 2");
            if (turno == 1) {
                setterInd(juego, 2, 1);
            }
            else {
                setterInd(juego, 2, 2);
            }
        cambiarJuego();
        siguienteTurno();
        }
        else {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Ocupada");
        }
        printf("DELAY...\n");
        g_timeout_add(2000, (GSourceFunc) delayComputadora, NULL);
        //computadora();
    }
    revisarGanes();
} 

// Casilla 3
void on_button4_clicked(GtkButton *b) {

    if (modo == 1) {           // Jugador vs Jugador
        movimientoJugador(3);
    }
    else {     // Jugador vs Robot
        casilla = 3; 
        int actual = getterInd(juego, 3); 
        if (actual == 0) {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Elegida: 3");
            if (turno == 1) {
                setterInd(juego, 3, 1);
            }
            else {
                setterInd(juego, 3, 2);
            }
        cambiarJuego();
        siguienteTurno();
        }
        else {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Ocupada");
        }
        printf("DELAY...\n");
        g_timeout_add(2000, (GSourceFunc) delayComputadora, NULL);
        //computadora();
    }
    revisarGanes();
} 

// Casilla 4
void on_button5_clicked(GtkButton *b) {

    if (modo == 1) {           // Jugador vs Jugador
        movimientoJugador(4);
    }
    else {     // Jugador vs Robot
        casilla = 4; 
        int actual = getterInd(juego, 4); 
        if (actual == 0) {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Elegida: 4");
            if (turno == 1) {
                setterInd(juego, 4, 1);
            }
            else {
                setterInd(juego, 4, 2);
            }
        cambiarJuego();
        siguienteTurno();
        }
        else {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Ocupada");
        }
        printf("DELAY...\n");
        g_timeout_add(2000, (GSourceFunc) delayComputadora, NULL);
        //computadora();
    }
    revisarGanes();
}  

// Casilla 5
void on_button6_clicked(GtkButton *b) {

    if (modo == 1) {           // Jugador vs Jugador
        movimientoJugador(5);
    }
    else {     // Jugador vs Robot
        casilla = 5; 
        int actual = getterInd(juego, 5); 
        if (actual == 0) {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Elegida: 5");
            if (turno == 1) {
                setterInd(juego, 5, 1);
            }
            else {
                setterInd(juego, 5, 2);
            }
        cambiarJuego();
        siguienteTurno();
        }
        else {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Ocupada");
        }
        printf("DELAY...\n");
        g_timeout_add(2000, (GSourceFunc) delayComputadora, NULL);
        //computadora();
    }
    revisarGanes();
} 

// Casilla 6
void on_button7_clicked(GtkButton *b) {

    if (modo == 1) {           // Jugador vs Jugador
        movimientoJugador(6);
    }
    else {     // Jugador vs Robot
        casilla = 6; 
        int actual = getterInd(juego, 6); 
        if (actual == 0) {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Elegida: 6");
            if (turno == 1) {
                setterInd(juego, 6, 1);
            }
            else {
                setterInd(juego, 6, 2);
            }
        cambiarJuego();
        siguienteTurno();
        }
        else {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Ocupada");
        }
        printf("DELAY...\n");
        g_timeout_add(2000, (GSourceFunc) delayComputadora, NULL);
        //computadora();
    }
    revisarGanes();
} 

// Casilla 7
void on_button8_clicked(GtkButton *b) {

    if (modo == 1) {           // Jugador vs Jugador
        movimientoJugador(7);
    }
    else {     // Jugador vs Robot
        casilla = 7; 
        int actual = getterInd(juego, 7); 
        if (actual == 0) {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Elegida: 7");
            if (turno == 1) {
                setterInd(juego, 7, 1);
            }
            else {
                setterInd(juego, 7, 2);
            }
        cambiarJuego();
        siguienteTurno();
        }
        else {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Ocupada");
        }
        printf("DELAY...\n");
        g_timeout_add(2000, (GSourceFunc) delayComputadora, NULL);
        //computadora();
    }
    revisarGanes();
} 

// Casilla 8
void on_button9_clicked(GtkButton *b) {

    if (modo == 1) {           // Jugador vs Jugador
        movimientoJugador(8);
    }
    else {     // Jugador vs Robot
        casilla = 8; 
        int actual = getterInd(juego, 8); 
        if (actual == 0) {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Elegida: 8");
            if (turno == 1) {
                setterInd(juego, 8, 1);
            }
            else {
                setterInd(juego, 8, 2);
            }
        cambiarJuego();
        siguienteTurno();
        }
        else {
            gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Casilla Ocupada");
        }
        printf("DELAY...\n");
        g_timeout_add(2000, (GSourceFunc) delayComputadora, NULL);
        //computadora();
    }
    revisarGanes();
} 

// Boton de pruebas
void on_button10_clicked(GtkButton *b) {
    gtk_label_set_text(GTK_LABEL(label), (const gchar*) "Imprimiendo Gato");

    printGato();
    printGanes();

}

// Boton para empezar en modo jugador vs jugador
void on_buttonJugador_clicked(GtkButton *b) {

    juego = -1;
    GtkWidget *juego = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    GtkWidget *inicio = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));
    
    gtk_widget_hide(window2);
    gtk_widget_show_all(window);

    modo = 1; // Jugador vs Jugador
    elegir = true;
    printf("MODO DE JUEGO: %d\n\n\n", modo); // BORRAR
    printf("ZONA DE JUEGO: %d\n\n\n", juego); // BORRAR

}

// Boton para empezar en modo jugador vs robot
void on_buttonRobot_clicked(GtkButton *b) {

    juego = 4;
    GtkWidget *juego = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    GtkWidget *inicio = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));
    
    gtk_widget_hide(window2);
    gtk_widget_show_all(window);

    modo = 2; // Jugador vs Computadora
    turno = 1;
    elegir = false;
    printf("MODO DE JUEGO: %d\n\n\n", modo); // BORRAR
    printf("ZONA DE JUEGO: %d\n\n\n", juego); // BORRAR
}

// Boton para ver las instrucciones
void on_buttonInstrucciones_clicked(GtkButton *b) {

    GtkWidget *juego = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    GtkWidget *inicio = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));
    GtkWidget *instrucciones = GTK_WIDGET(gtk_builder_get_object(builder, "window3"));
    
    gtk_widget_hide(window2);
    gtk_widget_show_all(window3);

    
}
// Cambiar simbolo J1
void on_entry1_changed(GtkEntry *e) {

    sprintf(tmp1, "%s", gtk_entry_get_text(e));
}

// Cambiar simbolo J2
void on_entry2_changed(GtkEntry *e) {

    sprintf(tmp2, "%s", gtk_entry_get_text(e));
}


// Boton para ver regresarse
void on_buttonRegresar_clicked(GtkButton *b) {

    GtkWidget *juego = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    GtkWidget *inicio = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));
    GtkWidget *instrucciones = GTK_WIDGET(gtk_builder_get_object(builder, "window3"));
    
    gtk_widget_hide(window3);
    gtk_widget_show_all(window2);

}


int main(int argc, char *argv[]) {

    int value1, value2, value3, value4, value5, value6, value7, value8, value9;
    int i, j;

    resetGato();
/*
    setterInd(3,0,1);
    setterInd(3,1,2);
    setterInd(3,2,1);
    setterInd(3,3,1);
    setterInd(3,4,2);
    setterInd(3,5,1);
    setterInd(3,6,2);
    setterInd(3,7,1);
    setterInd(3,8,2);
*/
    value1 = checkGaneInd(0);
    value2 = checkGaneInd(1);
    value3 = checkGaneInd(2);
    value4 = checkGaneInd(3);
    value5 = checkGaneInd(4);
    value6 = checkGaneInd(5);
    value7 = checkGaneInd(6);
    value8 = checkGaneInd(7);
    value9 = checkGaneInd(8);

    printf("\nCHECK GANE IND: 0:%d, 1:%d, 2:%d, 3:%d, 4:%d, 5:%d, 6:%d, 7:%d, 8:%d \n", value1, value2, value3, value4, value5, value6, value7, value8, value9);

    value1 = siguienteSubGato(0);
    value2 = siguienteSubGato(1);
    value3 = siguienteSubGato(2);
    value4 = siguienteSubGato(3);
    value5 = siguienteSubGato(4);
    value6 = siguienteSubGato(5);
    value7 = siguienteSubGato(6);
    value8 = siguienteSubGato(7);
    value9 = siguienteSubGato(8);


    printf("\nSIGUIENTES SUB-GATOS: 0:%d, 1:%d, 2:%d, 3:%d, 4:%d, 5:%d, 6:%d, 7:%d, 8:%d \n", value1, value2, value3, value4, value5, value6, value7, value8, value9);

    // ------------------------------------------------------------------------------------------------------------------------ //

    gtk_init(&argc, &argv);
    
    // Abrir archivo de Glade
    builder = gtk_builder_new_from_file("SuperGato.glade");

    // Punteros a widgets
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    window2 = GTK_WIDGET(gtk_builder_get_object(builder, "window2"));
    window3 = GTK_WIDGET(gtk_builder_get_object(builder, "window3"));
    fixed = GTK_WIDGET(gtk_builder_get_object(builder, "fixed"));
    fixed2 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed2"));
    fixed3 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed3"));

    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    button2 = GTK_WIDGET(gtk_builder_get_object(builder, "button2"));
    button3 = GTK_WIDGET(gtk_builder_get_object(builder, "button3"));
    button4 = GTK_WIDGET(gtk_builder_get_object(builder, "button4"));
    button5 = GTK_WIDGET(gtk_builder_get_object(builder, "button5"));
    button6 = GTK_WIDGET(gtk_builder_get_object(builder, "button6"));
    button7 = GTK_WIDGET(gtk_builder_get_object(builder, "button7"));
    button8 = GTK_WIDGET(gtk_builder_get_object(builder, "button8"));
    button9 = GTK_WIDGET(gtk_builder_get_object(builder, "button9"));
    button10 = GTK_WIDGET(gtk_builder_get_object(builder, "button10"));

    buttonJugador = GTK_WIDGET(gtk_builder_get_object(builder, "buttonJugador"));
    buttonRegresar = GTK_WIDGET(gtk_builder_get_object(builder, "buttonRegresar"));
    buttonRobot = GTK_WIDGET(gtk_builder_get_object(builder, "buttonRobot"));
    buttonInstrucciones = GTK_WIDGET(gtk_builder_get_object(builder, "buttonInstrucciones"));

    label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
    label2 = GTK_WIDGET(gtk_builder_get_object(builder, "label2"));
    label3 = GTK_WIDGET(gtk_builder_get_object(builder, "label3"));
    grid = GTK_GRID(gtk_builder_get_object(builder, "grid"));

    entry1 = GTK_WIDGET(gtk_builder_get_object(builder, "entry1"));
    entry2 = GTK_WIDGET(gtk_builder_get_object(builder, "entry2"));

    // Conectar senales para acciones
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window2, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(window3, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    g_signal_connect(button1, "clicked", G_CALLBACK(on_button1_clicked), NULL);
    g_signal_connect(button2, "clicked", G_CALLBACK(on_button2_clicked), NULL);
    g_signal_connect(button3, "clicked", G_CALLBACK(on_button3_clicked), NULL);
    g_signal_connect(button4, "clicked", G_CALLBACK(on_button4_clicked), NULL);
    g_signal_connect(button5, "clicked", G_CALLBACK(on_button5_clicked), NULL);
    g_signal_connect(button6, "clicked", G_CALLBACK(on_button6_clicked), NULL);
    g_signal_connect(button7, "clicked", G_CALLBACK(on_button7_clicked), NULL);
    g_signal_connect(button8, "clicked", G_CALLBACK(on_button8_clicked), NULL);
    g_signal_connect(button9, "clicked", G_CALLBACK(on_button9_clicked), NULL);
    g_signal_connect(button10, "clicked", G_CALLBACK(on_button10_clicked), NULL);

    g_signal_connect(buttonRobot, "clicked", G_CALLBACK(on_buttonRobot_clicked), NULL);
    g_signal_connect(buttonJugador, "clicked", G_CALLBACK(on_buttonJugador_clicked), NULL);
    g_signal_connect(buttonInstrucciones, "clicked", G_CALLBACK(on_buttonInstrucciones_clicked), NULL);
    g_signal_connect(buttonRegresar, "clicked", G_CALLBACK(on_buttonRegresar_clicked), NULL);

    g_signal_connect(entry1, "changed", G_CALLBACK(on_entry1_changed), NULL);
    g_signal_connect(entry2, "changed", G_CALLBACK(on_entry2_changed), NULL);

    // Decidir quien va primero aleatoriamente
    int empezar = randNum();
    if (empezar % 2 == 0) {
        turno = 1;    
    }
    else {
        turno = 2;
    }
    printf("Primer Turno: %d\n\n", turno);

    // Grid
    gtk_widget_set_size_request(GTK_WIDGET(grid), 297, 297);
    iniciarGrid(grid);

    // Timer - Turnos
    g_timeout_add(100, (GSourceFunc) actualizarTurnos, label2);

    // Timer - Ganar
    g_timeout_add_seconds(1, (GSourceFunc) actualizarGanes, label3);

    // Timer - Grid
    g_timeout_add(100, (GSourceFunc) actualizarGrid, grid);

    // Timer - Computadora
    g_timeout_add(100, (GSourceFunc) actualizarElegir, label3);

    // Tamanos y apariencia de pantallas
    gtk_window_set_default_size(GTK_WINDOW(window), 900, 500);
    gtk_window_set_default_size(GTK_WINDOW(window2), 900, 500);
    gtk_window_set_default_size(GTK_WINDOW(window3), 900, 500);
    gtk_widget_hide(window);
    gtk_widget_hide(window3);
    gtk_widget_show_all(window2);

    // GTK main loop
    gtk_main();

    return EXIT_SUCCESS;
}
