#include "hexboard.h"
#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <cmath>

HexBoard::HexBoard(QObject *parent)
    : QGraphicsScene(parent) {
    drawBoard();
}

void HexBoard::drawBoard() {
    for (int i = 0; i < hexengine::HEXBOARD_SIZE; ++i) {
        addHexagon(i);
    }

    // Add column labels: a, b, c, d, e, f, g, h, i, k, l (skipping j)
    const char *labels[] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "k", "l"};
    const qreal size = 30.0;
    for (int q = -5; q <= 5; ++q) {
        const int r_end = (q <= 0) ? 5 : (5 - q);
        const qreal x = size * 1.5 * q;
        const qreal y = size * 1.7320508075688773 * (r_end + q / 2.0);

        auto *label = new QGraphicsTextItem(labels[q + 5]);
        QFont font = label->font();
        font.setBold(true);
        font.setPointSize(12);
        label->setFont(font);

        // Center the text horizontally under the bottom hexagon
        QRectF rect = label->boundingRect();
        label->setPos(x - rect.width() / 2.0, y + size * 0.866);

        addItem(label);
    }

    setSceneRect(itemsBoundingRect());
}

void HexBoard::addHexagon(int index) {
    const hexengine::HexCubeCoords &hex_coords = hexengine::get_hex_qrs(index);
    qreal size = 30.0;

    // Flat-topped hex coordinates
    qreal x = size * 1.5 * hex_coords.q;
    qreal y = size * 1.7320508075688773 * (hex_coords.r + hex_coords.q / 2.0); // hardcoded sqrt(3.0)

    QPolygonF hexagon;
    for (int i = 0; i < 6; ++i) {
        qreal angle_deg = 60.0 * i;
        qreal angle_rad = 3.14159265358979323846 / 180.0 * angle_deg; // hardcoded pi
        hexagon << QPointF(x + size * std::cos(angle_rad),
                           y + size * std::sin(angle_rad));
    }

    auto *hexItem = new QGraphicsPolygonItem(hexagon);

    // Determine color based on q, r coordinates
    int colorIndex = (hex_coords.q - hex_coords.r + 10) % 3;

    QColor color;
    switch (colorIndex) {
        case 0: color = QColor(188, 125, 64); // dark color
            break;
        case 1: color = QColor(232, 171, 111); // middle color
            break;
        case 2: color = QColor(255, 227, 174); // light color
            break;
        default: ;
    }

    hexItem->setBrush(QBrush(color));
    hexItem->setPen(QPen(Qt::black)); // hexagon border

    addItem(hexItem);
}
