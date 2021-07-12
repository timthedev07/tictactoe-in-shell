#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

struct MinimaxObj
{
    int value;
    vector<int> action;
};
vector<int> minimax(vector<vector<char> > board);
vector<vector<char> > initial_state();
void print_board(vector<vector<char> > board);
char player(vector<vector<char> > board);
vector<vector<char> > result(vector<vector<char> > board, vector<int> action);
char winner(vector<vector<char> > board);
vector<vector<int> > actions(vector<vector<char> > board);
bool terminal(vector<vector<char> > board);
int utility(vector<vector<char> > board);
MinimaxObj *max_value(vector<vector<char> > board);
MinimaxObj *min_value(vector<vector<char> > board);
