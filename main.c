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
#define SPECIAL1 0xe0 // Ư��Ű�� 0xe0 + key ������ �����ȴ�.
#define SPECIAL2 0x00 // keypad ��� 0x00 + key �� �����ȴ�.

int possibility[14][13];

//ī�� Ÿ��
typedef struct {
	int num;
	int color;
	int use;
	int secret;
}CARD;
//�� Ÿ��
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
//�� �ѹ� �ʱ�ȭ
void initCardNum(DECK *deck) {
	deck->cardNum = 0;
	deck->winner = FALSE;
	deck->jokerIndex[0] = DECK_SIZE;
	deck->jokerIndex[1] = DECK_SIZE;
}

int isEmpty(DECK *deck) {
	return (deck->cardNum == 0); //��������� TRUE
}

int isFull(DECK *deck) {
	return (deck->cardNum == (DECK_SIZE - 1)); //�� �������� TRUE
}
void push(DECK *deck, CARD element) {
	if (isFull(deck)) {
		printf("���� �� �� �ֽ��ϴ�.\n");
		return;
	}
	else {
		deck->card[++deck->cardNum] = element;
	}
}

void eliminatemyDeckPossibility(DECK *myDeck) {
	int i, j,index;
	//����� �и� ���� ����
	for (index = 1; index <= myDeck->cardNum; index++) {
		if (myDeck->card[index].secret == FALSE) {
			//��Ŀ�� ��
			if (myDeck->card[index].num == '-') {
				//���� ��Ŀ�� ��
				if (myDeck->card[index].color == BLACK) {
					//������ �� ����
					for (i = 2; i < 14; i++)
						possibility[i][index] = 0;
					//���� ���� �� ����
					for (j = 1; j <= myDeck->cardNum; j++)
						if (myDeck->card[j].num == '-' && myDeck->card[j].color == BLACK)
							possibility[12][j];

				}//�Ͼ� ��Ŀ�� ��
				else if (myDeck->card[index].color == WHITE) {
					//������ �� ����
					for (i = 2; i < 14; i++)
						possibility[i][index] = 0;
					//���� ���� �� ����
					for (j = 1; j <= myDeck->cardNum; j++)
						if (myDeck->card[j].num == '-' && myDeck->card[j].color == WHITE)
							possibility[12][j];
				}
			}
			else {//��Ŀ�� �ƴ� ��
				 //������ �� ����
				for (i = 2; i < 14; i++)
					possibility[i][index] = 0;
				//index���� ���� ������ ����(���� ����)
				for (j = 1; j < index; j++)
					for (i = possibility[0][index] + 1; i < 13; i++) 
						possibility[i][j] = 0;
				
				//index���� ū ������ ����(�����ʻ���)
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
//ó���� ���ǥ �ʱ�ȭ
void initpossibility(DECK *myDeck) {
	int i, j;
	//0�� ī���ȣ, 1�� ī�� ���� �ֱ�
	for (j = 1; j <= myDeck->cardNum; j++) {
		possibility[0][j] = myDeck->card[j].num;
		possibility[1][j] = myDeck->card[j].color;
	}
	//2����� �������ʴ�� �ֱ�
	for (i = 2; i < 13; i++)
		for (j = 1; j < 13; j++)
			possibility[i][j] = i-1;

	for (j = 1; j < 13; j++) {
		possibility[13][j] = 45;
	}
}
//���ǥ�� ����� ����ȭ
void synchronizePossibility(DECK *myDeck, int cardNum, int cardColor) {
	int i, j, insertionPoint = 1;
	//�ε��� �˾Ƴ���
	for (i = 1; i <= myDeck->cardNum; i++) {
		if (myDeck->card[i].num == cardNum &&myDeck->card[i].color == cardColor)
			break;
		insertionPoint++;
	}
	//��ĭ �ű��
	if (cardNum != '-') {
		for (i = 0; i < 14; i++)
			for (j = myDeck->cardNum; j >= insertionPoint; j--) {
				possibility[i][j] = possibility[i][j-1];
			}
		//����� ���ο� �� �ֱ�
		possibility[0][insertionPoint] = cardNum;
		possibility[1][insertionPoint] = cardColor;
		for (i = 2; i < 13; i++)
			possibility[i][insertionPoint] = i - 1;
		possibility[13][insertionPoint] = 45;

	}
	else if (cardNum == '-') {
		//������ ��Ŀ�� ��
		if (cardColor == BLACK) {
			for (i = 0; i < 14; i++)
				for (j = myDeck->cardNum; j >= myDeck->jokerIndex[0]; j--) {
					possibility[i][j] = possibility[i][j - 1];
				}
			//����� ���ο� �� �ֱ�
			possibility[0][myDeck->jokerIndex[0]] = cardNum;
			possibility[1][myDeck->jokerIndex[0]] = cardColor;
			for (i = 2; i < 13; i++)
				possibility[i][myDeck->jokerIndex[0]] = i - 1;
			possibility[13][myDeck->jokerIndex[0]] = 45;
		}//�Ͼ�� ��Ŀ�� ��
		else if (cardColor == WHITE) {
			for (i = 0; i < 14; i++)
				for (j = myDeck->cardNum; j >= myDeck->jokerIndex[1]; j--) {
					possibility[i][j] = possibility[i][j - 1];
				}
			//����� ���ο� �� �ֱ�
			possibility[0][myDeck->jokerIndex[1]] = cardNum;
			possibility[1][myDeck->jokerIndex[1]] = cardColor;
			for (i = 2; i < 13; i++)
				possibility[i][myDeck->jokerIndex[1]] = i - 1;
			possibility[13][myDeck->jokerIndex[1]] = 45;
		}
	}
}
//��ǻ�Ϳ� �ִ� ���� ����
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
		//���
		deck->card[i].num = num ;
		deck->card[i].color = BLACK;
		deck->card[i].use = FALSE;
		deck->card[i].secret = TRUE;
		i++;
		//���
		deck->card[i].num = num;
		deck->card[i].color = WHITE;
		deck->card[i].use = FALSE;
		deck->card[i].secret = TRUE;
		//��ȣ ����
		num++;
	}
	//��Ŀ �ʱ�ȭ
	deck->card[0].num = '-';
	deck->card[0].color = BLACK;
	deck->card[0].use = FALSE;
	deck->card[0].secret = TRUE;
	deck->card[1].num = '-';
	deck->card[1].color = WHITE;
	deck->card[1].use = FALSE;
	deck->card[1].secret = TRUE;
	//���� ����
	deck->cardNum = 24;
	deck->jokerIndex[0] = DECK_SIZE;
	deck->jokerIndex[1] = DECK_SIZE;
}
//ī�� ����
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
//��Ŀ ��ġ�� �ֱ�	
void jokerInsertion(DECK *deck, int turn) {
	int i, count=0;
	//��ǻ�� ��
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
						//���� ��Ŀ�� �Ͼ� ��Ŀ�� �ڸ��� ���� ���
						if (deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[0] != deck->cardNum - 1)
							deck->jokerIndex[1]++;
						else if ((deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[0] == deck->cardNum - 1))
							deck->jokerIndex[1]--;
					}
					if (deck->card[deck->cardNum].color == WHITE && deck->jokerIndex[1] == DECK_SIZE) {
						deck->jokerIndex[1] = (rand() % deck->cardNum) + 1; 
						//���� ��Ŀ�� �Ͼ� ��Ŀ�� �ڸ��� ���� ���
						if (deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[1] != deck->cardNum - 1)
							deck->jokerIndex[0]++;
						else if ((deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[1] == deck->cardNum - 1))
							deck->jokerIndex[0]--;
					}
					break;
		}
	}
	//�÷��̾� ��
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
						//���� ��Ŀ�� �Ͼ� ��Ŀ�� �ڸ��� ���� ���
						if (deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[0] != deck->cardNum - 1)
							deck->jokerIndex[1]++;
						else if((deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[0] == deck->cardNum - 1))
							deck->jokerIndex[1]--;
					}
					if (deck->card[deck->cardNum].color == WHITE && deck->jokerIndex[1] == DECK_SIZE) {
							deck->jokerIndex[1] = whiteJokerInput();
							//���� ��Ŀ�� �Ͼ� ��Ŀ�� �ڸ��� ���� ���
							if (deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[1] != deck->cardNum - 1)
								deck->jokerIndex[0]++;
							else if ((deck->jokerIndex[0] == deck->jokerIndex[1] && deck->jokerIndex[1] == deck->cardNum - 1))
								deck->jokerIndex[0]--;
					}
					break;
		}
	}
}
//��������
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
	//��Ŀ �ִ��� Ȯ��
	if (deck->card[deck->cardNum - 1].num == '-' || deck->card[deck->cardNum].num == '-')
		if (deck->jokerIndex[0] == DECK_SIZE || deck->jokerIndex[1] == DECK_SIZE)
			jokerInsertion(deck,turn);
	//�Ͼ�� ��Ŀ ��ġ �ֱ�
	if (deck->jokerIndex[1] != DECK_SIZE) {
		key = deck->card[deck->cardNum];
		for (i = deck->cardNum; i >= deck->jokerIndex[1]; i--) {
			memcpy(&deck->card[i + 1], &deck->card[i], sizeof(CARD));
		}
		deck->card[deck->jokerIndex[1]] = key;

	}
	//������ ��Ŀ ��ġ �ֱ�
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
//ó�� �� ����
void chooseFirstdeck(DECK *deck, DECK *myDeck, DECK *computDeck,int turn) {
	int i, selection, count = 0;
	gotoxy(WIDTH/2-5,HEIGHT-10);
	//�ڽ��� ī�� ����
	do{
		textcolor(BLACK, WHITE);
		gotoxy(75, 43); printf("ó���� ������ 4���� ī�带 ���ÿ�");
		gotoxy(85, 45); scanf("%d", &selection);
		selection--;
		if (deck->card[selection].use == FALSE) {
					deck->card[selection].use = TRUE;
					push(myDeck, deck->card[selection]);
					deck->cardNum--;
					count++;
					//�ٽ� �׸���
					middleShow(deck);
				}
		else if(deck->card[selection].use ==TRUE) {
			gotoxy(83, 43);
			printf("                                            ");
			printf("�߸� �Է��ϼ̽��ϴ�.");
			Sleep(200);
			gotoxy(WIDTH / 2 - 5, HEIGHT - 5);
			printf("                   ");
		}
		gotoxy(WIDTH / 2 - 5, HEIGHT - 5);
	} while (count != 4);
	//���� �����
	textcolor(BLACK, WHITE);
	gotoxy(75, 43); printf("                                            ");
	gotoxy(83, 45); printf("                                            ");
	//�ڽ��� �� ����
	insertion_sort(myDeck,turn);
	initpossibility(myDeck);
	//��ǻ���� ī�� ����
	turn = COMPUTER;
	for (i = 0; i < 4; i++) {
		do {
			selection = rand() % 24;
		} while (deck->card[selection].use != FALSE);
		deck->card[selection].use = TRUE;
		push(computDeck, deck->card[selection]);
		deck->cardNum--;
	}
	//��ǻ�� �� ����
	insertion_sort(computDeck,turn);
	//�ٽ� �׸���(�������� �����)
	middleShow(deck);
	turn = PLAYER;
}

