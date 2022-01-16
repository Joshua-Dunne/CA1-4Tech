#include "..\include\Evaluator.h"

Evaluator::~Evaluator()
{
    // when the evaluator enters it's destructor,
    // we need to free up any memory from the leftover trees
    tree.cleanUp(tree.getRoot());
}

/// <summary>
/// Using generated valid modes,
/// evaluate the strength of each board play
/// </summary>
/// <param name="turnPlayer">Current turn player</param>
/// <param name="board">the board to estimate on</param>
/// <returns>value of each play, where the play is made</returns>
void Evaluator::evaluate(int currPlayer, Board& board, int depth)
{
    if (currPlayer > 2 && currPlayer < 1)
    {
        std::cout << "Error! Please only enter either 1 or 2 for evaluate function." << std::endl;
        return;
    }

    if (depth < maxDepth)
    {
        if (depth == 0)
        {
            for (int i = 0; i < 16; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    if(board.m_boardData[i][j] != currPlayer && board.m_boardData[i][j] != 0)
                        opponentMoves.push_back(std::pair<int, int>(i, j));
                }
            }

        }

        // get all the valid places where a move could be played on the board
        // stored within this class
        std::vector<std::pair<int, int>> moves = board.getVaildMoves();

        if (nullptr == tree.moveUp())
        {
            tree.setRoot(new Node(-1, -1, 9999, board));
        }

        // now we will go through each possible move on the current board
        // and evaluate how much weight there is for the move
        Board newBoard;

        for (size_t i = 0; i < moves.size(); i++)
        {
            Board newBoard = board;
            setBoard(newBoard, moves[i], currPlayer);
            int estimate = evaluateTemplateWeight(newBoard, currPlayer);  

            tree.addChild(new Node(moves[i].first, moves[i].second, estimate, newBoard));
        }

        Node* temp = tree.m_current;
        for (size_t i = 0; i < temp->children.size(); i++)
        {
            tree.setCurrent(temp);
            tree.moveTo(static_cast<int>(i));
            newBoard = tree.m_current->board;
            evaluate(currPlayer, newBoard, depth + 1);
        } 
    }  
}

/// <summary>
/// Set up template board with required information
/// </summary>
/// <param name="t_board">Board to look at</param>
/// <param name="t_play">Where the next play is to be made</param>
/// <param name="t_currPlayer">What player is making the play</param>
void Evaluator::setBoard(Board& t_board, std::pair<int, int>& t_play, int& t_currPlayer)
{
    t_board.m_boardData[t_play.first][t_play.second] = t_currPlayer;

    int opponent = 1;

    // set the last played turn on the board
    if(t_currPlayer == 1)
        opponent = 2;

    // set any opponent moves now
    for (auto pair : opponentMoves)
    {
        t_board.m_boardData[pair.first][pair.second] = opponent;
    }

    retuneWeights(t_board, t_currPlayer);
}

/// <summary>
/// Using the template board that is set up
/// with the next possible play,
/// we can predict if a win will happen on this board play.
/// </summary>
/// <param name="t_currPlayer">What player to check for</param>
/// <returns>If a board play wins, the win value is returned to boost the board total</returns>
int Evaluator::predictWin(Board t_board, int& t_currPlayer)
{
    // Row Checks
    for (int i = 0; i < 16; i++)
    {
        if (t_board.m_boardData[i][0] == t_currPlayer && t_board.m_boardData[i][1] == t_currPlayer
            && t_board.m_boardData[i][2] == t_currPlayer && t_board.m_boardData[i][3] == t_currPlayer)
        {
            return winWeightIncrease;
        }
    }

    // Col Checks
    for (int colSet = 0; colSet <= 12; colSet += 4)
    {
        for (int i = 0; i < 4; i++)
        {
            if (t_board.m_boardData[colSet][i] == t_currPlayer && t_board.m_boardData[colSet + 1][i] == t_currPlayer
                && t_board.m_boardData[colSet + 2][i] == t_currPlayer && t_board.m_boardData[colSet + 3][i] == t_currPlayer)
            {
                return winWeightIncrease;
            }
        }
        
    }

    for (int first = 0, second = 1, third = 2, fourth = 3; first <= 12; first += 4, second += 4, third += 4, fourth += 4)
    {
        if (templateBoard.m_boardData[first][3] == t_currPlayer || templateBoard.m_boardData[second][2] == t_currPlayer
            || templateBoard.m_boardData[third][1] == t_currPlayer || templateBoard.m_boardData[fourth][0] == t_currPlayer)
        {
            return winWeightIncrease;
        }

        if (templateBoard.m_boardData[first][0] == t_currPlayer || templateBoard.m_boardData[second][1] == t_currPlayer
            || templateBoard.m_boardData[third][2] == t_currPlayer || templateBoard.m_boardData[fourth][3] == t_currPlayer)
        {
            return winWeightIncrease;
        }
  
    }

    return 0; // if no win has been predicted on this play, return no total change
}

