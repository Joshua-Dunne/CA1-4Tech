#include "..\include\AI.h"

AI::AI(int t_playNum) : playNum(t_playNum)
{
}

void AI::makePlay(Board& t_board)
{
	// pick the best move to make
	PickedMove decision = getMove(t_board);

	// update the board where the play should be made
	if (t_board.m_boardData[decision.x][decision.y] == 0)
		t_board.m_boardData[decision.x][decision.y] = playNum;
	else
		throw std::string("Error! AI tried to play on an already existing move @: "
			+ std::to_string(decision.x) + ", " + std::to_string(decision.y));
}

PickedMove AI::getMove(Board& t_board)
{
	PickedMove decidingMove;
	// first we will go through all the boards,
	// and get the best move on each board
	// Evaluates values of plays and where plays can be made
	Evaluator eval;
	eval.maxDepth = maxDepth;
	eval.evaluate(playNum, t_board, 0);
	eval.tree.toRoot();
	Node* bestMove = miniMax(0, eval.tree.getRoot());

	decidingMove.x = bestMove->x;
	decidingMove.y = bestMove->y;

	std::cout << "Picked " << bestMove->x << ", " << bestMove->y << " @ value: " << bestMove->value << std::endl;

	return decidingMove;
}

/// <summary>
/// Minimax implementation as found on the slides
/// </summary>
/// <returns>next Node to play</returns>
Node* AI::miniMax(int t_currentDepth, Node* t_workingNode)
{
	t_currentDepth++;

	if (t_currentDepth > maxDepth)
	{
		return t_workingNode;
	}

	std::vector<Node*> workingNodes;


	for (size_t i = 0; i < t_workingNode->children.size(); i++)
	{
		workingNodes.push_back(miniMax(t_currentDepth, t_workingNode->children[i]));
	}

	if (workingNodes.size() > 0)
	{ // in the event that we are at the end of the board
		// there might be a chance the nodes to go through
		// will be zero, as there's no possible spots left
		// to play a move.
		// If this happens, we simply return the t_workingNode variable
		// as a catch all.
		Node* nodeToReturn = workingNodes[0];

		if (min)
		{
			min = false; // flip between min/max
			for (size_t i = 1; i < workingNodes.size(); i++)
			{
				if (workingNodes[i]->value < nodeToReturn->value && workingNodes[i]->value >= 0)
					nodeToReturn = workingNodes[i];
			}			
			return nodeToReturn;
		}

		if (!min)
		{
			min = true; // flip between min/max
			for (size_t i = 1; i < workingNodes.size(); i++)
			{
				if (workingNodes[i]->value > nodeToReturn->value)
					nodeToReturn = workingNodes[i];
			}
			return nodeToReturn;
		}
	}
	


	// in case of emergency, return the passed in node
	return t_workingNode;
}
