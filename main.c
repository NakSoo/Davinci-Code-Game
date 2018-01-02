#include <stdio.h>
#include <stdlib.h>
#include <time.h> //rand
#include <Windows.h>
#include <string.h> //memcpy

#define DECK_SIZE 24
#define TRUE 1
#define FALSE 0
#define WHITE 15
#define BLACK 0
#define WIDTH 150
#define HEIGHT 50
#define PLAYER 0
#define COMPUTER 1

#define UP 72
#define DOWN 80
#define SPECIAL1 0xe0 // 특수키는 0xe0 + key 값으로 구성된다.
#define SPECIAL2 0x00 // keypad 경우 0x00 + key 로 구성된다.

int possibility[14][13];

//카드 타입
typedef struct {
	int num;
	int color;
	int use;
	int secret;
}CARD;
//덱 타입
typedef struct {
	CARD card[DECK_SIZE];
	int cardNum; //tos
	int jokerIndex[2];
	int winner;
}DECK;

typedef struct {
	int stack[13];
	int tos;
}chooseStack;

void initStack(chooseStack *stack) {
	stack->tos = 0;
}

void choosepush(chooseStack *stack,int element) {
	stack->tos++;
	stack->stack[stack->tos] = element;
}
//덱 넘버 초기화
void initCardNum(DECK *deck) {
	deck->cardNum = 0;
	deck->winner = FALSE;
	deck->jokerIndex[0] = DECK_SIZE;
	deck->jokerIndex[1] = DECK_SIZE;
}

int isEmpty(DECK *deck) {
	return (deck->cardNum == 0); //비어있으면 TRUE
}

int isFull(DECK *deck) {
	return (deck->cardNum == (DECK_SIZE - 1)); //꽉 차있으면 TRUE
}
void push(DECK *deck, CARD element) {
	if (isFull(deck)) {
		printf("덱이 꽉 차 있습니다.\n");
		return;
	}
	else {
		deck->card[++deck->cardNum] = element;
	}
}

