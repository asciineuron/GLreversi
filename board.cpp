#include <iostream>
#include <utility>
#include <string>
#include "board.h"

using namespace std;

Board::Board::Board()
{
	//starting positions of disks:
	data[3][3] = WHITE;
	data[4][4] = WHITE;
	data[3][4] = BLACK;
	data[4][3] = BLACK;
    data[6][4] = EMPTY;
}


bool Board::test_right(pair<int, int> position, int player_to_move) 
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (data[position.second][position.first + i] != opposite_color) return false;
    
    for (; (position.first + i) < sizeof(data[position.second])/sizeof(*data[position.second]) && data[position.second][position.first + i] == opposite_color; i++) {}
    
	if ((position.first + i) < sizeof(data[position.second])/sizeof(*data[position.second]))
	{
		//if next spot still within board (where matching piece would be)
		tester = data[position.second][position.first + i];
		if (tester == player_to_move)
		{
			result = true;
		}	
	} else
    {
        result = false;
    }
    return result;
}

bool Board::test_left(pair<int, int> position, int player_to_move)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (data[position.second][position.first - i] != opposite_color) return false;
    
    for (; (position.first - i) > -1 && data[position.second][position.first - i] == opposite_color; i++) {}
	
	if (position.first - i > -1)
	{
		tester = data[position.second][position.first - i];
		if (tester == player_to_move)
		{
			result = true;
		}
	} else
    {
        result = false;
    }
    
    return result;
}

bool Board::test_up(pair<int, int> position, int player_to_move)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (data[position.second - i][position.first] != opposite_color) return false;
    
    for (; (position.second - i > -1) && data[position.second - i][position.first] == opposite_color; i++) {}
       	
    if (position.second - i > -1)
	{
		tester = data[position.second - i][position.first];
        //cout << "tester" << tester << " player " << player_to_move << endl;
		if (tester == player_to_move)
		{
            result = true;
		}
	} else
    {
        result = false;
    }
    return result;
}
     
bool Board::test_down(pair<int, int> position, int player_to_move)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    //check that adjacent is opposite color before checking all
    if (data[position.second + i][position.first] != opposite_color) return false;
    
    for (; (position.second + i) < sizeof(data)/sizeof(*data) && data[position.second + i][position.first] == opposite_color; i++) {}
	
	if (position.second + i < sizeof(data)/sizeof(*data))
	{
		tester = data[position.second + i][position.first];
		//cout << "tester" << tester << " player " << player_to_move << endl;
        if (tester == player_to_move)
		{
			result = true;
		}
	} else
    {
        result = false;
    }	
    return result;
}
     
bool Board::test_up_left(pair<int, int> position, int player_to_move)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (data[position.second - i][position.first - i] != opposite_color) return false;
    
    for (; (position.second - i > -1) && ((position.first - i) > -1) && data[position.second - i][position.first - i] == opposite_color; i++){}
    
    if ((position.second - i > -1) && (position.first - i))
    {
        tester = data[position.second - i][position.first - i];
        if (tester == player_to_move)
        {
            result = true;
        }
    } else
    {
        result = false;
    }
    return result;
}
  
bool Board::test_up_right(pair<int, int> position, int player_to_move)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (data[position.second - i][position.first + i] != opposite_color) return false;
    
    for (; (position.second - i > -1) && ((position.first + i) < sizeof(data[position.second])/sizeof(*data[position.second])) && data[position.second - i][position.first + i] == opposite_color; i++) {}
	
	if ((position.second - i > -1) && (position.first + i) < sizeof(data[position.second])/sizeof(*data[position.second]))
	{
		tester = data[position.second - i][position.first + i];
		if (tester == player_to_move)
		{
			result = true;
		}
	} else
    {
        result = false;
    }
    return result;
}

bool Board::test_down_left(pair<int, int> position, int player_to_move)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (data[position.second + i][position.first - i] != opposite_color) return false;
    
    for (; (position.first - i > -1) && ((position.second + i) < sizeof(data)/sizeof(*data)) && data[position.second + i][position.first - i] == opposite_color; i++) {}
    
    if (position.second - i > -1 && position.second + i < sizeof(data)/sizeof(*data))
    {
            tester = data[position.second + i][position.first - i];
            if (tester == player_to_move)
            {
                    result = true;
            }
    } else
    {
        result = false;
    }
    return result;
}

