#ifndef EVAL_H
#define EVAL_H

#include <iostream>
#include "Board.h"
#include "BoardTree.h"

class Evaluator
{
public:
	std::vector<std::pair<int, std::pair<int, int>>> evaluate(int currPlayer, Board board, int depth);
	BoardTree tree;

	int maxDepth = 2;

private:
	void setBoard(Board& board, std::pair<int, int>& t_play, int& t_currPlayer);
	int evaluateTemplateWeight(int& t_currPlayer);
	int predictWin(int& t_currPlayer);
	void resetTemplate(Board& toCopy);

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

	Board templateBoard;
};

#endif