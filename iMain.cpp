#pragma comment(lib, "winmm.lib")
#include "iGraphics.h"
#include <bits/stdc++.h>
#include <windows.h>
#include <mmsystem.h>
#include <bits/stdc++.h>
#define MAX_LINE 1024
using namespace std;


enum {HOME, LEVEL, GAME, END, LEAD, HOW};
// SYS 0 - home screen
// SYS 1 - level screen
// SYS 2 - game screen
// SYS 3 - end screen
// SYS 4 - leaderboards screen
// SYS 5 - how to play screen


// Global variables & flags
int sys = HOME; // starting screen
int level = 0;

int hour = 0; // timer related global variable
int minute = 0; 
int sec = 0;

int click_x = -1;
int click_y = -1;

int input_num = 0; // for taking number input in the sudoku grid
int mode = 0;
int input_flag = 0;

int musicOn = 1; // controlling music

int name_mode = 0; // for taking name input for the leaderboards
int name_len = 0;
char name_str[100];

int error_flag = 0; // for error checking after submission
int num_error = 0;



int cell_border[9][9][4]; // for storing the coordinates of all the cells in the sudoku board


// three boards based on levels. level 1 is easy, level 2 is mid, level 3 is hard.
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


// these are temporary boards which will be continuosly updated during the game
int player_board_level1[9][9];
int player_board_level2[9][9];
int player_board_level3[9][9];


// returns the first empty cell of a board
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

// if we input a number in a cell, this function will check if the input was valid or not according to the rules of sudoku
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

// prints the current board in terminal to visualize the process
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

// backtracking algorithm for solving the entire sudoku board
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


// draws the sudoku grid in igraphics window
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

// place number in the cells 
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
	iShowBMP(15, 455, "resources\\back.bmp");
}

// triggers backtracking
void draw_auto_solve_button() {
	iShowBMP(50, 455, "resources\\solve.bmp");
}

void draw_submit_button() {
	iShowBMP(85, 455, "resources\\submit.bmp");
}

// music on_off toggler
void draw_music_button() {
	if (musicOn == 0) {
		iShowBMP(15, 455, "resources\\music_off.bmp");
	} else {
		iShowBMP(15, 455, "resources\\music_on.bmp");
	}
}

// set timer global variables to 0
void reset_timer() {
	hour = 0;
	minute = 0; 
	sec = 0;
}

// update the sec += 1, and calculate the hour and min according to that
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

// draw the timer in the game screen
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

// draw the timer in the end screen
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

	iText(55, 200, "~ TOTAL TIME NEEDED TO COMPLETE THE LEVEL ~", GLUT_BITMAP_9_BY_15);
	iText(80, 170, cstr , GLUT_BITMAP_HELVETICA_18);
}

// taking the name input after finishing a level for leaderboards 
void data_input_for_end() {
	iText(72, 110, "SHOWCASE YOUR NAME IN THE LEADERBOARDS", GLUT_BITMAP_9_BY_15);

	iRectangle(150, 70, 200, 30);

	if (name_mode == 1) {
		iSetColor(255, 0, 0);
		iRectangle(150, 70, 200, 30);
		iSetColor(255, 255, 255);

		string str(name_str);
		char* cstr = new char[std::strlen(name_str) + 1];
		strcpy(cstr, name_str);

		iText(165, 80, cstr);
	}

}

// populate the cell_border array
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

// selecting a valid cell and after selecting a valid cell, the border color will be changed
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


// inputing a number in the valid cell
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
		error_flag = 0;
	}
}

// checking if a submission is right or wrong by counting total errors (error = 0 means right solution)
int win_check() {
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
	int errors = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (current_bo[i][j] != solved_bo[i][j]) {
				errors++;
			}
		}
	}
	return errors;
}

// overriding cmp function for sorting vector of pairs
bool cmp(pair<string, int> a, pair <string, int> b) {
	return a.second < b.second;
}

