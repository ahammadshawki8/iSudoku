#pragma comment(lib, "winmm.lib")
#include "iGraphics.h"
#include <bits/stdc++.h>
#include <windows.h>
#include <mmsystem.h>

using namespace std;


enum {HOME, LEVEL, GAME, END};
// SYS 0 - home screen
// SYS 1 - level screen
// SYS 2 - game screen
// SYS 3 - end scene

int sys = HOME;
int level = 0;
int hour = 0;
int minute = 0; 
int sec = 0;
int click_x = -1;
int click_y = -1;

int input_num = 0;
int mode = 0;
int input_flag = 0;
bool musicOn = 1;



int cell_border[9][9][4];

int board_level1[9][9] = {
    {5,3,0,0,7,0,0,0,0},
    {6,0,0,1,9,5,0,0,0},
    {0,9,8,0,0,0,0,6,0},
    {8,0,0,0,6,0,0,0,3},
    {4,0,0,8,0,3,0,0,1},
    {7,0,0,0,2,0,0,0,6},
    {0,6,0,0,0,0,2,8,0},
    {0,0,0,4,1,9,0,0,5},
    {0,0,0,0,8,0,0,7,9}
};

int board_level2[9][9] = {
	{7,8,0,4,0,0,1,2,0},
	{6,0,0,0,7,5,0,0,9},
	{0,0,0,6,0,1,0,7,8},
	{0,0,7,0,4,0,2,6,0},
	{0,0,1,0,5,0,9,3,0},
	{9,0,4,0,6,0,0,0,5},
	{0,7,0,3,0,0,0,1,2},
	{1,2,0,0,0,7,4,0,0},
	{0,4,9,2,0,6,0,0,7}
};

int board_level3[9][9] = {
    {8,0,0,0,0,0,0,0,0},
    {0,0,3,6,0,0,0,0,0},
    {0,7,0,0,9,0,2,0,0},
    {0,5,0,0,0,7,0,0,0},
    {0,0,0,0,4,5,7,0,0},
    {0,0,0,1,0,0,0,3,0},
    {0,0,1,0,0,0,0,6,8},
    {0,0,8,5,0,0,0,1,0},
    {0,9,0,0,0,0,4,0,0}
};

int player_board_level1[9][9];
int player_board_level2[9][9];
int player_board_level3[9][9];



pair <int, int> find_empty(int bo[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
			if (bo[i][j] == 0) {
				return {i, j};
			}
		}
	}
    return {-1, -1};
}

bool valid(int bo[9][9], int num, pair <int, int> pos){
    // Check row
    for (int i = 0; i < 9; i++) {
        if ((bo[pos.first][i] == num) && (pos.second != i)) {
            return false;
		}
	}
    // Check column
	for (int i = 0; i < 9; i++) {
        if ((bo[i][pos.second] == num) && (pos.first != i)) {
            return false;
		}
	}
    // Check box
    int box_x = pos.second / 3;
    int box_y = pos.first / 3;

    for (int i = box_y*3; i < box_y*3 + 3; i++) {
        for (int j = box_x * 3; j < box_x*3 + 3; j++) {
            if ((bo[i][j] == num) && !((i == pos.first) && (j == pos.second))) {
                return false;
			}
		}	
	}
    return true;
}

void print_board(int bo[9][9]){
    for (int i = 0; i < 9; i++) {
        if ((i % 3 == 0) && (i != 0)) {
            cout << "- - - - - - - - - - - - -\n";
		}

        for (int j = 0; j < 9; j++) {
            if ((j % 3 == 0) && (j != 0)) {
				cout << " | ";
			}

            if (j == 8) {
                cout << bo[i][j] << "\n";
			}
            else {
                cout << bo[i][j] << " ";
			}
		}
	}
}

bool solve(int bo[9][9]) {
	int row, col;
    pair <int, int> find = find_empty(bo);
    if (find.first == -1 && find.second == -1) {
        return true;
	} else {
        row = find.first;
		col = find.second;
	}

    for (int i = 1; i < 10; i++) {
        if (valid(bo, i, {row, col})) {
            bo[row][col] = i;
			// print_board(bo);
			// Sleep(1000);

			if (solve(bo)) {
				return true;
			}
			
			bo[row][col] = 0;
		}


	}
    return false;
}