/// <summary>
/// Evaluate total cost of the board,
/// based on the number of spaces with weights left
/// </summary>
/// <returns>Total weight of board</returns>
int Evaluator::evaluateTemplateWeight(Board& t_boardToEval, int& t_currPlayer)
{
    int total = 0;

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (t_boardToEval.m_boardData[i][j] == 0)
            {
                // wins are predicted during retuneWeights function
                // so we don't have to decide if a win has happened
                total += m_predictedWeights[i][j];
            }
        }
    }

    total += predictWin(t_boardToEval, t_currPlayer);

    return total;
}

/// <summary>
/// Using the current board passed in,
/// detect better moves and re-adjust
/// the weights in order to
/// make the AI play better
/// </summary>
void Evaluator::retuneWeights(Board& t_boardToWeigh, int t_currPlayer)
{
    // first reset the weights back to the defaults for this new board calculation
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            m_predictedWeights[i][j] = m_startingWeights[i][j];
        }
    }
    
    // then remove all weights on plays already made
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (t_boardToWeigh.m_boardData[i][j] == 1 || t_boardToWeigh.m_boardData[i][j] == 2)
            {
                m_predictedWeights[i][j] = -999;
            }
        }
    }

    // now that all the weights are evaluated, we'll check for potentially good plays
    // using the number of plays the current player has in a row
    // also, since we already evaluate if a board play is a win,
    // we don't have to adjust weights for potential wins

    rowRetune(t_boardToWeigh, t_currPlayer); // retune weights based on row
    colRetune(t_boardToWeigh, t_currPlayer); // retune weights based on col
    diagonalRetune(t_boardToWeigh, t_currPlayer); // retune weights based on diagonal plays
}

void Evaluator::rowRetune(Board& t_boardToWeigh, int t_currPlayer)
{
    for (int rowCheck = 0; rowCheck < 16; rowCheck++)
    {
        bool rowHasOpponent = false;

        for (int i = 0; i < 4; i++)
        {
            if (t_boardToWeigh.m_boardData[rowCheck][i] != t_currPlayer && t_boardToWeigh.m_boardData[rowCheck][i] != 0)
            { // if the opponent has made a play on this row
                rowHasOpponent = true;
                break; // don't bother weighing it
            }
        }


        if (rowHasOpponent)
            continue; // since we found a play from the opponent on this row, continue on to the next
            


        // since we now know that the opponent hasn't made a play on this row
        // we can now make changes to the predicted weights, based on
        // the strength of a play
        
        // starting from left to right,
        // find where the first empty space is
        int space = -1;

        for (int i = 0; i < 4; i++)
        {
            if (t_boardToWeigh.m_boardData[rowCheck][i] == 0)
            {
                space = i;
                break;
            }

        }

        if (space == -1) // if there somehow isn't an empty space, continue to the next
            continue;

        // starting from these empty plays, count the number of
        // other plays by the current player that are adjacent to it

        int numOfPlays = 0;

        // gauge how many plays there are from the first empty space
        // if there is a good empty space to play on,
        // all of the empty spaces should be considered
        // working with rows means we need the Y on our 2D array

        int current = space;

        // check left first
        for (; current >= 0; current--)
        {
            if (t_boardToWeigh.m_boardData[rowCheck][current] == t_currPlayer)
            {
                // only count spaces where a player piece exists
                numOfPlays++;
            }
        }

        // reset current counter
        current = space;
        // check right next

        for (; current < 4; current++)
        {
            if (t_boardToWeigh.m_boardData[rowCheck][current] == t_currPlayer)
            {
                // only count spaces where a player piece exists
                numOfPlays++;
            }

        }

        // now that we know how many plays are to the left and right
        // we can judge the strength of those empty spaces on our
        // predicted weight board

        // if there is only 1 other play on the board
        switch (numOfPlays)
        {
        case 0:
            // no plays means don't do anything
            break;
        case 1:
            // 1 play means we should priortize this space
            for (int i = 0; i < 4; i++)
            {
                if (t_boardToWeigh.m_boardData[rowCheck][i] == 0)
                {
                    m_predictedWeights[rowCheck][i] += singleWeightIncrease;
                }
            }
            break;
        case 2:
            // 2 plays means we should super prioritize one of the empty spaces
            for (int i = 0; i < 4; i++)
            {
                if (t_boardToWeigh.m_boardData[rowCheck][i] == 0)
                {
                    m_predictedWeights[rowCheck][i] += doubleWeightIncrease;
                }
            }
            break;
        case 3:
            // 3 plays means this play is a winning move, and will gain a big score increase
            for (int i = 0; i < 4; i++)
            {
                if (t_boardToWeigh.m_boardData[rowCheck][i] == 0)
                {
                    m_predictedWeights[rowCheck][i] += tripleWeightIncrease;
                }
            }
            break;
        }
    }
}

