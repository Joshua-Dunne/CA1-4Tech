#ifndef EVAL_H
#define EVAL_H

#include <iostream>
#include "Board.h"
#include "BoardTree.h"

class Evaluator
{
public:
	~Evaluator();
	void evaluate(int currPlayer, Board& board, int depth);
	BoardTree tree;

	int maxDepth = 0;

private:
	void setBoard(Board& board, std::pair<int, int>& t_play, int& t_currPlayer);
	int evaluateTemplateWeight(Board& t_boardToEval, int& t_currPlayer);

	void retuneWeights(Board& t_boardToWeigh, int t_currPlayer);
	void rowRetune(Board& t_boardToWeigh, int t_currPlayer);
	void colRetune(Board& t_boardToWeigh, int t_currPlayer);
	void diagonalRetune(Board& t_boardToWeigh, int t_currPlayer);
	int predictWin(Board t_board, int& t_currPlayer);

	std::vector<std::pair<int, int>> opponentMoves;

	int singleWeightIncrease = 25;
	int doubleWeightIncrease = 100;
	int tripleWeightIncrease = 250;
	int winWeightIncrease = 10000;

	// pre-weights that are used as a basis for calculation
	// corners have higher priority by default
	int m_startingWeights[16][4] =
	{
		{40,10,10,40},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{40,10,10,40},
		{40,10,10,40},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{40,10,10,40},
		{40,10,10,40},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{40,10,10,40},
		{40,10,10,40},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{40,10,10,40}
	};

	// predictedWeights start the same as startingWeights,
	// but is changed as evaluation proceeds
	int m_predictedWeights[16][4] =
	{
		{40,10,10,40},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{40,10,10,40},
		{40,10,10,40},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{40,10,10,40},
		{40,10,10,40},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{40,10,10,40},
		{40,10,10,40},
		{10,5 ,5 ,10},
		{10,5 ,5 ,10},
		{40,10,10,40}
	};

	Board templateBoard;
};

#endif