void draw_grid() {
	double shift_x = 0; 
	double shift_y = -20;

	double dx = 500/9;
	double dy = 400/9;
	for (int i = 0; i < 8; i++) {
		if ((i+1)%3 == 0) {
			iSetColor(255, 0, 0);
			iLine(dx + dx * i + shift_x, dy + shift_y, dx + dx * i + shift_x, dy*10 + shift_y);
			iSetColor(255, 255, 255);
		} else {
			iLine(dx + dx * i + shift_x, dy + shift_y, dx + dx * i + shift_x, dy*10 + shift_y);
		}
	}

	for (int i = 0; i < 10; i++) {
		if ((i)%3 == 0) {
			iSetColor(255, 0, 0);
			iLine(0 + shift_x, dy + dy * i + shift_y, dx * 9 + 10 + shift_x, dy + dy * i + shift_y);
			iSetColor(255, 255, 255);
		} else {
			iLine(0 + shift_x, dy + dy * i + shift_y, dx * 9 + 10 + shift_x, dy + dy * i + shift_y);
		}
	}
}


void place_numbers_in_grid(int bo[9][9], int init_bo[9][9]) {
	double shift_x = 500/18 - 5; 
	double shift_y = - 110;

	double dx = 500/9;
	double dy = 400/9;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (bo[i][j] != 0) {
				string temp = to_string(bo[i][j]);
				char* cstr = new char[temp.size() + 1];
				strcpy(cstr, temp.c_str());
				if (init_bo[i][j] == 0) {
					iSetColor(255, 0, 0);
					iText(dx*j + shift_x, (500 - dy*i) + shift_y, cstr , GLUT_BITMAP_TIMES_ROMAN_24);
					iSetColor(255, 255, 255);
				} else {
					iText(dx*j + shift_x, (500 - dy*i) + shift_y, cstr , GLUT_BITMAP_TIMES_ROMAN_24);
				}
			}
		}
	}
}

void draw_back_button() {
	iSetColor(255, 0, 0);
	iFilledCircle(25, 475, 10, 1000);
	iSetColor(255, 255, 255);
}

void draw_auto_solve_button() {
	iSetColor(255, 165, 0);
	iFilledCircle(60, 475, 10, 1000);
	iSetColor(255, 255, 255);
}

void draw_submit_button() {
	iSetColor(0, 255, 0);
	iFilledCircle(95, 475, 10, 1000);
	iSetColor(255, 255, 255);
}

void reset_timer() {
	hour = 0;
	minute = 0; 
	sec = 0;
}

void update_timer() {
	sec++;
	if (sec >= 60) {
		sec = 0;
		minute++;
	} 
	if (minute >= 60) {
		minute = 0;
		hour++;
	}
}

void draw_timer() {
	ostringstream oss_h;
    oss_h << setw(2) << setfill('0') << hour;
    string formattedhour = oss_h.str();

	ostringstream oss_m;
    oss_m << setw(2) << setfill('0') << minute;
    string formattedminute = oss_m.str();

	ostringstream oss_s;
    oss_s << setw(2) << setfill('0') << sec;
    string formattedsec = oss_s.str();

	string temp = "TIMER: " + formattedhour + ":" + formattedminute + ":" + formattedsec;
	char* cstr = new char[temp.size() + 1];
	strcpy(cstr, temp.c_str());
	iText(350, 470, cstr , GLUT_BITMAP_HELVETICA_18);
}

void draw_timer_for_end() {
	ostringstream oss_h;
    oss_h << setw(2) << setfill('0') << hour;
    string formattedhour = oss_h.str();

	ostringstream oss_m;
    oss_m << setw(2) << setfill('0') << minute;
    string formattedminute = oss_m.str();

	ostringstream oss_s;
    oss_s << setw(2) << setfill('0') << sec;
    string formattedsec = oss_s.str();

	string temp = formattedhour + " HOURS  " + formattedminute + " MINUTES  " + formattedsec + " SECONDS";
	char* cstr = new char[temp.size() + 1];
	strcpy(cstr, temp.c_str());

	iText(55, 150, "~ TOTAL TIME NEEDED TO COMPLETE THE LEVEL ~", GLUT_BITMAP_9_BY_15);
	iText(80, 120, cstr , GLUT_BITMAP_HELVETICA_18);
}


