#include <iostream>
#include <string>
#include <array>
#include <random>
#include "colors.hpp"

using namespace std;

#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H


class TicTacToe {
private:
    string player1;
    string player2;
    array<array<string, 3>, 3> board;
    bool endGame = false;
    int games = 0;
    int turn = 1;
    int player1Score = 0;
    int player2Score = 0;
    int tieScore = 0;
    bool _playAgain = false;
    void resetBoard();
    bool fillField(int row, int col);
    static void showMessage(const string&);
    string printBoard();
    string printWinner();
    string printTie();
    static string printInvalid(const string& message);
    string printTurn();
    void verifyBoard();
    bool _verifyLine();
    bool _verifyColumn();
    bool _verifyDiagonal();
    bool _verifyTie();
    void _playCPU();
    void _playMenu();

public:
    TicTacToe();
    TicTacToe(string player1, string player2);
    void play();
    void run();
    void reset();
    string viewScore() const;
    static string showMenu();
    void menu();
    void setPlayer(int, const string&);
};

// Constructor

TicTacToe::TicTacToe() {
    showMessage("Welcome to Tic Tac Toe!");
    string name;
    showMessage("Please enter your name: ");
    cin >> name;
    setPlayer(1, name);
    resetBoard();
}
TicTacToe::TicTacToe(string player1, string player2) {
    this->player1 = move(player1);
    this->player2 = move(player2);
    resetBoard();
}

// Private functions

void TicTacToe::resetBoard() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = " ";
        }
    }
    endGame = false;
    turn = 1;
}

bool TicTacToe::fillField(int row, int col) {
    if ((row < 1 or row > 3) or (col < 1 or col > 3)) {
        showMessage(printInvalid("Invalid input"));
        return false;
    }
    row--; col--;

    if (board[col][row] == " ") {
        if (turn % 2 == 0) {
            board[col][row] = color::blue + "O" + color::off;
        } else {
            board[col][row] = color::green + "X" + color::off;
        }
        return true;
    } else {
        if (player2 != "CPU")
            showMessage(printInvalid("Field is already filled"));
    }

    return false;
}

void TicTacToe::showMessage(const string& message) {
    cout << message << endl;
}

string TicTacToe::printBoard() {
    string board_string = "";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board_string += " " + board[i][j] + " ";
            if (j != 2) board_string += "|";
        }
        if (i != 2) board_string += "\n---+---+---\n";
    }

    return board_string;
}

string TicTacToe::printWinner() {
    if (turn % 2 == 0) {
        player2Score++;
        return color::blue + player2 + " wins!" + color::off;
    } else {
        player1Score++;
        return color::green + player1 + " wins!" + color::off;
    }
}

string TicTacToe::printTie() {
    tieScore++;
    return color::yellow + "Tie!" + color::off;
}

string TicTacToe::printInvalid(const string& message) {
    return color::red + message + color::off;
}

string TicTacToe::printTurn() {
    if (turn % 2 == 0) {
        return color::blue + player2 + "'s turn" + color::off;
    } else {
        return color::green + player1 + "'s turn" + color::off;
    }
}

void TicTacToe::verifyBoard() {
    if (_verifyTie()) {
        endGame = true;
        showMessage(printTie());
        return;
    }
    bool line, column, diagonal;
    line = _verifyLine();
    column = _verifyColumn();
    diagonal = _verifyDiagonal();
    if (line or column or diagonal) {
        endGame = true;
        showMessage(printWinner());
        return;
    }

    turn++;
}

bool TicTacToe::_verifyLine() {
    for (int i = 0; i < 3; i++) {
        string comparison = board[i][0];
        bool line = true;
        for (int j = 1; j < 3; j++) {
            if (comparison != board[i][j] or comparison == " ") {
                line = false;
                break;
            };
        }

        if (line) return true;
    }

    return false;
}

bool TicTacToe::_verifyColumn() {
    for (int i = 0; i < 3; i++) {
        string comparison = board[0][i];
        bool column = true;
        for (int j = 1; j < 3; j++) {
            if (comparison != board[j][i] or comparison == " ") {
                column = false;
                break;
            }
        }

        if (column) return true;
    }

    return false;
}

