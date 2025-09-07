#include <iostream>
#include <vector>
#include <limits>
using namespace std;

vector<int> board(10, 2);
bool aiPlaysX;
int turn = 1;

void printBoard() {
    cout << "\n";
    for (int i = 1; i <= 9; i++) {
        char mark = (board[i] == 3) ? 'X' : (board[i] == 5) ? 'O' : ' ';
        cout << " " << mark << " ";
        if (i % 3 == 0) cout << "\n";
        else cout << "|";
    }
    cout << "\n";
}

int checkWinner() {
    vector<vector<int>> lines = {
        {1,2,3}, {4,5,6}, {7,8,9},
        {1,4,7}, {2,5,8}, {3,6,9},
        {1,5,9}, {3,5,7}
    };

    for (auto line : lines) {
        int prod = board[line[0]] * board[line[1]] * board[line[2]];
        if (prod == 27) return 3;
        if (prod == 125) return 5;
    }
    return 0;
}

bool isFull() {
    for (int i = 1; i <= 9; i++)
        if (board[i] == 2) return false;
    return true;
}

int alphaBeta(bool isMax, int depth, int alpha, int beta, int ai, int human) {
    int winner = checkWinner();
    if (winner == ai) return 10 - depth;
    if (winner == human) return depth - 10;
    if (isFull()) return 0;

    if (isMax) {
        int best = -1000;
        for (int i = 1; i <= 9; i++) {
            if (board[i] == 2) {
                board[i] = ai;
                best = max(best, alphaBeta(false, depth+1, alpha, beta, ai, human));
                board[i] = 2;
                alpha = max(alpha, best);
                if (beta <= alpha) break;
            }
        }
        return best;
    } else {
        int best = 1000;
        for (int i = 1; i <= 9; i++) {
            if (board[i] == 2) {
                board[i] = human;
                best = min(best, alphaBeta(true, depth+1, alpha, beta, ai, human));
                board[i] = 2;
                beta = min(beta, best);
                if (beta <= alpha) break;
            }
        }
        return best;
    }
}

void aiMove() {
    int ai = aiPlaysX ? 3 : 5;
    int human = aiPlaysX ? 5 : 3;
    int bestScore = -1000, bestMove = 0;

    for (int i = 1; i <= 9; i++) {
        if (board[i] == 2) {
            board[i] = ai;
            int score = alphaBeta(false, 0, -1000, 1000, ai, human);
            board[i] = 2;
            if (score > bestScore) {
                bestScore = score;
                bestMove = i;
            }
        }
    }

    cout << "AI chooses square " << bestMove << "\n";
    board[bestMove] = ai;
    turn++;
}

void humanMove() {
    int move;
    while (true) {
        cout << "Your move (1-9): ";
        cin >> move;
        if (move >= 1 && move <= 9 && board[move] == 2) {
            board[move] = (turn % 2 == 1) ? 3 : 5;
            turn++;
            break;
        } else {
            cout << "Invalid move. Try again.\n";
        }
    }
}

int main() {
    cout << "Welcome to Tic-Tac-Toe with Alpha-Beta AI!\n";
    char choice;
    cout << "Should AI play as X and start first? (y/n): ";
    cin >> choice;
    aiPlaysX = (choice == 'y' || choice == 'Y');

    printBoard();

    while (turn <= 9) {
        if ((turn % 2 == 1) == aiPlaysX) aiMove();
        else humanMove();

        printBoard();

        int winner = checkWinner();
        if (winner == 3) {
            cout << ((aiPlaysX && winner == 3) ? "AI wins!\n" : "Human wins!\n");
            return 0;
        }
        if (winner == 5) {
            cout << ((aiPlaysX && winner == 5) ? "AI wins!\n" : "Human wins!\n");
            return 0;
        }
    }

    cout << "It's a draw!\n";
    return 0;
}
