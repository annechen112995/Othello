#include "player.hpp"
#include "limits.h"
#include <unistd.h>

#define SIZE 8
#define DEPTH 2

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {

    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

    board = new Board();
 
    player_a = side;
    if (player_a == BLACK) {
        player_opp = WHITE;
        std::cerr << "Player_A is BLACK" << std::endl;
    }
    else {
        player_opp = BLACK;
        std::cerr << "Player_A is WHITE" << std::endl;
    }

}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}

/*
 * Runs 
 */

int Player::minimax(Board *node, int depth, bool max_player) {
    int best_score, current_score, it;
    std::vector<Move *> pos_move;
    // helper to take care of sides
    Side current_side;
    if (DEPTH % 2 == 1) {
        (depth % 2 == 1) ? current_side = player_a : current_side = player_opp;
    }
    else {
        (depth % 2 == 0) ? current_side = player_a : current_side = player_opp;
    }

    if (depth == 0 || !node->hasMoves(current_side)) { // depth is 0 or the current board has no more possible moves
        return node->countScore(player_a);
    }

    if (max_player == true) {
        best_score = INT_MIN;
        node->allmoves(pos_move, player_a); // store all the possible movements in pos_move vector
        for (int i = 0; i < (int)pos_move.size(); i++) {
            Board *temp = node->copy();
            temp->doMove(pos_move[i], player_a); // move
            current_score = minimax(temp, depth - 1, false);
            if (current_score > best_score) {
                best_score = current_score;
                it = i;
            }
            delete temp;
        }
        if (depth == DEPTH) { // if the decided best score is from the choices of the move that needs to be made
            next_move = new Move(pos_move[it]->getX(), pos_move[it]->getY());
        }
        return best_score;
    }
    else { 
        best_score = INT_MAX;
        node->allmoves(pos_move, player_opp);
        for (int i = 0; i < (int)pos_move.size(); i++) {
            Board *temp = node->copy();
            temp->doMove(pos_move[i], player_opp); // move
            current_score = minimax(temp, depth - 1, false);
            if (current_score < best_score) {
                best_score = current_score;
            }
            delete temp;
        }
        return best_score;
    }
}


/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */

    board->doMove(opponentsMove, player_opp);

    // top left most move
    /* for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Move *move_a = new Move(i, j);
            if (board->checkMove(move_a, player_a)) {
                board->doMove(move_a, player_a);
                return move_a;
            }
        }
    }

    return nullptr;
    */

    // Simple Player
    /*
    // go through all the squares and check if they have possible moves
    // then copy board and simulate the move and check the score
    int current_score, best_score = INT_MIN;
    int count = 0;
    Move *current = new Move(0, 0);
    Move *best = new Move(0, 0);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            current->setX(i);
            current->setY(j);
            if (board->checkMove(current, player_a)) {
                count++;
                Board *temp = board->copy();
                temp->doMove(current, player_a);
                current_score = temp->countScore(player_a);
                if (current_score > best_score) {
                    best_score = current_score;
                    best->setX(current->getX());
                    best->setY(current->getY());
                }
                delete temp;
            }
        }
    }
    delete current;
    if (count == 0) {
        return NULL;
    }
    else {
        board->doMove(best, player_a);
        return best;
    }
    */
      
    // Minimax Algorithm
  
    // use helper function: Player::minimax
    // the best move at current state is saved at variable next_move
    if (!board->hasMoves(player_a)) {
        return NULL;
    }
    minimax(board, DEPTH, true);
    board->doMove(next_move, player_a);
    return next_move;
}
