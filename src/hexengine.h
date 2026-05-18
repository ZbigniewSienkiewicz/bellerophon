//
// Created by zbyszek on 17.05.2026.
//

#ifndef GLAUCUS_HEXENGINE_H
#define GLAUCUS_HEXENGINE_H

namespace hexengine {
    constexpr int HEXBOARD_SIZE = 91;

    enum PieceType { Empty, King, Pawn, Knight, Bishop, Rook, Queen };

    enum PieceTurn { WhiteTurn, BlackTurn };

    enum PieceSide { None, WhiteSide, BlackSide };

    struct Piece {
        PieceType type;
        PieceSide side;
    };

    struct HexCubeCoords {
        int q;
        int r;
        int s;
    };

    using Board = Piece[HEXBOARD_SIZE];

    struct HBoard {
        Board board;
        PieceTurn turn;
    };

    /**
     * Initializes the `hex_qrs` table with coordinates for a hexagonal grid.
     *
     */
    void init_hex_qrs_table();

    const HexCubeCoords &get_hex_qrs(int index);

    const Piece &get_piece(int index);

    void set_piece(int index, Piece piece);

    PieceTurn get_turn();

    void set_turn(PieceTurn turn);
}
#endif //GLAUCUS_HEXENGINE_H
