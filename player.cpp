#include "player.hpp"

#define SIZE 8

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
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Move *move_a = new Move(i, j);
            if (board->checkMove(move_a, player_a)) {
                board->doMove(move_a, player_a);
                return move_a;
            }
        }
    }

    return nullptr;
}