void generate_cell_border() {
	double shift_x = 0; 
	double shift_y = -80;

	double dx = 500/9;
	double dy = 400/9;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			cell_border[i][j][0] = dx * j;
			cell_border[i][j][1] = (500 - dy*i) + shift_y;
			cell_border[i][j][2] = dx * (j+1);
			cell_border[i][j][3] = (500 - dy*(i+1)) + shift_y;
			if (j == 8) {
				cell_border[i][j][2] += 5;
			}
		}
	}
}

pair <int, int> select_cell(int mx, int my, int level) {
	int bo[9][9];

	if (level == 1) {
		memcpy(bo, board_level1, sizeof(board_level1));
	} else if (level == 2) {
		memcpy(bo, board_level2, sizeof(board_level2));
	} else {
		memcpy(bo, board_level3, sizeof(board_level3));
	}

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (mx >= cell_border[i][j][0] && mx <= cell_border[i][j][2] && my <= cell_border[i][j][1] && my >= cell_border[i][j][3]) {
				if (bo[i][j] == 0) {
					mode = 1;
					iSetColor(0, 255, 0);
					iLine(cell_border[i][j][0], cell_border[i][j][1], cell_border[i][j][0], cell_border[i][j][3]);
					iLine(cell_border[i][j][2], cell_border[i][j][1], cell_border[i][j][2], cell_border[i][j][3]);
					iLine(cell_border[i][j][0], cell_border[i][j][1], cell_border[i][j][2], cell_border[i][j][1]);
					iLine(cell_border[i][j][0], cell_border[i][j][3], cell_border[i][j][2], cell_border[i][j][3]);
					iSetColor(255, 255, 255);

					return {i, j};
				}
			}
		}
	}
	return {-1, -1};
}



void write_cell() {
	pair <int, int> pos = select_cell(click_x, click_y, level);
	int bo[9][9];
	if (level == 1) {
		memcpy(bo, player_board_level1, sizeof(player_board_level1));
	} else if (level == 2) {
		memcpy(bo, player_board_level2, sizeof(player_board_level2));
	} else {
		memcpy(bo, player_board_level3, sizeof(player_board_level3));
	}
	if(mode == 1 && pos.first != -1 && pos.second != -1 && input_num != 0 && bo[pos.first][pos.second] == 0) {
		iSetColor(255, 0, 0);
		double shift_x = 500/18 - 5; 
		double shift_y = - 110;

		double dx = 500/9;
		double dy = 400/9;

		string temp = to_string(input_num);
		char* cstr = new char[temp.size() + 1];
		strcpy(cstr, temp.c_str());

		iText(dx*pos.second + shift_x, (500 - dy*pos.first) + shift_y, cstr, GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(255, 255, 255);
	}
	if (input_flag == 1) {
		switch(level) {
			case 1:
				player_board_level1[pos.first][pos.second] = input_num;
				break;
			case 2:
				player_board_level2[pos.first][pos.second] = input_num;
				break;
			case 3:
				player_board_level3[pos.first][pos.second] = input_num;
				break;
		}
		input_flag = 0;
		input_num = 0;
	}
}

bool win_check() {
	int current_bo[9][9];
	int solved_bo[9][9];
	switch(level) {
		case 1:
			memcpy(current_bo, player_board_level1, sizeof(player_board_level1));
			memcpy(solved_bo, board_level1, sizeof(board_level1));
			solve(solved_bo);
			break;
		case 2:
			memcpy(current_bo, player_board_level2, sizeof(player_board_level2));
			memcpy(solved_bo, board_level2, sizeof(board_level2));
			solve(solved_bo);
			break;
		case 3:
			memcpy(current_bo, player_board_level3, sizeof(player_board_level3));
			memcpy(solved_bo, board_level3, sizeof(board_level3));
			solve(solved_bo);
			break;
	}
	bool solved_flag = 1;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (current_bo[i][j] != solved_bo[i][j]) {
				solved_flag = 0;
				break;
			}
		}
	}
	return solved_flag;
}


// add music
// vesion control and documentation







