#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <QGraphicsScene>
#include "hexengine.h"

class QGraphicsSceneMouseEvent;

class HexBoard : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit HexBoard(QObject *parent = nullptr);

    struct PieceSource {
        hexengine::Piece piece;
        QString path;
    };

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QPointF m_startPos;

private:
    static const PieceSource piece_sources[];
    void drawBoard();
    void drawPieces();
    void addHexagon(int index);
    static const QString &get_piece_path(hexengine::Piece piece);
    qreal m_maxZ;
    qreal m_originalZ;
};

#endif // HEXBOARD_H
