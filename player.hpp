#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
#include <map>
using namespace std;

class Player {

private:
    Side side;
    Side opSide;
    Board board;
    int getWeight(Move *move);
    int worstScore(Board *b,int depth, int currD);
    int calculateScore(Board *b);
    int pieceScore(int i, int j, Board *b);
public:
    Player(Side side);
    ~Player();
    Move *doMove(Move *opponentsMove, int msLeft);
    Move *doMove2(Move *opponentsMove, int msLeft);
    int worstScore(Board *b,int depth, int currD, int alpha, int beta);
    int bestestScore(Board *b, int depth, int currD, int alpha, int beta);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