// reading data from csv file (csv file is our offline database)
vector <pair <string, int>> read_data() {
	FILE *file = fopen("record.csv", "r");
    if (!file) {
        perror("Could not open file");
    }

    char line[MAX_LINE];

	vector <pair <string, int>> v;
    while (fgets(line, MAX_LINE, file)) {
        char * token = strtok(line, ",");
		int i = 0;
		pair <string, string> p;
		int val;
        while (token) {
            if (i == 0) {
				p.first = token;
			} else {
				p.second = token;
				p.second[p.second.size() - 1] = '\0';
				val = stoi(p.second);
			}
			i++;
			token = strtok(NULL, ",");
        }
		v.push_back({p.first, val});
    }

	sort(v.begin(), v.end(), cmp);

    fclose(file);
	return v;
}

// writing data to csv file (csv file is our offline database)
void write_data(const char * line) {
	FILE *file = fopen("record.csv", "a+");
    if (!file) {
        perror("Could not open file");
    }

    fprintf(file, line);
    fclose(file);
}

// show information (name & time) in the leaderboard page
void show_leaders() {
	vector <pair <string, int>> v = read_data();

	double shift_x = 210; 
	double shift_y = - 248;

	double dx = 60;
	double dy = 38;

	for (int i = 0; i < 6; i++) {
		pair <string, int> pa = v[i];

		
		char* cstr = new char[pa.first.size() + 1];
		strcpy(cstr, pa.first.c_str());

		int hour_rec = pa.second / 3600;
		int minute_rec = (pa.second % 3600) / 60;
		int sec_rec = pa.second % 60;

		ostringstream oss_h;
		oss_h << setw(2) << setfill('0') << hour_rec;
		string formattedhour = oss_h.str();

		ostringstream oss_m;
		oss_m << setw(2) << setfill('0') << minute_rec;
		string formattedminute = oss_m.str();

		ostringstream oss_s;
		oss_s << setw(2) << setfill('0') << sec_rec;
		string formattedsec = oss_s.str();

		string temp = formattedhour + "H : " + formattedminute + "M : " + formattedsec + "S";
		char* cstr2 = new char[temp.size() + 1];
		strcpy(cstr2, temp.c_str());

		iText(dx, (500 - dy*i) + shift_y, cstr, GLUT_BITMAP_9_BY_15);
		iText(dx + shift_x, (500 - dy*i) + shift_y, cstr2, GLUT_BITMAP_9_BY_15);
	}
}

// if a solution is incorrect, write how many errors are in the board
void write_error() {
	string temp = "THERE ARE " + to_string(num_error) + " ERROR(S) IN YOUR SOLUTION";
	char* cstr = new char[temp.size() + 1];
	strcpy(cstr, temp.c_str());
	iText(90, 430, cstr, GLUT_BITMAP_9_BY_15);
}





void iDraw() {
	iClear();

	// music control
	if (musicOn == 1) {
		PlaySound("resources\\bgm1.wav", NULL, SND_LOOP | SND_ASYNC);
		musicOn = 2;
	}

	if (musicOn == 0) {
		PlaySound(NULL, NULL, SND_PURGE);
	}


	// functionalities and transition to different screens
	switch(sys) {
		case HOME:
			iShowBMP(10, 0, "resources\\poster.bmp");
			draw_music_button();
			break;
		case LEVEL:
			iShowBMP(10, 0, "resources\\level.bmp");
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
			if (error_flag) write_error();
			break;
		case END:
			iShowBMP(10, 50, "resources\\end.bmp");
			iSetColor(255, 255, 255);
			draw_back_button();
			draw_timer_for_end();
			data_input_for_end();
			iText(30, 30, "PRESS (END) TO EXIT OR (HOME) TO RESTART THE GAME", GLUT_BITMAP_9_BY_15);
			break;
		case LEAD:
			iShowBMP(10, 10, "resources\\lead.bmp");
			show_leaders();
			draw_back_button();
			break;
		case HOW:
			iShowBMP(10, 20, "resources\\rules.bmp");
			draw_back_button();
			break;
	}
	
}


// haven't used this function
void iMouseMove(int mx, int my) {
	printf("x = %d, y= %d\n",mx,my);
	if (mx < 100 && my < 100) {
		printf("x = %d, y= %d\n",mx,my);
	}
}

