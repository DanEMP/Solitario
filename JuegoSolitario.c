#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 52

//ESTRUCTURAS
typedef struct{

	char paloCarta;
	int color;
	int estado;
	int numeroCarta;
}Carta;

typedef struct{

	Carta C;
	int base;
	int top;
}Pila;

//ARREGLOS DE ESTRUCTURAS
Carta V[MAX];
Pila pilas[14];

//FUNCIONES
void menuInterno(Pila pila[]);

void generarPilas(Pila pila[]);
void mostrarPilas(Pila pila[]);
void crearLimites(Pila pila[]);
void crearCartas(Carta cartas[]);
void shuffle(Carta aux[]);
void mostrarCarta(Pila pila[],int j);

int jugadaValidaPilasDeJuego(Pila pila[],int o, int d);
int jugadaValidaPilasDeSalida(Pila pila[], int o, int d);
int executeOrder(Pila pila[], int *contador);

void exchange(Carta cartas[], int i, int j);
void exchangePosition(Pila pila[], int a, int b);

void push(Pila pila[], int i, Carta x);
void pushVariasCartas(Pila pila[], int o, int d, int c);

int cartaTop(Pila pila[],int o, int d, int c);
Carta pop(Pila pila[], int i, Carta x);

int sizePila(Pila pila[],int i);
int pilaLlena(Pila pila[], int i);
int pilaVacia(Pila pila[],int i);
int gano(Pila pila[]);

void overflowLocal(Pila pila[],int i);
void printPila(Pila pila[], int i);




int main(){

	int opc;
	printf("\n\t\tBienvenido - Juego \"Solitario\" ");
	while(opc){
		printf("\n\nEscoge la opción deseada: \n\n1. Reglas del juego \n2. Comandos del Juego \n3. Comenzar Juego \n4. Salir \n\n\t");
		scanf("%d", &opc);

		switch(opc){

		case 1:
			printf("\n\nREGLAS DEL JUEGO:\n\n");
			printf("1.- Existen 7 \'Pilas de Juego\' en el tablero central. En ellas solo se pueden colocar cartas\n");
			printf("    que formen una secuencia descendente consecutiva y de colores alternos.\n\n");
			printf("2.- Existe una \'Pila de Reserva\', dónde las cartas restantes esperan ser descubiertas.\n\n");
			printf("3.- Existe una \'Pila de Descarte\', esta se llenará cada vez que tomemos una carta de la \'Pila de Reserva\'.\n");
			printf("    La carta colocada en la \'Pila de Descarte\' podrá moverse a las \'Pilas de Juego\', respetando la regla 1, \n"
					"    o a las Pilas de Salida.\n\n");
			printf("4.- Existen cuatro \'Pilas de Salida\', en ellas se iran apilando una carta a la vez, de un mismo palo\n\n");
			printf("5.- Si se logra llenar cada una de las 13 cartas de un mismo palo en cada Pila de Salida\n");
			printf("    se gana el juego. De lo contrario, se pierde cuando no se puede hacer ningun movimiento.\n\n");

			break;

		case 2:

			printf("\n\nCOMANDOS DEL JUEGO:\n \nA continuación verás el numero asignado a cada pila y comandos para funciones extra, \ncon ellos podrás realizar movimientos."
					" Presentamos algunos ejemplos al final\n");
			printf("\n-----------------------------------------");
			printf("\nPILAS DE DESCARTE & RESERVA\n\nPila de Descarte: 0 \nPila de Reserva: 1\n\n");
			printf("-------------------------------------------");
			printf("\n\nPILAS DE IZQ. A DER. EN TABLERO CENTRAL\n\nPila de Juego: 2 \nPila de Juego: 3 \nPila de Juego: 4 \nPila de Juego: 5 \nPila de Juego: 6 \nPila de Juego: 7 \nPila de Juego: 8 \n\n");
			printf("-------------------------------------------");
			printf("\n\nPILAS DE SALIDA DE IZQ. A DER. \n\nDiamantes: 9 \nCorazones: 10 \nTrebol: 11 \nEspadas: 12 \n\n");
			printf("-------------------------------------------");
			printf("\n\nCOMANDOS EXTRA");
			printf("\nMover Carta de Pila de Reserva a Pila de Descarte: d \nMover mas de una carta: c \nEmpezar nuevo juego: n \n\n");
			printf("-------------------------------------------");

			printf("\n[Carta] [Pila origen] [Pila destino]\n");
			printf("Por ejemplo: 2 3, mover de la pila 2 a la pila 3\n");
			printf("Por ejemplo: 4 9, mover de la pila 4 a la pila de diamantes\n");
			printf("Por ejemplo: 0 3, mover de la pila descarte a la pila 4\n");
			printf("Por ejemplo: 0 10, mover de la pila descarte a la pila corazones\n");
			printf("Por ejemplo: 5 3 3, mover de la pila 5, a la pila 3, desde la carta 3\n");
			printf("Por ejemplo: 11 6, error! no se puede mover una carta de las pilas de salida\n");
			printf("Por ejemplo: d, para barajar la carta descarte y poner otra carta\n");
			printf("Por ejemplo: c, para mover mas de una carta\n");
			printf("Por ejemplo: n, empezar un nuevo juego\n");

			break;

		case 3:

			menuInterno(pilas);

			break;

		case 4:
			printf("\n\n\t ¡Saliste del Juego! Nos vemos Pronto.");
			exit(0);

			break;

		default:
			printf("Lo sentimos, ingresa una opcion valida.\n\n");
			break;
		}

	}


	return 0;
}