bool Board::test_down_right(pair<int, int> position, int player_to_move)
{
    int tester = EMPTY;
    int opposite_color = -1 * player_to_move;
    int i = 1;
    
    bool result = false;
    
    if (data[position.second + i][position.first + i] != opposite_color) return false;
    
    for (; (position.second + i < sizeof(data)/sizeof(*data)) && (position.first + i < sizeof(data[position.second])/sizeof(*data[position.second])) && data[position.second + i][position.first + i] == opposite_color; i++) {}
    
    if ((position.second + i < sizeof(data)/sizeof(*data)) && (position.first + i < sizeof(data[position.second])/sizeof(*data[position.second])))
    {
        tester = data[position.second + i][position.first + i];
        if (tester == player_to_move)
        {
            result = true;
        }
    } else
    {
        result = false;
    }
    
    return result;
}

bool Board::is_valid_move(pair<int, int> position, int player_to_move) //checks in each direction to see if touching opposite color and bounding them in a straight line with another of player's disk
{
    if (data[position.second][position.first] != EMPTY) return false; //if position is occupied

	return (test_up(position, player_to_move) || test_down(position, player_to_move) || test_left(position, player_to_move) || test_right(position, player_to_move) || test_up_left(position, player_to_move) || test_up_right(position, player_to_move) || test_down_left(position, player_to_move) || test_down_right(position, player_to_move));
}

void Board::enact_move(pair<int, int> position, int player_to_move) //same as above, but changes color of bound disks
{
    //check each direction, then clear pieces
	cout << "good!" << endl;
    int opposite_color = -1*player_to_move;
    data[position.second][position.first] = player_to_move;
    int i = 1;
    if (test_right(position, player_to_move))
    {
        //no need for rest of function, already tested above
        for (; (position.first + i) < sizeof(data[position.second])/sizeof(*data[position.second]) && data[position.second][position.first + i] == opposite_color; i++)
        {
            data[position.second][position.first + i] = player_to_move; 
        }
    }
    i = 1;
    if (test_left(position, player_to_move))
    {
        for (; (position.first - i) > -1 && data[position.second][position.first - i] == opposite_color; i++)
        {
            data[position.second][position.first - i] = player_to_move;
        }
    }
    i = 1;
    if (test_up(position, player_to_move))
    {
        for (; (position.second - i > -1) && data[position.second - i][position.first] == opposite_color; i++) 
        {
            data[position.second - i][position.first] = player_to_move;
        }
    }
    i = 1;
    if (test_down(position, player_to_move))
    {
        for (; (position.second + i) < sizeof(data)/sizeof(*data) && data[position.second + i][position.first] == opposite_color; i++)
        {
            data[position.second + i][position.first] = player_to_move;
        }
    }
    i = 1;
    if (test_up_right(position, player_to_move))
    {
        for (; (position.second - i > -1) && ((position.first + i) < sizeof(data[position.second])/sizeof(*data[position.second])) && data[position.second - i][position.first + i] == opposite_color; i++) 
        {
            data[position.second - i][position.first + i] = player_to_move;
        }
    }
    i = 1;
    if (test_up_left(position, player_to_move))
    {
        for (; (position.second - i > -1) && ((position.first - i) > -1) && data[position.second - i][position.first - i] == opposite_color; i++)
        {
            data[position.second - i][position.first - i] = player_to_move;
        }
    }
    i = 1;
    if (test_down_left(position, player_to_move))
    {
         for (; (position.first - i > -1) && ((position.second + i) < sizeof(data)/sizeof(*data)) && data[position.second + i][position.first - i] == opposite_color; i++) 
         {
             data[position.second + i][position.first - i] = player_to_move;
        }
    }
    i = 1;
    if (test_down_right(position, player_to_move))
    {
        for (; (position.second + i < sizeof(data)/sizeof(*data)) && (position.first + i < sizeof(data[position.second])/sizeof(*data[position.second])) && data[position.second + i][position.first + i] == opposite_color; i++) 
        {
            data[position.second + i][position.first + i] = player_to_move;
        }
    }
}


void Board::print_data(pair<int, int> score, int player_to_move)
{
    //terminal
    cout << "\n  A B C D E F G H" << endl;
	for (int row = 0; row < sizeof(data)/sizeof(*data); row++)
	{
		cout << (row + 1);
		for (int col = 0; col < sizeof(data[row])/sizeof(*data[row]); col++)
		{
			if (data[row][col] == WHITE)
			{
				cout << " W";
			} else if (data[row][col] == BLACK)
			{
				cout << " B";
			} else
			{
				cout << " -";
			}
		}
		cout << " " << (row + 1) << endl;
	}
	cout << "  A B C D E F G H\n" << endl;
	if (player_to_move == WHITE)
	{
		cout << "White's turn" << endl;
	} else if (player_to_move == BLACK)
	{		
		cout << "Black's turn" << endl;
	}
	cout << "White: " << score.first << " Black: " << score.second << "\n\n->";
    //OpenGL in render_board
    
}