void Evaluator::colRetune(Board& t_boardToWeigh, int t_currPlayer)
{
    // Check to see if opponent has played on this column
    for (int colSet = 0; colSet <= 12; colSet += 4)
    {
        for (int i = 0; i < 4; i++)
        {
            if (t_boardToWeigh.m_boardData[colSet][i] != t_currPlayer && t_boardToWeigh.m_boardData[colSet + 1][i] != t_currPlayer
                && t_boardToWeigh.m_boardData[colSet + 2][i] != t_currPlayer && t_boardToWeigh.m_boardData[colSet + 3][i] != t_currPlayer)
            {
                if (t_boardToWeigh.m_boardData[colSet][i] != 0 && t_boardToWeigh.m_boardData[colSet + 1][i] != 0
                    && t_boardToWeigh.m_boardData[colSet + 2][i] != 0 && t_boardToWeigh.m_boardData[colSet + 3][i] != 0)
                {
                    break;
                }
            }

            // if the opponent is not on this column
            // count the number of plays the current player has made
            // on this column

            int numOfPlays = 0;

            t_boardToWeigh.m_boardData[colSet][i] == t_currPlayer ? numOfPlays++ : numOfPlays;
            t_boardToWeigh.m_boardData[colSet + 1][i] == t_currPlayer ? numOfPlays++ : numOfPlays;
            t_boardToWeigh.m_boardData[colSet + 2][i] == t_currPlayer ? numOfPlays++ : numOfPlays;
            t_boardToWeigh.m_boardData[colSet + 3][i] == t_currPlayer ? numOfPlays++ : numOfPlays;

            // now that we know how many plays have been made,
            // we can add to any potential empty spaces an additional weighting
            // based on any plays next to that space

            int weightToAdd = 0;

            switch (numOfPlays)
            { // we don't want to do anything with 0 plays, so we don't include the case here
            case 1:
                // if there's 1 play, any empty slots can gain a small weighting
                weightToAdd = singleWeightIncrease;

                break;
            case 2:
                // if there's 2 plays, we should priortize trying to finish the line,
                // so we give it a medium weighting
                weightToAdd = doubleWeightIncrease;

                break;
            case 3:
                // if the predicts 3 plays, we should heavily prioritize finishing the line,
                // so we give it a heavy weighting
                weightToAdd = tripleWeightIncrease;

                break;
            }

            m_predictedWeights[colSet][i] += weightToAdd;
            m_predictedWeights[colSet + 1][i] += weightToAdd;
            m_predictedWeights[colSet + 2][i] += weightToAdd;
            m_predictedWeights[colSet + 3][i] += weightToAdd;
        }

    }
}

