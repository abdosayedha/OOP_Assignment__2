#include <iostream>
#include "BoardGame_Classes.h"
#include "FourBoard.h"
#include "FourUI.h"

void show_rules() {
    std::cout << "=============================\n";
    std::cout << "      4x4 Move Tic-Tac-Toe   \n";
    std::cout << "=============================\n\n";

    std::cout << "Rules of the Game:\n";
    std::cout << "1. The board is 4x4.\n";
    std::cout << "2. Each player begins with 4 pieces placed on the board.\n";
    std::cout << "3. You do NOT place new pieces — you MOVE your existing pieces.\n";
    std::cout << "4. On your turn:\n";
    std::cout << "   - Select one of your pieces (source position).\n";
    std::cout << "   - Move it to an EMPTY adjacent cell (up, down, left, right).\n";
    std::cout << "5. The goal is to make THREE of your pieces in a row:\n";
    std::cout << "   - Horizontally\n";
    std::cout << "   - Vertically\n";
    std::cout << "   - Diagonally\n";
    std::cout << "6. If you create 3-in-a-row → YOU WIN.\n";
    std::cout << "7. If your opponent does → YOU LOSE.\n";
    std::cout << "8. If no moves lead to a win and board cannot produce 3-in-a-row → it's a draw.\n\n";

    std::cout << "Press Enter to start the game...";
    std::cin.ignore();
}

int main() {

    show_rules();  // ⭐️ عرض القواعد قبل بدء اللعبة

    FourBoard* board = new FourBoard();
    FourUI* ui = new FourUI();

    Player<char>* p1 = new Player<char>("Player 1 (X)", 'X', PlayerType::HUMAN);
    Player<char>* p2 = new Player<char>("Player 2 (O)", 'O', PlayerType::HUMAN);
    p1->set_board_ptr(board);
    p2->set_board_ptr(board);
    Player<char>* players[2] = { p1, p2 };

    GameManager<char> gm(board, players, ui);
    gm.run();

    delete ui;
    delete p1;
    delete p2;
    delete board;
    return 0;
}
