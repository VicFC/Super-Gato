;Supergato Logica                    Proyecto2Arqui.asm
;
;    Objective: Provide various functions to be used by the C interface in the "Supergato" game
;    Input/Output: Varies
;
global resetGato, getterInd, setterInd, granGetter, checkGaneInd, granCheckGane, siguienteSubGato, randNum, turnoComputadora

;-----------------------------------------------------------------------
; Función pone a todas las casillas del gato en 0 
; 0 = vacio
; 1 = circulo  "O"
; 2 = equis    "X"
; *Ejecutar siempre al inicio del programa, de lo contrario no se setean
; las variables y el programa no corre correctamente
;-----------------------------------------------------------------------
resetGato:
    enter   0,0
    mov     ecx, 81
    mov     edx, gatosInd

;Loopea por cada bit en gatosInd, seteando cada uno en 0
resetGatoAux1:
    mov     byte [edx], 0
    inc     edx
    loop    resetGatoAux1
    mov     edx, granGato
    mov     ecx, 9

;Ahora loopea por granGato
resetGatoAux2:
    mov     byte [edx], 0
    inc     edx
    loop    resetGatoAux2
    sub     edx, edx
    leave
    ret

;-----------------------------------------------------------------------
; Retorna el estado de una casilla específica. Recibe 2 enteros:
; int 1 = sub-gato
; int 2 = casilla en dicho sub-gato
; Retorna:
; Numero del 0 al 2 (Estado de la casilla)
;-----------------------------------------------------------------------
getterInd:
    enter   0,0
    sub     eax, eax
    sub     ecx, ecx
    sub     edx, edx
    mov     ecx, [ebp+8]
    mov     edx, gatosInd
    cmp     ecx, 0
    je      getterIndAux

;Le anade a la direccion de edx (gatosInd) 9 por cada posicion (int 1), esto para llegar al subgato indicado
getterIndLoop:
    add     edx, 9
    loop    getterIndLoop
getterIndAux:                ;edx ahora apunta al subgato correcto
    mov     ecx, [ebp+12]
    add     edx, ecx         ;edx ahora apunta al cuadro correcto del subgato
    mov     al, byte [edx]
    leave
    ret

;-----------------------------------------------------------------------
; Asigna un estado a una casilla específica. Recibe 3 enteros:
; int 1 = sub-gato
; int 2 = casilla en dicho sub-gato
; int 3 = valor asignado
;-----------------------------------------------------------------------
setterInd:
    enter   0,0
    sub     ecx, ecx
    sub     edx, edx
    mov     ecx, [ebp+8]
    mov     edx, gatosInd
    cmp     ecx, 0
    je      setterIndAux

;Le anade a la direccion de edx (gatosInd) 9 por cada posicion (int 1), esto para llegar al subgato indicado
setterIndLoop:
    add     edx, 9
    loop    setterIndLoop
setterIndAux:                ;edx ahora apunta al subgato correcto
    mov     ecx, [ebp+12]
    add     edx, ecx         ;edx ahora apunta al cuadro correcto del subgato
    sub     ecx, ecx
    mov     cl, byte [ebp+16]
    mov     byte [edx], cl    ;Se cambia el valor del cuadro a int 3 (1 o 2)
    leave
    ret

;-----------------------------------------------------------------------
; Retorna el estado de un sub-gato específico. Recibe 1 entero:
; int 1 = numero de sub-gato
; Retorna:
; Numero del 0 al 2 (Estado del gato)
; *Nuestro programa no tiene empates en los subgatos
;-----------------------------------------------------------------------
granGetter:
    enter   0,0
    sub     eax, eax
    sub     edx, edx
    sub     ecx, ecx
    mov     edx, granGato
    mov     ecx, [ebp+8]
    add     edx, ecx        ;edx ahora apunta al estado del subgato correcto
    mov     al, byte [edx]
    leave
    ret

;-----------------------------------------------------------------------
; Retorna el estado de un sub-gato específico. Recibe 1 entero:
; int 1 = numero de sub-gato
; *SOLO SE USA INTERNAMENTE, C NO TIENE ACCESO A ESTA FUNCION
;-----------------------------------------------------------------------
granSetter: 
    sub     edx, edx
    sub     ecx, ecx
    mov     edx, granGato
    mov     ecx, [ebp+8]
    add     edx, ecx        ;edx ahora apunta al estado del subgato correcto
    sub     ecx, ecx
    mov     cl, al
    mov     byte [edx], cl  ;Se reemplaza el valor por el indicado en al
    leave
    ret

