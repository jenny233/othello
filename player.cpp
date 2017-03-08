#include "player.hpp"

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
    this->board = Board();
    this->side = side;
    if(side == BLACK) this->opSide = WHITE;
    else this->opSide = BLACK;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the influence of corners, edges, etc. on the score of a particular
 * move.
 */
int Player::getWeight(Move *move){
    int x = move->getX();
    int y = move->getY();
    if((x == 0 || x == 7) && (y == 0 || y == 7)){   // Corners
        return 15;
    }else if(x == 0 || x == 7 || y == 0 || y == 7){ // Edges
        if(x == 1 || x == 6 || y == 1 || y == 6){   // One next to corner
            return -5;
        }else{
            return 10;                               // Normal Edges
        }
    }else if((x == 1 || x == 6) && (y == 1 || y == 6)){ // Diag. next to corner
        return -10;
    }else{
        return 0;
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

    board.doMove(opponentsMove, opSide);
    std::map<Move*, int> nextMoves;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *move = new Move(i, j);
            if (board.checkMove(move, side)){
                Board* newBoard = board.copy();
                newBoard->doMove(move, side);
                int score = newBoard->count(side) - board.count(side);
                nextMoves[move] = score + getWeight(move);
                delete newBoard;
                //std::cerr << i << " " << j << " score: " << score << std::endl;
            }else{
                delete move;
            }
        }
    }
    Move* bestMove = nullptr;
    int bestScore = -1000000000;
    for(std::map<Move*,int>::iterator it = nextMoves.begin(); it != nextMoves.end(); it++){
        if(it->second > bestScore){
            bestMove = it->first;
        }
        //delete it->first;
    }
    //std::cerr << "play move "<<bestMove->getX()<<" "<<bestMove->getY() << std::endl;
    board.doMove(bestMove, side);
    return bestMove;

}
