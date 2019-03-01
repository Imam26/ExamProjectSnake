#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <fstream>
using namespace std;
#pragma warning(disable:4996)

#define STRING 25
int posX_Start = 1;//начальные координаты поля
int posY_Start = 4;
int width = 60;//ширина поля
int height = 20;//длина поля
int ColorSnake = 0x09;
int ColorFruit = 0x0C;
int ColorWall = 0xBD;
int ColorScore = 0x05;
int ColorSelect = 0xBD;
int ColorDefault = 0x0A;
int length = 3;//длина змейки
int X_Cl = 0, Y_Cl = 0; //координаты для зачистки
int score = 0;//счет

void SetColor(WORD wAttributes) {
	HANDLE hOUTPUT = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOUTPUT, wAttributes);
}
void SetPos(int x, int y) {
	COORD cd;
	cd.X = x;
	cd.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cd);
}
enum
{
	ESC = 27,
	ENTER = 13,
	SPACE = 32,
	CURSOR1 = 0,
	CURSOR2 = 224,
	UP = 72,
	DOWN = 80,
	LEFT = 75,
	RIGTH = 77,

}keyboard, key;

struct Snake {
	int X;
	int Y;
}mas[100];

struct Fruit {
	int X;
	int Y;
	void Pos_Fr() {
		X = (rand() % (width - 1)) + posX_Start + 1;
		Y = (rand() % (height - 1)) + posY_Start + 1;

	}
	void Print_Fr() {
		SetColor(ColorFruit);
		SetPos(X, Y);
		cout << "F";
	}

}Fr;

void Draw(Fruit &F, int cl) {

	for (int i = posX_Start; i <= width + posX_Start; i++) {
		SetColor(ColorWall);
		SetPos(i, posY_Start);
		cout << " ";
		SetColor(ColorWall);
		SetPos(i, posY_Start + height);
		cout << " ";
	}

	SetPos(X_Cl, Y_Cl);
	SetColor(0);
	cout << " ";

	// очистка за змейкой

	for (int i = 0; i < length; i++) {
		if (cl == 0) {
			length = 3;
			F.X = posX_Start + 2;
			F.Y = posY_Start + 2;
			F.Print_Fr();
		}
		else {
			SetPos(mas[i].X, mas[i].Y);
			SetColor(ColorSnake);
			cout << "@";
		}
	}

	X_Cl = mas[length - 1].X;
	Y_Cl = mas[length - 1].Y;

	if (mas[0].X == F.X&&mas[0].Y == F.Y) {
		Beep(1000, 20); //звук
		length++;
		for (int i = 0; i < 1;) {// проверка координат Fruit чтобы не совпадали с координатами Snake;
			F.Pos_Fr();
			int k = 0;
			for (int j = 0; j < length; j++) {

				if (F.X == mas[j].X&&F.Y == mas[j].Y) {
					k++;
				}
			}
			if (k == 0) {
				F.Print_Fr();
				i++;
			}
		}
		score += 5;
	}

	for (int j = posY_Start; j <= height + posY_Start; j++) {
		SetPos(posX_Start, j);
		SetColor(ColorWall);
		cout << " ";
		SetColor(ColorWall);
		SetPos(posX_Start + width, j);
		cout << " ";
	}
	SetPos(posX_Start, posY_Start + height + 2);
	SetColor(ColorScore);
	cout << "Набранные очки: " << score;
	SetPos(posX_Start + width - 14, posY_Start + height + 2);
	SetColor(ColorScore);
	cout << "Space - Пауза";
}

int Show_Menu(char mas[][STRING], int count, int posX, int posY) {
	int select = 0;

	int y = posY;
	int key;
	for (; true;) {
		posY = y;
		for (int i = 0; i < count; i++) {

			SetPos(posX, posY);
			for (int j = 0; j < STRING; j++) {

				(select == i) ? SetColor(ColorSelect) : SetColor(ColorDefault);
				if (mas[i][j] == '\0') {
					break;
				}
				cout << mas[i][j];
			}
			posY++;
		}
		key = _getch();
		switch (key) {
		case ENTER: return select;
		case CURSOR1:
		case CURSOR2:
			key = _getch();
			if (key == UP) {
				if (select > 0) {
					select--;
				}
			}
			else if (key == DOWN) {
				if (select < count - 1) {
					select++;
				}
			}break;
		}
	}
}

int Menu_Space(char mas[][STRING], int sz, int posX, int posY) { //Меню при нажатии SPACE
	int select = 0;
	return select = Show_Menu(mas, sz, posX, posY);
}

void Clear_Menu(int sz, int posX, int posY) {
	int p = posX;
	for (int i = 0; i < sz; i++) {
		for (int j = 0; j < STRING; j++) {
			SetPos(posX++, posY);
			SetColor(0);
			cout << " ";
		}
		posX = p;
		posY++;
	}
}

