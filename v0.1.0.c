#include <stdio.h>
#include <string.h>
#include <time.h>//time_tt
#include <stdlib.h>

int seed;//randomizer seed;
int response=0;//value returned by move() function
char board[3][3]; /* This array is responsible for storing current gamestate. It is available globally in order to allow*/
char p1[1024]; /*Nazwa pierwszego gracza */
char p2[1024];/*Nazwa drugiego gracza */
char buffer[20];
char small_buffer[3];
typedef enum bool {false=0, true=1} bool;
	bool ruc=false;


void printer(void);
int ruch(char, bool);
int wincond(void);
int pvp(int choice);
int analyze();

int main()
{
	int choice;
	printf("Chcesz zagrać przeciw komputerowi(1) czy przeciw człowiekowi(2)?(1/2)");
	do{
		fgets(small_buffer, sizeof small_buffer, stdin);
		choice= strtol(small_buffer,NULL,10);
	}
	while(choice!=1 && choice!=2);
	pvp(choice);
	return 0;
}



int pvp(int choice){
	time_t tt;//sprawdzamy czas od 01.01.1970
	int i,p;
	int new_game=0;
	int input=0;
	while(new_game<1){
		seed = time(&tt);
		srand(seed); /* Zmienna bez określonej wartości przyjmuje losowe wartości*/
		if(choice==1)
			strncpy(p2,"Komputer", sizeof(p2)-1);
		for(i=0;i<3;i++){
			for(p=0;p<3;p++)
				board[i][p]='#';}
		printf("Cześć! Podaj swoje imię:\nImię:");
		fgets(p1, sizeof p1, stdin);
		p1[strcspn(p1, "\n")] = 0; /*Usuwa newline z stdin w p1*/
		fseek(stdin,0,SEEK_END);
		if(choice==2){
			printf("%s, jak ma na imię Twój przeciwnik?\nImię:", p1);
			fgets(p2, sizeof p2, stdin);
			p2[strcspn(p2, "\n")] = 0;/*Usuwa newline z stdin w p2*/
		}
		printf("Oto plansza! Zapoznajcie się z nią.\n\n");
		printer();/*Pokaz planszę na ekranie - plansza to zwykła tablica, którą będziemy modyfikować*/
		if(seed%2==0){
			printf("Zaczyna %s!\n", p2);
			ruc=false;
		}
		else{
			printf("Zaczyna %s!\n", p1);
			ruc=true;
		}
		for(i=0;i<9;i++)/*Można wykonać maksymalnie 9 ruchów by zapełnić planszę, dlatego i<9*/
		{
			if(choice==2){
			printf("%s, które pole chcesz zapełnić?\n", ruc==true?p1:p2);
			fgets(buffer, sizeof buffer, stdin);
			input= strtol(buffer,NULL,10);
			response = ruch(input, ruc);/*Przechowujemy odpowiedź funkcji ruch w zmiennej response*/
			while(response == 0) { /*Funkcja response zwraca 0 jeśli wprowadzono znak spoza dozwolonego zakresu.
				Prosimy wtedy zawodnika o powtórzenie jego ruchu.*/
				printf("Wprowadzono złą liczbę lub pole jest już zajęte!\n");
				fgets(buffer, sizeof buffer, stdin);
				input= strtol(buffer,NULL,10);
				response = ruch(input, ruc);}}
			else if(ruc==true && choice==1){
				printf("%s, które pole chcesz zapełnić?\n",p1);
				fgets(buffer, sizeof buffer, stdin);
				input= strtol(buffer,NULL,10);
				response = ruch(input, ruc);/*Przechowujemy odpowiedź funkcji ruch w zmiennej response*/
				while(response == 0) { /*Funkcja response zwraca 0 jeśli wprowadzono znak spoza dozwolonego zakresu.
					Prosimy wtedy zawodnika o powtórzenie jego ruchu.*/
					printf("Wprowadzono złą liczbę lub pole jest już zajęte!\n");
					fgets(buffer, sizeof buffer, stdin);
					input= strtol(buffer,NULL,10);
					response = ruch(input, ruc);}}
			else if(ruc==false){
				input=analyze();
				printf("Komputer wybrał pole %d!\n",input);
				response = ruch(input, ruc);/*Przechowujemy odpowiedź funkcji ruch w zmiennej response*/
				while(response == 0) { /*Funkcja response zwraca 0 jeśli wprowadzono znak spoza dozwolonego zakresu.
					Prosimy wtedy komputer o powtórzenie jego ruchu.*/
				input=analyze();
				response = ruch(input, ruc);}}
			printer();/*Pokaż planszę*/
			ruc = !ruc;/*Zmiana wartości ruc pozwala przeciwnikowi na ruch w następnej turze(następnym obrocie pętli)*/
			if(i>=4 && (wincond()==1 || wincond()==2)){
				printf("Wygrywa %s!\n", wincond()==1?p1:p2);
				break;
			}/*Wincond sprawdza warunek wygranej - jeśli któryś z graczy go spełni,
			zwraca identyfikator gracza i przerywa pętlę*/
		}
		if(wincond()==0)
			printf("Remis!\n");/*Jeśli na końcu gry nikt nie wygrał, wyświetl odpowiedni komunikat*/

		printf("Czy chcesz zagrać jeszcze raz?(T/n)\n");
		fgets(small_buffer, sizeof small_buffer, stdin);
		small_buffer[strcspn(small_buffer, "\n")] = 0;
		if(small_buffer[0]=='T'||small_buffer[0]=='t'){
			continue;
		}
		if(small_buffer[0]=='N'||small_buffer[0]=='n'){
			new_game++;
		}
		else
			return 0;
}
	return 0;

}

