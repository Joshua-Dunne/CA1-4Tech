#include "..\include\Evaluator.h"

/// <summary>
/// Using generated valid modes,
/// evaluate the strength of each board play
/// </summary>
/// <param name="turnPlayer">Current turn player</param>
/// <param name="board">the board to estimate on</param>
/// <returns>value of each play, where the play is made</returns>
std::vector<std::pair<int, std::pair<int, int>>> Evaluator::evaluate(int currPlayer, Board board, int depth)
{
    if (currPlayer > 2 && currPlayer < 1)
    {
        std::cout << "Error! Please only enter either 1 or 2 for evaluate function." << std::endl;
        return std::vector<std::pair<int, std::pair<int, int>>>();
    }

    std::vector<std::pair<int, std::pair<int, int>>> boardEstimates;

    // get all the valid places where a move could be played on the board
    // stored within this class
    std::vector<std::pair<int, int>> moves = board.getVaildMoves();

    if (nullptr == tree.moveUp())
    {
        int estimate = evaluateTemplateWeight(currPlayer);
        tree.setRoot(new Node(-1, -1, 9999, board));
    }

    // now we will go through each possible move on the current board
    // and evaluate how much weight there is for the move
    for (size_t i = 0; i < moves.size(); i++)
    {
        setBoard(board, moves[i], currPlayer);
        int estimate = evaluateTemplateWeight(currPlayer);
        std::pair<int, std::pair<int, int>> curr;
        curr.first = estimate;
        curr.second = moves[i];
        boardEstimates.push_back(curr); 

        tree.addChild(new Node(moves[i].first, moves[i].second, estimate, templateBoard));

        resetTemplate(board);
    }

    Node* temp = tree.m_current;

    for (size_t i = 0; i < temp->children.size(); i++)
    {
        if (depth < maxDepth)
        {
            tree.m_current = temp;
            tree.moveTo(static_cast<int>(i));
            templateBoard.m_boardData[temp->children[i]->x][temp->children[i]->y] = currPlayer;
            evaluate(currPlayer, templateBoard, depth + 1);
        }
    }
    

    // return the final calculated weights
    return boardEstimates;
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
            templateBoard.m_boardData[i][j] = t_board.m_boardData[i][j];
        }
    }

    templateBoard.m_boardData[t_play.first][t_play.second] = t_currPlayer;
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
            if (templateBoard.m_boardData[i][j] == 0)
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
        if (templateBoard.m_boardData[i][0] == t_currPlayer && templateBoard.m_boardData[i][1] == t_currPlayer
            && templateBoard.m_boardData[i][2] == t_currPlayer && templateBoard.m_boardData[i][3] == t_currPlayer)
        {
            return winPredictionValue;
        }

        if (templateBoard.m_boardData[0][i] == t_currPlayer && templateBoard.m_boardData[1][i] == t_currPlayer
            && templateBoard.m_boardData[2][i] == t_currPlayer && templateBoard.m_boardData[3][i] == t_currPlayer)
        {
            return winPredictionValue;
        }
    }

    // Diagonal Checks (2D)
    if (templateBoard.m_boardData[0][3] == t_currPlayer && templateBoard.m_boardData[1][2] == t_currPlayer
        && templateBoard.m_boardData[2][1] == t_currPlayer && templateBoard.m_boardData[3][0] == t_currPlayer)
    {
        return winPredictionValue;
    }
    
    if (templateBoard.m_boardData[0][0] == t_currPlayer && templateBoard.m_boardData[1][1] == t_currPlayer
        && templateBoard.m_boardData[2][2] == t_currPlayer && templateBoard.m_boardData[3][3] == t_currPlayer)
    {
        return winPredictionValue;
    }

    return 0; // if no win has been predicted on this play, return no total change
}

/// <summary>
/// Reset template board back to zero state
/// </summary>
void Evaluator::resetTemplate(Board& toCopy)
{
    templateBoard = toCopy;
}
