//
// Created by zbyszek on 17.05.2026.
//

#ifndef GLAUCUS_HEXENGINE_H
#define GLAUCUS_HEXENGINE_H

#include <string>
#include <vector>
#include <cstdint>

namespace hexengine {
    constexpr int HEXBOARD_SIZE = 91;
    constexpr int QRS_SIZE = 3;
    constexpr int MAX_KING_MOVES = 12;
    constexpr int MAX_KNIGHT_MOVES = 12;
    constexpr int ROOK_RAYS = 6;
    constexpr int MAX_ROOK_RAY_LENGTH = 10;
    constexpr int BISHOP_RAYS = 6;
    constexpr int MAX_BISHOP_RAY_LENGTH = 5;
    constexpr int MAX_PAWN_MOVE_TYPE_MOVES = 2;
    constexpr int PAWN_MOVE_TYPE = 2;
    constexpr int PAWN_COLOR = 2;
    constexpr int MAX_Q = 5;
    constexpr int MAX_R = 5;
    constexpr int MAX_S = 5;

    enum PieceType { Empty, King, Pawn, Knight, Bishop, Rook, Queen };

    enum PieceTurn { WhiteColor, BlackColor };

    enum PieceSide { None, White, Black };

    enum PawnMoveType { Forward, Capture };

    struct Piece {
        PieceType type;
        PieceSide side;
    };

    struct CubeCoords {
        int q;
        int r;
        int s;
    };

    using BoardArray = Piece[HEXBOARD_SIZE];
    using Color = PieceTurn;

    struct Move {
        int from{};
        int to{};
        Piece captured_piece{};
        PieceType promotion = PieceType::Empty;
        bool is_en_passant = false;
        int prev_en_passant_square = -1;
    };

    struct HBoard {
        BoardArray board_array;
        PieceTurn turn;
        int en_passant_square = -1;
        int king_pos[2] = {-1, -1};
    };

    HBoard &get_main_board();

    void init_hexengine();

    void setup_board(HBoard &board);

    void setup_startup_board(HBoard &board);

    const CubeCoords &get_hex_qrs(int index);

    const Piece &get_piece(const HBoard &board, int index);

    void set_piece(HBoard &board, int index, Piece piece);

    PieceTurn get_turn(const HBoard &board);

    void set_turn(HBoard &board, PieceTurn turn);

    void set_turn(PieceTurn turn);

    std::string move_to_uci(const Move &move);

    bool is_checked(HBoard &board, const Move &move);

    bool is_king_in_check(const HBoard &board, PieceSide side);

    bool is_insufficient_material(const HBoard &board);

    void get_legal_king_moves(HBoard &board, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_king_moves_at(HBoard &board, int index, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_knight_moves(HBoard &board, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_knight_moves_at(HBoard &board, int index, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_rook_moves(HBoard &board, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_rook_moves_at(HBoard &board, int index, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_bishop_moves(HBoard &board, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_bishop_moves_at(HBoard &board, int index, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_queen_moves(HBoard &board, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_queen_moves_at(HBoard &board, int index, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_pawn_moves(HBoard &board, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_pawn_moves_at(HBoard &board, int index, std::vector<Move> &moves, bool only_captures = false);

    void get_legal_moves_at(HBoard &board, int index, std::vector<Move> &moves);

    std::vector<Move> get_all_legal_moves(HBoard &board, bool only_captures = false);

    Move get_random_move(HBoard &board);

    void make_move(HBoard &board, const Move &move);

    void unmake_move(HBoard &board, const Move &move);

    struct PerftResults {
        std::uint64_t nodes = 0;
        std::uint64_t captures = 0;
        std::uint64_t en_passants = 0;
        std::uint64_t promotions = 0;
        std::uint64_t promotion_captures = 0;
        std::uint64_t checks = 0;
        std::uint64_t checkmates = 0;
        std::uint64_t stalemates = 0;
    };

    std::uint64_t perft(HBoard &board, int depth);

    PerftResults perfts(HBoard &board, int depth);

    void perftd(HBoard &board, int depth);

    std::string smart_int_print(std::uint64_t val);

    extern const int king_moves[HEXBOARD_SIZE][MAX_KING_MOVES];
    extern const int knight_moves[HEXBOARD_SIZE][MAX_KNIGHT_MOVES];
    extern const int rook_moves[HEXBOARD_SIZE][ROOK_RAYS][MAX_ROOK_RAY_LENGTH];
    extern const int bishop_moves[HEXBOARD_SIZE][BISHOP_RAYS][MAX_BISHOP_RAY_LENGTH];
    extern const int pawn_moves[HEXBOARD_SIZE][PAWN_COLOR][PAWN_MOVE_TYPE][MAX_PAWN_MOVE_TYPE_MOVES];
}
#endif //GLAUCUS_HEXENGINE_H