;-----------------------------------------------------------------------
; Revisa si hay un ganador en el rango indicado y retorna el resultado:
; Para checkGaneInd, recibe 1 entero:
; int 1 = sub-gato
; Retorna:
; Numero del 0 al 3 = 1. Gane jugador 1 / 2. Gane Jugador 2 o Computadora / 3. Empate / 0. Sin decidir 
; * checkGaneInd nunca va a dar empate, por lo que no se considera el 3
;-----------------------------------------------------------------------

;Si se llama granCheckGane, ubica edx en granGato, para chequear por un gane en el gato general
granCheckGane:
    enter   0,0
    sub     si,si
    sub     ecx, ecx
    sub     edx, edx
    mov     edx, granGato
    jmp     checkGane

;Si se llama checkGaneInd, ubica edx en el subgato indicado, para chequear por un gane en este
checkGaneInd:
    enter   0,0
    sub     si,si
    mov     si,1
    sub     ecx, ecx
    sub     edx, edx
    mov     ecx, [ebp+8]
    mov     edx, gatosInd
    cmp     ecx, 0
    je      checkGane
checkGaneIndLoop:
    add     edx, 9
    loop    checkGaneIndLoop    

;edx ahora apunta al subgato correcto (checkGaneInd) o al gato general (granCheckGane)
checkGane:                
    mov     edi,edx
    sub     eax,eax
    mov     ecx, 3

;Compara los 3 cuadros de cada linea horizontal para ver si coinciden con al y cl
checkGaneHor:
    push    ecx
    mov     al, byte [edx]
    cmp     al, 0            ;Si hay un cero en la linea, se brincan las otras comparaciones
    je      checkGaneHorLoop
    mov     cl, byte [edx+1]
    cmp     al, cl
    jne     checkGaneHorLoop
    mov     cl, byte [edx+2]
    cmp     al, cl
    jne     checkGaneHorLoop
    jmp     checkFound        ;Si coinciden, se llega al final y se brinca a checkFound

;Si no coincidieron, se brinca al loop y se chequea la siguiente fila
checkGaneHorLoop:
    add     edx, 3
    pop     ecx
    loop    checkGaneHor
    sub     eax,eax
    sub     ecx,ecx
    mov     ecx, 3
    mov     edx, edi

;Compara los 3 cuadros de cada columna para ver si coinciden con al y cl
checkGaneVer:
    push    ecx
    mov     al, byte [edx]
    cmp     al, 0            ;Si hay un cero en la linea, se brincan las otras comparaciones
    je      checkGaneVerLoop
    mov     cl, byte [edx+3]
    cmp     al, cl
    jne     checkGaneVerLoop
    mov     cl, byte [edx+6]
    cmp     al, cl
    jne     checkGaneVerLoop
    jmp     checkFound        ;Si coinciden, se llega al final y se brinca a checkFound

;Si no coincidieron, se brinca al loop y se chequea la siguiente fila
checkGaneVerLoop:
    inc     edx
    pop     ecx
    loop    checkGaneVer

;Compara las 2 diagonales con al y cl
checkGaneDia:
    sub     eax,eax
    sub     ecx,ecx
    mov     edx, edi
    mov     al, byte [edx+4]
    cmp     al, 0            ;Si el centro es 0, brinca directo al check por empate
    je      checkEmpate
    mov     cl, byte [edx]
    cmp     al, cl
    jne     checkGaneDiaAux
    mov     cl, byte [edx+8]
    cmp     al, cl
    jne     checkGaneDiaAux
    jmp     checkFound       ;Si la primera diagonal coincide, se llega al final y se brinca a checkFound

;Si no coincidieron, se brinca a la segunda diagonal
checkGaneDiaAux:
    mov     cl, byte [edx+2]
    cmp     al, cl
    jne     checkEmpate
    mov     cl, byte [edx+6]
    cmp     al, cl
    jne     checkEmpate
    jmp     checkFound        ;Si la segunda diagonal coincide, se llega al final y se brinca a checkFound

;Si no coincidieron, se chequea por empate
checkEmpate:
    sub     eax,eax
    sub     ecx,ecx
    mov     ecx, 9

;Se chequea el estado de cada casilla con loop
checkEmpateLoop:
    cmp     byte [edx], 0
    je      checkGaneFail    ;Si se encuentra un 0, no hay empate todavia, se brinca a checkGaneFail
    cmp     byte [edx], 1
    je      empateAdd1
empateAdd2:                    ;Si el estado es 2, se incrementa el contador de 2 (ah)
    inc     ah
    jmp     checkEmpateLoopAux
