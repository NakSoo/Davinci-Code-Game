#include <stdio.h>
#include <conio.h> //키보드 제어 getch ->enter키 안눌러도 바로 입력
#include <Windows.h>  
#include <stdlib.h>
//화면 크기
#define WIDTH 180
#define HEIGHT 50
//true/false
#define TRUE 1
#define FALSE 0
//색깔
#define BLACK	0
#define BLUE1	1
#define GREEN1	2
#define CYAN1	3
#define RED1	4
#define MAGENTA1 5
#define YELLOW1	6
#define GRAY1	7
#define GRAY2	8
#define BLUE2	9
#define GREEN2	10
#define CYAN2	11
#define RED2	12
#define MAGENTA2 13
#define YELLOW2	14
#define WHITE	15

#define DECK_SIZE 24
typedef struct {
	char num;
	int color;
	int use;
	int secret;
}CARD;

typedef struct {
	CARD card[DECK_SIZE];
	int cardNum; //tos
}DECK;

void gotoxy(int x, int y) //내가 원하는 위치로 커서 이동
{
	COORD pos; // Windows.h 에 정의   
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void textcolor(int fg_color, int bg_color)  //색 바꾸기  
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color | bg_color << 4);
}

void removeCursor(void) { // 커서를 안보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void showCursor(void) { // 커서를 보이게 한다
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void cls(int bg_color, int text_color) // 화면 지우기
{
	char cmd[200];
	int cols = WIDTH, lines = HEIGHT;
	system("cls");
	sprintf(cmd, "COLOR %x%x", bg_color, text_color);
	system(cmd);
	sprintf(cmd, "mode con: cols=%d lines=%d", cols, lines);
	system(cmd);
}

//시작화면
void billboard() {
	textcolor(RED2, GRAY2);
	//DAVINCI
	gotoxy(40, 10); printf("■■■■        ■■■      ■      ■      ■■■      ■      ■      ■■■        ■■■      ");
	gotoxy(40, 11); printf("■      ■    ■      ■    ■      ■        ■        ■      ■    ■      ■        ■        ");
	gotoxy(40, 12); printf("■      ■    ■      ■    ■      ■        ■        ■■    ■    ■                ■        ");
	gotoxy(40, 13); printf("■      ■    ■      ■    ■      ■        ■        ■  ■  ■    ■                ■        ");
	gotoxy(40, 14); printf("■      ■    ■■■■■    ■      ■        ■        ■    ■■    ■                ■        ");
	gotoxy(40, 15); printf("■      ■    ■      ■      ■  ■          ■        ■      ■    ■      ■        ■        ");
	gotoxy(40, 16);	printf("■■■■      ■      ■        ■          ■■■      ■      ■      ■■■        ■■■      ");
	//CODE
	gotoxy(60, 20); printf("  ■■■        ■■■      ■■■■      ■■■■■    ");
	gotoxy(60, 21); printf("■      ■    ■      ■    ■      ■    ■            ");
	gotoxy(60, 22);	printf("■            ■      ■    ■      ■    ■            ");
	gotoxy(60, 23);	printf("■            ■      ■    ■      ■    ■■■■      ");
	gotoxy(60, 24);	printf("■            ■      ■    ■      ■    ■            ");
	gotoxy(60, 25);	printf("■      ■    ■      ■    ■      ■    ■            ");
	gotoxy(60, 26);	printf("  ■■■        ■■■      ■■■■      ■■■■■    ");
}

void numOne(int x, int y, int color){
	if(color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃    ■    ┃");
	gotoxy(x, y+2); printf("┃    ■    ┃");
	gotoxy(x, y+3); printf("┃    ■    ┃");
	gotoxy(x, y+4); printf("┃    ■    ┃");
	gotoxy(x, y+5); printf("┃    ■    ┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");
}
void numTwo(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃■■■■■┃");
	gotoxy(x, y+2); printf("┃        ■┃");
	gotoxy(x, y+3); printf("┃■■■■■┃");
	gotoxy(x, y+4); printf("┃■        ┃");
	gotoxy(x, y+5); printf("┃■■■■■┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void numThree(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃■■■■■┃");
	gotoxy(x, y+2); printf("┃        ■┃");
	gotoxy(x, y+3); printf("┃■■■■■┃");
	gotoxy(x, y+4); printf("┃        ■┃");
	gotoxy(x, y+5); printf("┃■■■■■┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void numFour(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃■      ■┃");
	gotoxy(x, y+2); printf("┃■      ■┃");
	gotoxy(x, y+3); printf("┃■■■■■┃");
	gotoxy(x, y+4); printf("┃        ■┃");
	gotoxy(x, y+5); printf("┃        ■┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void numFive(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃■■■■■┃");
	gotoxy(x, y+2); printf("┃■        ┃");
	gotoxy(x, y+3); printf("┃■■■■■┃");
	gotoxy(x, y+4); printf("┃        ■┃");
	gotoxy(x, y+5); printf("┃■■■■■┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void numSix(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃■■■■■┃");
	gotoxy(x, y+2); printf("┃■        ┃");
	gotoxy(x, y+3); printf("┃■■■■■┃");
	gotoxy(x, y+4); printf("┃■      ■┃");
	gotoxy(x, y+5); printf("┃■■■■■┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void numSeven(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y); printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃■■■■■┃");
	gotoxy(x, y+2); printf("┃        ■┃");
	gotoxy(x, y+3); printf("┃        ■┃");
	gotoxy(x, y+4); printf("┃        ■┃");
	gotoxy(x, y+5); printf("┃        ■┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void numEight(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃■■■■■┃");
	gotoxy(x, y+2); printf("┃■      ■┃");
	gotoxy(x, y+3); printf("┃■■■■■┃");
	gotoxy(x, y+4); printf("┃■      ■┃");
	gotoxy(x, y+5); printf("┃■■■■■┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void numNine(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃■■■■■┃");
	gotoxy(x, y+2); printf("┃■      ■┃");
	gotoxy(x, y+3); printf("┃■■■■■┃");
	gotoxy(x, y+4); printf("┃        ■┃");
	gotoxy(x, y+5); printf("┃■■■■■┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void numTen(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃■  ■■■┃");
	gotoxy(x, y+2); printf("┃■  ■  ■┃");
	gotoxy(x, y+3); printf("┃■  ■  ■┃");
	gotoxy(x, y+4); printf("┃■  ■  ■┃");
	gotoxy(x, y+5); printf("┃■  ■■■┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void numEleven(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃ ■    ■ ┃");
	gotoxy(x, y+2); printf("┃ ■    ■ ┃");
	gotoxy(x, y+3); printf("┃ ■    ■ ┃");
	gotoxy(x, y+4); printf("┃ ■    ■ ┃");
	gotoxy(x, y+5); printf("┃ ■    ■ ┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void Joker(int x, int y, int color) {
	if (color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);     printf("┏━━━━━┓");
	gotoxy(x, y + 1); printf("┃          ┃");
	gotoxy(x, y + 2); printf("┃          ┃");
	gotoxy(x, y + 3); printf("┃■■■■■┃");
	gotoxy(x, y + 4); printf("┃          ┃");
	gotoxy(x, y + 5); printf("┃          ┃");
	gotoxy(x, y + 6); printf("┗━━━━━┛");

}
void overturnCard(int x, int y,CARD card) {
	if(card.color)
		textcolor(BLACK, WHITE);
	else
		textcolor(WHITE, BLACK);
	gotoxy(x, y);   printf("┏━━━━━┓");
	gotoxy(x, y+1); printf("┃          ┃");
	gotoxy(x, y+2); printf("┃          ┃");
	gotoxy(x, y+3); printf("┃          ┃");
	gotoxy(x, y+4); printf("┃          ┃");
	gotoxy(x, y+5); printf("┃          ┃");
	gotoxy(x, y+6); printf("┗━━━━━┛");

}
void eraser(int x, int y) {
	textcolor(BLACK, WHITE);
	
	gotoxy(x, y);     printf("              ");
	gotoxy(x, y + 1); printf("              ");
	gotoxy(x, y + 2); printf("              ");
	gotoxy(x, y + 3); printf("              ");
	gotoxy(x, y + 4); printf("              ");
	gotoxy(x, y + 5); printf("              ");
	gotoxy(x, y + 6); printf("              ");
	gotoxy(x, y + 7); printf("              ");

}
//중간 출력
void middleShow(DECK *deck) {
	int i;
	
	for (i = 0; i < DECK_SIZE/2; i++) {
		if (deck->card[i].use == TRUE) {
			eraser(i * 15, HEIGHT / 2 - 14);
			gotoxy(i * 15 + 5, HEIGHT / 2 - 7);
			printf("    ");
		}
		else {
			overturnCard(i * 15, HEIGHT / 2 - 14, deck->card[i]);
			gotoxy(i * 15 + 5, HEIGHT / 2 - 7);
			printf("<%d>", i + 1);
		}
	}
	
	for (i = 0; i < DECK_SIZE/2; i++) {
		if (deck->card[i+12].use == TRUE) {
			eraser(i * 15, HEIGHT / 2 - 5);
			gotoxy(i * 15 + 5, HEIGHT / 2 + 2);
			printf("    ");
		}
		else {
			overturnCard(i * 15, HEIGHT / 2 - 5, deck->card[i+12]);
			gotoxy(i * 15 + 5, HEIGHT / 2 + 2);
			printf("<%d>", i + 13);
		}
	}	
}
//들어온 숫자를 그리기
void numManage(int x, int y, CARD card) {
	switch (card.num) {
	case 1: numOne(x, y, card.color); break;
	case 2: numTwo(x, y, card.color); break;
	case 3: numThree(x, y, card.color); break;
	case 4: numFour(x, y, card.color); break;
	case 5: numFive(x, y, card.color); break;
	case 6: numSix(x, y, card.color); break;
	case 7: numSeven(x, y, card.color); break;
	case 8: numEight(x, y, card.color); break;
	case 9: numNine(x, y, card.color); break;
	case 10:numTen(x, y, card.color); break;
	case 11:numEleven(x, y, card.color); break;
	case '-': Joker(x, y, card.color); break;
	default:;
	}
}
//자신의 덱 그리기
void showMydeck(DECK *myDeck) {
	int i,x,y;
	for (i = 0; i < myDeck->cardNum; i++) {
		x = (WIDTH / 2 - (myDeck->cardNum/2) * 15) + i*15 ;
		y = 30;
		//밝혀진것 빨간줄
		if (myDeck->card[i+1].secret == FALSE) {
			textcolor(RED2, RED2);
			gotoxy(x, y + 7); printf("              ");
		}
		else {
			textcolor(BLACK,WHITE);
			gotoxy(x, y + 7); printf("              ");
		}
		numManage(x, y, myDeck->card[i+1]);
		gotoxy(WIDTH/2 + (i * (15) - 15), 37);
	}
	
	
}
//컴퓨터 덱 그리기
void showComputdeck(DECK *computDeck) {
	int i, x, y;
	for (i = 0; i < computDeck->cardNum; i++) {
		x = (WIDTH / 2 - (computDeck->cardNum / 2) * 15) + i * 15;
		y = 2;

		if (computDeck->card[i+1].secret == FALSE)
			numManage(x, y, computDeck->card[i+1]);
		else if(computDeck->card[i+1].secret==TRUE)
			overturnCard(x, y, computDeck->card[i+1]);

		gotoxy((WIDTH / 2 - (computDeck->cardNum / 2) * 15) + i * 15 +5, 9);
		printf("<%d>", i+1);
		
	}
	
}
//검은조커 입력받기
int blackJokerInput() {
	int position;
	gotoxy(75, 43); printf("검은 조커의 위치를 지정해 주세요");
	gotoxy(85, 45); scanf("%d", &position);
	//지우기
	textcolor(BLACK, WHITE);
	gotoxy(75, 43); printf("                                 ");
	gotoxy(85, 45); printf("   ");
	return position--;
}
//하얀조커 입력받기
int whiteJokerInput() {
	int position;
	gotoxy(75, 43); printf("하얀 조커의 위치를 지정해 주세요");
	gotoxy(85, 45); scanf("%d", &position);
	//지우기
	textcolor(BLACK, WHITE);   
	gotoxy(75, 43); printf("                                  ");
	gotoxy(85, 45); printf("   ");
	return position--;
}

void drawBox() {
	int i;
	gotoxy(0, HEIGHT / 2 - 15);
	for (i = 0; i < 90; i++)
		printf("─");

	gotoxy(0, HEIGHT / 2 + 3);
	for (i = 0; i < 90; i++)
		printf("─");

	gotoxy(0, 39);
	for (i = 0; i < 90; i++)
		printf("─");
	gotoxy(1, 29); printf("ME");
	gotoxy(1, 0); printf("COM");

	for (i = 40; i < 50; i++) {
		gotoxy(45,i); printf("│");
		gotoxy(135, i); printf("│");
	}
	gotoxy(85, 41); printf("Message Box");
	textcolor(RED2, WHITE);
	gotoxy(15, 41); printf("AI's comment");
	textcolor(BLUE2, WHITE);
	gotoxy(150, 41); printf("My latest picked Card");
}

void youWin() {
	textcolor(RED2, GRAY2);
	gotoxy(40, 10); printf("■      ■      ■■■      ■      ■    ");
	gotoxy(40, 11); printf("■      ■    ■      ■    ■      ■    ");
	gotoxy(40, 12); printf("  ■  ■      ■      ■    ■      ■    ");
	gotoxy(40, 13); printf("    ■        ■      ■    ■      ■    ");
	gotoxy(40, 14); printf("    ■        ■      ■    ■      ■    ");
	gotoxy(40, 15); printf("    ■        ■      ■    ■      ■    ");
	gotoxy(40, 16);	printf("    ■          ■■■        ■■■      ");


	gotoxy(60, 20); printf("■      ■    ■■■■■    ■      ■    ");
	gotoxy(60, 21); printf("■      ■        ■        ■      ■    ");
	gotoxy(60, 22);	printf("■      ■        ■        ■■    ■    ");
	gotoxy(60, 23);	printf("■  ■  ■        ■        ■  ■  ■  ");
	gotoxy(60, 24);	printf("■  ■  ■        ■        ■    ■■    ");
	gotoxy(60, 25);	printf("■■  ■■        ■        ■      ■    ");
	gotoxy(60, 26);	printf("■      ■    ■■■■■    ■      ■    ");

}

void youLose() {
	textcolor(RED2, GRAY2);
	gotoxy(40, 10); printf("■      ■      ■■■      ■      ■    ");
	gotoxy(40, 11); printf("■      ■    ■      ■    ■      ■    ");
	gotoxy(40, 12); printf("  ■  ■      ■      ■    ■      ■    ");
	gotoxy(40, 13); printf("    ■        ■      ■    ■      ■    ");
	gotoxy(40, 14); printf("    ■        ■      ■    ■      ■    ");
	gotoxy(40, 15); printf("    ■        ■      ■    ■      ■    ");
	gotoxy(40, 16);	printf("    ■          ■■■        ■■■      ");


	gotoxy(60, 20); printf("■              ■■■        ■■■■    ■■■■■    ");
	gotoxy(60, 21); printf("■            ■      ■    ■            ■            ");
	gotoxy(60, 22);	printf("■            ■      ■    ■            ■            ");
	gotoxy(60, 23);	printf("■            ■      ■    ■■■■      ■■■■      ");
	gotoxy(60, 24);	printf("■            ■      ■            ■    ■            ");
	gotoxy(60, 25);	printf("■            ■      ■            ■    ■");
	gotoxy(60, 26);	printf("■■■■■      ■■■      ■■■■      ■■■■■    ");

}