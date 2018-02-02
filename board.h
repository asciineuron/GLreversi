#ifndef BOARD_H
#define OARD_MOVER_H

#include <iostream>
#include <utility>
#include <string>

using namespace std;

class Board
{
public:
    const int WHITE = 1;
    const int BLACK = -1;
    const int EMPTY = 0;
    
    int data[8][8];
    
    Board();
    
    bool test_right(pair<int, int> position, int player_to_move); //pointer to data
    bool test_left(pair<int, int> position, int player_to_move);
    bool test_up(pair<int, int> position, int player_to_move);
    bool test_down(pair<int, int> position, int player_to_move);
    bool test_up_left(pair<int, int> position, int player_to_move);
    bool test_up_right(pair<int, int> position, int player_to_move);
    bool test_down_left(pair<int, int> position, int player_to_move);
    bool test_down_right(pair<int, int> position, int player_to_move);
    bool is_valid_move(pair<int, int> position, int player_to_move); //checks in each direction to see if touching opposite color and bounding them in a straight line with another of player's disk
    void enact_move(pair<int, int> position, int player_to_move); //same as above, but changes color of bound disks
    void print_data(pair<int, int> score, int player_to_move);
    
private:
    
};

#endif
