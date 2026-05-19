//
// Created by zbyszek on 17.05.2026.
//

#include "hexengine.h"

#include <cassert>
#include <QString>

class QString;

namespace hexengine {
    namespace {
        HexCubeCoords hex_qrs[HEXBOARD_SIZE];
        HBoard main_board = {{{PieceType::Empty, PieceSide::None}}, PieceTurn::WhiteTurn};
    }

    void init_hex_qrs_table() {
        int index = 0;
        for (int q = -5; q <= 5; ++q) {
            const int r_start = (q <= 0) ? (-5 - q) : -5;
            const int r_end = (q <= 0) ? 5 : (5 - q);
            for (int r = r_start; r <= r_end; ++r) {
                hex_qrs[index].q = q;
                hex_qrs[index].r = r;
                hex_qrs[index].s = -q - r;
                index++;
            }
        }
    }

    void init_hexengine() {
        init_hex_qrs_table();
        setup_board(main_board);
    }

    void setup_board(HBoard &board) {
        set_piece(5, {PieceType::King, PieceSide::White});
        set_piece(85, {PieceType::King, PieceSide::Black});
        set_piece(10, {PieceType::Knight, PieceSide::White});
        set_piece(86, {PieceType::Knight, PieceSide::White});
        set_piece(20, {PieceType::Knight, PieceSide::Black});
        set_piece(79, {PieceType::Knight, PieceSide::Black});
        set_turn(PieceTurn::WhiteTurn);
    }

    const HexCubeCoords &get_hex_qrs(const int index) {
        assert(index >=0 && index<HEXBOARD_SIZE);
        return hex_qrs[index];
    }

    const Piece &get_piece(const int index) {
        assert(index>=0 && index<HEXBOARD_SIZE);
        return main_board.board[index];
    }

    void set_piece(const int index, const Piece piece) {
        assert(index>=0 && index<HEXBOARD_SIZE);
        main_board.board[index] = piece;
    }

    PieceTurn get_turn() {
        return main_board.turn;
    }

    void set_turn(const PieceTurn turn) {
        main_board.turn = turn;
    }
}