int ruch(char pole, bool gracz){
	int i,p;
	for(i=0;i<3;i++){
		for(p=0;p<3;p++){
			if(pole-1==i*3+p)
				if (board[i][p] == '#'){
					board[i][p] = (gracz) ? 'X' : 'O'; return 3;
					}
	/*Jeśli pole jest wolne(wartość elementu tablicy to "#"), to wstaw znak. W przeciwnym wypadku zwróć komunikat błędu.*/
}}
	return 0;
}

void printer(void)
{
	printf("%c %c %c\n%c %c %c\n%c %c %c \n \n", board[0][0], board[0][1], board[0][2],board[1][0], board[1][1], board[1][2], board[2][0], board[2][1], board[2][2]);
	/*Pokaż na ekranie wszystkie elementy tablicy w odpowiedniej kolejności*/
}
int wincond(){
	int i,p;
	for(i=0;i<3;i++){
		for(p=0;p<3;p++){
			if((board[i][p]=='X' && board[i][p+1]=='X' && board[i][p+2]=='X' && p==0) || (board[i][p]=='X' && board[i+1][p]=='X' && board[i+2][p]=='X')||
			(board[i][i]=='X' && board[i+1][i+1]=='X' && board[i+2][i+2]=='X') || (board[2][0]=='X' && board[1][1]=='X' && board[0][2]=='X')){
				return 1;
			}
			else if((board[i][p]=='O' && board[i][p+1]=='O' && board[i][p+2]=='O' && p==0) || (board[i][p]=='O' && board[i+1][p]=='O' && board[i+2][p]=='O')||
						(board[i][i]=='O' && board[i+1][i+1]=='O' && board[i+2][i+2]=='O') ||  (board[2][0]=='O' && board[1][1]=='O' && board[0][2]=='O')){
							return 2;
						}
		}			/*Sprawdza, czy odpowiednie pola są zajęte. Jeśli któryś z graczy zapełni
					3 pola wymagane do wygranej to funkcja zwróci odpowiednią wartość, W przeciwnym wypadku
					zwraca wartość 0, świadczącą o remisie.*/
	}
				return 0;
}
int analyze(){
	int i,p,pole,x,d;
	x=0;
	d=0;
	pole=0;
	for(i=0;i<3;i++){
		for(p=0;p<3;p++){
			if((board[i][p-2]=='O' && board[i][p-1]=='O' && board[i][p]=='#' && p==2)||(board[i][p+1]=='O' && board[i][p+2]=='O' && board[i][p]=='#' && p==0)
			||(board[i][p-1]=='O' && board[i][p+1]=='O' && board[i][p]=='#' && p==1) || (board[i+1][p]=='O' && board[i+2][p]=='O' && board[i][p]=='#' && i==0)
			|| (board[i-1][p]=='O' && board[i+1][p]=='O' && board[i][p]=='#' && i==1) || (board[i-1][p]=='O' && board[i-2][p]=='O' && board[i][p]=='#' && i==2)
			|| (board[i][i]=='#' && board[i+1][i+1]=='O' && board[i+2][i+2]=='O' && p==i)	|| (board[i][i]=='#' && board[i-1][i-1]=='O' && board[i-2][i-2]=='O' && p==i)
			|| (board[i][i]=='#' && board[i-1][i-1]=='O' && board[i+1][i+1]=='O' && p==i)	|| (board[i][p]=='#' && board[i-1][p+1]=='O' && board[i-2][p+2]=='O' && i==2 && p==0)
			|| (board[i][p]=='#' && board[i+1][p-1]=='O' && board[i-1][p+1]=='O' && i==1 && p==1) || (board[i][p]=='#' && board[i+1][p-1]=='O' && board[i+2][p-2]=='O' && i==0 && p==2)	){
				pole=i*3+p+1;
				return pole;}
			}}
		for(i=0;i<3;i++){
			for(p=0;p<3;p++){
				if((board[i][p-2]=='X' && board[i][p-1]=='X' && board[i][p]=='#' && p==2)||(board[i][p+1]=='X' && board[i][p+2]=='X' && board[i][p]=='#' && p==0)
				||(board[i][p-1]=='X' && board[i][p+1]=='X' && board[i][p]=='#' && p==1) || (board[i+1][p]=='X' && board[i+2][p]=='X' && board[i][p]=='#' && i==0)
				|| (board[i-1][p]=='X' && board[i+1][p]=='X' && board[i][p]=='#' && i==1) || (board[i-1][p]=='X' && board[i-2][p]=='X' && board[i][p]=='#' && i==2)
				|| (board[i][i]=='#' && board[i+1][i+1]=='X' && board[i+2][i+2]=='X' && p==i)	|| (board[i][i]=='#' && board[i-1][i-1]=='X' && board[i-2][i-2]=='X' && p==i)
				|| (board[i][i]=='#' && board[i-1][i-1]=='X' && board[i+1][i+1]=='X' && p==i)	|| (board[i][p]=='#' && board[i-1][p+1]=='X' && board[i-2][p+2]=='X' && i==2 && p==0)
				|| (board[i][p]=='#' && board[i+1][p-1]=='X' && board[i-1][p+1]=='X' && i==1 && p==1) || (board[i][p]=='#' && board[i+1][p-1]=='X' && board[i+2][p-2]=='X' && i==0 && p==2)	 	){
					pole=i*3+p+1;
					return pole;}}}
		if(board[1][1]=='#'){
			return 5;}

		for(i=0;i<3;i++){
			for(p=0;p<3;p++){
				if(board[i][p]=='#'){
					x=rand();
					d=rand();
					d=d%3;
					x=x%3;
					if(board[x][d]=='#')
						return (3*x+d+1);
							}
						}}}
