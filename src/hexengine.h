//
// Created by zbyszek on 17.05.2026.
//

#ifndef GLAUCUS_HEXENGINE_H
#define GLAUCUS_HEXENGINE_H

#include <string>
#include <vector>

namespace hexengine {
    constexpr int HEXBOARD_SIZE = 91;
    constexpr int MAX_KING_MOVES = 12;
    constexpr int MAX_KNIGHT_MOVES = 12;
    constexpr int ROOK_RAYS = 6;
    constexpr int MAX_ROOK_RAY_LENGTH = 10;
    constexpr int BISHOP_RAYS = 6;
    constexpr int MAX_BISHOP_RAY_LENGTH = 5;
    constexpr int MAX_Q = 5;
    constexpr int MAX_R = 5;
    constexpr int MAX_S = 5;

    enum PieceType { Empty, King, Pawn, Knight, Bishop, Rook, Queen };

    enum PieceTurn { WhiteTurn, BlackTurn };

    enum PieceSide { None, White, Black };

    struct Piece {
        PieceType type;
        PieceSide side;
    };

    struct CubeCoords {
        int q;
        int r;
        int s;
    };

    using Board = Piece[HEXBOARD_SIZE];

    struct Move {
        int from;
        int to;
        Piece captured_piece;
    };

    struct HBoard {
        Board board;
        PieceTurn turn;
    };

    const HBoard& get_main_board();

    extern int king_moves[HEXBOARD_SIZE][MAX_KING_MOVES];
    extern int knight_moves[HEXBOARD_SIZE][MAX_KNIGHT_MOVES];
    extern int rook_moves[HEXBOARD_SIZE][ROOK_RAYS][MAX_ROOK_RAY_LENGTH];
    extern int bishop_moves[HEXBOARD_SIZE][BISHOP_RAYS][MAX_BISHOP_RAY_LENGTH];

    /**
     * Initializes the `hex_qrs` table with coordinates for a hexagonal grid.
     *
     */
    void init_hex_qrs_table();

    void init_hexengine();

    void setup_board(HBoard &board);

    const CubeCoords &get_hex_qrs(int index);

    const Piece &get_piece(int index);

    void set_piece(int index, Piece piece);

    PieceTurn get_turn();

    void set_turn(PieceTurn turn);

    void init_king_moves();

    void init_knight_moves();

    void init_rook_moves();

    void init_bishop_moves();

    std::string move_to_uci(const Move &move);
    
    bool is_checked(const HBoard &board, const Move &move);
    bool is_king_in_check(const HBoard &board, PieceSide side);

    bool make_move(const Move &move);

    std::vector<Move> get_legal_king_moves(const HBoard &board, bool only_captures = false);

    std::vector<Move> get_legal_king_moves_at(const HBoard &board, int index, bool only_captures = false);

    std::vector<Move> get_legal_knight_moves(const HBoard &board, bool only_captures = false);

    std::vector<Move> get_legal_knight_moves_at(const HBoard &board, int index, bool only_captures = false);

    std::vector<Move> get_legal_rook_moves(const HBoard &board, bool only_captures = false);

    std::vector<Move> get_legal_rook_moves_at(const HBoard &board, int index, bool only_captures = false);

    std::vector<Move> get_legal_moves_at(const HBoard &board, int index);

    std::vector<Move> get_all_legal_moves(const HBoard &board, bool only_captures = false);
}
#endif //GLAUCUS_HEXENGINE_H
