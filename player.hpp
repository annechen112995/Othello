#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <vector>
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

protected: 
	Board *board;
	Side player_a; 
	Side player_opp;

	vector<int> arr_score;
    int minimax(Board *node, int depth, bool max_player);
    Move * next_move;

};

#endif