void eliminatemyDeckPossibility(DECK *myDeck) {
	int i, j,index;
	//상대의 패를 보고 삭제
	for (index = 1; index <= myDeck->cardNum; index++) {
		if (myDeck->card[index].secret == FALSE) {
			//조커일 때
			if (myDeck->card[index].num == '-') {
				//검은 조커일 때
				if (myDeck->card[index].color == BLACK) {
					//발켜진 줄 삭제
					for (i = 2; i < 14; i++)
						possibility[i][index] = 0;
					//색깔 같은 곳 삭제
					for (j = 1; j <= myDeck->cardNum; j++)
						if (myDeck->card[j].num == '-' && myDeck->card[j].color == BLACK)
							possibility[12][j];

				}//하얀 조커일 때
				else if (myDeck->card[index].color == WHITE) {
					//발켜진 줄 삭제
					for (i = 2; i < 14; i++)
						possibility[i][index] = 0;
					//색깔 같은 곳 삭제
					for (j = 1; j <= myDeck->cardNum; j++)
						if (myDeck->card[j].num == '-' && myDeck->card[j].color == WHITE)
							possibility[12][j];
				}
			}
			else {//조커가 아닐 때
				 //발켜진 줄 삭제
				for (i = 2; i < 14; i++)
					possibility[i][index] = 0;
				//index보다 작은 곳에서 삭제(왼쪽 삭제)
				for (j = 1; j < index; j++)
					for (i = possibility[0][index] + 1; i < 13; i++) 
						possibility[i][j] = 0;
				
				//index보다 큰 곳에서 삭제(오른쪽삭제)
				for (j = index + 1; j <= myDeck->cardNum; j++)
					for (i = 2; i <= possibility[0][index] + 1; i++)
						possibility[i][j] = 0;
			}
		}
	}

	for (i = 1; i <= myDeck->cardNum; i++) {
		if (myDeck->card[i].secret == TRUE) {
			if(myDeck->card[i].num != 45)
				possibility[(myDeck->card[i].num) + 1][i] = myDeck->card[i].num;
			else
				possibility[13][i] = 45;
		}
	}
}
//처음에 목록표 초기화
void initpossibility(DECK *myDeck) {
	int i, j;
	//0행 카드번호, 1행 카드 색깔 넣기
	for (j = 1; j <= myDeck->cardNum; j++) {
		possibility[0][j] = myDeck->card[j].num;
		possibility[1][j] = myDeck->card[j].color;
	}
	//2행부터 숫자차례대로 넣기
	for (i = 2; i < 13; i++)
		for (j = 1; j < 13; j++)
			possibility[i][j] = i-1;

	for (j = 1; j < 13; j++) {
		possibility[13][j] = 45;
	}
}
//목록표와 상대패 동기화
void synchronizePossibility(DECK *myDeck, int cardNum, int cardColor) {
	int i, j, insertionPoint = 1;
	//인덱스 알아내기
	for (i = 1; i <= myDeck->cardNum; i++) {
		if (myDeck->card[i].num == cardNum &&myDeck->card[i].color == cardColor)
			break;
		insertionPoint++;
	}
	//한칸 옮기기
	if (cardNum != '-') {
		for (i = 0; i < 14; i++)
			for (j = myDeck->cardNum; j >= insertionPoint; j--) {
				possibility[i][j] = possibility[i][j-1];
			}
		//빈곳에 새로운 것 넣기
		possibility[0][insertionPoint] = cardNum;
		possibility[1][insertionPoint] = cardColor;
		for (i = 2; i < 13; i++)
			possibility[i][insertionPoint] = i - 1;
		possibility[13][insertionPoint] = 45;

	}
	else if (cardNum == '-') {
		//검정색 조커일 때
		if (cardColor == BLACK) {
			for (i = 0; i < 14; i++)
				for (j = myDeck->cardNum; j >= myDeck->jokerIndex[0]; j--) {
					possibility[i][j] = possibility[i][j - 1];
				}
			//빈곳에 새로운 것 넣기
			possibility[0][myDeck->jokerIndex[0]] = cardNum;
			possibility[1][myDeck->jokerIndex[0]] = cardColor;
			for (i = 2; i < 13; i++)
				possibility[i][myDeck->jokerIndex[0]] = i - 1;
			possibility[13][myDeck->jokerIndex[0]] = 45;
		}//하얀색 조커일 때
		else if (cardColor == WHITE) {
			for (i = 0; i < 14; i++)
				for (j = myDeck->cardNum; j >= myDeck->jokerIndex[1]; j--) {
					possibility[i][j] = possibility[i][j - 1];
				}
			//빈곳에 새로운 것 넣기
			possibility[0][myDeck->jokerIndex[1]] = cardNum;
			possibility[1][myDeck->jokerIndex[1]] = cardColor;
			for (i = 2; i < 13; i++)
				possibility[i][myDeck->jokerIndex[1]] = i - 1;
			possibility[13][myDeck->jokerIndex[1]] = 45;
		}
	}
}
//컴퓨터에 있는 덱을 빼기
void eliminateComputNum(DECK *computDeck, DECK *myDeck) {
	int i,j;

	for (i = 1; i <= computDeck->cardNum; i++) {
		for (j = 1; j <= myDeck->cardNum; j++) {
			if (computDeck->card[i].color == possibility[1][j]) {
				if (computDeck->card[i].num != '-')
					possibility[computDeck->card[i].num + 1][j] = 0;
				else {
					possibility[13][j] = 0;
				}
			}
		}
	}

}
void initDeck(DECK *deck) {
	int i;
	int num = 1;
	for (i = 2; i < 24; i++) {
		//흑색
		deck->card[i].num = num ;
		deck->card[i].color = BLACK;
		deck->card[i].use = FALSE;
		deck->card[i].secret = TRUE;
		i++;
		//백색
		deck->card[i].num = num;
		deck->card[i].color = WHITE;
		deck->card[i].use = FALSE;
		deck->card[i].secret = TRUE;
		//번호 증가
		num++;
	}
	//조커 초기화
	deck->card[0].num = '-';
	deck->card[0].color = BLACK;
	deck->card[0].use = FALSE;
	deck->card[0].secret = TRUE;
	deck->card[1].num = '-';
	deck->card[1].color = WHITE;
	deck->card[1].use = FALSE;
	deck->card[1].secret = TRUE;
	//덱의 숫자
	deck->cardNum = 24;
	deck->jokerIndex[0] = DECK_SIZE;
	deck->jokerIndex[1] = DECK_SIZE;
}
//카드 섞기
void shuffleCard(DECK *deck) {
	int i, object1,object2;
	CARD temp;
	
	for (i = 0; i < 400; i++) {
		object1 = rand() % DECK_SIZE;
		object2 = rand() % DECK_SIZE;

		temp = deck->card[object1];
		deck->card[object1] = deck->card[object2];
		deck->card[object2] = temp;
	}
}
//조커 위치에 넣기	
void jokerInsertion(DECK *deck, int turn) {
	int i, count=0;
	//컴퓨터 턴
	if (turn) {
		for (i = 1; i <= deck->cardNum; i++) {
			if (deck->card[i].num == '-')
				count++;
		}
		switch (count) {
			case 1: if (deck->card[deck->cardNum].color == BLACK && deck->jokerIndex[0] == DECK_SIZE) {
						deck->jokerIndex[0] = (rand() % deck->cardNum) + 1; break;
					}
					else if (deck->card[deck->cardNum].color == WHITE && deck->jokerIndex[1] == DECK_SIZE) {
							deck->jokerIndex[1] = (rand() % deck->cardNum) + 1; break;
					}
					break;
			case 2: if (deck->card[deck->cardNum - 1].color == BLACK && deck->jokerIndex[0] == DECK_SIZE) {
						deck->jokerIndex[0] = (rand() % deck->cardNum) + 1;
						//검은 조커와 하얀 조커의 자리가 같을 경우
						if (deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[0] != deck->cardNum - 1)
							deck->jokerIndex[1]++;
						else if ((deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[0] == deck->cardNum - 1))
							deck->jokerIndex[1]--;
					}
					if (deck->card[deck->cardNum].color == WHITE && deck->jokerIndex[1] == DECK_SIZE) {
						deck->jokerIndex[1] = (rand() % deck->cardNum) + 1; 
						//검은 조커와 하얀 조커의 자리가 같을 경우
						if (deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[1] != deck->cardNum - 1)
							deck->jokerIndex[0]++;
						else if ((deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[1] == deck->cardNum - 1))
							deck->jokerIndex[0]--;
					}
					break;
		}
	}
	//플레이어 턴
	else {
		for (i = 1; i <= deck->cardNum; i++) {
			if (deck->card[i].num == '-')
				count++;
		}
		showMydeck(deck);
		switch (count) {
			case 1: if (deck->card[deck->cardNum].color == BLACK && deck->jokerIndex[0]==DECK_SIZE) {
						deck->jokerIndex[0] = blackJokerInput();
					}
					else if (deck->card[deck->cardNum].color == WHITE && deck->jokerIndex[1] == DECK_SIZE) {
						deck->jokerIndex[1] = whiteJokerInput();
					}
					break;
			case 2: if (deck->card[deck->cardNum-1].color == BLACK && deck->jokerIndex[0] == DECK_SIZE) {
						deck->jokerIndex[0] = blackJokerInput();
						//검은 조커와 하얀 조커의 자리가 같을 경우
						if (deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[0] != deck->cardNum - 1)
							deck->jokerIndex[1]++;
						else if((deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[0] == deck->cardNum - 1))
							deck->jokerIndex[1]--;
					}
					if (deck->card[deck->cardNum].color == WHITE && deck->jokerIndex[1] == DECK_SIZE) {
							deck->jokerIndex[1] = whiteJokerInput();
							//검은 조커와 하얀 조커의 자리가 같을 경우
							if (deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[1] != deck->cardNum - 1)
								deck->jokerIndex[0]++;
							else if ((deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[1] == deck->cardNum - 1))
								deck->jokerIndex[0]--;
					}
					break;
		}
	}
}
//삽입정렬
void insertion_sort(DECK *deck, int turn) {
	int i, j;
	CARD key;
	for (i = 2; i <= deck->cardNum; i++) {
		key = deck->card[i];
		for (j = i - 1; (j >= 1) && (deck->card[j].num >= key.num); j--) {
			if (deck->card[j].num == key.num && deck->card[j].color < key.color) {
				break;
			}
			deck->card[j + 1] = deck->card[j];
		}
		deck->card[j + 1] = key;
	}
	//조커 있는지 확인
	if (deck->card[deck->cardNum - 1].num == '-' || deck->card[deck->cardNum].num == '-')
		if (deck->jokerIndex[0] == DECK_SIZE || deck->jokerIndex[1] == DECK_SIZE)
			jokerInsertion(deck,turn);
	//하얀색 조커 위치 넣기
	if (deck->jokerIndex[1] != DECK_SIZE) {
		key = deck->card[deck->cardNum];
		for (i = deck->cardNum; i >= deck->jokerIndex[1]; i--) {
			memcpy(&deck->card[i + 1], &deck->card[i], sizeof(CARD));
		}
		deck->card[deck->jokerIndex[1]] = key;

	}
	//검은색 조커 위치 넣기
	if (deck->jokerIndex[0]!=DECK_SIZE) {
		if (deck->card[deck->cardNum].num == '-' && deck->card[deck->cardNum].color == BLACK)
			key = deck->card[deck->cardNum];
		else
			key = deck->card[deck->cardNum - 1];
		for (i = deck->cardNum; i >= deck->jokerIndex[0]; i--) {
			memcpy(&deck->card[i + 1], &deck->card[i], sizeof(CARD));
			deck->card[i] = deck->card[i + 1];
		}
		deck->card[deck->jokerIndex[0]] = key;	
	}

}
//처음 덱 고르기
void chooseFirstdeck(DECK *deck, DECK *myDeck, DECK *computDeck,int turn) {
	int i, selection, count = 0;
	gotoxy(WIDTH/2-5,HEIGHT-10);
	//자신의 카드 선택
	do{
		textcolor(BLACK, WHITE);
		gotoxy(75, 43); printf("처음에 가져올 4개의 카드를 고르시오");
		gotoxy(85, 45); scanf("%d", &selection);
		selection--;
		if (deck->card[selection].use == FALSE) {
					deck->card[selection].use = TRUE;
					push(myDeck, deck->card[selection]);
					deck->cardNum--;
					count++;
					//다시 그리기
					middleShow(deck);
				}
		else if(deck->card[selection].use ==TRUE) {
			gotoxy(83, 43);
			printf("                                            ");
			printf("잘못 입력하셨습니다.");
			Sleep(200);
			gotoxy(WIDTH / 2 - 5, HEIGHT - 5);
			printf("                   ");
		}
		gotoxy(WIDTH / 2 - 5, HEIGHT - 5);
	} while (count != 4);
	//쓴것 지우기
	textcolor(BLACK, WHITE);
	gotoxy(75, 43); printf("                                            ");
	gotoxy(83, 45); printf("                                            ");
	//자신의 덱 정렬
	insertion_sort(myDeck,turn);
	initpossibility(myDeck);
	//컴퓨터의 카드 선택
	turn = COMPUTER;
	for (i = 0; i < 4; i++) {
		do {
			selection = rand() % 24;
		} while (deck->card[selection].use != FALSE);
		deck->card[selection].use = TRUE;
		push(computDeck, deck->card[selection]);
		deck->cardNum--;
	}
	//컴퓨터 덱 정렬
	insertion_sort(computDeck,turn);
	//다시 그리기(가져간것 지우기)
	middleShow(deck);
	turn = PLAYER;
}

