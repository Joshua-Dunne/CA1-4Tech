#ifndef EVAL_H
#define EVAL_H

#include <iostream>
#include "Board.h"

class Evaluator
{
public:
	std::vector<int> evaluate(int currPlayer, Board& board);
private:
	void setBoard(Board& board, std::pair<int, int>& t_play, int& t_currPlayer);
	int evaluateTemplateWeight(int& t_currPlayer);
	int predictWin(int& t_currPlayer);
	void resetTemplate();

	int winPredictionValue = 300;

	// pre-weights that are used as a basis for calculation
	// corners have higher priority by default
	int startingWeights[4][4] =
	{
		{20,10,10,20},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{20,10,10,20}
	};

	int templateBoard[4][4] =
	{
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0},
		{0,0,0,0}
	};
};

#endif