#include "tictactoe.h"
#include <iostream>
#include <unistd.h>
#include <term.h>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

// the constant variables
const char X = 'X';
const char O = 'O';
const char EMPTY = ' ';

// prototypes
bool play(char user);
void ClearScreen();
bool ValidOneDigit(string line);
bool gameEnds(vector<vector<char> > board, char player);
vector<int> promptAction(vector<vector<char> > board);

// main function to be ran
int main()
{
    char player;

    while (true)
    {
        // clearing the terminal
        ClearScreen();

        // asking for input
        string buffer; 
        while (toupper(player) != 'O' && toupper(player) != 'X')
        {
            cout << "Play as(X or O): ";
            getline(cin, buffer);
            if (buffer == "" || buffer.empty()) {
                continue;
            }
            player = buffer.at(0);
        }
        // converts letter to uppercase to match the constant variable defined in the tic tac toe file
        player = toupper(player);

        cout << "You are now playing as: ";
        cout << player << endl;

        // plays the game and at the end, get a response that tells the program
        // whether or not to continue playing a new round
        bool pagain = play(player);

        // if response is no, quit
        if (!pagain)
        {
            cout << endl
                    << "Have a nice day!" << endl;
            break;
        }
        cout << endl;
    }
}

/**
 * Plays the tic tac toe game. Returns true if the user wants to play again, and false otherwise
 */
bool play(char user)
{
    // setting up the board
    vector<vector<char> > board = initial_state();

    cout << "Let's start this game!\n";
    print_board(board);

    // if the user chose to be O, then, let AI make the first move randomly
    if (user == O)
    {
        cout << "\nAI Thinking...\n";

        // get the ai move

        vector<int> move;
        move.push_back(rand() % 3);
        move.push_back(rand() % 3);

        // apply the move to the board
        board = result(board, move);

        // print the board
        print_board(board);
    }

    // repeat
    while (true)
    {
        vector<int> action = promptAction(board);

        // apply action to board and print the board
        board = result(board, action);
        print_board(board);

        // check if someone wins or there is a tie
        if (gameEnds(board, user))
        {
            break;
        }

        cout << "\nAI Thinking...\n";

        // get the ai move
        vector<int> move = minimax(board);

        // apply the move to the board
        board = result(board, move);

        // print the board
        print_board(board);
        if (gameEnds(board, user))
        {
            break;
        }
    }

    // asking whether if the user wants to play again.
    char resp;
    while (tolower(resp) != 'y' && tolower(resp) != 'n')
    {
        cout << endl
             << "Play again? [Y/n] ";
        cin >> resp;
    }

    return tolower(resp) == 'y' ? true : false;
}

//g++ tictactoe.cpp play.cpp -o main.out

void ClearScreen()
{

    system("clear");
}

bool ValidOneDigit(string line)
{
    if (line.length() > 1)
    {
        return false;
    }

    if (isdigit(line[0]))
    {
        int intval = line[0] - '0';
        if (intval < 3)
        {
            return true;
        }
        return false;
    }

    return false;
}

/**
 * Returns a boolean that tells whether or not the given board is in the terminal state
 * whether it's someone wins, or there is a tie.
 */
bool gameEnds(vector<vector<char> > board, char player)
{
    if (terminal(board))
    {
        char _winner = winner(board);
        cout << "Game ends: ";
        if (_winner != EMPTY)
        {
            if (_winner == player)
            {
                cout << "\033[32m"
                     << "You won!"
                     << "\033[0m" << endl;
            }
            else
            {
                cout << "\033[31m"
                     << "You lost!"
                     << "\033[0m" << endl;
            }
        }
        else
        {
            cout << "Tie." << endl;
        }
        return true;
    }
    return false;
}

vector<int> promptAction(vector<vector<char> > board)
{
    // setting up the string for storing input
    string response;

    // setting up the variables to store i and j
    int i;
    int j;

    cout << "\nYour turn now!\n";

    // asking for the row number
    cout << "Row number: ";
    cin >> response;
    while (!ValidOneDigit(response))
    {
        cout << "\nPlease enter a valid number from 0 to 2, try again!\n";
        cout << "Row number: ";
        cin >> response;
    }

    // convert response to integer
    i = response[0] - '0';

    // asking for the column number
    cout << "Column number: ";
    cin >> response;
    while (!ValidOneDigit(response))
    {
        cout << "\nPlease enter a valid number from 0 to 2, try again!\n";
        cout << "Column number: ";
        cin >> response;
    }

    // convert response to integer
    j = response[0] - '0';

    vector<int> action;
    action.push_back(i);
    action.push_back(j);

    // if the prompted action tries points to a non-empty block
    if (board.at(action.at(0)).at(action.at(1)) != EMPTY)
    {
        cout << "Place already occupied, try somewhere else." << endl;
        action = promptAction(board);
    }

    return action;
}