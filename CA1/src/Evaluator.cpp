#include "..\include\Evaluator.h"

/// <summary>
/// Using generated valid modes,
/// evaluate the strength of each board play
/// </summary>
/// <param name="turnPlayer">Current turn player</param>
/// <param name="board">the board to estimate on</param>
/// <returns>value of each play, where the play is made</returns>
void Evaluator::evaluate(int currPlayer, Board& board, int depth, std::pair<int, int> t_lastPlay)
{
    if (currPlayer > 2 && currPlayer < 1)
    {
        std::cout << "Error! Please only enter either 1 or 2 for evaluate function." << std::endl;
        return;
    }

    if (depth < maxDepth)
    {
        // get all the valid places where a move could be played on the board
        // stored within this class
        std::vector<std::pair<int, int>> moves = board.getVaildMoves();

        if (nullptr == tree.moveUp())
        {
            tree.setRoot(new Node(-1, -1, 9999, board));
        }

        // now we will go through each possible move on the current board
        // and evaluate how much weight there is for the move
        for (size_t i = 0; i < moves.size(); i++)
        {
            resetTemplate();
            setBoard(board, moves[i], currPlayer, t_lastPlay);
            int estimate = evaluateTemplateWeight(currPlayer);

            if (estimate == 0)
            {
                std::cout << "0" << std::endl;
            }

            tree.addChild(new Node(moves[i].first, moves[i].second, estimate, templateBoard));
        }

        Node* temp = tree.m_current;
        for (size_t i = 0; i < temp->children.size(); i++)
        {
            tree.m_current = temp;
            tree.moveTo(static_cast<int>(i));
            copyBoard(temp->board);
            evaluate(currPlayer, templateBoard, depth + 1, t_lastPlay);
        } 
    }

    if(depth == 0) // only reset template once the depth is finished
        resetTemplate();
    
}

