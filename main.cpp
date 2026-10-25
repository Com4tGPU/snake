//snake in C++

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

// Using conio.h for non-blocking keyboard input _kbhit() and _getch()
#ifdef _WIN32
#include <conio.h>
#else
// Minimal inline emulation of conio functions if on POSIX/Linux systems
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
bool _kbhit() {
	termios term;
	tcgetattr(0, &term);
	termios term2 = term;
	term2.c_lflag &= ~ICANON;
	term2.c_lflag &= ~ECHO;
	tcsetattr(0, TCSANOW, &term2);
	int byteswaiting;
	ioctl(0, FIONREAD, &byteswaiting);
	tcsetattr(0, TCSANOW, &term);
	return byteswaiting > 0;
}
char _getch() {
	char buf = 0;
	struct termios old = { 0 };
	if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0) perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ~ICANON");
	return buf;
}
#endif

using namespace std;

// Game Settings
const int WIDTH = 20;
const int HEIGHT = 20;

struct Point {
	int x, y;
};

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// Game State Variables
bool gameOver;
Point snakeHead;
vector<Point> snakeBody;
Point food;
Direction dir;
int score;

void Setup() {
	srand(time(NULL));
	gameOver = false;
	dir = STOP;

	// Position snake head in the center
	snakeHead.x = WIDTH / 2;
	snakeHead.y = HEIGHT / 2;
	snakeBody.clear();

	// Spawn initial food
	food.x = rand() % WIDTH;
	food.y = rand() % HEIGHT;

	score = 0;
}

void Draw() {
	// Clear the console screen cleanly
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif

	// Top Border
	for (int i = 0; i < WIDTH + 2; i++) cout << "#";
	cout << endl;

	// Game Grid
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			// Left border
			if (x == 0) cout << "#";

			// Print game objects
			if (x == snakeHead.x && y == snakeHead.y) {
				cout << "O"; // Snake Head
			}
			else if (x == food.x && y == food.y) {
				cout << "F"; // Food
			}
			else {
				bool isBodyPart = false;
				for (const auto& part : snakeBody) {
					if (part.x == x && part.y == y) {
						cout << "o"; // Snake Body segment
						isBodyPart = true;
						break;
					}
				}
				if (!isBodyPart) {
					cout << " "; // Empty space
				}
			}

			// Right border
			if (x == WIDTH - 1) cout << "#";
		}
		cout << endl;
	}

	// Bottom Border
	for (int i = 0; i < WIDTH + 2; i++) cout << "#";
	cout << endl;

	// Display Stats
	cout << "Score: " << score << endl;
	cout << "Controls: W (Up), A (Left), S (Down), D (Right) | X to Quit" << endl;
}

void Input() {
	if (_kbhit()) {
		switch (_getch()) {
		case 'a':
		case 'A':
		case 'h':
		case 'H':
			if (dir != RIGHT) dir = LEFT;
			break;
		case 'd':
		case 'D':
		case 'l':
		case 'L':
			if (dir != LEFT) dir = RIGHT;
			break;
		case 'w':
		case 'W':
		case 'k':
		case 'K':
			if (dir != DOWN) dir = UP;
			break;
		case 's':
		case 'S':
		case 'j':
		case 'J':
			if (dir != UP) dir = DOWN;
			break;
		case 'x':
		case 'X':
			gameOver = true;
			break;
		}
	}
}

void Logic() {
	if (dir == STOP) return;

	// Save previous positions of the body to shift them forward
	Point prevCoord = snakeHead;

	// Move Head
	switch (dir) {
	case LEFT:
		snakeHead.x--;
		break;
	case RIGHT:
		snakeHead.x++;
		break;
	case UP:
		snakeHead.y--;
		break;
	case DOWN:
		snakeHead.y++;
		break;
	default:
		break;
	}

	// Wall Collision Check
	if (snakeHead.x < 0 || snakeHead.x >= WIDTH || snakeHead.y < 0 || snakeHead.y >= HEIGHT) {
		gameOver = true;
		return;
	}

	// Self Collision Check
	for (const auto& part : snakeBody) {
		if (part.x == snakeHead.x && part.y == snakeHead.y) {
			gameOver = true;
			return;
		}
	}

	// Food Eaten Check
	if (snakeHead.x == food.x && snakeHead.y == food.y) {
		score += 10;
		// Grow snake: insert the old head position at the front of the body array
		snakeBody.insert(snakeBody.begin(), prevCoord);

		// Respawn Food
		food.x = rand() % WIDTH;
		food.y = rand() % HEIGHT;
	}
	else {
		// If food wasn't eaten, shift body segments forward
		if (!snakeBody.empty()) {
			snakeBody.insert(snakeBody.begin(), prevCoord);
			snakeBody.pop_back();
		}
	}
}

int main() {
	Setup();

	// Main Game Loop
	while (!gameOver) {
		Draw();
		Input();
		Logic();
		// Adjust the sleep time to change the difficulty/speed of the game
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	cout << "\n===============================" << endl;
	cout << "   GAME OVER! Final Score: " << score << endl;
	cout << "===============================" << endl;

	return 0;
}