void Manage(Snake *mas, char menu_sp[][STRING], int sz, int posX, int posY) {

	bool a = true;

	if (kbhit()) {
		int kl = getch();
		if (kl == CURSOR1 || kl == CURSOR2) {
			switch (getch()) {
			case UP: keyboard = UP; key = UP; break;
			case DOWN:keyboard = DOWN; key = DOWN; break;
			case LEFT:keyboard = LEFT; key = LEFT; break;
			case RIGTH:keyboard = RIGTH; key = RIGTH; break;
			}
		}
		else if (kl == SPACE) { keyboard = SPACE; };
	}
	while (a) {
		switch (keyboard) {
		case UP: --mas[0].Y;   a = false; break;
		case DOWN:++mas[0].Y;  a = false; break;
		case LEFT: --mas[0].X; a = false; break;
		case RIGTH:++mas[0].X; a = false; break;
		case SPACE:
			if (!Menu_Space(menu_sp, sz, posX, posY))
			{
				Clear_Menu(sz, posX, posY);
				keyboard = key;

			}
			else { Clear_Menu(sz, posX, posY); return; }
			break;
		}
	}
	if (mas[0].X == posX_Start) {
		mas[0].X = posX_Start + width - 1;
	}
	else if (mas[0].X == posX_Start + width) {
		mas[0].X = posX_Start + 1;
	}
	if (mas[0].Y == posY_Start) {
		mas[0].Y = posY_Start + height - 1;
	}
	else if (mas[0].Y == posY_Start + height) {
		mas[0].Y = posY_Start + 1;
	}
}

void New_Snake(Snake *mas) {

	if (keyboard != SPACE) {
		for (int i = length - 1; i > 0; i--) {
			mas[i] = mas[i - 1];
		}
	}

}

bool Game_Over(Snake *mas) {

	for (int i = 1; i < length; i++) {

		if (mas[0].X == mas[i].X&&mas[0].Y == mas[i].Y) {
			return false;
		}
	}
	return true;
}

void Record(char name[], int score, int posX, int posY) {

	char buf[20];
	int num;

	ifstream file;
	file.open(name);
	file.getline(buf, sizeof(buf));
	file.close();
	num = atoi(buf);
	if (score > num) {
		itoa(score, buf, 10);
		ofstream rec;
		rec.open(name);
		rec << buf;
		rec.close();
		rec.open(name, ios::app);
		SetPos(posX, posY++);
		cout << "Поздравляем вы побили рекорд!!!";
		SetPos(posX, posY++);
		cout << "Вы набрали " << score << " очков";
		SetPos(posX, posY);
		cout << "Введите ваше имя:";
		cin.getline(buf, sizeof(buf));
		rec << "\n" << buf;
		rec.close();
	}
	else {

		SetPos(posX, posY++);
		cout << "Конец игры";
		SetPos(posX, posY);
		cout << "Ваш результат: " << score << " очков";
		cin.get();
	}
}

void Print_Record(char name[], int posX, int posY) {

	char buf[2][20];
	ifstream file;
	file.open(name);
	int i = 0;
	for (; i<2; i++)
	{
		file.getline(buf[i], sizeof(buf[i]));
	}
	SetPos(posX, posY++);
	SetColor(ColorDefault);
	cout << "Имя игрока: " << buf[i - 1];
	SetPos(posX, posY++);
	SetColor(ColorDefault);
	cout << "Количество очков: " << buf[0];
	SetPos(posX, posY++);
	SetColor(ColorWall);
	cout << "ENTER - назад";
	file.close();
	cin.get();
	SetColor(0);
	system("cls");
}

void main() {

	system("chcp 1251 > nul");
	srand(time(0));
	mas[0].X = 20;//начальная позиция змейки по Х
	mas[0].Y = 20;//начальная позиция змейки по Y
	int posX = width + posX_Start + 5;// позиции меню
	int posY = posY_Start + 5;
	keyboard = UP;
	key = UP;// хранит код клавишы, для корректного выхода из паузы(SPACE); 
	bool game_ov = true;
	int speed = 300;// скорость змейки
	int sz1 = 2;//количество пунктов меню
	int sz2 = 4;
	int sz3 = 6;
	char menu_sp[][STRING] = {
		"Продолжить",
		"Выход в меню"
	};
	char Main_Menu[][STRING] = {
		"Начать игру",
		"Выбрать уровень",
		"Рекорд игры",
		"Выход"
	};
	char Level[][STRING] = {
		"Уровень 1(профи)",
		"Уровент 2",
		"Уровент 3",
		"Уровент 4",
		"Уровент 5",
		"Уровент 6(малыш)"
	};

	int select;
	char path[] = { "Record.txt" }; //имя файла который хранит рекорд игры

	while (true) {
		int x_s;//присваивает выбранный уровень скорости змейки 
		int cl = 0;//при запуске новой игры эта переменная нужна чтобы задать начальную длину змейки в функции Draw();
		select = Show_Menu(Main_Menu, sz2, posX, posY);
		switch (select) {
		case 0:
			system("cls");
			Fr.Pos_Fr();//начальные координаты фрукта
			while (game_ov) {
				New_Snake(mas);//увеличение змейки
				Manage(mas, menu_sp, sz1, posX, posY); //управление змейкой
				Draw(Fr, cl);//прорисовка
				cl++;
				Sleep(speed);
				game_ov = Game_Over(mas);
			}
			mas[0].X = 20;
			mas[0].Y = 20;
			keyboard = UP;
			game_ov = true;
			Record(path, score, height / 2 + 2, height / 2 + 2);
			score = 0;
			system("cls");
			break;
		case 1:
			Clear_Menu(sz2, posX, posY);
			x_s = Show_Menu(Level, sz3, posX, posY);
			speed = (x_s + 1) * 80;
			Clear_Menu(sz3, posX, posY);
			break;
		case 2:
			Clear_Menu(sz2, posX, posY);
			Print_Record(path, posX, posY);
			break;
		case 3: exit(0);
		}
	}
	cin.get(); cin.get();
}