void iDraw() {
	iClear();
	if (musicOn == 1) {
		PlaySound("bgm1.wav", NULL, SND_LOOP | SND_ASYNC);
		musicOn = 0;
	}

	switch(sys) {
		case HOME:
			iShowBMP(10, 50, "poster.bmp");
			iSetColor(255, 255, 255);
			iText(110, 30, "PRESS ANY KEY TO CONTINUE", GLUT_BITMAP_HELVETICA_18);
			break;
		case LEVEL:
			iShowBMP(10, 0, "level.bmp");
			draw_back_button();
			break;
		case GAME:
			draw_back_button();
			draw_auto_solve_button();
			draw_submit_button();
			draw_grid();
			draw_timer();
			switch(level) {
				case 1:
					place_numbers_in_grid(player_board_level1, board_level1);
					generate_cell_border();
					break;
				case 2:
					place_numbers_in_grid(player_board_level2, board_level2);
					break;
				case 3:
					place_numbers_in_grid(player_board_level3, board_level3);
					break;
			}
			write_cell();
			break;
		case END:
			iShowBMP(10, 20, "END.bmp");
			iSetColor(255, 255, 255);
			draw_timer_for_end();
			iText(55, 30, "PRESS (X) TO EXIT OR (C) TO RESTART THE GAME", GLUT_BITMAP_9_BY_15);
			break;
	}
	
}


void iMouseMove(int mx, int my) {
	printf("x = %d, y= %d\n",mx,my);
	if (mx < 100 && my < 100) {
		printf("x = %d, y= %d\n",mx,my);
	}
}


void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		printf("x = %d, y= %d\n",mx,my);
		// first sys screens should be handled first.
		if (sys == LEVEL) {
			if (mx >= 125 && mx <= 375 && my >= 230 && my <= 280) {
				// level 1
				level = 1;
				memcpy(player_board_level1, board_level1, sizeof(board_level1));
				sys = GAME;
				reset_timer();
			}

			if (mx >= 125 && mx <= 375 && my >= 150 && my <= 200) {
				// level 2
				level = 2;
				memcpy(player_board_level2, board_level2, sizeof(board_level2));
				sys = GAME;
				reset_timer();
			}

			if (mx >= 125 && mx <= 375 && my >= 70 && my <= 120) {
				// level 3
				level = 3;
				memcpy(player_board_level3, board_level3, sizeof(board_level3));
				sys = GAME;
				reset_timer();
			}

			if (sqrt((mx-25)*(mx-25) + (my-475)*(my-475)) <= 10) {
				sys = HOME;
			}
		}

		else if (sys == GAME) {
			if (sqrt((mx-25)*(mx-25) + (my-475)*(my-475)) <= 10) {
				sys = LEVEL;
				reset_timer();
			}

			if (sqrt((mx-60)*(mx-60) + (my-475)*(my-475)) <= 10) {
				cout << "trig" << "\n";
				switch (level) {
					case 1:
						memcpy(player_board_level1, board_level1, sizeof(board_level1));
						solve(player_board_level1);
						break;
					case 2:
						memcpy(player_board_level2, board_level2, sizeof(board_level2));
						solve(player_board_level2);
						break;
					case 3:
						memcpy(player_board_level3, board_level3, sizeof(board_level3));
						solve(player_board_level3);
						break;
				}
			}

			if (sqrt((mx-95)*(mx-95) + (my-475)*(my-475)) <= 10) {
				if (win_check()) {
					sys = END;
					iPauseTimer(0);
				}
			}

			click_x = mx;
			click_y = my;
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
	}
}


void iKeyboard(unsigned char key) {
	if ((sys == HOME) && (key)) {
		sys = LEVEL;
	} else if (sys == END) {
		if (key == 'x') {
			exit(0);
		} else if (key == 'c') {
			iResumeTimer(0);
			sys = HOME;
		}
	}

	if(sys == GAME && mode == 1)
	{
        if(key == '\r')
		{
			mode = 0;
			printf("%d\n", input_num);
			input_flag = 1;
		}
		else
		{
			input_num = key - '0';
		}
	}
}


void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_END) {
		exit(0);
	}
}


int main() {
	memcpy(player_board_level1, board_level1, sizeof(board_level1));
	memcpy(player_board_level2, board_level2, sizeof(board_level2));
	memcpy(player_board_level3, board_level3, sizeof(board_level3));

	iSetTimer(1000, update_timer);
	iInitialize(500, 500, "SUDOKU by Ahammad");
	return 0;
}
