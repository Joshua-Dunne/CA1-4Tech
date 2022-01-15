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
	void copyBoard(Board& toCopy);
	void resetTemplate();

	void retuneWeights(int t_currPlayer);
	void rowRetune(Board& t_boardToWeigh, int t_currPlayer);
	void colRetune(Board& t_boardToWeigh, int t_currPlayer);
	void diagonalRetune(Board& t_boardToWeigh, int t_currPlayer);

	// pre-weights that are used as a basis for calculation
	// corners have higher priority by default
	int m_startingWeights[16][4] =
	{
		{20,10,10,20},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{20,10,10,20},
		{20,10,10,20},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{20,10,10,20},
		{20,10,10,20},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{20,10,10,20},
		{20,10,10,20},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{20,10,10,20}
	};

	// predictedWeights start the same as startingWeights,
	// but is changed as evaluation proceeds
	int m_predictedWeights[16][4] =
	{
		{20,10,10,20},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{20,10,10,20},
		{20,10,10,20},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{20,10,10,20},
		{20,10,10,20},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{20,10,10,20},
		{20,10,10,20},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{20,10,10,20}
	};

	Board templateBoard;
};

#endif