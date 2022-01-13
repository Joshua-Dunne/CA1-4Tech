#include "..\include\AI.h"

AI::AI(int t_playNum, std::vector<Board>& t_boards) : playNum(t_playNum), m_boards(t_boards)
{
}

void AI::makePlay()
{
	pickedBoard = 0;

	// pick the best move to make
	PickedMove decision = getMove();

	// update the board where the play should be made
	m_boards[pickedBoard].m_boardData[decision.x][decision.y] = playNum;
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
	for (auto& board : m_boards)
	{
		// Evaluates values of plays and where plays can be made
		Evaluator eval;
		plays = eval.evaluate(playNum, board, 0);
		int value = 999999;

		// go through each play and determine the best play
		// the best play will have the highest value
		for (size_t i = 0; i < plays.size(); i++)
		{
			// the higher the value of a play,
			// the better the play is
			// also make sure the space the AI is trying to play on isn't full
			if (plays[i].first < value && board.m_boardData[plays[i].second.first][plays[i].second.second] == 0)
			{
				pickedBoard = currBoard - 1;
				value = plays[i].first;
				decidingMove.x = plays[i].second.first;
				decidingMove.y = plays[i].second.second;
			}
		}

		trees.push_back(eval.tree);

		currBoard++; // starts on 1, incremenets to 2, 3, 4
	}

	return decidingMove;
}