void menuInterno(Pila pila[]){

	generarPilas(pila);
	mostrarPilas(pila);

	int cont = -1;
	int flag = 0;

	while(flag!=1){
		printf("\nIngresa tu jugada: \n");
		flag=executeOrder(pila,&cont);
		mostrarPilas(pilas);
	}

	printf("\n\nGANASTE EL JUEGO");
}

void generarPilas(Pila pila[]){

	int k,i,j,aux=24;

	crearLimites(pilas);
	Carta cartas[52];
	crearCartas(cartas);
	shuffle(cartas);

	//CREACIÓN DE PILA DE RESERVA / ESTADO 0 = CARTA CERRADA
	for(k=0;k<24;k++){
		push(pila,1,cartas[k]);
		V[pila[1].base+k+1].estado=0;
	}

	//CREACION DE PILAS DE JUEGO
	for(i=2;i<=8;i++){
		for(j=0;j<i-1;j++){
			push(pila,i,cartas[aux]);
			V[pila[i].base+j+1].estado=0;
			aux++;
		}
		V[pila[i].top].estado=1;
	}
}

void mostrarPilas(Pila pila[]){

	printf("\n-----------------------------------------");
	printf("\nPILAS DE DESCARTE & RESERVA\n\nPila de Descarte 0: "); printPila(pila,0);
	printf("\nPila de Reserva 1: [*]\n\n");

	printf("-------------------------------------------");
	printf("\n\nPILAS EN EL TABLERO CENTRAL");
	printf("\n\nPila de Juego 2: "); printPila(pila,2);
	printf("\nPila de Juego 3: ");	 printPila(pila,3);
	printf("\nPila de Juego 4: ");	 printPila(pila,4);
	printf("\nPila de Juego 5: ");	 printPila(pila,5);
	printf("\nPila de Juego 6: ");	 printPila(pila,6);
	printf("\nPila de Juego 7: ");	 printPila(pila,7);
	printf("\nPila de Juego 8: ");	 printPila(pila,8);

	printf("\n\n-------------------------------------------");

	printf("\n\nPILAS DE SALIDA");
	printf("\n\nDiamantes 9:  ");	printPila(pila,9);
	printf("\nCorazones 10:  ");		printPila(pila,10);
	printf("\nTrebol 11: ");		printPila(pila,11);
	printf("\nEspadas 12: ");		printPila(pila,12);
	printf("\n\n-------------------------------------------");


}

void crearLimites(Pila pila[]){

	//Creación de las 13 pilas
	int i;
	for(i=0;i<=12;i++){
		pila[i].base=pila[i].top= (int)(i*((float)MAX/13));

	}
}

void crearCartas(Carta cartas[]){
	int a=1, b=1, c=1, i;

	for(i=0;i<52;i++){

		if(i<=12){
			cartas[i].paloCarta='D';
			cartas[i].color=1;
			cartas[i].numeroCarta= i+1;
		}

		if(i > 12 && i <= 25) {
			cartas[i].paloCarta = 'C';// Corazones
			cartas[i].color = 1; //color rojo
			cartas[i].numeroCarta = a++;
		}

		if(i > 25 && i <= 38) {
			cartas[i].paloCarta = 'T';// Trebol
			cartas[i].color = 0; //color negro
			cartas[i].numeroCarta = b++;
		}

		if(i > 38) {
			cartas[i].paloCarta = 'E';// Espadas
			cartas[i].color = 0; //color negro
			cartas[i].numeroCarta = c++;
		}
	}
}

void shuffle(Carta aux[]){
	int i=51;

	while(i>=0){
		srand(time(NULL));

		if(i==0){
			int randomPosition = rand()%51+1;
			exchange(aux,randomPosition,i);
		}else{
			int randomPosition = rand()%i+1;
			exchange(aux,randomPosition,i);
		}

		i--;
	}
}

