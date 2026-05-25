#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <QGraphicsScene>
#include <QKeyEvent>
#include "hexengine.h"

class QGraphicsSceneMouseEvent;

class HexBoard : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit HexBoard(QObject *parent = nullptr);
    void random_vs_random_game();
    void setup_startup_position();

    struct PieceSource {
        hexengine::Piece piece;
        QString path;
    };

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
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
    void performNextRandomMove();
    hexengine::Move showPromotionMenu(const std::vector<hexengine::Move>& promotionMoves, QPoint screenPos);
    qreal m_maxZ;
    qreal m_originalZ;
    bool m_stopRandomGame = false;
    bool m_randomGameActive = false;
    QGraphicsItem* m_pressedItem = nullptr;
    QList<QGraphicsEllipseItem*> m_highlights;
    QGraphicsEllipseItem* m_whiteKingCheckHighlight = nullptr;
    QGraphicsEllipseItem* m_blackKingCheckHighlight = nullptr;
};

#endif // HEXBOARD_H
