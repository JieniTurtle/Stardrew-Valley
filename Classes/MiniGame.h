#pragma once
#include "cocos2d.h"
USING_NS_CC;


// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2
#define INF 2147483647
#define _INF -2147483647

extern int myFlag;
extern int enermyFlag;


struct coordinate {
    int x = -1;
    int y = -1;
};
struct Nude {
    int board[BOARD_SIZE][BOARD_SIZE] = { {0} };
    int state = 0;
    void arr_input(int x, int y, int playerround)
    {
        board[x][y] = playerround;
    }
};

extern Nude A;

bool judgeInRange(coordinate temp);
int getColor(coordinate target);
void place(coordinate target);

class Player {
private:
    Nude* my_nude_ = &A;
    int score_board_[BOARD_SIZE][BOARD_SIZE], my_score_board_[BOARD_SIZE][BOARD_SIZE], enermy_score_board_[BOARD_SIZE][BOARD_SIZE];
    int importance_board_[BOARD_SIZE][BOARD_SIZE];
    coordinate target_coordinate_;

    int evaluate_side_score(int side_score_board[BOARD_SIZE][BOARD_SIZE], int side);
public:
    Player(Nude& init_board);
    void evaluate_score();
    void best(int& x, int& y);
    int gaming(int max_depth, int upper_max_score, int first);
};

class GobangScene : public Scene {
private:
    coordinate transfer_vision_to_cell(Vec2 click_position);
    Vec2 transfer_cell_to_vision(coordinate board_position);
    void close(Ref* obj);   // exit mini-game
    void on_mouse_up(Event* event);
    int turn_flag_ = 1;
    Player* my_player;

public:
    static GobangScene* create();
    bool init();

};