empateAdd1:                    ;Si el estado es 1, se incrementa el contador de 1 (al)
    inc     al
checkEmpateLoopAux:
    inc     edx
    loop    checkEmpateLoop
    cmp     si, 0              ;Si no quedan espacios vacios en el gato y estamos chequeando granGato (si = 0), se brinca a granEmpate
    je      granEmpate
    cmp     ah, al
    jg      ganeEmpate2        ;Si hay mas 2 que 1 en el tablero (ah > al) y es un subgato, brinca a ganeEmpate2
ganeEmpate1:                   ;Si hay mas 1 que 2 en el tablero (ah > al) y es un subgato, brinca a ganeEmpate1
    sub     eax,eax
    mov     al, 1
    jmp     checkFound         ;Se asigna 1 a al y brinca a checkFound
ganeEmpate2:
    sub     eax,eax
    mov     al, 2
    jmp     checkFound         ;Se asigna 2 a al y brinca a checkFound
checkGaneFail:                 ;Si no se encontro un gane o empate, se retorna 0
    sub     eax,eax
    mov     eax,0
    leave
    ret
checkFound:                    ;Si se encontro un gane o empate, se settea el ganador apropiado almacenado en al, con granSetter
    cmp     si,1
    je      granSetter
    leave
    ret    
granEmpate:                    ;Si hubo un empate en el juego general, se retorna
    sub     eax,eax
    mov     eax,3
    leave
    ret

;-----------------------------------------------------------------------
; Revisa si el siguiente espacio ideal del gran gato esta habilitado,
; de lo contrario indica al siguiente disponible.
; int 1 = ultima posicion en el sub-gato actual
; *NUNCA USAR CUANDO EL GATO ESTA VACIO, CYCLO INFINITO
; *Usado exclusivamente por la computadora
; Retorna:
; Numero del 0 al 8, siguiente casilla disponible
;-----------------------------------------------------------------------
siguienteSubGato:
    enter   0,0
    sub     eax, eax
    sub     edx, edx
    sub     ecx, ecx
    mov     edx, granGato
    mov     ecx, [ebp+8]
    add     edx, ecx                 ;edx ahora apunta al estado del subgato correcto
siguienteGranGatoLoop:
    cmp     byte [edx], 0            ;Si no hay ganador en esta casilla, se retorna este numero
    je      siguienteGranGatoExit
    cmp     ecx, 8
    je      siguienteGranGatoCaso8   ;Si la que se chequa actualmente es 8, se brinca a siguienteGranGatoCaso8
    inc     edx
    inc     ecx
    jmp     siguienteGranGatoLoop    ;Si la casilla no esta habilitada (ya hay ganador), se brinca a la siguiente y se chequea esta
siguienteGranGatoCaso8:
    sub     edx, 8
    mov     ecx, 0
    jmp     siguienteGranGatoLoop    ;Del cuadro 8 se pasa de vuelta al cuadro 0 (para mantenerse dentro del rango de memoria)
siguienteGranGatoExit:               ;Cuando se encuentra un cuadro disponible, se retorna
    mov     al, cl
    leave
    ret

;-----------------------------------------------------------------------
; Inteligentemente genera el siguiente movimeinto de la computadora
; en un subgato indicado.
; int 1 = sub-gato
; int 2 = Puntero a variable que retorna el siguiente subgato
; Retorna:
; Numero del -1 al 8 (Casilla jugada, considera casillas ya en uso en subgato, no en granGato. Retorna por medio de 2da variable ingresada)
; *-1 Solo en caso de error (Todos los espacios ya en uso)
;-----------------------------------------------------------------------
turnoComputadora:
    enter   0,0
    push    ebx
    push    esi
    sub     ecx, ecx
    mov     ecx, [ebp+8]
    mov     edx, gatosInd
    cmp     ecx, 0
    je      checkHor
findGatoInd:
    add     edx, 9
    loop    findGatoInd
;edx ahora apunta al subgato correcto

;--------------------------------------------------------------------------------
;Primero se chequea si alguien esta a punto de ganar en las lineas horizontales con bl y bh

checkHor:
    mov     esi, edx
    sub     eax,eax
    mov     ah, -1
    mov     ecx, 3

;Se compara cada posible par de casillas por cada fila en el subgato con bl y bh, llamando checkCasos para verificar su estado
;al = recuadro restante chequeado cada call de checkCasos
checkHorLoop:
    mov     bl, byte [edx+1]
    mov     bh, byte [edx+2]
    call    checkCasos
    inc     al
    mov     bl, byte [edx]
    call    checkCasos
    inc     al
    mov     bh, byte [edx+1]
    call    checkCasos

