#include "player.hpp"

#define DEPTH 3

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
            return -10;
        }else{
            return 10;                               // Normal Edges
        }
    }else if((x == 1 || x == 6) && (y == 1 || y == 6)){ // Diag. next to corner
        return -10;
	}
     else if (( x == 1 || x == 6) || (y == 1 || y == 6))
     {
		 return -5;
	 }
    else{
        return 0;
    }
}

/**
 * Compute the worst score that can happen after doing a certain move on
 * our side.
 * 
 * @param 
 * Board *b    The board after our move is performed
 * int depth   The number of steps we want to trace down
 * int currD   The current depth, used for DFS
 * 
 * @return
 * int worst   The worst score that can happen
 *             One round of speculation = 1 x opponent + 1 x us
 */
int Player::worstScore(Board *b,int depth, int currD, int alpha, int beta){
    
    //std::cerr << "depth: "<<depth << " " << currD << std::endl;
    
    
    if(depth == currD){
        // return the difference of stones between us and opponent
        //return b->count(this->side) - b->count(this->opSide);
        return calculateScore(b);
    }
    
    int worst = 100000000;
    
    // Get a list of moves the opponent can make
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Board *opBoard = b->copy();
            Move *opMove = new Move(i, j);
            if (opBoard->checkMove(opMove, opSide)){
                
                //std::cerr<<"opMove: "<<i<<" "<<j<<std::endl;
                
                opBoard->doMove(opMove, opSide);
                
                // For each of the opponent's moves
                // get a list of moves we can make to react
                for(int k = 0; k < 8; k++){
                    for(int l = 0; l < 8; l++){
                        // Calculate a worst score for each of them
                        Board *ourBoard = opBoard->copy();
                        Move *ourMove = new Move(k, l);
                        if(ourBoard->checkMove(ourMove, this->side)){
                            
                            //std::cerr<<"\tourMove: "<<k<<" "<<l;
                            
                            
                            ourBoard->doMove(ourMove, this->side);
                            int score = bestestScore(ourBoard, depth, currD+1, alpha, beta);
                            
                            //std::cerr<<"\tscore: "<<score<<std::endl;
                            
                            if (score < worst) worst = score;
                            if (alpha < worst)
                            {
								alpha = worst;
							}

										
                        }
                        delete ourBoard;
                        delete ourMove;
                    }
                }      
            }
            delete opBoard;
            delete opMove;
        }
    }
    //std::cerr<<"worst score: "<<worst<<std::endl;
    return worst;
}
    
/**
 * Compute the best score that can happen for an opponent after doing a certain move on
 * our side.
 * 
 * @param 
 * Board *b    The board after our move is performed
 * int depth   The number of steps we want to trace down
 * int currD   The current depth, used for DFS
 * 
 * @return
 * int min    The minimum score that can happen for an opponent
 *             One round of speculation = 1 x opponent + 1 x us
 */
int Player::bestestScore(Board *b,int depth, int currD, int alpha, int beta){
    
    //std::cerr << "depth: "<<depth << " " << currD << std::endl;
    
    
    if(depth == currD){
        // return the number of pieces on this->side
        //return b->count(this->side);
        return calculateScore(b);
    }
    
    int min = -100000000;
    
    // Get a list of moves the opponent can make
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Board *opBoard = b->copy();
            Move *opMove = new Move(i, j);
            if (opBoard->checkMove(opMove, opSide)){
                
                //std::cerr<<"opMove: "<<i<<" "<<j<<std::endl;
                
                opBoard->doMove(opMove, opSide);
                int score = worstScore(opBoard, depth, currD + 1, alpha, beta);
                if (score > min)
                {
					min = score;
				}
				if (min < beta) beta = min;

				if (beta > alpha) return beta;

				
            }
            delete opBoard;
            delete opMove;
        }
    }
    //std::cerr<<"worst score: "<<worst<<std::endl;
    return min;
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

    // Find the list of all possible moves
    // Calculate the worst score of these moves
    // Find the best worst score out of these moves
    // Do the move
    
    board.doMove(opponentsMove, opSide);
    
    
    if(!board.hasMoves(side)){
        return 0;
    }
    int alpha = -10000;
    int beta = 10000;
    
    // Best score is the max out of all the worst scores
    int bestScore = -1000000000;
    // Best move is the move with the best score
    Move* bestMove = new Move(0,0);
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *move = new Move(i, j);
            Board* newBoard = board.copy();
            if (newBoard->checkMove(move, side)){
                
                
                //std::cerr<<"--------Our Move: "<<i<<" "<<j<<"---------"<<std::endl;
                
                
                newBoard->doMove(move, side);
                int score = this->worstScore(newBoard, DEPTH, 0, alpha, beta);
                int score2 = this->bestestScore(newBoard, DEPTH, 0, alpha, beta);

                alpha = score;
                beta = score2;
                if(score > bestScore){
                    bestScore = score;
                    bestMove->setX(move->getX());
                    bestMove->setY(move->getY());
                    //std::cerr <<"new best score: " << score << std::endl;
                }
                
            }
            delete newBoard;
            delete move;
        }
    }
    
    //std::cerr <<"do move" <<std::endl;
    board.doMove(bestMove, side);
    return bestMove;

}


int Player::calculateScore(Board *b){
    int score = 0;
    for(int i=0; i<8; i++){
        for(int j=0; j<8; j++){
            
            if(b->get(this->side, i, j)){
                score += pieceScore(i, j, b);
            }else if(b->get(this->opSide, i, j)){
                score -= pieceScore(i, j, b);
            }
        }
    }
    return score;
}

int Player::pieceScore(int i, int j, Board *b){
    int score = 0;
    if( (i==0 || i==7) && (j==0 || j==7) ){   // Corners GOOD!!
        score += 30;
    }else if( i==0 || i==7 || j==0 || j==7 ){ // Edges
        
        if( i==1 || i==6 || j==1 || j==6 ){    // Edge next to corner BAD!!
            score -= 5;
        }else{                                 // Not next to corner GOOD!!
            score += 5;
        }
        
    }else if(i==1 && j==1 && !b->occupied(0, 0)){// Corner next to corner BAAAAAD!!!
        score -= 10;
    }else if(i==1 && j==6 && !b->occupied(0, 7)){
        score -= 10;
    }else if(i==6 && j==1 && !b->occupied(7, 0)){
        score -= 10;
    }else if(i==6 && j==6 && !b->occupied(7, 7)){
        score -= 10;
    }
    return score;
}
