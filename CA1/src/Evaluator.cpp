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

        copyBoard(board);
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
    resetTemplate();

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
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            templateBoard.m_boardData[i][j] = t_board.m_boardData[i][j];
        }
    }

    templateBoard.m_boardData[t_play.first][t_play.second] = t_currPlayer;

    retuneWeights(t_currPlayer);
}

/// <summary>
/// Evaluate total cost of the board,
/// based on the number of spaces with weights left
/// </summary>
/// <returns>Total weight of board</returns>
int Evaluator::evaluateTemplateWeight(int& t_currPlayer)
{
    int total = 0;

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (templateBoard.m_boardData[i][j] == 0)
            {
                // wins are predicted during retuneWeights function
                // so we don't have to decide if a win has happened
                total += m_predictedWeights[i][j];
            }
        }
    }

    return total;
}

/// <summary>
/// Using the current board passed in,
/// detect better moves and re-adjust
/// the weights in order to
/// make the AI play better
/// </summary>
void Evaluator::retuneWeights(int t_currPlayer)
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
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (templateBoard.m_boardData[i][j] == 1 || templateBoard.m_boardData[i][j] == 2)
            {
                m_predictedWeights[i][j] = 0;
            }
        }
    }

    // now that all the weights are evaluated, we'll check for potentially good plays
    // using the number of plays the current player has in a row
    // also, since we already evaluate if a board play is a win,
    // we don't have to adjust weights for potential wins

    rowRetune(templateBoard, t_currPlayer); // retune weights based on row
    colRetune(templateBoard, t_currPlayer); // retune weights based on col
    diagonalRetune(templateBoard, t_currPlayer); // retune weights based on diagonal plays
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
                    m_predictedWeights[rowCheck][i] += 10;
                }
            }
            break;
        case 2:
            // 2 plays means we should super prioritize one of the empty spaces
            for (int i = 0; i < 4; i++)
            {
                if (t_boardToWeigh.m_boardData[rowCheck][i] == 0)
                {
                    m_predictedWeights[rowCheck][i] += 20;
                }
            }
            break;
        case 3:
            // 3 plays means this play is a winning move, and will gain a big score increase
            for (int i = 0; i < 4; i++)
            {
                if (t_boardToWeigh.m_boardData[rowCheck][i] == 0)
                {
                    m_predictedWeights[rowCheck][i] += 100;
                }
            }
            break;
        }
    }
}

void Evaluator::colRetune(Board& t_boardToWeigh, int t_currPlayer)
{
    for (int colCheck = 0; colCheck < 13; colCheck += 4)
    {
        bool colHasOpponent = false;

        for (int i = 0; i < 4; i++)
        {
            if (t_boardToWeigh.m_boardData[i][colCheck] != t_currPlayer && t_boardToWeigh.m_boardData[i][colCheck] != 0)
            { // if the opponent has made a play on this row
                colHasOpponent = true;
                break; // don't bother weighing it
            }
        }

        if (colHasOpponent)
            continue; // since we found a play from the opponent on this row, continue on to the next


        // since we now know that the opponent hasn't made a play on this row
        // we can now make changes to the predicted weights, based on
        // the strength of a play

        // starting from left to right,
        // find where the first empty space is
        int space = -1;

        for (int i = 0; i < 4; i++)
        {
            if (t_boardToWeigh.m_boardData[i][colCheck] == 0)
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
            if (t_boardToWeigh.m_boardData[current][colCheck] == t_currPlayer)
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
            if (t_boardToWeigh.m_boardData[current][colCheck] == t_currPlayer)
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
                if (t_boardToWeigh.m_boardData[i][colCheck] == 0)
                {
                    m_predictedWeights[i][colCheck] += 10;
                }
            }
        case 2:
            // 2 plays means we should super prioritize one of the empty spaces
            for (int i = 0; i < 4; i++)
            {
                if (t_boardToWeigh.m_boardData[i][colCheck] == 0)
                {
                    m_predictedWeights[i][colCheck] += 20;
                }
            }
            break;

        case 3:
            // 3 plays means this play is a winning move, and will gain a big score increase
            for (int i = 0; i < 4; i++)
            {
                if (t_boardToWeigh.m_boardData[i][colCheck] == 0)
                {
                    m_predictedWeights[i][colCheck] += 100;
                }
            }
            break;
        }
    }
}

