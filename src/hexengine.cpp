//
// Created by zbyszek on 17.05.2026.
//

#include "hexengine.h"

#include <cassert>
#include <algorithm>
#include <cmath>
#include <iostream>

namespace hexengine {
    namespace {
        CubeCoords hex_qrs[HEXBOARD_SIZE];
        HBoard main_board = {{{PieceType::Empty, PieceSide::None}}, PieceTurn::WhiteColor};

        bool is_promotion_hexagon(int index, PieceSide side) {
            const auto &coord = hex_qrs[index];
            if (side == PieceSide::White) {
                int r_start = (coord.q <= 0) ? (-5 - coord.q) : -5;
                return coord.r == r_start;
            } else if (side == PieceSide::Black) {
                int r_end = (coord.q <= 0) ? 5 : (5 - coord.q);
                return coord.r == r_end;
            }
            return false;
        }
    }

    int king_moves[HEXBOARD_SIZE][MAX_KING_MOVES];
    int knight_moves[HEXBOARD_SIZE][MAX_KNIGHT_MOVES];
    int rook_moves[HEXBOARD_SIZE][ROOK_RAYS][MAX_ROOK_RAY_LENGTH];
    int bishop_moves[HEXBOARD_SIZE][BISHOP_RAYS][MAX_BISHOP_RAY_LENGTH];
    int pawn_moves[HEXBOARD_SIZE][PAWN_COLOR][PAWN_MOVE_TYPE][MAX_PAWN_MOVE_TYPE_MOVES];

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
        init_king_moves();
        init_knight_moves();
        init_rook_moves();
        init_bishop_moves();
        init_pawn_moves();
        setup_board(main_board);
    }

    const HBoard &get_main_board() {
        return main_board;
    }

    void setup_board(HBoard &board) {
        // set_piece(board, 60, {PieceType::King, PieceSide::White});
        // set_piece(board, 51, {PieceType::King, PieceSide::Black});
        // set_piece(board, 29, {PieceType::Knight, PieceSide::White});
        // set_piece(board, 69, {PieceType::Knight, PieceSide::White});
        // set_piece(board, 21, {PieceType::Knight, PieceSide::Black});
        // set_piece(board, 61, {PieceType::Knight, PieceSide::Black});
        // set_piece(board, 20, {PieceType::Rook, PieceSide::White});
        // set_piece(board, 13, {PieceType::Rook, PieceSide::Black});
        // set_piece(board, 77, {PieceType::Rook, PieceSide::White});
        // set_piece(board, 70, {PieceType::Rook, PieceSide::Black});
        // set_piece(board, 48, {PieceType::Bishop, PieceSide::White});
        // set_piece(board, 49, {PieceType::Bishop, PieceSide::White});
        // set_piece(board, 50, {PieceType::Bishop, PieceSide::White});
        // set_piece(board, 40, {PieceType::Bishop, PieceSide::Black});
        // set_piece(board, 41, {PieceType::Bishop, PieceSide::Black});
        // set_piece(board, 42, {PieceType::Bishop, PieceSide::Black});
        // set_piece(board, 39, {PieceType::Queen, PieceSide::White});
        // set_piece(board, 30, {PieceType::Queen, PieceSide::Black});

        // set_piece(board, 12, {PieceType::Pawn, PieceSide::White});
        // set_piece(board, 19, {PieceType::Pawn, PieceSide::White});
        // set_piece(board, 27, {PieceType::Pawn, PieceSide::White});
        // set_piece(board, 36, {PieceType::Pawn, PieceSide::White});
        // set_piece(board, 46, {PieceType::Pawn, PieceSide::White});
        // set_piece(board, 57, {PieceType::Pawn, PieceSide::White});
        // set_piece(board, 67, {PieceType::Pawn, PieceSide::White});
        // set_piece(board, 76, {PieceType::Pawn, PieceSide::White});
        // set_piece(board, 84, {PieceType::Pawn, PieceSide::White});
        // set_piece(board, 6, {PieceType::Pawn, PieceSide::Black});
        // set_piece(board, 14, {PieceType::Pawn, PieceSide::Black});
        // set_piece(board, 23, {PieceType::Pawn, PieceSide::Black});
        // set_piece(board, 33, {PieceType::Pawn, PieceSide::Black});
        // set_piece(board, 44, {PieceType::Pawn, PieceSide::Black});
        // set_piece(board, 54, {PieceType::Pawn, PieceSide::Black});
        // set_piece(board, 63, {PieceType::Pawn, PieceSide::Black});
        // set_piece(board, 71, {PieceType::Pawn, PieceSide::Black});
        // set_piece(board, 78, {PieceType::Pawn, PieceSide::Black});

        set_piece(board, 14, {PieceType::Pawn, PieceSide::White});
        set_piece(board, 41, {PieceType::Pawn, PieceSide::White});
        set_piece(board, 49, {PieceType::Pawn, PieceSide::Black});
        set_piece(board, 30, {PieceType::Queen, PieceSide::Black});
        set_piece(board, 39, {PieceType::Queen, PieceSide::White});

        set_turn(board, PieceTurn::WhiteColor);
    }

    const CubeCoords &get_hex_qrs(const int index) {
        assert(index >=0 && index<HEXBOARD_SIZE);
        return hex_qrs[index];
    }

    const Piece &get_piece(const HBoard &board, const int index) {
        assert(index>=0 && index<HEXBOARD_SIZE);
        return board.board[index];
    }

    void set_piece(HBoard &board, const int index, const Piece piece) {
        assert(index>=0 && index<HEXBOARD_SIZE);
        board.board[index] = piece;
    }

    PieceTurn get_turn(const HBoard &board) {
        return board.turn;
    }

    void set_turn(HBoard &board, const PieceTurn turn) {
        board.turn = turn;
    }

    void set_turn(const PieceTurn turn) {
        main_board.turn = turn;
    }

    void init_king_moves() {
        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            for (int j = 0; j < MAX_KING_MOVES; ++j) {
                king_moves[i][j] = -1;
            }

            int move_idx = 0;
            const auto &source = hex_qrs[i];

            for (int j = 0; j < HEXBOARD_SIZE; ++j) {
                if (i == j) continue;

                const CubeCoords &target = hex_qrs[j];
                const int dq = std::abs(source.q - target.q);
                const int dr = std::abs(source.r - target.r);
                const int ds = std::abs(source.s - target.s);
                const int distance = std::max({dq, dr, ds});

                bool can_move = false;
                if (distance == 1) {
                    can_move = true;
                } else if (distance == 2) {
                    if (source.q != target.q && source.r != target.r && source.s != target.s) {
                        can_move = true;
                    }
                }

                if (can_move) {
                    if (move_idx < MAX_KING_MOVES) {
                        king_moves[i][move_idx++] = j;
                    }
                }
            }
        }
    }

    void init_knight_moves() {
        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            for (int j = 0; j < MAX_KNIGHT_MOVES; ++j) {
                knight_moves[i][j] = -1;
            }

            int move_idx = 0;
            const auto &source = hex_qrs[i];

            for (int j = 0; j < HEXBOARD_SIZE; ++j) {
                if (i == j) continue;

                const CubeCoords &target = hex_qrs[j];
                const int dq = std::abs(source.q - target.q);
                const int dr = std::abs(source.r - target.r);
                const int ds = std::abs(source.s - target.s);
                const int distance = std::max({dq, dr, ds});

                bool can_move = false;
                if (distance == 3) {
                    if (source.q != target.q && source.r != target.r && source.s != target.s) {
                        can_move = true;
                    }
                }

                if (can_move) {
                    if (move_idx < MAX_KNIGHT_MOVES) {
                        knight_moves[i][move_idx++] = j;
                    }
                }
            }
        }
    }

    void init_rook_moves() {
        struct Delta {
            int dq, dr, ds;
        };
        const Delta directions[ROOK_RAYS] = {
            {0, -1, 1}, // 1) q same, r-1
            {0, 1, -1}, // 2) q same, r+1
            {-1, 0, 1}, // 3) q-1, r same
            {1, 0, -1}, // 4) q+1, r same
            {1, -1, 0}, // 5) q+1, r-1
            {-1, 1, 0} // 6) q-1, r+1
        };

        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            const auto &source = hex_qrs[i];
            for (int d = 0; d < ROOK_RAYS; ++d) {
                const auto &delta = directions[d];
                int current_q = source.q;
                int current_r = source.r;
                int current_s = source.s;

                int step = 0;
                while (true) {
                    current_q += delta.dq;
                    current_r += delta.dr;
                    current_s += delta.ds;

                    if (std::abs(current_q) > MAX_Q || std::abs(current_r) > MAX_R || std::abs(current_s) > MAX_S) {
                        break;
                    }

                    int target_index = -1;
                    for (int j = 0; j < HEXBOARD_SIZE; ++j) {
                        if (hex_qrs[j].q == current_q && hex_qrs[j].r == current_r) {
                            target_index = j;
                            break;
                        }
                    }

                    if (target_index != -1) {
                        rook_moves[i][d][step++] = target_index;
                    } else {
                        break;
                    }

                    //if (step > MAX_ROOK_RAY_LENGTH - 1) break;
                }
                rook_moves[i][d][step] = -1;
            }
        }
    }

    void init_bishop_moves() {
        struct Delta {
            int dq, dr, ds;
        };
        const Delta directions[BISHOP_RAYS] = {
            {-1, -1, 2}, // 1) q decreases by one, r decreases by one
            {1, 1, -2},  // 2) q increases by one, r increases by one
            {-2, 1, 1},  // 3) q decreases by two, r increases by one
            {2, -1, -1}, // 4) q increases by two, r decreases by one
            {-1, 2, -1}, // 5) q decreases by one, r increases by two
            {1, -2, 1}   // 6) q increases by one, r decreases by two
        };

        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            const auto &source = hex_qrs[i];
            for (int d = 0; d < BISHOP_RAYS; ++d) {
                const auto &delta = directions[d];
                int current_q = source.q;
                int current_r = source.r;
                int current_s = source.s;

                int step = 0;
                while (true) {
                    current_q += delta.dq;
                    current_r += delta.dr;
                    current_s += delta.ds;

                    if (std::abs(current_q) > MAX_Q || std::abs(current_r) > MAX_R || std::abs(current_s) > MAX_S) {
                        break;
                    }

                    int target_index = -1;
                    for (int j = 0; j < HEXBOARD_SIZE; ++j) {
                        if (hex_qrs[j].q == current_q && hex_qrs[j].r == current_r) {
                            target_index = j;
                            break;
                        }
                    }

                    if (target_index != -1) {
                        bishop_moves[i][d][step++] = target_index;
                    } else {
                        break;
                    }

                    //if (step > MAX_BISHOP_RAY_LENGTH - 1) break;
                }
                bishop_moves[i][d][step] = -1;
            }
        }
    }

    void init_pawn_moves() {
        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            for (int c = 0; c < PAWN_COLOR; ++c) {
                for (int t = 0; t < PAWN_MOVE_TYPE; ++t) {
                    for (int m = 0; m < MAX_PAWN_MOVE_TYPE_MOVES; ++m) {
                        pawn_moves[i][c][t][m] = -1;
                    }
                }
            }
        }

        auto get_index = [](const int q, const int r) {
            for (int i = 0; i < HEXBOARD_SIZE; ++i) {
                if (hex_qrs[i].q == q && hex_qrs[i].r == r) return i;
            }
            return -1;
        };

        const int white_starts[] = {12, 19, 27, 36, 46, 57, 67, 76, 84};
        const int black_starts[] = {6, 14, 23, 33, 44, 54, 63, 71, 78};

        auto is_starting = [](const int index, const int starts[], const int size) {
            for (int i = 0; i < size; ++i) {
                if (starts[i] == index) return true;
            }
            return false;
        };

        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            const auto &coord = hex_qrs[i];
            const int q = coord.q;
            const int r = coord.r;

            // White Pawns
            {
                // Forward
                int fwd_idx = 0;
                const int f1 = get_index(q, r - 1);
                if (f1 != -1) {
                    pawn_moves[i][Color::WhiteColor][PawnMoveType::Forward][fwd_idx++] = f1;
                    if (is_starting(i, white_starts, 9)) {
                        const int f2 = get_index(q, r - 2);
                        if (f2 != -1) {
                            pawn_moves[i][Color::WhiteColor][PawnMoveType::Forward][fwd_idx++] = f2;
                        }
                    }
                }
                // Capture
                int cap_idx = 0;
                const int c1 = get_index(q - 1, r);
                if (c1 != -1) pawn_moves[i][Color::WhiteColor][PawnMoveType::Capture][cap_idx++] = c1;
                const int c2 = get_index(q + 1, r - 1);
                if (c2 != -1) pawn_moves[i][Color::WhiteColor][PawnMoveType::Capture][cap_idx++] = c2;
            }

            // Black Pawns
            {
                // Forward
                int fwd_idx = 0;
                int f1 = get_index(q, r + 1);
                if (f1 != -1) {
                    pawn_moves[i][Color::BlackColor][PawnMoveType::Forward][fwd_idx++] = f1;
                    if (is_starting(i, black_starts, 9)) {
                        const int f2 = get_index(q, r + 2);
                        if (f2 != -1) {
                            pawn_moves[i][Color::BlackColor][PawnMoveType::Forward][fwd_idx++] = f2;
                        }
                    }
                }
                // Capture
                int cap_idx = 0;
                const int c1 = get_index(q - 1, r + 1);
                if (c1 != -1) pawn_moves[i][Color::BlackColor][PawnMoveType::Capture][cap_idx++] = c1;
                const int c2 = get_index(q + 1, r);
                if (c2 != -1) pawn_moves[i][Color::BlackColor][PawnMoveType::Capture][cap_idx++] = c2;
            }
        }

        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            for (int c = 0; c < PAWN_COLOR; ++c) {
                for (int t = 0; t < PAWN_MOVE_TYPE; ++t) {
                    std::cout << "pawn_moves[" << i << "][" << c << "][" << t << "] = {";
                    for (int m = 0; m < MAX_PAWN_MOVE_TYPE_MOVES; ++m) {
                        std::cout << pawn_moves[i][c][t][m] << (m == MAX_PAWN_MOVE_TYPE_MOVES - 1 ? "" : ", ");
                    }
                    std::cout << "}" << std::endl;
                }
            }
        }
    }

    std::string move_to_uci(const Move &move) {
        constexpr char col_labels[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'k', 'l'};

        auto index_to_uci = [&](const int idx) -> std::string {
            const CubeCoords &c = hex_qrs[idx];
            const int col_idx = c.q + 5;
            const int r_max = 5 - std::max(c.q, 0);
            const int row = r_max - c.r + 1;
            return std::string(1, col_labels[col_idx]) + std::to_string(row);
        };

        std::string res = index_to_uci(move.from) + index_to_uci(move.to);
        if (move.promotion != PieceType::Empty) {
            if (move.promotion == PieceType::Queen) res += 'q';
            else if (move.promotion == PieceType::Rook) res += 'r';
            else if (move.promotion == PieceType::Knight) res += 'n';
            else if (move.promotion == PieceType::Bishop) res += 'b';
        }
        return res;
    }

    bool is_checked(const HBoard &board, const Move &move) {
        HBoard simulated_board = board;
        const Piece moving_piece = simulated_board.board[move.from];
        simulated_board.board[move.to] = moving_piece;
        if (move.promotion != PieceType::Empty) {
            simulated_board.board[move.to].type = move.promotion;
        }
        simulated_board.board[move.from] = {PieceType::Empty, PieceSide::None};

        const PieceSide my_side = moving_piece.side;
        return is_king_in_check(simulated_board, my_side);
    }

    bool is_king_in_check(const HBoard &board, PieceSide side) {
        const PieceSide opponent_side = (side == PieceSide::White) ? PieceSide::Black : PieceSide::White;

        int king_pos = -1;
        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            if (board.board[i].type == PieceType::King && board.board[i].side == side) {
                king_pos = i;
                break;
            }
        }

        if (king_pos == -1) return false;

        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            if (board.board[i].side == opponent_side) {
                if (board.board[i].type == PieceType::King) {
                    for (int j = 0; j < MAX_KING_MOVES; ++j) {
                        if (king_moves[i][j] == king_pos) return true;
                        if (king_moves[i][j] == -1) break;
                    }
                } else if (board.board[i].type == PieceType::Knight) {
                    for (int j = 0; j < MAX_KNIGHT_MOVES; ++j) {
                        if (knight_moves[i][j] == king_pos) return true;
                        if (knight_moves[i][j] == -1) break;
                    }
                } else if (board.board[i].type == PieceType::Rook) {
                    for (int r = 0; r < ROOK_RAYS; ++r) {
                        for (int s = 0; s < MAX_ROOK_RAY_LENGTH; ++s) {
                            const int target = rook_moves[i][r][s];
                            if (target == -1) break;
                            if (target == king_pos) return true;
                            if (board.board[target].type != PieceType::Empty) break;
                        }
                    }
                } else if (board.board[i].type == PieceType::Bishop) {
                    for (int r = 0; r < BISHOP_RAYS; ++r) {
                        for (int s = 0; s < MAX_BISHOP_RAY_LENGTH; ++s) {
                            const int target = bishop_moves[i][r][s];
                            if (target == -1) break;
                            if (target == king_pos) return true;
                            if (board.board[target].type != PieceType::Empty) break;
                        }
                    }
                } else if (board.board[i].type == PieceType::Queen) {
                    for (int r = 0; r < ROOK_RAYS; ++r) {
                        for (int s = 0; s < MAX_ROOK_RAY_LENGTH; ++s) {
                            const int target = rook_moves[i][r][s];
                            if (target == -1) break;
                            if (target == king_pos) return true;
                            if (board.board[target].type != PieceType::Empty) break;
                        }
                    }
                    for (int r = 0; r < BISHOP_RAYS; ++r) {
                        for (int s = 0; s < MAX_BISHOP_RAY_LENGTH; ++s) {
                            const int target = bishop_moves[i][r][s];
                            if (target == -1) break;
                            if (target == king_pos) return true;
                            if (board.board[target].type != PieceType::Empty) break;
                        }
                    }
                }
            }
        }
        return false;
    }

    bool make_move(const Move &move) {
        // Basic validation: must be a legal move
        const auto legal_moves = get_legal_moves_at(main_board, move.from);
        bool legal = false;
        for (const auto &m: legal_moves) {
            if (m.to == move.to && m.promotion == move.promotion) {
                legal = true;
                break;
            }
        }
        if (!legal) return false;

        main_board.board[move.to] = main_board.board[move.from];
        if (move.promotion != PieceType::Empty) {
            main_board.board[move.to].type = move.promotion;
        }
        main_board.board[move.from] = {PieceType::Empty, PieceSide::None};
        main_board.turn = (main_board.turn == PieceTurn::WhiteColor) ? PieceTurn::BlackColor : PieceTurn::WhiteColor;
        return true;
    }

    std::vector<Move> get_legal_king_moves(const HBoard &board, const bool only_captures) {
        std::vector<Move> moves;
        const PieceSide side = (board.turn == PieceTurn::WhiteColor) ? PieceSide::White : PieceSide::Black;
        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            if (board.board[i].type == PieceType::King && board.board[i].side == side) {
                auto piece_moves = get_legal_king_moves_at(board, i, only_captures);
                moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_king_moves_at(const HBoard &board, const int index, const bool only_captures) {
        std::vector<Move> moves;
        const Piece piece = board.board[index];
        if (piece.type != PieceType::King) return moves;

        for (int j = 0; j < MAX_KING_MOVES; ++j) {
            const int target = king_moves[index][j];
            if (target == -1) break;

            if (board.board[target].side == piece.side || board.board[target].type == PieceType::King) continue;
            if (only_captures && board.board[target].type == PieceType::Empty) continue;

            Move move = {index, target, board.board[target]};
            if (!is_checked(board, move)) {
                moves.push_back(move);
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_knight_moves(const HBoard &board, const bool only_captures) {
        std::vector<Move> moves;
        const PieceSide side = (board.turn == PieceTurn::WhiteColor) ? PieceSide::White : PieceSide::Black;
        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            if (board.board[i].type == PieceType::Knight && board.board[i].side == side) {
                auto piece_moves = get_legal_knight_moves_at(board, i, only_captures);
                moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_knight_moves_at(const HBoard &board, const int index, const bool only_captures) {
        std::vector<Move> moves;
        const Piece piece = board.board[index];
        if (piece.type != PieceType::Knight) return moves;

        for (int j = 0; j < MAX_KNIGHT_MOVES; ++j) {
            const int target = knight_moves[index][j];
            if (target == -1) break;

            if (board.board[target].side == piece.side || board.board[target].type == PieceType::King) continue;
            if (only_captures && board.board[target].type == PieceType::Empty) continue;

            Move move = {index, target, board.board[target]};
            if (!is_checked(board, move)) {
                moves.push_back(move);
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_rook_moves(const HBoard &board, const bool only_captures) {
        std::vector<Move> moves;
        const PieceSide side = (board.turn == PieceTurn::WhiteColor) ? PieceSide::White : PieceSide::Black;
        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            if (board.board[i].type == PieceType::Rook && board.board[i].side == side) {
                auto piece_moves = get_legal_rook_moves_at(board, i, only_captures);
                moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_rook_moves_at(const HBoard &board, const int index, const bool only_captures) {
        std::vector<Move> moves;
        const Piece piece = board.board[index];
        if (piece.type != PieceType::Rook) return moves;

        for (int r = 0; r < ROOK_RAYS; ++r) {
            for (int s = 0; s < MAX_ROOK_RAY_LENGTH; ++s) {
                const int target = rook_moves[index][r][s];
                if (target == -1) break;

                const Piece &target_piece = board.board[target];
                if (target_piece.side == piece.side || target_piece.type == PieceType::King) break;

                if (!(only_captures && target_piece.type == PieceType::Empty)) {
                    Move move = {index, target, target_piece};
                    if (!is_checked(board, move)) {
                        moves.push_back(move);
                    }
                }

                if (target_piece.type != PieceType::Empty) break;
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_bishop_moves(const HBoard &board, const bool only_captures) {
        std::vector<Move> moves;
        const PieceSide side = (board.turn == PieceTurn::WhiteColor) ? PieceSide::White : PieceSide::Black;
        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            if (board.board[i].type == PieceType::Bishop && board.board[i].side == side) {
                auto piece_moves = get_legal_bishop_moves_at(board, i, only_captures);
                moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_bishop_moves_at(const HBoard &board, const int index, const bool only_captures) {
        std::vector<Move> moves;
        const Piece piece = board.board[index];
        if (piece.type != PieceType::Bishop) return moves;

        for (int r = 0; r < BISHOP_RAYS; ++r) {
            for (int s = 0; s < MAX_BISHOP_RAY_LENGTH; ++s) {
                const int target = bishop_moves[index][r][s];
                if (target == -1) break;

                const Piece &target_piece = board.board[target];
                if (target_piece.side == piece.side || target_piece.type == PieceType::King) break;

                if (!(only_captures && target_piece.type == PieceType::Empty)) {
                    Move move = {index, target, target_piece};
                    if (!is_checked(board, move)) {
                        moves.push_back(move);
                    }
                }

                if (target_piece.type != PieceType::Empty) break;
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_queen_moves(const HBoard &board, const bool only_captures) {
        std::vector<Move> moves;
        const PieceSide side = (board.turn == PieceTurn::WhiteColor) ? PieceSide::White : PieceSide::Black;
        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            if (board.board[i].type == PieceType::Queen && board.board[i].side == side) {
                auto piece_moves = get_legal_queen_moves_at(board, i, only_captures);
                moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_queen_moves_at(const HBoard &board, const int index, const bool only_captures) {
        std::vector<Move> moves;
        const Piece piece = board.board[index];
        if (piece.type != PieceType::Queen) return moves;

        // Rook-like moves
        for (int r = 0; r < ROOK_RAYS; ++r) {
            for (int s = 0; s < MAX_ROOK_RAY_LENGTH; ++s) {
                const int target = rook_moves[index][r][s];
                if (target == -1) break;

                const Piece &target_piece = board.board[target];
                if (target_piece.side == piece.side || target_piece.type == PieceType::King) break;

                if (!(only_captures && target_piece.type == PieceType::Empty)) {
                    Move move = {index, target, target_piece};
                    if (!is_checked(board, move)) {
                        moves.push_back(move);
                    }
                }

                if (target_piece.type != PieceType::Empty) break;
            }
        }

        // Bishop-like moves
        for (int r = 0; r < BISHOP_RAYS; ++r) {
            for (int s = 0; s < MAX_BISHOP_RAY_LENGTH; ++s) {
                const int target = bishop_moves[index][r][s];
                if (target == -1) break;

                const Piece &target_piece = board.board[target];
                if (target_piece.side == piece.side || target_piece.type == PieceType::King) break;

                if (!(only_captures && target_piece.type == PieceType::Empty)) {
                    Move move = {index, target, target_piece};
                    if (!is_checked(board, move)) {
                        moves.push_back(move);
                    }
                }

                if (target_piece.type != PieceType::Empty) break;
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_pawn_moves(const HBoard &board, bool only_captures) {
        std::vector<Move> moves;
        const PieceSide side = (board.turn == PieceTurn::WhiteColor) ? PieceSide::White : PieceSide::Black;
        for (int i = 0; i < HEXBOARD_SIZE; ++i) {
            if (board.board[i].type == PieceType::Pawn && board.board[i].side == side) {
                auto piece_moves = get_legal_pawn_moves_at(board, i, only_captures);
                moves.insert(moves.end(), piece_moves.begin(), piece_moves.end());
            }
        }
        return moves;
    }

    std::vector<Move> get_legal_pawn_moves_at(const HBoard &board, int index, bool only_captures) {
        std::vector<Move> moves;
        const Piece piece = board.board[index];
        if (piece.type != PieceType::Pawn) return moves;

        const PieceSide side = piece.side;
        const int color_idx = (side == PieceSide::White) ? Color::WhiteColor : Color::BlackColor;

        auto add_pawn_moves = [&](int to, Piece captured) {
            if (is_promotion_hexagon(to, side)) {
                for (PieceType promo : {PieceType::Queen, PieceType::Rook, PieceType::Knight, PieceType::Bishop}) {
                    Move m = {index, to, captured, promo};
                    if (!is_checked(board, m)) {
                        moves.push_back(m);
                    }
                }
            } else {
                Move m = {index, to, captured, PieceType::Empty};
                if (!is_checked(board, m)) {
                    moves.push_back(m);
                }
            }
        };

        // Forward
        if (!only_captures) {
            for (int m = 0; m < MAX_PAWN_MOVE_TYPE_MOVES; ++m) {
                int target = pawn_moves[index][color_idx][PawnMoveType::Forward][m];
                if (target == -1) break;

                if (board.board[target].type != PieceType::Empty) break; // Blocked

                add_pawn_moves(target, {PieceType::Empty, PieceSide::None});
            }
        }

        // Capture
        for (int m = 0; m < MAX_PAWN_MOVE_TYPE_MOVES; ++m) {
            int target = pawn_moves[index][color_idx][PawnMoveType::Capture][m];
            if (target == -1) break;

            const Piece &target_piece = board.board[target];
            if (target_piece.type != PieceType::Empty && target_piece.side != side && target_piece.type != PieceType::King) {
                add_pawn_moves(target, target_piece);
            }
        }

        return moves;
    }

    std::vector<Move> get_legal_moves_at(const HBoard &board, const int index) {
        const Piece piece = board.board[index];
        switch (piece.type) {
            case PieceType::King: return get_legal_king_moves_at(board, index);
            case PieceType::Pawn: return get_legal_pawn_moves_at(board, index);
            case PieceType::Knight: return get_legal_knight_moves_at(board, index);
            case PieceType::Rook: return get_legal_rook_moves_at(board, index);
            case PieceType::Bishop: return get_legal_bishop_moves_at(board, index);
            case PieceType::Queen: return get_legal_queen_moves_at(board, index);
            default: return {};
        }
    }

    std::vector<Move> get_all_legal_moves(const HBoard &board, const bool only_captures) {
        std::vector<Move> moves;
        auto k_moves = get_legal_king_moves(board, only_captures);
        moves.insert(moves.end(), k_moves.begin(), k_moves.end());
        auto p_moves = get_legal_pawn_moves(board, only_captures);
        moves.insert(moves.end(), p_moves.begin(), p_moves.end());
        auto kn_moves = get_legal_knight_moves(board, only_captures);
        moves.insert(moves.end(), kn_moves.begin(), kn_moves.end());
        auto r_moves = get_legal_rook_moves(board, only_captures);
        moves.insert(moves.end(), r_moves.begin(), r_moves.end());
        auto b_moves = get_legal_bishop_moves(board, only_captures);
        moves.insert(moves.end(), b_moves.begin(), b_moves.end());
        auto q_moves = get_legal_queen_moves(board, only_captures);
        moves.insert(moves.end(), q_moves.begin(), q_moves.end());

        return moves;
    }
}