/// <summary>
/// Set up template board with required information
/// </summary>
/// <param name="t_board">Board to look at</param>
/// <param name="t_play">Where the next play is to be made</param>
/// <param name="t_currPlayer">What player is making the play</param>
void Evaluator::setBoard(Board& t_board, std::pair<int, int>& t_play, int& t_currPlayer, std::pair<int, int> t_lastPlay)
{
    copyBoard(t_board);

    templateBoard.m_boardData[t_play.first][t_play.second] = t_currPlayer;

    // set the last played turn on the board
    if(t_currPlayer == 1)
        templateBoard.m_boardData[t_lastPlay.first][t_lastPlay.second] = 2;
    else
        templateBoard.m_boardData[t_lastPlay.first][t_lastPlay.second] = 1;

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
    for (int i = 0; i < 16; i++)
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
    for (int colCheck = 0; colCheck <= 12; colCheck += 4)
    {
        bool colHasOpponent = false;

        

        for (int col = 0; col < 4; col++)
        {
            if (t_boardToWeigh.m_boardData[colCheck][col] != t_currPlayer 
                && t_boardToWeigh.m_boardData[colCheck][col] != 0)
            { // if the opponent has made a play on this row
                colHasOpponent = true;
            }
            else if (t_boardToWeigh.m_boardData[colCheck + 1][col] != t_currPlayer 
                && t_boardToWeigh.m_boardData[colCheck + 1][col] != 0)
            { 
                colHasOpponent = true;
            }
            else if (t_boardToWeigh.m_boardData[colCheck + 2][col] != t_currPlayer 
                && t_boardToWeigh.m_boardData[colCheck + 2][col] != 0)
            { 
                colHasOpponent = true;
            }
            else if (t_boardToWeigh.m_boardData[colCheck + 3][col] != t_currPlayer 
                && t_boardToWeigh.m_boardData[colCheck + 3][col] != 0)
            { 
                colHasOpponent = true;
            }
        }

        if (colHasOpponent)
            continue; // since we found a play from the opponent on this row, continue on to the next


        // since we now know that the opponent hasn't made a play on this row
        // we can now make changes to the predicted weights, based on
        // the strength of a play

        // starting from left to right,
        // find where the first empty space is
        int colFound = 0;
        bool colFail = true;

        for (; colFound < 4; colFound++)
        {
            if (t_boardToWeigh.m_boardData[colCheck][colFound] == 0)
            { // break if a 0 is found
                colFail = false;
                break;
            }
            else if (t_boardToWeigh.m_boardData[colCheck + 1][colFound] == 0)
            {
                colFail = false;
                break;
            }
            else if (t_boardToWeigh.m_boardData[colCheck + 2][colFound] == 0)
            {
                colFail = false;
                break;
            }
            else if (t_boardToWeigh.m_boardData[colCheck + 3][colFound] == 0)
            {
                colFail = false;
                break;
            }
        }

        if (colFail) // if colFail triggers, that means we couldn't find a space in this col
            continue;

        // since we know a space is in this col, 
        // count the number of plays that have been madeon it

        int numOfPlays = 0;

        if (t_boardToWeigh.m_boardData[colCheck][0] == t_currPlayer)
        { // break if a 0 is found
            numOfPlays++;
        }
        
        if (t_boardToWeigh.m_boardData[colCheck + 1][0] == t_currPlayer)
        {
            numOfPlays++;
        }
        
        if (t_boardToWeigh.m_boardData[colCheck + 2][0] == t_currPlayer)
        {   
            numOfPlays++;
        }
        
        if (t_boardToWeigh.m_boardData[colCheck + 3][0] == t_currPlayer)
        {
            numOfPlays++;
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
            if (t_boardToWeigh.m_boardData[colCheck][colFound] == 0)
            { // break if a 0 is found
                m_predictedWeights[colCheck][colFound] += 10;
            }
            if (t_boardToWeigh.m_boardData[colCheck + 1][colFound] == 0)
            {
                m_predictedWeights[colCheck + 1][colFound] += 10;
            }
            if (t_boardToWeigh.m_boardData[colCheck + 2][colFound] == 0)
            {
                m_predictedWeights[colCheck + 2][colFound] += 10;
            }
            if (t_boardToWeigh.m_boardData[colCheck + 3][colFound] == 0)
            {
                m_predictedWeights[colCheck + 3][colFound] += 10;
            }
            break;
        case 2:
            // 2 plays means we should super prioritize one of the empty spaces
            if (t_boardToWeigh.m_boardData[colCheck][colFound] == 0)
            { // break if a 0 is found
                m_predictedWeights[colCheck][colFound] += 20;
            }
            if (t_boardToWeigh.m_boardData[colCheck + 1][colFound] == 0)
            {
                m_predictedWeights[colCheck + 1][colFound] += 20;
            }
            if (t_boardToWeigh.m_boardData[colCheck + 2][colFound] == 0)
            {
                m_predictedWeights[colCheck + 2][colFound] += 20;
            }
            if (t_boardToWeigh.m_boardData[colCheck + 3][colFound] == 0)
            {
                m_predictedWeights[colCheck + 3][colFound] += 20;
            }
            break;

        case 3:
            // 3 plays means this play is a winning move, and will gain a big score increase
            if (t_boardToWeigh.m_boardData[colCheck][colFound] == 0)
            { // break if a 0 is found
                m_predictedWeights[colCheck][colFound] += 100;
            }
            if (t_boardToWeigh.m_boardData[colCheck + 1][colFound] == 0)
            {
                m_predictedWeights[colCheck + 1][colFound] += 100;
            }
            if (t_boardToWeigh.m_boardData[colCheck + 2][colFound] == 0)
            {
                m_predictedWeights[colCheck + 2][colFound] += 100;
            }
            if (t_boardToWeigh.m_boardData[colCheck + 3][colFound] == 0)
            {
                m_predictedWeights[colCheck + 3][colFound] += 100;
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

    for (int first = 0, second = 1, third = 2, fourth = 3; first <= 12; first += 4, second += 4, third += 4, fourth += 4)
    {
        // first we'll check and see if the opponent has made a diagonal play at any point
        bool opponentPlayed = false;
        if (templateBoard.m_boardData[first][3] != t_currPlayer || templateBoard.m_boardData[second][2] != t_currPlayer
            || templateBoard.m_boardData[third][1] != t_currPlayer || templateBoard.m_boardData[fourth][0] != t_currPlayer)
        {
            if (templateBoard.m_boardData[first][3] != 0 || templateBoard.m_boardData[second][2] != 0
                || templateBoard.m_boardData[third][1] != 0 || templateBoard.m_boardData[fourth][0] != 0)
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
            templateBoard.m_boardData[first][3] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[second][2] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[third][1] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[fourth][0] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;

            switch (timesPlayed)
            {
            case 0:
                // no plays means don't do anything
                break;
            case 1:
                // 1 play means we should priortize this space
                // NOTE: timesPlayed is put at the end as I don't want to do anything if it isn't 0
                templateBoard.m_boardData[first][3] == 0 ? m_predictedWeights[first][3] += 10 : timesPlayed;
                templateBoard.m_boardData[second][2] == 0 ? m_predictedWeights[second][2] += 10 : timesPlayed;
                templateBoard.m_boardData[third][1] == 0 ? m_predictedWeights[third][1] += 10 : timesPlayed;
                templateBoard.m_boardData[fourth][0] == 0 ? m_predictedWeights[fourth][0] += 10 : timesPlayed;
                break;
            case 2:
                // 2 plays means we should super prioritize one of the empty spaces
                templateBoard.m_boardData[first][3] == 0 ? m_predictedWeights[first][3] += 20 : timesPlayed;
                templateBoard.m_boardData[second][2] == 0 ? m_predictedWeights[second][2] += 20 : timesPlayed;
                templateBoard.m_boardData[third][1] == 0 ? m_predictedWeights[third][1] += 20 : timesPlayed;
                templateBoard.m_boardData[fourth][0] == 0 ? m_predictedWeights[fourth][0] += 20 : timesPlayed;
                break;

            case 3:
                // 3 plays means this play is a winning move, and will gain a big score increase
                templateBoard.m_boardData[first][3] == 0 ? m_predictedWeights[first][3] += 100 : timesPlayed;
                templateBoard.m_boardData[second][2] == 0 ? m_predictedWeights[second][2] += 100 : timesPlayed;
                templateBoard.m_boardData[third][1] == 0 ? m_predictedWeights[third][1] += 100 : timesPlayed;
                templateBoard.m_boardData[fourth][0] == 0 ? m_predictedWeights[fourth][0] += 100 : timesPlayed;
                break;
            }
        }

        opponentPlayed = false;

        // now we check the other side
        if (templateBoard.m_boardData[first][0] != t_currPlayer || templateBoard.m_boardData[second][1] != t_currPlayer
            || templateBoard.m_boardData[third][2] != t_currPlayer || templateBoard.m_boardData[fourth][3] != t_currPlayer)
        {
            if (templateBoard.m_boardData[first][0] != 0 || templateBoard.m_boardData[second][1] != 0
                || templateBoard.m_boardData[third][2] != 0 || templateBoard.m_boardData[fourth][3] != 0)
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
            templateBoard.m_boardData[first][0]   == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[second][1] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[third][2]   == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;
            templateBoard.m_boardData[fourth][3] == t_currPlayer ? timesPlayed++ : timesPlayed = timesPlayed;

            switch (timesPlayed)
            {
            case 0:
                // no plays means don't do anything
                break;
            case 1:
                // 1 play means we should priortize this space
                templateBoard.m_boardData[first][0] == 0 ? m_predictedWeights[first][0] += 10 : timesPlayed;
                templateBoard.m_boardData[second][1] == 0 ? m_predictedWeights[second][1] += 10 : timesPlayed;
                templateBoard.m_boardData[third][2] == 0 ? m_predictedWeights[third][2] += 10 : timesPlayed;
                templateBoard.m_boardData[fourth][3] == 0 ? m_predictedWeights[fourth][3] += 10 : timesPlayed;
                break;
            case 2:
                // 2 plays means we should super prioritize one of the empty spaces
                templateBoard.m_boardData[first][0] == 0 ? m_predictedWeights[first][0] += 20 : timesPlayed;
                templateBoard.m_boardData[second][1] == 0 ? m_predictedWeights[second][1] += 20 : timesPlayed;
                templateBoard.m_boardData[third][2] == 0 ? m_predictedWeights[third][2] += 20 : timesPlayed;
                templateBoard.m_boardData[fourth][3] == 0 ? m_predictedWeights[fourth][3] += 20 : timesPlayed;
                break;

            case 3:
                // 3 plays means this play is a winning move, and will gain a big score increase
                templateBoard.m_boardData[first][0] == 0 ? m_predictedWeights[first][0] += 100 : timesPlayed;
                templateBoard.m_boardData[second][1] == 0 ? m_predictedWeights[second][1] += 100 : timesPlayed;
                templateBoard.m_boardData[third][2] == 0 ? m_predictedWeights[third][2] += 100 : timesPlayed;
                templateBoard.m_boardData[fourth][3] == 0 ? m_predictedWeights[fourth][3] += 100 : timesPlayed;
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
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            templateBoard.m_boardData[i][j] = 0;
        }
    }
}