// tracking down mouse movement and triggering different function when mouse clicks
void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		printf("x = %d, y= %d\n",mx,my);
		// first sys screens should be handled first.
		if (sys == HOME) {
			if (sqrt((mx-31)*(mx-31) + (my-472)*(my-472)) <= 14) {
				if (musicOn > 0) musicOn = 0;
				else musicOn = 1;
			}

			if (mx >= 125 && mx <= 375 && my >= 230 && my <= 280) {
				sys = LEVEL;
			}

			if (mx >= 125 && mx <= 375 && my >= 150 && my <= 200) {
				sys = LEAD;
			}

			if (mx >= 125 && mx <= 375 && my >= 70 && my <= 120) {
				sys = HOW;
			}
		}
		else if (sys == LEVEL) {
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

			if (sqrt((mx-31)*(mx-31) + (my-472)*(my-472)) <= 14) {
				sys = HOME;
			}
		}

		else if (sys == GAME) {
			if (sqrt((mx-31)*(mx-31) + (my-472)*(my-472)) <= 14) {
				sys = LEVEL;
				error_flag = 0;
				reset_timer();
			}

			if (sqrt((mx-66)*(mx-66) + (my-472)*(my-472)) <= 14) {
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

			if (sqrt((mx-101)*(mx-101) + (my-472)*(my-472)) <= 14) {
				num_error = win_check();
				if (num_error == 0) {
					sys = END;
					iPauseTimer(0);
				} else {
					error_flag = 1;
				}
			}

			click_x = mx;
			click_y = my;
		} else if (sys == END) {
			if (sqrt((mx-31)*(mx-31) + (my-472)*(my-472)) <= 14) {
				sys = LEVEL;
				iResumeTimer(0);
			}
			if(mx >= 150 && mx <= 350 && my >= 70 && my <= 100 && name_mode == 0) {
				name_mode = 1;
				// 150, 70, 200, 30
			}
		} else if (sys == LEAD) {
			if (sqrt((mx-31)*(mx-31) + (my-472)*(my-472)) <= 14) {
				sys = HOME;
			}
		} else if (sys == HOW) {
			if (sqrt((mx-31)*(mx-31) + (my-472)*(my-472)) <= 14) {
				sys = HOME;
			}
		}
	}

	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
	}
}

// tracking down keyboard presses, taking inputs and triggering different function
void iKeyboard(unsigned char key) {
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

	if (sys == END && name_mode == 1) {
		int i;
		if(name_mode == 1)
		{
			if(key == '\r')
			{
				name_mode = 0;

				char buffer[200];
				int time = hour * 3600 + minute * 60 + sec;
				std::sprintf(buffer, "%s,%d\n", name_str, time);
				const char* line = buffer;

				write_data(line);

				for(i = 0; i < name_len; i++)
					name_str[i] = 0;
				name_len = 0;

				vector <pair <string, int>> v = read_data();

				for (auto pa: v) {
					cout << pa.first << "-" << pa.second << "\n";
				}
				sys = LEAD;
				iResumeTimer(0);
			} else if (key == '\b') {
				name_str[name_len-1] = 0;
				name_len--;
			}
			else
			{
				name_str[name_len] = key;
				name_len++;
			}
		}
	}
}

// only used in the end screen
void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_END) {
		exit(0);
	}

	if (sys == END) {
		if (key == GLUT_KEY_END) {
			exit(0);
		} else if (key == GLUT_KEY_HOME) {
			iResumeTimer(0);
			sys = HOME;
		}
	}
}


int main() {
	// populating player board
	memcpy(player_board_level1, board_level1, sizeof(board_level1));
	memcpy(player_board_level2, board_level2, sizeof(board_level2));
	memcpy(player_board_level3, board_level3, sizeof(board_level3));

	iSetTimer(1000, update_timer); // updating the sec += 1 after 1000 milisec
	iInitialize(500, 500, "iSUDOKU by Ahammad"); // initializing the window
	return 0;
}