void mostrarCarta(Pila pila[], int j){
	if(V[j].estado==1){
		printf("[%d%c]",V[j].numeroCarta, V[j].paloCarta);
	}else{
		printf("[%c%c]", '*' , '*');
	}
}

int jugadaValidaPilasDeJuego(Pila pila[], int o, int d){

	//EVITAR MOVER O QUITAR DESDE LA PILA DE RESERVA
	if(d<2 || o==1 || o<0 || o>8){
		return 0;
	}

	//ERROR SI LA PILA ORIGEN ESTA VACIA
	if(pilaVacia(pila,o)){
		return 0;
	}

	//SOLO SE PUEDE MOVER EN LA PILA VACIA UNA CARTA K=13
	if(pilaVacia(pila,d)){
		if(V[pila[o].top].numeroCarta!=13){
			return 0;
		}
	}

	if(pilaVacia(pila,d)==0){
		//ERROR SI LOS COLORES SON IGUALES
		if(V[pila[o].top].color == V[pila[d].top].color){
			return 0;
		}
		//ERROR SI NO SON NUMEROS CONSECUTIVOS
		if(V[pila[o].top].numeroCarta != (V[pila[d].top].numeroCarta-1)){
			return 0;
		}

	}

	return 1;
}

int jugadaValidaPilasDeSalida(Pila pila[], int o, int d){
	if(d>12){
		return 0;
	}

	if(V[pila[o].top].paloCarta=='D' && d!=9){
		return 0;
	}

	if(V[pila[o].top].paloCarta=='C' && d!=10){
		return 0;
	}

	if(V[pila[o].top].paloCarta=='T' && d!=11){
		return 0;
	}

	if(V[pila[o].top].paloCarta=='E' && d!=12){
		return 0;
	}

	//CUANDO LA PILA NO ESTA VACIA
	if(pilaVacia(pila,d)==0){
		if(V[pila[o].top].numeroCarta != (V[pila[d].top].numeroCarta+1)){
			return 0;
		}
		return 1;
	}
}

int cartaTop(Pila pila[], int o, int d, int c){
	int i, index=-1;

	for(i=pila[o].top; i>=pila[o].base+1;i--){
		if(V[i].estado==0){
			break;
		}

		if(V[i].numeroCarta==c){
			index=i;
		}
	}

	if(index!=-1){
		//SI LA PILA DE DESTINO ESTAS VACIA SE PASARA DIRECTAMENTE LAS CARTAS
		if(pilaVacia(pila,d)){
			return index;
		}
		if(V[index].numeroCarta==(V[pila[d].top].numeroCarta-1) && V[index].color != V[pila[d].top].color){
			return index;
		}
	}

	return -1;//CUANDO NO SE ENCUENTRE ESA CARTA
}

int executeOrder(Pila pila[], int *contador){

	int o,d,c;
	char opc;
	Carta aux;

	//MOVER MAS DE UNA CARTA HACIA OTRA PILA.
	if(scanf("%d %d", &o, &d)==2){
		//VERIFICAR JUGADAS
		if(d<9){
			//PONER UNA CARTA 13=K EN UNA PILA VACIA
			if(pilaVacia(pila,d)){
				if(V[pila[o].top].numeroCarta==13){
					aux=pop(pila,o,aux);
					push(pila,d,aux);
					if(o!=0){
						V[pila[o].top].estado=1;
						V[pila[d].top].estado=1;
					}

				}
			}

			//MOVER MAS DE UNA CARTA SI EXISTE ALGUNA SUBPILA PARA MOVER
			if(jugadaValidaPilasDeJuego(pila,o,d)==1){
				aux=pop(pila,o,aux);
				push(pila,d,aux);
				V[pila[o].top].estado=1;
				V[pila[d].top].estado=1;
			}
		}else{
			if(jugadaValidaPilasDeSalida(pila,o,d)==1){
				aux=pop(pila,o,aux);
				push(pila,d,aux);
				V[pila[o].top].estado=1;
				V[pila[d].top].estado=1;
			}
		}
	}else{

		//OPCIONES EXTRAS
		if(scanf("%c",&opc)==1){
			//COMANDO PARA GANAR EL JUEGO, LETRA "M"
			if(opc=='w' || opc=='W'){
				return 1;
			}

			//MOVIMIENTO DE DE CARTA DE PILA RESERVA A DESCARTE
			if(opc=='d' || opc=='D'){
				//SI LA PILA DE RESERVA NO ESTA VACIA
				if(sizePila(pila,1)!=0){
					//SI LA PILA 0 ESTA VACIA, HACE UN POP A LA FILA 1 & DESPUES PUSH A PILA 0
					if(pilaVacia(pila,0)==1){
						aux=pop(pila,1,aux);
						push(pila,0,aux);
						V[pila[0].top].estado=1;
					}else{
						//SI LA PILA 0 NO ESTA VACIA SE INTERCAMBIA PILA 1 CON PILA 0
						(*contador)++;
						exchangePosition(pila, pila[0].top, pila[1].top - (*contador%(sizePila(pila,1)-1)));
						V[pila[0].top].estado=1;
					}
				}
			}

			if(opc=='c'){
				scanf("%d %d %d", &o, &d, &c);
				pushVariasCartas(pila,o,d,c);
			}

			if(opc=='n'){
				system("clear");
				printf("\n\n\n\n\n\n");
				main(pila);
			}

			return 0;
		}
	}

	if(pila){
		return 0;
	}
}


