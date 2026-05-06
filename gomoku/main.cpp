#include <stdio.h>
#include <graphics.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>


char HC = '@', BC = 'O';

//随机数生成
static short random(short min, short max) {
	Sleep(100);
	srand((unsigned int)time(NULL) ^ clock());
	return rand() % (max - min + 1) + min;
}

//数值棋盘初始化
static void boardSet(char board[21][21]) {
	int x, y;
	for (y = 0; y < 21; y++) {
		for (x = 0; x < 21; x++) {
			board[y][x] = ' ';
		}
	}
	for (y = 0; y < 21; y++) {
		board[y][0] = board[y][20] = '#';
	}
	for (x = 0; x < 21; x++) {
		board[0][x] = board[20][x] = '#';
	}
}

//AI权重棋盘初始化
static void AISet(int AISave[21][21]) {
	int x, y;
	for (y = 0; y < 21; y++) {
		for (x = 0; x < 21; x++) {
			AISave[y][x] = 0;
		}
	}
	for (y = 0; y < 21; y++) {
		AISave[y][0] = AISave[y][20] = -1;
	}
	for (x = 0; x < 21; x++) {
		AISave[0][x] = AISave[20][x] = -1;
	}
}

//窗口棋盘绘画
static void Load() {
	//窗口初始化
	initgraph(630, 630, EX_NOCLOSE);
	loadimage(NULL, _T("assets\\background1.png"));
}

//打印
static void PrintB(char array[21][21]) {
	int x, y;
	for (y = 0; y < 21; y++) {
		for (x = 0; x < 21; x++) {
			printf("%c ", array[y][x]);
		}
		printf("\n");
	}
}
static void PrintA(int AISave[21][21]) {
	int x, y;
	for (y = 0; y < 21; y++) {
		for (x = 0; x < 21; x++) {
			printf("%d ", AISave[y][x]);
		}
		printf("\n");
	}
}
//判断输赢
static int judgmentWin(char board[21][21], int x, int y, char chess) {
	int number, d;
	//横
	number = 1;
	for (d = 1;d <= 4;d++) {
		if (board[y][x + d] != chess) break;
		number++;
	}
	for (d = 1;d <= 4;d++) {
		if (board[y][x - d] != chess) break;
		number++;
	}
	if (number >= 5) return 1;
	//竖
	number = 1;
	for (d = 1;d <= 4;d++) {
		if (board[y + d][x] != chess) break;
		number++;
	}
	for (d = 1;d <= 4;d++) {
		if (board[y - d][x] != chess) break;
		number++;
	}
	if (number >= 5) return 1;
	//左斜
	number = 1;
	for (d = 1;d <= 4;d++) {
		if (board[y - d][x - d] != chess) break;
		number++;
	}
	for (d = 1;d <= 4;d++) {
		if (board[y + d][x + d] != chess) break;
		number++;
	}
	if (number >= 5) return 1;
	//右斜
	number = 1;
	for (d = 1;d <= 4;d++) {
		if (board[y - d][x + d] != chess) break;
		number++;
	}
	for (d = 1;d <= 4;d++) {
		if (board[y + d][x - d] != chess) break;
		number++;
	}
	if (number >= 5) return 1;
	else return 0;
}

