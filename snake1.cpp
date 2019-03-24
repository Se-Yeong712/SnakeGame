#include <windows.h> 
#include <iostream> 
#include <conio.h> //방향키 입력받기
#include <random> 
#include <time.h> //srand(time(NULL))

using namespace std;

#define LEFT 75 
#define RIGHT 77 
#define UP 72 
#define DOWN 80 

int level;//난이도
char board[40][20] = { ' ' };

void gotoxy(int x, int y)
{
	COORD pos = { x, y };//COORD 좌표값을 지정하는 구조체
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);//지정한 x, y 좌표로 커서를 이동시킴
}


struct position{
	int x;
	int y;
};
struct snake{
	int direction = RIGHT;
	int length = 3;
	position pos[101]; //뱀 길이 일단 101로 잡음.

};
int gameover = 0; //게임 오버 변수
snake snake1; //뱀

void printscreen(){// 판을 출력하는 함수
	for (int i = 0; i < 20; i++){
		for (int j = 0; j < 40; j++)
			cout<< board[j][i];
		cout << endl;
	}
}
void apple(){ //$를 만드는 함수, 뱀의 먹이(먹을 때마다 몸이 한 칸씩 길어진다)
	while (1){
		int x = rand() % 38 + 1; //랜덤으로 x, y좌표에 $가 생기게 한다.
		int y = rand() % 18 + 1;

		if (board[x][y] != 'O'){ //꼬리에서 $가 생기면 안 되니까 
			board[x][y] = '$'; //board에 
			gotoxy(x, y); //커서 이동
			printf("$"); //이동한 곳에 @를 출력
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
void obstacle(){ //장애물 #를 만드는 함수
	while (1){
		int x = rand() % 38 + 1; //랜덤으로 x, y좌표에 #가 생기게 한다.
		int y = rand() % 18 + 1;

		if (board[x][y] != 'O' && board[x][y] != '$'){ //뱀의 몸통이나 밥$에 장애물 #가 생기면 안 되니까 
			board[x][y] = '#'; //board에 
			gotoxy(x, y); //커서 이동
			printf("#"); //이동한 곳에 #를 출력
			return;
		}//if
	}
}


void update(){

	gotoxy(snake1.pos[snake1.length - 1].x, snake1.pos[snake1.length - 1].y);
	printf(" ");

	for (int i = snake1.length - 1; i >= 0; i--){
		snake1.pos[i + 1].x = snake1.pos[i].x;
		snake1.pos[i + 1].y = snake1.pos[i].y;
	}
	
	if (snake1.direction == RIGHT) snake1.pos[0].x++; //키보드 방향 오른쪽 눌렀을 때
	if (snake1.direction == LEFT) snake1.pos[0].x--; //키보드 방향 왼쪽 눌렀을 때
	if (snake1.direction == UP) snake1.pos[0].y--; //키보드 방향 위 쪽 눌렀을 때
	if (snake1.direction == DOWN) snake1.pos[0].y++; //키보드 방향 아래쪽 눌렀을 때

	if (board[snake1.pos[0].x][snake1.pos[0].y] == '#' || board[snake1.pos[0].x][snake1.pos[0].y] == 'O'){//벽에 닿거나 자신의 몸에 닿으면
		gameover = 1;//게임 오버
		return;
	}

	if (board[snake1.pos[0].x][snake1.pos[0].y] == '$'){//$를 성공적으로 먹으면
		apple();//밥 함수 호출
		obstacle();//장애물 함수 호출 ($를 먹으면 장애물이 하나씩 생김)
		snake1.length++;//꼬리가 길어진다.
	}
	gotoxy(45, 8);//이 위치에
	cout << "현재 뱀 꼬리의 개수 : " << snake1.length;//현재 뱀 꼬리의 개수 출력
	gotoxy(45, 9);

	board[snake1.pos[0].x][snake1.pos[0].y] = 'O';//뱀 저장
	board[snake1.pos[snake1.length - 1].x][snake1.pos[snake1.length - 1].y] = ' '; //빈 공간을 board에 저장

	gotoxy(snake1.pos[0].x, snake1.pos[0].y);
	cout << "0";
}//update함수



void explain(){ //설명하는 함수 (게임 튜토리얼)
	int key;

	cout << endl; cout << endl; cout << endl;
	cout << " + -------------------------- + " << endl;
	cout << " |     S N A K E  G A M E     | " << endl;
	cout << " + -------------------------- + " << endl;
	cout << endl;
	cout << "         <<게임 규칙>>" << endl;
	cout << "뱀의 머리가 꼬리에 닿지 않게 피하면서\n$를 먹으세요! $를 먹을수록 뱀의 꼬리가 하나씩 길어지고\n꼬리의 수가 50개가 되면 승리합니다!!\n" << endl;
	cout << "* 뱀이 벽(#)에 닿으면 GAME OVER!" << endl;
	cout << "* $를 먹을 때마다 장애물 #가 생기는데 #과 닿으면 GAME OVER!\n\n" << endl;
	cout << "<<<<<<<<<<<<<아무키나 누르면 게임이 시작됩니다>>>>>>>>>>>>>>>>>>";
	while (1){
		if (_kbhit()){ //키입력받음 
			key = _getch();
			return; //아니면 그냥 계속 진행 
		}
	}
	
}



int gamestart(){
	while (_kbhit()) _getch(); //버퍼에 있는 키값을 버림 
	explain();//설명하는 함수 호출
	system("cls"); //화면 깨끗하게 지우기
	int nan; //난이도 변수
	do{
		cout << "난이도를 선택하세요. 1~5" << endl;//난이도 올라갈수록 뱀의 속도가 빨라짐.
		cin >> nan; //난이도를 입력받기
	} while (nan > 5 || nan < 1);
	Sleep(100);
	system("cls"); //화면 지우기
	return 40 * (20 - (nan+10)); 
}


int main(){
	srand(time(NULL));

	level = gamestart();
	{
		for (int i = 0; i < 40; i++){//가로 벽 그리기
			board[i][0] = '#';
			board[i][19] = '#';
		}
		for (int i = 0; i < 20; i++){//세로 벽 그리기
			board[0][i] = '#';
			board[39][i] = '#';
		}
		printscreen();
	}//기본 판 출력 

	snake1.pos[0].x = 20; //처음 세 칸짜리 뱀 위치
	snake1.pos[0].y = 10; // "
	snake1.pos[1].x = 19; // "
	snake1.pos[1].y = 10; // " 
	snake1.pos[2].x = 18; // "
	snake1.pos[2].y = 10; // "
	apple(); //처음 @ 생산 
	while (gameover == 0){// 끝날 때까지
		Sleep(level); //기다린다.
		if (_kbhit()){//입력이 됬다.
			if (_getch() == 224){
				snake1.direction = _getch();//어떤 키를 입력했는지 
			}
		}
		if (snake1.length == 50){ //꼬리가 50개면
			cout << "이겼어요!" << endl; //승리
			return 0;
		}
		update(); //화면 업데이트
	}//while
	if (gameover == 1){ //게임 끝
		cout << "GAME OVER" << endl;
	}
	gotoxy(0, 20);
	return 0;
}


