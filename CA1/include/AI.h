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
	AI(int t_playNum);
	
	void makePlay(Board& t_board);
	std::vector<BoardTree> trees;
	
private:
	int playNum;
	int maxDepth = 1;
	bool min = false;
	int finalScore = 0;
	std::vector<Node*> predictedMoves;
	PickedMove getMove(Board& t_board);
	Node* miniMax(int t_currentDepth, Node* t_workingNode);

};

#endif