#include <iostream>
#include <cstdio>
#include <vector>
#include "tictactoe.h"

using namespace std;

const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';
const int ROWS = 3;
const int COLS = 3;

// helper prototypes
bool full(vector<vector<char> > board);
vector<vector<char> > deepcopy(vector<vector<char> > board);
char check_horizontally(vector<vector<char> > board);
char check_vertically(vector<vector<char> > board);
char check_diagonally(vector<vector<char> > board);

/**
 * Prints the board in a user friendly way.
 */
void print_board(vector<vector<char> > board)
{

    const string cyan = "\033[36m";

    string res = "";
    res.append(cyan);
    string first_line = "+";
    for (int l = 0; l < COLS; l++)
    {
        first_line.append("---+");
    }
    res.append(first_line);
    res.push_back('\n');

    for (int i = 0; i < ROWS; i++)
    {
        string row = "| ";
        for (int j = 0; j < COLS; j++)
        {
            if (board[i][j] == X)
            {
                row.append("\033[92m");
            }
            else
            {
                row.append("\033[93m");
            }
            row.push_back(board[i][j]);
            row.append("\033[0m");
            row.append(cyan);
            row.append(" | ");
        }
        row.push_back('\n');
        res.append(row);
        string line = "+";
        for (int k = 0; k < COLS; k++)
        {
            line.append("---+");
        }
        res.append(line);
        res.push_back('\n');
    }
    cout << res << "\033[0m";
}

/**
 * Returns the initial state of the board.
 */
vector<vector<char> > initial_state()
{
    vector<vector<char> > res;

    vector<char> buffer;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            buffer.push_back(EMPTY);
        }
        res.push_back(buffer);
        buffer.clear();
    }
    return res;
}

/**
 * Returns the next player for to play on the given board
 */
char player(vector<vector<char> > board)
{
    // set up counters for x and o
    unsigned short int xCounter = 0;
    unsigned short int oCounter = 0;

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board.at(i).at(j) == X)
            {
                xCounter++;
            }
            else if (board.at(i).at(j) == O)
            {
                oCounter++;
            }
        }
    }

    return xCounter > oCounter ? O : X;
}

/**
 * Returns the board that results from making move from the array {i, j}
 */
vector<vector<char> > result(vector<vector<char> > board, vector<int> action)
{

    vector<vector<char> > copy = deepcopy(board);
    copy[action[0]][action[1]] = player(board);
    return copy;
}

/**
 * Returns an array of actions available
 */
vector<vector<int> > actions(vector<vector<char> > board)
{
    vector<vector<int> > actions;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board.at(i).at(j) == EMPTY)
            {
                vector<int> action;
                action.push_back(i);
                action.push_back(j);
                actions.push_back(action);
            }
        }
    }
    return actions;
}

/**
 * Returns the winner of the game, if there is one, and empty otherwise.
 */
char winner(vector<vector<char> > board)
{
    char h = check_horizontally(board);
    char v = check_vertically(board);
    char d = check_diagonally(board);
    if (v != EMPTY || d != EMPTY || h != EMPTY)
    {
        if (h == X or v == X or d == X)
        {
            return X;
        }
        else
        {
            return O;
        }
    }
    else
    {
        return EMPTY;
    }
}

/**
 * Returns true is the game is over and False otherwise
 */
bool terminal(vector<vector<char> > board)
{
    if (winner(board) != EMPTY || full(board))
    {
        return true;
    }
    return false;
}

/**
 * Returns 1 if X has won the game, -1 if 0 has won the game, 0 otherwise
 * This function is expected to be called only when the board is terminal.
 */
int utility(vector<vector<char> > board)
{
    if (terminal(board))
    {
        if (winner(board) == X)
        {
            return 1;
        }
        else if (winner(board) == O)
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

/**
 * Returns the optimal action for the current player on the board
 */
vector<int> minimax(vector<vector<char> > board)
{
    char curr_player = player(board);
    MinimaxObj *buffer;
    if (curr_player == X)
    {
        buffer = max_value(board);
    }
    else
    {
        buffer = min_value(board);
    }
    return buffer->action;
}

/**
 * Calculates the optimal move that maximizes the value for the given board.
 */
MinimaxObj *max_value(vector<vector<char> > board)
{
    // set up the res object that is going to be returned
    MinimaxObj *res = new MinimaxObj();

    // if terminal return whatever the utility function gets for the board
    if (terminal(board))
    {
        res->value = utility(board);
        return res;
    }
    // set the curr best score to be as low as possible
    res->value = -2147483647;

    // get all actions
    vector<vector<int> > all_actions = actions(board);

    // iterating over the actions and apply the logic
    for (int i = 0, n = all_actions.size(); i < n; i++)
    {
        // store the current action in a variable
        vector<int> action = all_actions.at(i);

        // get the minimum possible value this action can make
        MinimaxObj *buffer = min_value(result(board, action));
        int v = buffer->value;

        // if it is greater than the current best
        if (v > res->value)
        {
            // update the current best to be v
            res->value = v;

            // and update the action
            res->action = action;

            // if the value is already 1, return the object
            if (res->value == 1)
            {
                return res;
            }
        }
    }

    return res;
}

/**
 * Calculates the optimal move that minimizes the value for the given board.
 */
MinimaxObj *min_value(vector<vector<char> > board)
{
    // set up the res object that is going to be returned
    MinimaxObj *res = new MinimaxObj();

    // if terminal return whatever the utility function gets for the board
    if (terminal(board))
    {
        res->value = utility(board);
        return res;
    }
    // set the curr best score to be as low as possible
    res->value = 2147483647;

    // get all actions
    vector<vector<int> > all_actions = actions(board);

    // iterating over the actions and apply the logic
    for (int i = 0, n = all_actions.size(); i < n; i++)
    {
        // store the current action in a variable
        vector<int> action = all_actions.at(i);

        // get the minimum possible value this action can make
        MinimaxObj *buffer = max_value(result(board, action));
        int v = buffer->value;

        // if it is greater than the current best
        if (v < res->value)
        {
            // update the current best to be v
            res->value = v;

            // and update the action
            res->action = action;

            // if the value is already 1, return the object
            if (res->value == -1)
            {
                return res;
            }
        }
    }

    return res;
}

/* ============================================================ */
// Helper functions =========================================== */
/* ============================================================ */

/**
 * Returns the deepcopy of a board with a type of `vector<vector<char> >`
 */
vector<vector<char> > deepcopy(vector<vector<char> > board)
{
    vector<vector<char> > copy;
    vector<char> buffer;
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            buffer.push_back(board.at(i).at(j));
        }
        copy.push_back(buffer);
        buffer.clear();
    }
    return copy;
}

/**
 * Returns a boolean representing whether the board has any empty spaces
 */
bool full(vector<vector<char> > board)
{
    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (board.at(i).at(j) == EMPTY)
            {
                return false;
            }
        }
    }
    return true;
}

char check_horizontally(vector<vector<char> > board)
{
    for (int i = 0; i < ROWS; i++)
    {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != EMPTY)
        {
            return board[i][0];
        }
    }
    return EMPTY;
}

char check_vertically(vector<vector<char> > board)
{
    for (int i = 0; i < COLS; i++)
    {
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != EMPTY)
        {
            return board[0][i];
        }
    }
    return EMPTY;
}

char check_diagonally(vector<vector<char> > board)
{
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != EMPTY)
    {
        return board[0][0];
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != EMPTY)
    {
        return board[0][2];
    }
    return EMPTY;
}