;Se loopea para verificar cada fila
checkHorLoopAux:
    xor     bx, bx
    inc     al
    add     edx, 3
    loop    checkHorLoop

;--------------------------------------------------------------------------------------------------
;Se chequea si alguien esta a punto de ganar en las columnas verticales con bl y bh

checkVer:
    dec     al
    dec     edx
    mov     ecx, 3

;Se compara cada posible par de casillas por cada columna en el subgato con bl y bh, llamando checkCasos para verificar su estado
;al = recuadro restante chequeado cada call de checkCasos
checkVerLoop:
    mov     bl, byte [edx-3]
    mov     bh, byte [edx-6]
    call    checkCasos
    sub     al, 3
    mov     bl, byte [edx]
    call    checkCasos
    sub     al, 3
    mov     bh, byte [edx-3]
    call    checkCasos

;Se loopea para verificar cada columna
checkVerLoopAux:
    xor     bx, bx
    add     al, 5
    dec     edx
    loop    checkVerLoop

;--------------------------------------------------------------------------------------------------
;Se chequea si alguien esta a punto de ganar en las diagonales con bl y bh

;Se compara cada posible par de casillas por cada diagonal en el subgato con bl y bh, llamando checkCasos para verificar su estado
;al = recuadro restante chequeado cada call de checkCasos
checkDiag:
    xor     bx, bx
    sub     edx, 5
    xor     al, al
    mov     bl, byte [edx+4]
    mov     bh, byte [edx+8]
    call    checkCasos
    add     al, 2
    mov     bh, byte [edx+6]
    call    checkCasos
    add     al, 4
    mov     bh, byte [edx+2]
    call    checkCasos
    add     al, 2
    mov     bh, byte [edx]
    call    checkCasos
    sub     al, 4
    mov     bl, byte [edx+8]
    call    checkCasos
    mov     bl, byte [edx+2]
    mov     bh, byte [edx+6]
    call    checkCasos

;Si ah es mayor o igual a 0 (El jugador esta a un movimiento de ganar en la posicion ah), se brinca a returnAh con la posicion ganadora en ah
checkJugadorCasi:
    cmp     ah, 0
    jge     returnAh
    jmp     checkCentro

;--------------------------------------------------------------------------------------------------
;Esta funcion interna chequea si bl y bh son iguales, su valor y si el espacio entre estas esta disponible

checkCasos:
    cmp     bl, 0            ;Si bl es igual a 0, retornar sin alterar valores (Nadie va a ganar en este par)
    je      checkCasosExit
    cmp     bl, bh           ;Si bl y bh son iguales, chequear si el espacio restante en la linea esta vacio
    je      checkEmpty
    ret
checkEmpty:
    push    ecx
    mov     cl, al
    add     esi, ecx
    cmp     byte [esi], 0    ;Si el espacio esta vacio, chequear el valor de bl
    je      checkCasosEqual
    sub     esi, ecx
    pop     ecx              ;Si no, retornar sin alterar valores
    ret
checkCasosEqual:
    sub     esi, ecx
    pop     ecx
    cmp     bl, 1
    je      checkCasosUno    ;Si 1 va a ganar en esta linea, almacenar la posicion para bloquear el gane en ah
    add     esp, 4           ;Si 2 va a ganar (la computadora), brincar a returnAl con la posicion ganadora en al
    jmp     returnAl
checkCasosUno:
    mov     ah, al
checkCasosExit:
    ret

;--------------------------------------------------------------------------------------------------
;Si no hay nadie a punto de ganar, chequear si el centro esta vacio

checkCentro:
    cmp     byte [edx+4], 0
    je      returnCenterRand    ;Si el centro esta vacio, brincar a returnCenterRand
    push    edx

;--------------------------------------------------------------------------------------------------
;Si el centro no esta vacio o aleatoriamente se decidio que no se va a jugar el centro este turno, se chequean las esquinas

;Se va a chequear el valor de cada una de los cuadros adjacentes a cada esquina, verificando cual seria la esquina mas ventajosa para jugar 
;bl y bh = Cuadros adjacentes a cada esquina
;al = Esquina chequeada por cada llamada de checkEsquinasCasos
;ch = Actual valor de la esquina mas ventajosa (Suma del valor de sus adjacentes)
;ah = Actual esquina mas ventajosa
checkEsquinas:
    pop     edx
    xor     eax,eax
    xor     ecx,ecx
    mov     al, 0
    mov     ah, -1
    mov     ch, -1