bool TicTacToe::_verifyDiagonal() {
    string comparison = board[1][1]; // center

    if (comparison == " ") return false;
    if (comparison == board[0][0] and comparison == board[2][2]) return true;
    if (comparison == board[0][2] and comparison == board[2][0]) return true;

    return false;
}

bool TicTacToe::_verifyTie() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == " ") return false;
        }
    }

    return true;
}

void TicTacToe::_playCPU() {
    int row, col;
    do {
        srand(time(nullptr));
        row = rand() % 3 + 1;
        col = rand() % 3 + 1;
    } while (!fillField(row, col));
}

void TicTacToe::_playMenu() {
    char exitInput;
    do {
        play();
        showMessage("Do you want to continue? (Y/n) ");
        cin >> exitInput;
        exitInput = (char) tolower(exitInput);
        if (exitInput == 'n') {
            showMessage("Back to main menu");
            _playAgain = false;
            return;
        } else if (exitInput == 'y') {
            showMessage("Starting new game");
            _playAgain = true;
        }
    } while (exitInput != 'n' or exitInput != 'y');
}

// Public functions

void TicTacToe::play() {
    while (!endGame) {
        showMessage(printTurn());
        showMessage(printBoard());
        if (player2 == "CPU" and turn % 2 == 0) {
            _playCPU();
            verifyBoard();
        }
        else {
            int row, col;
            showMessage("Enter row and column (1-3): ");
            cin >> row >> col;
            if (fillField(row, col)) {
                verifyBoard();
            }
        }
    }
    showMessage(printBoard());
    resetBoard();
    games++;
}

void TicTacToe::run() {
    menu();
}

void TicTacToe::reset() {
    resetBoard();
    turn = 1;
    endGame = false;
    games = 0;
    player1Score = 0;
    player2Score = 0;
    tieScore = 0;
}

string TicTacToe::viewScore() const {
    return color::purples + "Games: " + to_string(games) +
        "\nPlayer 1 - " + player1 + ": " + to_string(player1Score) +
        "\nPlayer 2 - " + player2 + ": " + to_string(player2Score) +
        "\nTie: " + to_string(tieScore) + color::off;
}

string TicTacToe::showMenu() {
    return "1. Play\n2. Show Score\n3. Reset Score\n4. Change Player Name\n0. Exit";
}

void TicTacToe::menu() {
    int option;
    while (true) {
        showMessage(showMenu());
        cin >> option;
        switch (option) {
            case 1: {
                int playOption;
                const string msg = color::cyan + "1. Player vs CPU\n2. Player vs Player\n0. Back" + color::off;
                showMessage(msg);
                cin >> playOption;
                switch (playOption) {
                    case 1:
                        player2 = "CPU";
                        break;
                    case 2: {
                        string player2Name;
                        showMessage("Enter player 2 name: ");
                        cin >> player2Name;
                        setPlayer(2, player2Name);
                        break;
                    }
                    case 0:
                        break;
                    default:
                        showMessage("Invalid option");
                        break;
                }
                _playMenu();

                break;
            }
            case 2: {
                showMessage(viewScore());
                break;
            }
            case 3: {
                reset();
                showMessage("Score reseted");
                break;
            }
            case 4: {
                int playerId;
                string playerName;
                showMessage("Enter player Id (1 or 2): ");
                cin >> playerId;
                showMessage("Enter player name: ");
                cin >> playerName;
                setPlayer(playerId, playerName);
                break;
            }
            case 0: {
                showMessage("Bye!");
                exit(0);
            }
            default: {
                showMessage(printInvalid("Invalid option"));
                break;
            }
        }
    }
}

void TicTacToe::setPlayer(int id, const string& name) {
    if (id == 1) {
        player1 = name;
    } else if (id == 2) {
        player2 = name;
    }
}

#endif //TIC_TAC_TOE_H