//내 턴
void myGuessTurn(DECK *deck, DECK *myDeck, DECK *computDeck,int turn) {
	int selection,guessNum,i,secretCount=0;
	CARD tmp;

	if (deck->cardNum > 0) {
		//자신이 가져올 카드 고르기
		textcolor(BLACK, WHITE);
		gotoxy(75, 43); printf("가져올 카드의 번호를 고르시오");
		gotoxy(90, 45); scanf("%d", &selection);
		selection--;
	
		while (TRUE) {
			if (deck->card[selection].use == FALSE) {
				deck->card[selection].use = TRUE;
				push(myDeck, deck->card[selection]);
				deck->cardNum--;
				middleShow(deck);
				tmp = deck->card[selection];
				numManage(155, 43, tmp);
				break;
			}
			else if (deck->card[selection].use == TRUE) {
				gotoxy(75,43); printf("이 카드는 선택 할 수 없습니다.");
				Sleep(300);
				gotoxy(75, 45); printf("                              ");
				gotoxy(75, 43); printf("가져올 카드의 번호를 고르시오.");
				gotoxy(90, 45); scanf("%d", &selection);
			}
		}
		insertion_sort(myDeck, turn);
		synchronizePossibility(myDeck, deck->card[selection].num, deck->card[selection].color);
		//카드 보여주기
		showMydeck(myDeck);
		
		textcolor(BLACK, WHITE);
		gotoxy(83, 43);
		printf("                                            ");
		gotoxy(83, 45);
		printf("                                            ");
	}
	//추측하여 정답 맞추기
	go:
	while (TRUE) {
		textcolor(BLACK, WHITE);
		gotoxy(65,43); printf("상대방 카드의 위치번호와 추측한 숫자를 입력하세요.");
		gotoxy(85, 45); scanf("%d %d", &selection, &guessNum);

 		gotoxy(65,43); printf("                                                        ");
		gotoxy(85, 45); printf("                 ");
		//평가
		//성공했을시
		if (computDeck->card[selection].num == guessNum) {
			computDeck->card[selection].secret = FALSE;
			showComputdeck(computDeck);
			
			textcolor(BLACK, WHITE);
			gotoxy(87, 43); printf("성공!!!");
			Sleep(2000);

			secretCount = 0;
			for (i = 1; i <= computDeck->cardNum; i++) {
				if (computDeck->card[i].secret == FALSE)
					secretCount++;
			}

			if (computDeck->cardNum == secretCount) {
				myDeck->winner = TRUE;
				return;
			}
			//계속 진행여부 묻기
			while (1) {
				gotoxy(80, 43); printf("(1) Go, (2) Stop");
				gotoxy(90, 45); scanf("%d", &selection);

				gotoxy(90, 45); printf("     ");
				switch (selection) {
				case 1: goto go; break;
				case 2: return;
				default: break;
				}
			}
		}//실패 했을 시
		else if (deck->cardNum > 0) {
			if (computDeck->card[selection].num != guessNum) {
				gotoxy(87, 43); printf("실패...");
				Sleep(3000);
				gotoxy(87, 43); printf("        ");
				//방금 집어 넣은 것 패 까기
				for (i = 1; i <= myDeck->cardNum; i++) {
					if (myDeck->card[i].num == tmp.num && myDeck->card[i].color == tmp.color) {
						myDeck->card[i].secret = FALSE;
						eliminatemyDeckPossibility(myDeck);
						break;
					}
				}
				showMydeck(myDeck);
				return;
			}
		}
	}


}
//컴퓨터 턴(Ai)
void computGuessTurn(DECK *deck, DECK *myDeck, DECK *computDeck,int turn) {
	int i,j,startPoint=1;
	int min, minIndex = 1, secondCount;
	int secretCount = 0, count = 0 ,selection;
	CARD tmp;
	chooseStack stack;
	initStack(&stack);
	
	if (deck->cardNum > 0) {
		//덱에서 카드 선택
		while (TRUE) {
			selection = rand() % 24;
			if (deck->card[selection].use == FALSE) {
				deck->card[selection].use = TRUE;
				push(computDeck, deck->card[selection]);
				tmp = deck->card[selection];
				deck->cardNum--;
				middleShow(deck);
				break;
			}
		}
		insertion_sort(computDeck, turn);
		eliminateComputNum(computDeck,myDeck);
		showComputdeck(computDeck);
	}
	//맞출 가능성 높은곳 탐색
	//처음에 작은 곳 탐색
	gotoxy(15, 43); printf("생각 중...");
	Sleep(3000);

	re:
	while (myDeck->card[startPoint].secret == FALSE)
		startPoint++;
	for (i = 2; i < 14; i++) {
		if (possibility[i][startPoint] != 0)
			count++;
	}
	min = count;
	minIndex = startPoint;

	secondCount = min;
	//가장 작은 곳 탐색
	for (i = 1; i <= myDeck->cardNum; i++) {
		count = 0;
		if (myDeck->card[i + startPoint].secret == TRUE) {
			for (j = 2; j < 14; j++) {
				if (possibility[j][i + startPoint] != 0)
					count++;
			}
			if (min > count) {
				//두 번째로 작은것 저장
				secondCount = min;
				//가장 작은 것
				min = count;
				minIndex = i + startPoint;
			}
		}
	}
	//가장 작은곳에 있는 가능성 스택에 넣기
	for (i = 2; i < 14; i++) {
		if (possibility[i][minIndex] != 0)
			choosepush(&stack, possibility[i][minIndex]);
	}
	//선택
	selection = stack.stack[(rand() % stack.tos)+1];

	if (selection != 45) {
		gotoxy(13, 43); printf("%d번째는 %d입니다....", minIndex, possibility[selection + 1][minIndex]);
	}
	else {
		gotoxy(13, 43); printf("%d번째는 조커입니다....", minIndex );
	}

	//성공 
	if (selection == myDeck->card[minIndex].num) {
		myDeck->card[minIndex].secret = FALSE;
		eliminatemyDeckPossibility(myDeck);
		printf("성공!!!");
		showMydeck(myDeck);

		textcolor(BLACK, WHITE);
		Sleep(4000);
		gotoxy(13, 43); printf("                                ");

		//끝났는지 판단
		secretCount = 0;
		for (i = 1; i <= myDeck->cardNum; i++) {
			if (myDeck->card[i].secret == FALSE)
				secretCount++;
		}
		if (myDeck->cardNum == secretCount) {
			computDeck->winner = TRUE;
			return;
		}
		if (secondCount < 4)
			goto re;
	}
	//실패
	else {
		if (deck->cardNum > 0) {
			for (i = 1; i <= computDeck->cardNum; i++) {
				if ((computDeck->card[i].num == tmp.num) && (computDeck->card[i].color == tmp.color)) {
					computDeck->card[i].secret = FALSE;
					//지우기
					if (selection != 45) {
						possibility[selection + 1][minIndex] = 0;
					}
					else
						possibility[13][minIndex] = 0;

					printf("실패...");
					gotoxy(15, 45); printf("제 패는 %d입니다.",tmp.num);
					showComputdeck(computDeck);
					
					textcolor(BLACK, WHITE);
					Sleep(4000);
					gotoxy(13, 43); printf("                                ");
					gotoxy(13, 45); printf("                                ");
				}
			}
		}
	}
	
}


