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
    int m_startIndex = -1;

private:
    static const PieceSource piece_sources[];
    void drawBoard();
    void drawPieces();
    void redrawPieces();
    void addHexagon(int index);
    static const QString &get_piece_path(hexengine::Piece piece);
    void placePieceAt(QGraphicsItem* item, int index);
    void highlightMoves(int index);
    void clearHighlights();
    void highlightKingIfChecked();
    hexengine::Move showPromotionMenu(const std::vector<hexengine::Move>& promotionMoves, QPoint screenPos);
    qreal m_maxZ;
    qreal m_originalZ;
    QGraphicsItem* m_pressedItem = nullptr;
    QList<QGraphicsEllipseItem*> m_highlights;
    QGraphicsEllipseItem* m_whiteKingCheckHighlight = nullptr;
    QGraphicsEllipseItem* m_blackKingCheckHighlight = nullptr;
};

#endif // HEXBOARD_H
