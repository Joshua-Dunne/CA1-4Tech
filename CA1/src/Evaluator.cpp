#include "..\include\Evaluator.h"

/// <summary>
/// Using generated valid modes,
/// evaluate the strength of each board play
/// </summary>
/// <param name="turnPlayer">Current turn player</param>
std::vector<int> Evaluator::evaluate(int currPlayer, Board& board)
{
    if (currPlayer > 2 && currPlayer < 1)
    {
        std::cout << "Error! Please only enter either 1 or 2 for evaluate function." << std::endl;
        return std::vector<int>(-1);
    }

    std::vector<int> weightOfEachBoard;

    // get all the valid places where a move could be played on the board
    // stored within this class
    std::vector<std::pair<int, int>> moves = board.getVaildMoves();

    // now we will go through each possible move on the current board
    // and evaluate how much weight there is for the move
    for (auto pair : moves)
    {
        setBoard(board, pair, currPlayer);
        weightOfEachBoard.push_back(evaluateTemplateWeight(board, pair, currPlayer));
        resetTemplate();
    }

    // return the final calculated weights
    return weightOfEachBoard;
}

/// <summary>
/// Set up template board with required information
/// </summary>
/// <param name="t_board">Board to look at</param>
/// <param name="t_play">Where the next play is to be made</param>
/// <param name="t_currPlayer">What player is making the play</param>
void Evaluator::setBoard(Board& t_board, std::pair<int, int>& t_play, int& t_currPlayer)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            templateBoard[i][j] = t_board.m_boardData[i][j];
        }
    }

    templateBoard[t_play.first][t_play.second] = t_currPlayer;
}

/// <summary>
/// Evaluate total cost of the board,
/// based on the number of spaces with weights left
/// </summary>
/// <returns>Total weight of board</returns>
int Evaluator::evaluateTemplateWeight(int& t_currPlayer)
{
    int total = 0;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (templateBoard[i][j] == 0)
            {
                total += startingWeights[i][j];
            }
        }
    }

    // add to the total if a win is possible on this play
    total += predictWin(t_currPlayer);

    return total;
}

/// <summary>
/// Using the template board that is set up
/// with the next possible play,
/// we can predict if a win will happen on this board play.
/// </summary>
/// <param name="t_currPlayer">What player to check for</param>
/// <returns>If a board play wins, the win value is returned to boost the board total</returns>
int Evaluator::predictWin(int& t_currPlayer)
{
    // Row and Col Checks (2D)
    for (int i = 0; i < 4; i++)
    {
        if (templateBoard[i][0] == t_currPlayer && templateBoard[i][1] == t_currPlayer
            && templateBoard[i][2] == t_currPlayer && templateBoard[i][3] == t_currPlayer)
        {
            return winPredictionValue;
        }

        if (templateBoard[0][i] == t_currPlayer && templateBoard[1][i] == t_currPlayer
            && templateBoard[2][i] == t_currPlayer && templateBoard[3][i] == t_currPlayer)
        {
            return winPredictionValue;
        }
    }

    // Diagonal Checks (2D)
    if (templateBoard[0][3] == t_currPlayer && templateBoard[1][2] == t_currPlayer
        && templateBoard[2][1] == t_currPlayer && templateBoard[3][0] == t_currPlayer)
    {
        return winPredictionValue;
    }
    
    if (templateBoard[0][0] == t_currPlayer && templateBoard[1][1] == t_currPlayer
        && templateBoard[2][2] == t_currPlayer && templateBoard[3][3] == t_currPlayer)
    {
        return winPredictionValue;
    }

    return 0; // if no win has been predicted on this play, return no total change
}

/// <summary>
/// Reset template board back to zero state
/// </summary>
void Evaluator::resetTemplate()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            templateBoard[i][j] = 0;
        }
    }
}
