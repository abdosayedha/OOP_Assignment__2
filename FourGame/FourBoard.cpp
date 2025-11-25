#include "FourBoard.h"
#include "BoardGame_Classes.h"
#include <cstdlib>
#include <iostream>

// ================= FourMove =================
FourMove::FourMove(int sx, int sy, int dx, int dy, char sym)
    : Move<char>(dx, dy, sym), src_x(sx), src_y(sy), dst_x(dx), dst_y(dy) {}

int FourMove::get_src_x() const { return src_x; }
int FourMove::get_src_y() const { return src_y; }
int FourMove::get_dst_x() const { return dst_x; }
int FourMove::get_dst_y() const { return dst_y; }

// ================= FourBoard =================
FourBoard::FourBoard() : Board<char>(4, 4) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < columns; j++)
            board[i][j] = '-';

    // initial positions example
    board[0][0] = 'O'; board[0][1] = 'X'; board[0][2] = 'O'; board[0][3] = 'X';
    board[3][0] = 'X'; board[3][1] = 'O'; board[3][2] = 'X'; board[3][3] = 'O';

    n_moves = 0;
}

bool FourBoard::valid_src_dst(int sx, int sy, int dx, int dy, char sym) const {
    if (!in_bounds(sx, sy) || !in_bounds(dx, dy)) return false;
    if (sx == dx && sy == dy) return false;
    if (board[sx][sy] != sym) return false;
    if (board[dx][dy] != '-') return false;
    int dr = std::abs(dx - sx);
    int dc = std::abs(dy - sy);
    return (dr == 1 && dc == 0) || (dr == 0 && dc == 1);
}

bool FourBoard::update_board(Move<char>* move) {
    FourMove* fm = static_cast<FourMove*>(move);
    if (!fm) return false;

    int sx = fm->get_src_x();
    int sy = fm->get_src_y();
    int dx = fm->get_dst_x();
    int dy = fm->get_dst_y();
    char sym = fm->get_symbol();

    if (!valid_src_dst(sx, sy, dx, dy, sym)) return false;

    board[dx][dy] = sym;
    board[sx][sy] = '-';
    n_moves++;

    return true;
}

bool FourBoard::check_three_in_a_row(char sym) const {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c <= 1; c++)
            if (board[r][c] == sym && board[r][c + 1] == sym && board[r][c + 2] == sym) return true;

    for (int c = 0; c < 4; c++)
        for (int r = 0; r <= 1; r++)
            if (board[r][c] == sym && board[r + 1][c] == sym && board[r + 2][c] == sym) return true;

    for (int r = 0; r <= 1; r++)
        for (int c = 0; c <= 1; c++)
            if (board[r][c] == sym && board[r + 1][c + 1] == sym && board[r + 2][c + 2] == sym) return true;

    for (int r = 0; r <= 1; r++)
        for (int c = 2; c <= 3; c++)
            if (board[r][c] == sym && board[r + 1][c - 1] == sym && board[r + 2][c - 2] == sym) return true;

    return false;
}

bool FourBoard::is_win(Player<char>* p) {
    if (!p) return false;
    return check_three_in_a_row(p->get_symbol());
}

bool FourBoard::is_lose(Player<char>* p) {
    if (!p) return false;
    char opp = (p->get_symbol() == 'X') ? 'O' : 'X';
    return check_three_in_a_row(opp);
}

bool FourBoard::is_draw(Player<char>* p) {
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            if (board[r][c] == '-') return false;
    return true;
}

bool FourBoard::game_is_over(Player<char>* p) {
    return is_win(p) || is_lose(p) || is_draw(p);
}