//人类方下棋绘画
static void Hplace(int x, int y, char board[21][21]) {
	board[y][x] = HC;
	setfillcolor(BLACK);
	solidcircle(x * 30 + 15, y * 30 + 15, 12);
	if (judgmentWin(board, x, y, HC)) {
		//输出胜利
		setbkmode(TRANSPARENT);
		settextstyle(50, 0, _T("Courier"));
		settextcolor(GREEN);
		RECT r = { 0, 0, 630, 630 };
		drawtext(_T("获胜"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		settextstyle(30, 0, _T("Courier"));
		settextcolor(BLACK);
		RECT a = { 0, 0, 630, 700 };
		drawtext(_T("按下任意键退出程序"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		ExMessage ck = getmessage(EX_KEY);
		exit(0);
	}
}

//机器方下棋绘画
static void Bplace(int x, int y, char board[21][21]) {
	board[y][x] = BC;
	setfillcolor(WHITE);
	solidcircle(x * 30 + 15, y * 30 + 15, 12);
	if (judgmentWin(board, x, y, BC)) {
		//输出胜利
		setbkmode(TRANSPARENT);
		settextstyle(50, 0, _T("Courier"));
		settextcolor(RED);
		RECT r = { 0, 0, 630, 630 };
		drawtext(_T("失败"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		settextstyle(30, 0, _T("Courier"));
		settextcolor(BLACK);
		RECT a = { 0, 0, 630, 700 };
		drawtext(_T("按下任意键退出程序"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		ExMessage ck = getmessage(EX_KEY);
		exit(0);
	}
}

//判断等级
static void SaveLevelJudgment(char board[21][21], int AISave[21][21], int y, int x, int dy1, int dx1, int dy2, int dx2, int sum) {
	if (board[y + dy1][x + dx1] == ' ') {
		AISave[y + dy1][x + dx1] = sum;
	}
	if (board[y + dy2][x + dx2] == ' ') {
		AISave[y + dy2][x + dx2] = sum;
	}
	if (sum == 3 && board[y + dy1][x + dx1] == ' ' && board[y + dy2][x + dx2] == ' ') {
		if (random(1, 2) == 1) {
			AISave[y + dy1][x + dx1] = 4;
		}
		else {
			AISave[y + dy2][x + dx2] = 4;
		}
	}
}

//判断进攻等级
static void AtLevelJudgment(char board[21][21], int AIAt[21][21], int y, int x, int dy1, int dx1, int dy2, int dx2, int sum) {
	if (board[y + dy1][x + dx1] == ' ') {
		AIAt[y + dy1][x + dx1] = sum;
	}
	if (board[y + dy2][x + dx2] == ' ') {
		AIAt[y + dy2][x + dx2] = sum;
	}
}
//生成防守等级
static void Savelevel(char board[21][21], int AISave[21][21], int x, int y) {
	int d1, d2;

	AISave[y][x] = 0;
	//横
	for (d1 = 1;d1 < 5;d1++) {
		if (board[y][x + d1] != HC) {
			break;
		}
	}
	for (d2 = 1;d2 < 5;d2++) {
		if (board[y][x - d2] != HC) {
			break;
		}
	}
	SaveLevelJudgment(board, AISave, y, x, 0, d1, 0, -d2, d1 + d2 - 1);
	//竖
	for (d1 = 1;d1 < 5;d1++) {
		if (board[y + d1][x] != HC) {
			break;
		}
	}
	for (d2 = 1;d2 < 5;d2++) {
		if (board[y - d2][x] != HC) {
			break;
		}
	}
	SaveLevelJudgment(board, AISave, y, x, d1, 0, -d2, 0, d1 + d2 - 1);
	//正斜
	for (d1 = 1;d1 < 5;d1++) {
		if (board[y - d1][x + d1] != HC) {
			break;
		}
	}
	for (d2 = 1;d2 < 5;d2++) {
		if (board[y + d2][x - d2] != HC) {
			break;
		}
	}
	SaveLevelJudgment(board, AISave, y, x, -d1, d1, d2, -d2, d1 + d2 - 1);
	//反斜
	for (d1 = 1;d1 < 5;d1++) {
		if (board[y + d1][x + d1] != HC) {
			break;
		}
	}
	for (d2 = 1;d2 < 5;d2++) {
		if (board[y - d2][x - d2] != HC) {
			break;
		}
	}
	SaveLevelJudgment(board, AISave, y, x, d1, d1, -d2, -d2, d1 + d2 - 1);


}

//生成进攻等级
static void Atlevel(char board[21][21], int AIAt[21][21], int x, int y) {
	int d1, d2;

	AIAt[y][x] = 0;
	//横
	for (d1 = 1;d1 < 5;d1++) {
		if (board[y][x + d1] != BC) {
			break;
		}
	}
	for (d2 = 1;d2 < 5;d2++) {
		if (board[y][x - d2] != BC) {
			break;
		}
	}
	AtLevelJudgment(board, AIAt, y, x, 0, d1, 0, -d2, d1 + d2 - 1);
	//竖
	for (d1 = 1;d1 < 5;d1++) {
		if (board[y + d1][x] != BC) {
			break;
		}
	}
	for (d2 = 1;d2 < 5;d2++) {
		if (board[y - d2][x] != BC) {
			break;
		}
	}
	AtLevelJudgment(board, AIAt, y, x, d1, 0, -d2, 0, d1 + d2 - 1);
	//正斜
	for (d1 = 1;d1 < 5;d1++) {
		if (board[y - d1][x + d1] != BC) {
			break;
		}
	}
	for (d2 = 1;d2 < 5;d2++) {
		if (board[y + d2][x - d2] != BC) {
			break;
		}
	}
	AtLevelJudgment(board, AIAt, y, x, -d1, d1, d2, -d2, d1 + d2 - 1);
	//反斜
	for (d1 = 1;d1 < 5;d1++) {
		if (board[y + d1][x + d1] != BC) {
			break;
		}
	}
	for (d2 = 1;d2 < 5;d2++) {
		if (board[y - d2][x - d2] != BC) {
			break;
		}
	}
	AtLevelJudgment(board, AIAt, y, x, d1, d1, -d2, -d2, d1 + d2 - 1);
}

//bot选择下在哪
static void botMain(char board[21][21], int AISave[21][21], int AIAt[21][21]) {
	int x, y, level = 4, Num = 0, randNum;
	static int first = 0;

	if (first == 0) {
		first = 1;
		Num = 0;
		for (y = 1;y <= 19;y++) {
			for (x = 1;x <= 19;x++) {
				if (AIAt[y][x] == 0 && board[y][x]==' ') {
					Num++;
				}
			}
		}
		randNum = random(1, Num);
		Num = 0;
		for (y = 1;y <= 19;y++) {
			for (x = 1;x <= 19;x++) {
				if (AIAt[y][x] == 0 && board[y][x] == ' ') {
					Num++;
					if (Num == randNum) {
						Bplace(x, y, board);
						Atlevel(board, AIAt, x, y);
						AIAt[y][x] = 0;
						AISave[y][x] = 0;
						return;
					}
				}
			}
		}
	}

	for (level = 4;level > 0;level--) {

		//进攻
		Num = 0;
		for (y = 1;y <= 19;y++) {
			for (x = 1;x <= 19;x++) {
				if (AIAt[y][x] == level) {
					Num++;
				}
			}
		}
		if (Num != 0) {
			randNum = random(1, Num);
			Num = 0;
			for (y = 1;y <= 19;y++) {
				for (x = 1;x <= 19;x++) {
					if (AIAt[y][x] == level) {
						Num++;
						if (Num == randNum) {
							Bplace(x, y, board);
							Atlevel(board, AIAt, x, y);
							AIAt[y][x] = 0;
							AISave[y][x] = 0;
							return;
						}
					}
				}
			}
		}
		//防守
		Num = 0;
		for (y = 1;y <= 19;y++) {
			for (x = 1;x <= 19;x++) {
				if (AISave[y][x] == level) {
					Num++;
				}
			}
		}
		if (Num != 0) {
			randNum = random(1, Num);
			Num = 0;
			for (y = 1;y <= 19;y++) {
				for (x = 1;x <= 19;x++) {
					if (AISave[y][x] == level) {
						Num++;
						if (Num == randNum) {
							Bplace(x, y, board);
							Atlevel(board, AIAt, x, y);
							AISave[y][x] = 0;
							AIAt[y][x] = 0;
							return;
						}
					}
				}
			}
		}
	}
}

//判平
static void draw(char board[21][21]) {
	int x, y, N = 0;
	for (y = 1;y <= 19;y++) {
		for (x = 1;x <= 19;x++) {
			if (board[y][x] == ' ') {
				N++;
			}
		}
	}
	if (N == 0) {
		setbkmode(TRANSPARENT);
		settextstyle(50, 0, _T("Courier"));
		settextcolor(YELLOW);
		RECT r = { 0, 0, 630, 630 };
		drawtext(_T("平局"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		settextstyle(30, 0, _T("Courier"));
		settextcolor(BLACK);
		RECT a = { 0, 0, 630, 700 };
		drawtext(_T("按下任意键退出程序"), &a, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		ExMessage ck = getmessage(EX_KEY);
		exit(0);
	}
}

int main() {
	//初始化

	Sleep(500);
	bool firstplace = 1;
	char board[21][21]{};
	int AISave[21][21]{};
	int AIAt[21][21]{};
	boardSet(board);
	AISet(AIAt);
	AISet(AISave);
	if (MessageBox(NULL, _T("是否先手？"), _T("提示"), MB_YESNO) == IDNO) {
		firstplace = 0;
	}
	Load();
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	outtextxy(0, 0, _T("按ESC退出程序"));
	if (firstplace == 0) {
		Sleep(1000);
		botMain(board, AISave, AIAt);
	}
	

	while (1) {
		//system("cls");
		//PrintB(board);
		//PrintA(AISave);
		//PrintA(AIAt);

		flushmessage();
		ExMessage ck = getmessage(EX_MOUSE | EX_KEY);
		//跳过鼠标移动，防止多次检测
		while (ck.message == WM_MOUSEMOVE) {
			ck = getmessage(EX_MOUSE | EX_KEY);
		}

		if (ck.message == WM_LBUTTONDOWN) {
			int x = ck.x / 30;
			int y = ck.y / 30;

			if (board[y][x] == ' ') {
				//下棋&判断输赢
				Hplace(x, y, board);


				AIAt[y][x] = 0;
				//下棋等级
				Savelevel(board, AISave, x, y);
				//平局
				draw(board);

			}
			else continue;
			//机器人下
			botMain(board, AISave, AIAt);
			//平局
			draw(board);

		}
		else if (ck.message == WM_KEYDOWN && ck.vkcode == VK_ESCAPE) {
			exit(0);
		}
	}

}