void Evaluator::diagonalRetune(Board& t_boardToWeigh, int t_currPlayer)
{
    // diagonal retune is different,
    // in that we only need to check 8 potential spots
    // so we will do it manually
    // we'll first do the top left to bottom right diagonal

    for (int first = 0, second = 1, third = 2, fourth = 3; first < 12; first += 4, second += 4, third += 4, fourth += 4)
    {
        // first we'll check and see if the opponent has made a diagonal play at any point
        bool opponentPlayed = false;
        if (templateBoard.m_boardData[first][fourth] != t_currPlayer && templateBoard.m_boardData[second][third] != t_currPlayer
            && templateBoard.m_boardData[third][second] != t_currPlayer && templateBoard.m_boardData[fourth][first] != t_currPlayer)
        {
            if (templateBoard.m_boardData[first][fourth] != 0 && templateBoard.m_boardData[second][third] != 0
                && templateBoard.m_boardData[third][second] != 0 && templateBoard.m_boardData[fourth][first] != 0)
            {
                opponentPlayed = true;
            }
        }

        // if the opponent has not made a play on the diagonal,
        // then we can begin the weighing process
        if (!opponentPlayed)
        {
            int timesPlayed = 0;
            // we will count how many times the current player has made on this diagonal
            templateBoard.m_boardData[first][fourth] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[second][third] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[third][second] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[fourth][first] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;

            switch (timesPlayed)
            {
            case 0:
                // no plays means don't do anything
                break;
            case 1:
                // 1 play means we should priortize this space
                // NOTE: timesPlayed is put at the end as I don't want to do anything if it isn't 0
                templateBoard.m_boardData[first][fourth] == 0 ? m_predictedWeights[first][fourth] += 10 : timesPlayed;
                templateBoard.m_boardData[second][third] == 0 ? m_predictedWeights[second][third] += 10 : timesPlayed;
                templateBoard.m_boardData[third][second] == 0 ? m_predictedWeights[third][second] += 10 : timesPlayed;
                templateBoard.m_boardData[fourth][first] == 0 ? m_predictedWeights[fourth][first] += 10 : timesPlayed;
                break;
            case 2:
                // 2 plays means we should super prioritize one of the empty spaces
                templateBoard.m_boardData[first][fourth] == 0 ? m_predictedWeights[first][fourth] += 20 : timesPlayed;
                templateBoard.m_boardData[second][third] == 0 ? m_predictedWeights[second][third] += 20 : timesPlayed;
                templateBoard.m_boardData[third][second] == 0 ? m_predictedWeights[third][second] += 20 : timesPlayed;
                templateBoard.m_boardData[fourth][first] == 0 ? m_predictedWeights[fourth][first] += 20 : timesPlayed;
                break;

            case 3:
                // 3 plays means this play is a winning move, and will gain a big score increase
                templateBoard.m_boardData[first][fourth] == 0 ? m_predictedWeights[first][fourth] += 100 : timesPlayed;
                templateBoard.m_boardData[second][third] == 0 ? m_predictedWeights[second][third] += 100 : timesPlayed;
                templateBoard.m_boardData[third][second] == 0 ? m_predictedWeights[third][second] += 100 : timesPlayed;
                templateBoard.m_boardData[fourth][first] == 0 ? m_predictedWeights[fourth][first] += 100 : timesPlayed;
                break;
            }
        }

        opponentPlayed = false;

        // now we check the other side
        if (templateBoard.m_boardData[first][first] != t_currPlayer && templateBoard.m_boardData[second][second] != t_currPlayer
            && templateBoard.m_boardData[third][third] != t_currPlayer && templateBoard.m_boardData[fourth][fourth] != t_currPlayer)
        {
            if (templateBoard.m_boardData[first][first] != 0 && templateBoard.m_boardData[second][second] != 0
                && templateBoard.m_boardData[third][third] != 0 && templateBoard.m_boardData[fourth][fourth] != 0)
            {
                opponentPlayed = true;
            }
        }

        // if the opponent has not made a play on the diagonal,
        // then we can begin the weighing process
        if (!opponentPlayed)
        {
            int timesPlayed = 0;
            // we will count how many times the current player has made on this diagonal
            templateBoard.m_boardData[first][first]   == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[second][second] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[third][third]   == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[fourth][fourth] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;

            switch (timesPlayed)
            {
            case 0:
                // no plays means don't do anything
                break;
            case 1:
                // 1 play means we should priortize this space
                templateBoard.m_boardData[first][first] == 0 ? m_predictedWeights[first][first] += 10 : timesPlayed;
                templateBoard.m_boardData[second][second] == 0 ? m_predictedWeights[second][second] += 10 : timesPlayed;
                templateBoard.m_boardData[third][third] == 0 ? m_predictedWeights[third][third] += 10 : timesPlayed;
                templateBoard.m_boardData[fourth][fourth] == 0 ? m_predictedWeights[fourth][fourth] += 10 : timesPlayed;
                break;
            case 2:
                // 2 plays means we should super prioritize one of the empty spaces
                templateBoard.m_boardData[first][first] == 0 ? m_predictedWeights[first][first] += 20 : timesPlayed;
                templateBoard.m_boardData[second][second] == 0 ? m_predictedWeights[second][second] += 20 : timesPlayed;
                templateBoard.m_boardData[third][third] == 0 ? m_predictedWeights[third][third] += 20 : timesPlayed;
                templateBoard.m_boardData[fourth][fourth] == 0 ? m_predictedWeights[fourth][fourth] += 20 : timesPlayed;
                break;

            case 3:
                // 3 plays means this play is a winning move, and will gain a big score increase
                templateBoard.m_boardData[first][first] == 0 ? m_predictedWeights[first][first] += 100 : timesPlayed;
                templateBoard.m_boardData[second][second] == 0 ? m_predictedWeights[second][second] += 100 : timesPlayed;
                templateBoard.m_boardData[third][third] == 0 ? m_predictedWeights[third][third] += 100 : timesPlayed;
                templateBoard.m_boardData[fourth][fourth] == 0 ? m_predictedWeights[fourth][fourth] += 100 : timesPlayed;
                break;
            }
        }
    }
}

/// <summary>
/// Copy a working board onto the Template board
/// </summary>
/// <param name="toCopy">Board to copy data from</param>
void Evaluator::copyBoard(Board& toCopy)
{
    templateBoard = toCopy;
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
            templateBoard.m_boardData[i][j] = 0;
        }
    }
}
