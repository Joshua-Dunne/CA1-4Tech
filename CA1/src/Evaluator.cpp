#include "..\include\Evaluator.h"

/// <summary>
/// Using generated valid modes,
/// evaluate the strength of each board play
/// </summary>
/// <param name="turnPlayer">Current turn player</param>
void Evaluator::evaluate(int currPlayer, Board& board)
{
        if (currPlayer > 2 || currPlayer < 1)
        {
            std::cout << "Error! Please only enter either 1 or 2 for evaluate function!" << std::endl;
            return;
        }

        // get all the valid places where a move could be played on the board
        // stored within this class
        std::vector<std::pair<int, int>> moves = board.getVaildMoves();

        // now we will go through each possible move on the current board
        // and evaluate how much weight there is for the move
}