void exchange(Carta cartas[], int i, int j){
	Carta aux;
	aux=cartas[i];
	cartas[i]=cartas[j];
	cartas[j]=aux;
}

void exchangePosition(Pila pila[], int a, int b){
	Carta aux;
	aux=V[a];
	V[a]=V[b];
	V[b]=aux;
}

void push(Pila pila[], int i, Carta x){
	pilas[13].base = MAX;
	if(pila[i].top == pila[i+1].base)
		overflowLocal(pila, i);//REACOMODA LAS PILAS PARA TENER UN ESPACIO LIBRE.
	pila[i].top = pila[i].top + 1;
	V[pila[i].top] = x;
}

void pushVariasCartas(Pila pila[], int o, int d, int c){
	Carta temp;
	int index = cartaTop(pila, o, d, c);
	int i;

	if(index!=-1){
		int count = pila[o].top-index;
		//EN LA PILA DESTINO SE HACE UN PUSH DE TODAS LAS CARTAS DE LA PILA ORGINEN HASTA EL TOPE
		Carta cart[count + 1];
		for(i = 0; i < count + 1; i++){
			cart[i]=pop(pila,o,temp);
		}

		for(i = 0; i<count+1; i++){
			push(pila,d,cart[count-i]);
		}

		//SI LA PILA NO ESTA VACIA
		if(pilaVacia(pila,o)==0){
			V[pila[o].top].estado = 1;
		}
	}
}

Carta pop(Pila pila[], int i, Carta x){
	if(pila[i].top== pila[i].base){
		x.numeroCarta=0;
	}else{
		x=V[pila[i].top];
		pila[i].top=pila[i].top-1;
	}

	return x;
}

void printPila(Pila pila[], int i){
	int j;

	for(j=pila[i].base+1; j<=pila[i].top; j++){
		mostrarCarta(pila,j);
	}

	printf("\n");
}

int sizePila(Pila pila[],int i){
	return (pila[i].top-pila[i].base);
}

int pilaLlena(Pila pila[], int i){
	return sizePila(pila,i)==13;
}

int pilaVacia(Pila pila[],int i){
	return pila[i].base==pila[i].top;
}

int gano(Pila pila[]){
	int j, k=0, i=9;

	for(j=0;j<4;j++){
		if(pilaLlena(pila,i+j)){
			k++;
		}
	}

	return k==4 ? 1:0;
}

void overflowLocal(Pila pila[], int i){
	int k, flat=0;
	int j=i;

	while(pila[j].top<MAX){
		j++;

		if(pila[j].top < pila[j+1].base){
			flat=1; //AL ENCONTRASE CON UN ESPACIO VACIO A LA DERECHA
			break;
		}
	}

	if(flat==1){
		//SE MUEVE UNA POSICION A LA DERECHA LAS PILAS (I+1) (I+2).... j
		for(k=pila[j].top; k>=pila[i+1].base+1;k--){
			V[k+1] = V[k];
		}

		//ACTUALIZAMOS BASES Y TOPS
		for(k=i+1; k<=j;k++){
			pila[k].base=pila[k].base+1;
			pila[k].top=pila[k].top+1;
		}
	}else{
		//SI NO SE ENCUENTRA UN ESPACIO A LA DERECHA, SE BUSCAR ALGUN ESPACIO A LA IZQUERDA. AL ENCONTRARLO, SE MUEVE LA PILA A LA IZQ.
		j=i;
		while(1){
			j--;
			if(pila[j].top < pila[+1].base){
				break;
			}
		}

		for(k=pila[j+1].base;k<pila[i].top;k++){
			V[k]=V[k+1];
		}

		//ACTUALIZAR BASES Y TOPS
		for(k=j+1; k<=i;k++){
			pila[k].base=pila[k].base-1;
			pila[k].top=pila[k].top-1;
		}
	}
}
