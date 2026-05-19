#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <QGraphicsScene>
#include "hexengine.h"

class HexBoard : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit HexBoard(QObject *parent = nullptr);

    struct PieceSource {
        hexengine::Piece piece;
        QString path;
    };

private:
    static const PieceSource piece_sources[];
    void drawBoard();
    void addHexagon(int index);
    static const QString &get_piece_path(hexengine::Piece piece);
};

#endif // HEXBOARD_H
