#ifndef AI_H
#define AI_H

#include <iostream>
#include <vector>
#include <string>
#include "Board.h"
#include "Evaluator.h"

/// <summary>
/// Struct to hold X,Y position to play, value of move picked
/// </summary>
struct PickedMove
{
	PickedMove() {}
	int x = -1; // default to -1 in case of error
	int y = -1;
};

class AI
{
public:
	AI() = delete;
	AI(int t_playNum, Board& t_board);
	
	void makePlay();

	std::vector<BoardTree> trees;
	
private:
	int playNum;
	int pickedBoard = -1;
	Board& m_board;
	int maxDepth = 2;

	PickedMove getMove();
	Node* miniMax(int t_currentDepth, Node* t_workingNode);

	bool min = false;
	int finalScore = 0;
	Node* smallest = nullptr;
	Node* biggest = nullptr;
	Node* bestMove = nullptr;
	std::vector<Node*> predictedMoves;

};

#endif