void Evaluator::diagonalRetune(Board& t_boardToWeigh, int t_currPlayer)
{
    for (int first = 0, second = 1, third = 2, fourth = 3; first <= 12; first += 4, second += 4, third += 4, fourth += 4)
    {
        bool hasOpponent = false;

        // First we will check to see if the opponent has played
        // on the diagonal on this board

        // First we check the Top Right to Bottom Left Diagonal
        if (t_boardToWeigh.m_boardData[first][3] != 0 && t_boardToWeigh.m_boardData[second][2] != 0
            && t_boardToWeigh.m_boardData[third][1] != 0 && t_boardToWeigh.m_boardData[fourth][0] != 0)
        {
            if (t_boardToWeigh.m_boardData[first][3] != t_currPlayer && t_boardToWeigh.m_boardData[second][2] != t_currPlayer
                && t_boardToWeigh.m_boardData[third][1] != t_currPlayer && t_boardToWeigh.m_boardData[fourth][0] != t_currPlayer)
            {
                hasOpponent = true;
            }
        }

        if (hasOpponent) continue;

        // since this diagonal doesn't have an opponent, we now need to see how many plays were made
        int playsMade = 0;

        t_boardToWeigh.m_boardData[first][3] == t_currPlayer ? playsMade++ : playsMade;
        t_boardToWeigh.m_boardData[second][2] == t_currPlayer ? playsMade++ : playsMade;
        t_boardToWeigh.m_boardData[third][1] == t_currPlayer ? playsMade++ : playsMade;
        t_boardToWeigh.m_boardData[fourth][0] == t_currPlayer  ? playsMade++ : playsMade;


        // now with the plays made, figure out how much weight should be given to empty spaces
        int weightToAdd = 0;

        switch (playsMade)
        { // we don't want to do anything with 0 plays, so we don't include the case here
        case 1:
            // if there's 1 play, any empty slots can gain a small weighting
            weightToAdd = singleWeightIncrease;
            break;
        case 2:
            // if there's 2 plays, we should priortize trying to finish the line,
            // so we give it a medium weighting
            weightToAdd = doubleWeightIncrease;
            break;
        case 3:
            // if the predicts 3 plays, we should heavily prioritize finishing the line,
            // so we give it a heavy weighting
            weightToAdd = tripleWeightIncrease;
            break;
        }

        // finally, adjust the predicted weights correctly
        if(t_boardToWeigh.m_boardData[first][3] == 0)   m_predictedWeights[first][3] += weightToAdd;
        if (t_boardToWeigh.m_boardData[second][2] == 0) m_predictedWeights[second][2] += weightToAdd;
        if (t_boardToWeigh.m_boardData[third][1] == 0)  m_predictedWeights[third][1] += weightToAdd;
        if (t_boardToWeigh.m_boardData[fourth][0] == 0) m_predictedWeights[fourth][0] += weightToAdd;

        // ----------------------------------------------------------------------------------
        // Now we will do it for Top Left to Bottom Right
        // first we will reset some variables
        hasOpponent = false;
        weightToAdd = 0;
        playsMade = 0;

        if (t_boardToWeigh.m_boardData[first][0] != 0 && t_boardToWeigh.m_boardData[second][1] != 0
            && t_boardToWeigh.m_boardData[third][2] != 0 && t_boardToWeigh.m_boardData[fourth][3] != 0)
        {
            if (t_boardToWeigh.m_boardData[first][0] != t_currPlayer && t_boardToWeigh.m_boardData[second][1] != t_currPlayer
                && t_boardToWeigh.m_boardData[third][2] != t_currPlayer && t_boardToWeigh.m_boardData[fourth][3] != t_currPlayer)
            {
                hasOpponent = true;
            }
        }

        if (hasOpponent) continue;

        t_boardToWeigh.m_boardData[first][0] == 2 ? playsMade++ : playsMade;
        t_boardToWeigh.m_boardData[second][1] == 2 ? playsMade++ : playsMade;
        t_boardToWeigh.m_boardData[third][2] == 2 ? playsMade++ : playsMade;
        t_boardToWeigh.m_boardData[fourth][3] == 2 ? playsMade++ : playsMade;


        switch (playsMade)
        { // we don't want to do anything with 0 plays, so we don't include the case here
        case 1:
            // if there's 1 play, any empty slots can gain a small weighting
            weightToAdd = singleWeightIncrease;
            break;
        case 2:
            // if there's 2 plays, we should priortize trying to finish the line,
            // so we give it a medium weighting
            weightToAdd = doubleWeightIncrease;
            break;
        case 3:
            // if the predicts 3 plays, we should heavily prioritize finishing the line,
            // so we give it a heavy weighting
            weightToAdd = tripleWeightIncrease;
            break;
        }

        // finally, adjust the predicted weights correctly
        if (t_boardToWeigh.m_boardData[first][0] == 0)   m_predictedWeights[first][0] += weightToAdd;
        if (t_boardToWeigh.m_boardData[second][1] == 0) m_predictedWeights[second][1] += weightToAdd;
        if (t_boardToWeigh.m_boardData[third][2] == 0)  m_predictedWeights[third][2] += weightToAdd;
        if (t_boardToWeigh.m_boardData[fourth][3] == 0) m_predictedWeights[fourth][3] += weightToAdd;
    }
}