//�� ��
void myGuessTurn(DECK *deck, DECK *myDeck, DECK *computDeck,int turn) {
	int selection,guessNum,i,secretCount=0;
	CARD tmp;

	if (deck->cardNum > 0) {
		//�ڽ��� ������ ī�� ����
		textcolor(BLACK, WHITE);
		gotoxy(75, 43); printf("������ ī���� ��ȣ�� ���ÿ�");
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
				gotoxy(75,43); printf("�� ī��� ���� �� �� �����ϴ�.");
				Sleep(300);
				gotoxy(75, 45); printf("                              ");
				gotoxy(75, 43); printf("������ ī���� ��ȣ�� ���ÿ�.");
				gotoxy(90, 45); scanf("%d", &selection);
			}
		}
		insertion_sort(myDeck, turn);
		synchronizePossibility(myDeck, deck->card[selection].num, deck->card[selection].color);
		//ī�� �����ֱ�
		showMydeck(myDeck);
		
		textcolor(BLACK, WHITE);
		gotoxy(83, 43);
		printf("                                            ");
		gotoxy(83, 45);
		printf("                                            ");
	}
	//�����Ͽ� ���� ���߱�
	go:
	while (TRUE) {
		textcolor(BLACK, WHITE);
		gotoxy(65,43); printf("���� ī���� ��ġ��ȣ�� ������ ���ڸ� �Է��ϼ���.");
		gotoxy(85, 45); scanf("%d %d", &selection, &guessNum);

 		gotoxy(65,43); printf("                                                        ");
		gotoxy(85, 45); printf("                 ");
		//��
		//����������
		if (computDeck->card[selection].num == guessNum) {
			computDeck->card[selection].secret = FALSE;
			showComputdeck(computDeck);
			
			textcolor(BLACK, WHITE);
			gotoxy(87, 43); printf("����!!!");
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
			//��� ���࿩�� ����
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
		}//���� ���� ��
		else if (deck->cardNum > 0) {
			if (computDeck->card[selection].num != guessNum) {
				gotoxy(87, 43); printf("����...");
				Sleep(3000);
				gotoxy(87, 43); printf("        ");
				//��� ���� ���� �� �� ���
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
//��ǻ�� ��(Ai)
void computGuessTurn(DECK *deck, DECK *myDeck, DECK *computDeck,int turn) {
	int i,j,startPoint=1;
	int min, minIndex = 1, secondCount;
	int secretCount = 0, count = 0 ,selection;
	CARD tmp;
	chooseStack stack;
	initStack(&stack);
	
	if (deck->cardNum > 0) {
		//������ ī�� ����
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
	//���� ���ɼ� ������ Ž��
	//ó���� ���� �� Ž��
	gotoxy(15, 43); printf("���� ��...");
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
	//���� ���� �� Ž��
	for (i = 1; i <= myDeck->cardNum; i++) {
		count = 0;
		if (myDeck->card[i + startPoint].secret == TRUE) {
			for (j = 2; j < 14; j++) {
				if (possibility[j][i + startPoint] != 0)
					count++;
			}
			if (min > count) {
				//�� ��°�� ������ ����
				secondCount = min;
				//���� ���� ��
				min = count;
				minIndex = i + startPoint;
			}
		}
	}
	//���� �������� �ִ� ���ɼ� ���ÿ� �ֱ�
	for (i = 2; i < 14; i++) {
		if (possibility[i][minIndex] != 0)
			choosepush(&stack, possibility[i][minIndex]);
	}
	//����
	selection = stack.stack[(rand() % stack.tos)+1];

	if (selection != 45) {
		gotoxy(13, 43); printf("%d��°�� %d�Դϴ�....", minIndex, possibility[selection + 1][minIndex]);
	}
	else {
		gotoxy(13, 43); printf("%d��°�� ��Ŀ�Դϴ�....", minIndex );
	}

	//���� 
	if (selection == myDeck->card[minIndex].num) {
		myDeck->card[minIndex].secret = FALSE;
		eliminatemyDeckPossibility(myDeck);
		printf("����!!!");
		showMydeck(myDeck);

		textcolor(BLACK, WHITE);
		Sleep(4000);
		gotoxy(13, 43); printf("                                ");

		//�������� �Ǵ�
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
	//����
	else {
		if (deck->cardNum > 0) {
			for (i = 1; i <= computDeck->cardNum; i++) {
				if ((computDeck->card[i].num == tmp.num) && (computDeck->card[i].color == tmp.color)) {
					computDeck->card[i].secret = FALSE;
					//�����
					if (selection != 45) {
						possibility[selection + 1][minIndex] = 0;
					}
					else
						possibility[13][minIndex] = 0;

					printf("����...");
					gotoxy(15, 45); printf("�� �д� %d�Դϴ�.",tmp.num);
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
		//�ʱ�ȭ
		initDeck(&deck);
		initCardNum(&myDeck);
		initCardNum(&computDeck);

		shuffleCard(&deck);

		//â ��ȯ
		system("mode con cols=180 lines=50");
	re:
		fflush(stdin);
		cls(8, 0);
		//���� â
		billboard();

		selection = 1;
		movement = 1;

		textcolor(12, 8);
		gotoxy(76, 35); printf("��");
		gotoxy(80, 35); printf("1. START");
		gotoxy(80, 38); printf("2. HOW TO PLAY");
		gotoxy(80, 41); printf("3. EXIT");
		//�޴�â
		while (1) {
			if (kbhit() != 0) {  // Ű���尡 ������ ������
				key = getch(); // key ���� �д´�
				if (key == 13)
					break;
				if (key == SPECIAL1 || key == SPECIAL2) { // ���� Ư��Ű
														// ���� ��� UP key�� ��� 0xe0 0x48 �ΰ��� ���ڰ� ���´�.
					key = getch();
					switch (key) {
					case UP: if (selection > 1) {
						gotoxy(76, movement + 34); printf("  "); selection--; movement -= 3; gotoxy(76, movement + 34); printf("��");
					}break;
					case DOWN:if (selection < 3) {
						gotoxy(76, movement + 34); printf("  "); selection++; movement += 3; gotoxy(76, movement + 34); printf("��");
					} break;
					default:;// ����Ű�� �ƴϸ� �����

					}
				}
			}
		}

		cls(WHITE, BLACK);
		switch (selection) {
		case 1:
			cls(WHITE, BLACK);
			drawBox();
			//�� �����ֱ�
			middleShow(&deck);
			//ó�� �� ����
			chooseFirstdeck(&deck, &myDeck, &computDeck, turn);
			//���п� ��ǻ�� �� ���̱�
			showMydeck(&myDeck);
			showComputdeck(&computDeck);
			//�Ϲ��� ��
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
			gotoxy(20, 10); printf("1. �߾ӿ��� 4���� �и� �����´�.");
			gotoxy(20, 12); printf("2. �а� ������� ���ĵ˴ϴ�.(���ڼ����� ���ĵǰ� ���ڰ� ������ �������� ����, ��Ŀ�� �ڽ��� ���ϴ� ��ġ�� �� �� �ֽ��ϴ�)");
			gotoxy(20, 14); printf("3. �ڽ��� ���� �Ǹ� �߾ӿ��� 1���� �и� �������� �����մϴ�. ���� ����� �и� �߸��մϴ�.");
			gotoxy(20, 15); printf("   ���߸� ����� �д� �������� Ʋ���� �ڽ��� �ֱٿ� ������ �а� ��뿡�� ���Դϴ�.");
			gotoxy(20, 17); printf("4. ����� �и� ���� �� ���� ����� �¸�");
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