;Se va a llamar checkEsquinasCasos una vez por cada esquina (indicada con al)
    mov     bl, byte [edx+1]
    mov     bh, byte [edx+3]
    call    checkEsquinasCasos
    mov     al, 2
    mov     bh, byte [edx+5]
    call    checkEsquinasCasos
    mov     al, 8
    mov     bl, byte [edx+7]
    call    checkEsquinasCasos
    mov     al, 6
    mov     bh, byte [edx+3]
    call    checkEsquinasCasos
;Si ah > 0 (hay esquinas disponibles), se llama returnAh, con la posicion de mayor valor en ah
    cmp     ah, 0
    jge     returnAh
    mov     al, 4
    cmp     byte [edx+4], 0
    je      returnAl        ;Se vuelve a verificar que el centro este vacio, en un caso extremo en el que todas las esquinas y bordes
    jmp     checkCentros

;Funcion interna, realiza las comparaciones
checkEsquinasCasos:
    push    ecx
    sub     ecx,ecx
    mov     cl, al
    cmp     byte [esi+ecx], 0
    je      checkEsquinasCasosUno    ;Si la esquina esta vacia, chequear el valor de bl, de lo contrario retorna sin cambiar valores
    pop     ecx
    ret
checkEsquinasCasosUno:
    pop     ecx
    cmp     bl, bh
    jne     checkEsquinasCasosSuma
    cmp     bl, 1
    jne     checkEsquinasCasosSuma
    add     esp, 4                  ;Si los 2 valores adjacentes son 1, entonces olvida el sistema de prioridad e intenta retornar la esquina en cuestion directamente
    jmp     returnAl
checkEsquinasCasosSuma:             ;Se suma el valor de los cuadros adjacentes
    mov     cl, bl
    add     cl, bh
    cmp     cl, ch
    jg      checkEsquinasCasosNuevoMax    ;Si es el valor mas alto por el momento, salta a checkEsquinasCasosNuevoMax, de lo contrario retorna sin cambiar valores
    ret
checkEsquinasCasosNuevoMax:               ;Establece la esquina actual como la de mayor prioridad, almacenando los valores en ch y ah
    mov     ch, cl
    mov     ah, al
    ret

;--------------------------------------------------------------------------------------------------
;Como ultima posibilidad, chequea los costados del gato

checkCentros:
    xor     eax,eax
    mov     ecx, 4
    add     edx, 1
    mov     al, 1
checkCentrosLoop:
    cmp     byte [edx], 0    ;Si el espacio esta disponible, ejecuta returnAl con el costado disponible en al
    je      returnAl
    add     edx, 2
    add     al, 2
    mov     al, -1           ;En caso de que no se encontrara ningun espacio vacio, corre returnAl con -1 en al (Caso error)
    jmp     returnAl

;--------------------------------------------------------------------------------------------------
;Los 3 diferentes modos de return del programa

;Si el centro esta vacio
returnCenterRand:
    push    edx
    rdtsc
    xor     edx, edx
    mov     ecx, 4
    div     ecx            ;Se usa rdtsc para generar un numero aleatorio entre 1 a 4 en edx
    cmp     edx, 1
    jg      checkEsquinas  ;Si edx es mayor a 1, no jugar en el centro y chequear las esquinas
    pop     edx
    mov     edx, [ebp+12]  ;De lo contrario, jugar en el centro en este subgato (posicion 4)
    mov     dword [edx], 4
    pop     esi
    pop     ebx
    leave
    ret

returnAh:                 ;Mueve el valor de ah a al, antes de correr returnAl
    mov     al, ah
returnAl:                 ;Mueve a la variable por referencia el valor almacenado en "al" y retorna al programa en C
    mov     edx, [ebp+12]
    xor     ah,ah
    mov     [edx], eax
    pop     esi
    pop     ebx
    leave
    ret

;-----------------------------------------------------------------------
; Retorna un numero aleatorio entre 0 y 8, para el modo vs computadora
;-----------------------------------------------------------------------
randNum:
    enter 0,0
    rdtsc            ;Coloca en edx:eax los ticks desde reset
    xor     edx, edx
    mov     ecx, 9
    div     ecx      ;Reduce el rango de 0 a 8
    mov     eax, edx ;Guarda el resultado en eax
    leave
    ret

section .data

gatosInd    resb    81     ; cada casilla individual de cada gato
granGato    resb    9      ; estado de cada sub-gato
