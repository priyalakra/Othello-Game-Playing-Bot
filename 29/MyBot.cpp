#include "Othello.h"
#include "OthelloBoard.h"
#include "OthelloPlayer.h"
#include <cstdlib>
using namespace std;
using namespace Desdemona;

class MyBot: public OthelloPlayer
{
    public:
        /**
         * Initialisation routines here
         * This could do anything from open up bestvaluecache of "best moves" to
         * spawning bestvaluebackground processing thread.
         */
        MyBot( Turn turn );

        /**
         * Play something
         */
        virtual Move play( const OthelloBoard& board );
        virtual int Function_beta(const OthelloBoard& board, int depth, int alpha_minimum, int beta_maximum);
        virtual int Function_alpha(const OthelloBoard& board, int depth, int alpha_minimum, int beta_maximum);
        
        virtual int sumvalue(const OthelloBoard& board, Turn turn);
        
        int move_num = 0;
        Turn myturncolor;
        int eval[8][8]={8,-7, 3, 3, 3, 3,-5, 8,
               -7, -4, 2, 2, 2, 2, -4,-5,
                3, 2, 1, 1, 1, 1, 2, 3,
                3, 2, 1, 1, 1, 1, 2, 3,
                3, 2, 1, 1, 1, 1, 2, 3,
                3, 2, 1, 1, 1, 1, 2, 3,
               -2, -2, 2, 2, 2, 2, -2,-2,
               8,-2, 3, 3, 3, 3,-2, 8};
    private:
};

MyBot::MyBot( Turn turn )
    : OthelloPlayer( turn )
{
}

// The following lines are _very_ important to create bestvaluebot module for Desdemona

extern "C" {
    OthelloPlayer* createBot( Turn turn )
    {
        return new MyBot( turn );
    }

    void destroyBot( OthelloPlayer* bot )
    {
        delete bot;
    }
}

int MyBot::sumvalue(const OthelloBoard& board, Turn turn)
{   int i,j,sum = 0;
   int diff = board.getValidMoves(turn).size() - board.getValidMoves(other(turn)).size();
    i=j=0;
    while(i<8){
        j=0;
        while(j<8){
          if(board.get(i,j)==turn)
          sum = sum+eval[i][j];
          if(board.get(i,j)==other(turn))
          sum = sum-eval[i][j];
          j++;
        }
    i++;
    }
  return sum+diff;
}

Move MyBot::play( const OthelloBoard& board )
{
  move_num++;
  int depth=2;
  if(move_num==1)
  {myturncolor = turn;
  }
    list<Move> moves = board.getValidMoves( turn );
    list<Move>::iterator it;
    int alpha_maximum = -1000;
    Move optimal_move = *(moves.begin());
    it = moves.begin();
    for(it=moves.begin(); it!= moves.end(); it++){
      OthelloBoard copy_board= board;
      copy_board.makeMove(turn,*it);
      int alphabest= Function_beta(copy_board,depth,alpha_maximum,1000);
      if(alphabest>alpha_maximum){
        alpha_maximum = alphabest;
        optimal_move = *it;
      }
    }
    return optimal_move;
}


int MyBot::Function_beta(const OthelloBoard& board, int depth, int alpha_minimum, int beta_maximum){
	Turn opponent;
  if(turn==RED)
  {
        opponent =BLACK;
  }
  else
  {
        opponent =RED;
  }
  list<Move> moves = board.getValidMoves( opponent );
  list<Move>::iterator it = moves.begin();
  int min_beta = beta_maximum;
  for(it=moves.begin(); it!=moves.end(); it++){
    Turn turn_opponent = opponent;
    OthelloBoard copy_board= board;
    copy_board.makeMove(turn_opponent,*it);
    int betabest = Function_alpha(copy_board,depth-1,alpha_minimum,min_beta);
    if(betabest<min_beta){
      min_beta = betabest;
      if(min_beta<alpha_minimum)return alpha_minimum;
    }
  }
  return min_beta;
}

int MyBot::Function_alpha(const OthelloBoard& board, int depth, int alpha_minimum, int beta_maximum){
  if(depth == 0){
    OthelloBoard copy_board= board;
    return sumvalue(copy_board,turn);
  }
  list<Move> moves = board.getValidMoves( turn );
  list<Move>::iterator it = moves.begin();
  int alpha_maximum = alpha_minimum;
  
  for(it=moves.begin(); it!= moves.end(); it++){
    Turn trn = turn;
    OthelloBoard copy_board= board;
    copy_board.makeMove(trn,*it);
    int bestvalue= Function_beta(copy_board,depth,alpha_maximum,beta_maximum);
    if(bestvalue>alpha_maximum){
      alpha_maximum = bestvalue;
      if(alpha_maximum>beta_maximum)return beta_maximum;
    }
  }
  return alpha_maximum;
}


