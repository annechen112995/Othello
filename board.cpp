#include <iostream>
#include "board.hpp"

#define SIZE 8
#define CENTER 1
#define CORNER 5
#define EDGE 3
#define DANGER -4

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 + * Counts the score from the heuristic function of the current state of the board
 + */
int Board::countScore(Side side) {
    int stone_a = 0, stone_opp = 0, score = 0; // board position score
 
    Side other;
    if (side == BLACK) {
        other = WHITE;
    }
    else {
        other = BLACK;
    }

    stone_a += countCorner(side);
    stone_a += countEdge(side);
    stone_a += countCenter(side);
    stone_opp += countCorner(other);
    stone_opp += countEdge(other);
    stone_opp += countCenter(other);

    //board position score = (# stones you have) - (# stones your opponent has)
    score = stone_a - stone_opp;

    return score;
}

/*
 * Counts the score made by corenrs
 */
int Board::countCorner(Side side) {
    int score = 0;
    if (get(side, 0, 0)) {
        score += CORNER;
    }
    if (get(side, 0, SIZE-1)) {
        score += CORNER;
    }
    if (get(side, SIZE-1, 0)) {
        score += CORNER;
    }
    if (get(side, SIZE-1, SIZE-1)) {
        score += CORNER;
    }
    return score;
}

/*
 * Counts the score made by edges
 */
int Board::countEdge(Side side) {
    int score = 0;
    for (int i = 1; i < SIZE - 1; i++) { // top and bottom edges
        if (i != 1 && i != SIZE - 2) {
            if (get(side, i, 0)) {
                score += EDGE;
            }
            if (get(side, i, SIZE - 1)) {
                score += EDGE;
            }
        }
        else { // (1, 0), (6, 0), (1, 7), (6, 7) are in the danger zone
            if (get(side, i, 0)) {
                score += DANGER;
            }
            if (get(side, i, SIZE - 1)) {
                score += DANGER;
            }
        }
    }
    for (int j = 1; j < SIZE - 1; j++) { // left and right edges
        if (j != 1 && j != SIZE - 2) {
            if (get(side, 0, j)) {
                score += EDGE;
            }
            if (get(side, SIZE - 1, j)) {
                score += EDGE;
            }
        }
        else { // (0, 1), (0, 6), (7, 1), (7, 6) are in the danger zone
            if (get(side, 0, j)) {
                score += DANGER;
            }
            if (get(side, SIZE - 1, j)) {
                score += DANGER;
            }            
        }
    }
    return score;
}

/*
 * Counts the score made by normal squares which are not on the edges
 */
int Board::countCenter(Side side) {
    int score = 0;
    for (int i = 1; i < SIZE - 1; i++) {
        for (int j = 1; j < SIZE - 1; j++) {
            // (1, 1), (6, 1), (6, 1), (6, 6) are in the danger zone
            if ((i==1 && j==1) || (i==1 && j==SIZE-2) || (i==SIZE-2 && j==1) || (i==SIZE-2 && j==SIZE-2)) {
                if (get(side, i, j)) {
                    score += DANGER;
                }
            }
            else {
                if (get(side, i, j)) {
                    score += CENTER;
                }
            }
        }
    }
    return score;
}

/*
 * Returns a vector of all the possible moves at the current board
 */
void Board::allmoves(std::vector<Move *> &vec, Side side) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            Move *current = new Move(i, j);
            if (checkMove(current, side)) {
                vec.push_back(current);
            }
        }
    }
}


/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
}
