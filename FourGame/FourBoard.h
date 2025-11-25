#pragma once
#include "BoardGame_Classes.h"
#include <cmath>
#include <algorithm>

// ================= FourMove =================
class FourMove : public Move<char> {
private:
    int src_x, src_y;
    int dst_x, dst_y;
public:
    FourMove(int sx, int sy, int dx, int dy, char sym);

    int get_src_x() const;
    int get_src_y() const;
    int get_dst_x() const;
    int get_dst_y() const;
};

// ================= FourBoard =================
class FourBoard : public Board<char> {
public:
    FourBoard();
    virtual ~FourBoard() {}

    bool update_board(Move<char>* move) override;
    bool is_win(Player<char>* p) override;
    bool is_lose(Player<char>* p) override;
    bool is_draw(Player<char>* p) override;
    bool game_is_over(Player<char>* p) override;

private:
    bool valid_src_dst(int sx, int sy, int dx, int dy, char sym) const;
    bool check_three_in_a_row(char sym) const;
    bool in_bounds(int r, int c) const { return r >= 0 && r < rows && c >= 0 && c < columns; }
};
#pragma once
