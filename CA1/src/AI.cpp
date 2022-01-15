#include "..\include\AI.h"

AI::AI(int t_playNum, Board& t_board) : playNum(t_playNum), m_board(t_board)
{
}

void AI::makePlay()
{
	// pick the best move to make
	PickedMove decision = getMove();

	// update the board where the play should be made

	if (m_board.m_boardData[decision.x][decision.y] == 0)
		m_board.m_boardData[decision.x][decision.y] = playNum;
	else
		throw std::string("Error! AI tried to play on an already existing move on Board " + std::to_string(pickedBoard + 1)
			+ " @: " + std::to_string(decision.x) + ", " + std::to_string(decision.y));
}

PickedMove AI::getMove()
{
	trees.clear();
	// Consists of 2 vectors within a pair
	// First is the value of each play
	// Second is where the play is made
	std::vector<std::pair<int, std::pair<int, int>>> plays;

	PickedMove decidingMove;
	int currBoard = 1;

	// first we will go through all the boards,
	// and get the best move on each board
	// Evaluates values of plays and where plays can be made
	Evaluator eval;
	plays = eval.evaluate(playNum, m_board, 0);

	std::vector<Node*> bestMoves;

	int treeCount = 0;

	eval.tree.toRoot();
	Node* bestMove = miniMax(0, eval.tree.m_current);

	decidingMove.x = bestMove->x;
	decidingMove.y = bestMove->y;

	return decidingMove;
}

/// <summary>
/// function MINIMAX(N)
/// begin
/// if N is deep enough then
/// return the estimated score of this leaf
/// else
/// Let N1, N2, .., Nm be the successors of N;
/// if N is a Min node then
/// return min{ MINIMAX(N1), .., MINIMAX(Nm) }
/// else
/// return max{ MINIMAX(N1), .., MINIMAX(Nm) }
/// end MINIMAX;
/// </summary>
/// <returns>next Node to play</returns>
Node* AI::miniMax(int t_currentDepth, Node* t_workingNode)
{
	if (t_currentDepth > maxDepth)
	{
		return t_workingNode;
	}

	std::vector<Node*> workingNodes;

	t_currentDepth++;

	for (size_t i = 0; i < t_workingNode->children.size(); i++)
	{
		workingNodes.push_back(miniMax(t_currentDepth, t_workingNode->children[i]));
	}

	
	Node* nodeToReturn = workingNodes[0];

	if (min)
	{
		min = false; // flip between min/max

		for (size_t i = 1; i < workingNodes.size(); i++)
		{
			if (workingNodes[i]->value < nodeToReturn->value)
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


	// in case of emergency, return the passed in node
	return t_workingNode;
}