int main() {
	int key, selection,movement;
	DECK deck, myDeck, computDeck;
	int turn = PLAYER;

	while (1) {
		srand(time(NULL));
		//초기화
		initDeck(&deck);
		initCardNum(&myDeck);
		initCardNum(&computDeck);

		shuffleCard(&deck);

		//창 변환
		system("mode con cols=180 lines=50");
	re:
		fflush(stdin);
		cls(8, 0);
		//시작 창
		billboard();

		selection = 1;
		movement = 1;

		textcolor(12, 8);
		gotoxy(76, 35); printf("▶");
		gotoxy(80, 35); printf("1. START");
		gotoxy(80, 38); printf("2. HOW TO PLAY");
		gotoxy(80, 41); printf("3. EXIT");
		//메뉴창
		while (1) {
			if (kbhit() != 0) {  // 키보드가 눌려져 있으면
				key = getch(); // key 값을 읽는다
				if (key == 13)
					break;
				if (key == SPECIAL1 || key == SPECIAL2) { // 만약 특수키
														// 예를 들어 UP key의 경우 0xe0 0x48 두개의 문자가 들어온다.
					key = getch();
					switch (key) {
					case UP: if (selection > 1) {
						gotoxy(76, movement + 34); printf("  "); selection--; movement -= 3; gotoxy(76, movement + 34); printf("▶");
					}break;
					case DOWN:if (selection < 3) {
						gotoxy(76, movement + 34); printf("  "); selection++; movement += 3; gotoxy(76, movement + 34); printf("▶");
					} break;
					default:;// 방향키가 아니면 멈춘다

					}
				}
			}
		}

		cls(WHITE, BLACK);
		switch (selection) {
		case 1:
			cls(WHITE, BLACK);
			drawBox();
			//덱 보여주기
			middleShow(&deck);
			//처음 덱 고르기
			chooseFirstdeck(&deck, &myDeck, &computDeck, turn);
			//내패와 컴퓨터 패 보이기
			showMydeck(&myDeck);
			showComputdeck(&computDeck);
			//턴바이 턴
			while (TRUE) {
				turn = PLAYER;
				myGuessTurn(&deck, &myDeck, &computDeck, turn);
				turn = COMPUTER;
				if (myDeck.winner == TRUE) {
					cls(8, 0);
					youWin();
					Sleep(5000);
					break;
				}
				computGuessTurn(&deck, &myDeck, &computDeck, turn);
				if (computDeck.winner == TRUE) {
					cls(8, 0);
					youLose();
					Sleep(5000);
					break;
				}
				turn = PLAYER;
			}

			break;
		case 2:
			cls(8, 12);
			gotoxy(20, 10); printf("1. 중앙에서 4개의 패를 가져온다.");
			gotoxy(20, 12); printf("2. 패가 순서대로 정렬됩니다.(숫자순으로 정렬되고 숫자가 같을시 검은색이 먼저, 조커는 자신이 원하는 위치에 둘 수 있습니다)");
			gotoxy(20, 14); printf("3. 자신의 턴이 되면 중앙에서 1개의 패를 가져오고 정렬합니다. 그후 상대의 패를 추리합니다.");
			gotoxy(20, 15); printf("   맞추면 상대의 패는 보여지고 틀리면 자신이 최근에 가져온 패가 상대에게 보입니다.");
			gotoxy(20, 17); printf("4. 상대의 패를 먼저 다 맞춘 사람이 승리");
			gotoxy(20, 20); printf("Press any key");

			while (1) {
				if (kbhit() != 0) {
					goto re;
				}
			}


		case 3:
			exit(1);

		}
	}
}