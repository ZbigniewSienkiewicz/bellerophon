#include "hexboard.h"
#include <QGraphicsTextItem>
#include <QGraphicsSvgItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <cmath>

constexpr int COLUMN_COUNT = 11;

const HexBoard::PieceSource HexBoard::piece_sources[] = {
    {{hexengine::PieceType::King, hexengine::PieceSide::White}, ":/res/images/pieces/king_white.svg" },
    {{hexengine::PieceType::King, hexengine::PieceSide::Black}, ":/res/images/pieces/king_black.svg" },
    {{hexengine::PieceType::Knight, hexengine::PieceSide::White}, ":/res/images/pieces/knight_white.svg" },
    {{hexengine::PieceType::Knight, hexengine::PieceSide::Black}, ":/res/images/pieces/knight_black.svg" },
    {{hexengine::PieceType::Bishop, hexengine::PieceSide::White}, ":/res/images/pieces/bishop_white.svg" },
    {{hexengine::PieceType::Bishop, hexengine::PieceSide::Black}, ":/res/images/pieces/bishop_black.svg" },
    {{hexengine::PieceType::Rook, hexengine::PieceSide::White}, ":/res/images/pieces/rook_white.svg" },
    {{hexengine::PieceType::Rook, hexengine::PieceSide::Black}, ":/res/images/pieces/rook_black.svg" },
    {{hexengine::PieceType::Queen, hexengine::PieceSide::White}, ":/res/images/pieces/queen_white.svg" },
    {{hexengine::PieceType::Queen, hexengine::PieceSide::Black}, ":/res/images/pieces/queen_black.svg" },
    {{hexengine::PieceType::Pawn, hexengine::PieceSide::White}, ":/res/images/pieces/pawn_white.svg" },
    {{hexengine::PieceType::Pawn, hexengine::PieceSide::Black}, ":/res/images/pieces/pawn_black.svg" }
};

HexBoard::HexBoard(QObject *parent)
    : QGraphicsScene(parent), m_maxZ(100.0), m_originalZ(0.0) {
    hexengine::init_hexengine();
    drawBoard();
    drawPieces();
    highlightKingIfChecked();
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

    // Add row labels (left side): 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    for (int n = 1; n <= COLUMN_COUNT; ++n) {
        const int q = (n <= 6) ? -5 : (n - COLUMN_COUNT);
        const int r = 6 - n;

        const qreal x = size * 1.5 * q;
        const qreal y = size * 1.7320508075688773 * (r + q / 2.0);

        auto *label = new QGraphicsTextItem(QString::number(n));
        QFont font = label->font();
        font.setBold(true);
        font.setPointSize(12);
        label->setFont(font);

        // Center the text vertically to the left of the leftmost vertex
        QRectF rect = label->boundingRect();
        label->setPos(x - size - rect.width() + 6.0, y - rect.height() - 4.0);

        addItem(label);
    }

    // Add row labels (right side): 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11
    for (int n = 1; n <= COLUMN_COUNT; ++n) {
        const int q = (n <= 6) ? 5 : (COLUMN_COUNT - n);
        const int r = (n <= 6) ? (1 - n) : -5;

        const qreal x = size * 1.5 * q;
        const qreal y = size * 1.7320508075688773 * (r + q / 2.0);

        auto *label = new QGraphicsTextItem(QString::number(n));
        QFont font = label->font();
        font.setBold(true);
        font.setPointSize(12);
        label->setFont(font);

        // Center the text vertically to the right of the rightmost vertex
        QRectF rect = label->boundingRect();
        label->setPos(x + size - 6.0, y - rect.height() - 4.0);

        addItem(label);
    }

    setSceneRect(itemsBoundingRect());
}

void HexBoard::drawPieces() {
    const hexengine::PieceTurn turn = hexengine::get_turn();
    for (int i = 0; i < hexengine::HEXBOARD_SIZE; ++i) {
        const hexengine::Piece piece = hexengine::get_piece(i);
        if (piece.type != hexengine::Empty) {
            QString path = get_piece_path(piece);
            if (!path.isEmpty()) {
                auto *pieceItem = new QGraphicsSvgItem(path);
                pieceItem->setZValue(m_maxZ++);
                
                const bool my_turn = (piece.side == hexengine::White && turn == hexengine::WhiteTurn) ||
                                     (piece.side == hexengine::Black && turn == hexengine::BlackTurn);
                pieceItem->setFlag(QGraphicsItem::ItemIsMovable, my_turn);
                pieceItem->setData(1, (int)piece.side);

                const hexengine::HexCubeCoords &hex_coords = hexengine::get_hex_qrs(i);
                constexpr qreal size = 30.0;
                const qreal x = size * 1.5 * hex_coords.q;
                const qreal y = size * 1.7320508075688773 * (hex_coords.r + hex_coords.q / 2.0);

                QRectF pieceRect = pieceItem->boundingRect();
                pieceItem->setPos(x - pieceRect.width() / 2.0, y - pieceRect.height() / 2.0);

                addItem(pieceItem);
            }
        }
    }
}

void HexBoard::updatePieceMovableFlags() {
    const hexengine::PieceTurn turn = hexengine::get_turn();
    for (auto* item : items()) {
        if (auto* pieceItem = qgraphicsitem_cast<QGraphicsSvgItem*>(item)) {
            hexengine::PieceSide side = static_cast<hexengine::PieceSide>(pieceItem->data(1).toInt());
            const bool my_turn = (side == hexengine::White && turn == hexengine::WhiteTurn) ||
                                 (side == hexengine::Black && turn == hexengine::BlackTurn);
            pieceItem->setFlag(QGraphicsItem::ItemIsMovable, my_turn);
        }
    }
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
    hexItem->setData(0, index); // Store index for later use if needed

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

    auto *textItem = new QGraphicsTextItem(QString::number(index));
    textItem->setZValue(1); // Ensure text is above hexagons
    QFont font = textItem->font();
    font.setPointSize(10);
    //font.setBold(true);
    textItem->setFont(font);
    textItem->setDefaultTextColor(Qt::black);

    // Center the text
    QRectF textRect = textItem->boundingRect();
    textItem->setPos(x - textRect.width() / 2.0, y - textRect.height() / 2.0);

    addItem(textItem);
}

const QString & HexBoard::get_piece_path(const hexengine::Piece piece) {
    static const QString empty = "";
    for (const auto & source : piece_sources) {
        if (source.piece.type == piece.type && source.piece.side == piece.side) {
            return source.path;
        }
    }
    return empty;
}

void HexBoard::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    clearHighlights();
    m_pressedItem = nullptr;
    m_startIndex = -1;

    QList<QGraphicsItem *> itemsAtPos = items(event->scenePos());
    QGraphicsSvgItem *pieceItem = nullptr;
    for (auto *i : itemsAtPos) {
        if (auto *p = qgraphicsitem_cast<QGraphicsSvgItem *>(i)) {
            pieceItem = p;
            break;
        }
    }

    if (pieceItem) {
        int index = -1;
        for (auto *i : itemsAtPos) {
            if (const auto *poly = qgraphicsitem_cast<QGraphicsPolygonItem *>(i)) {
                index = poly->data(0).toInt();
                break;
            }
        }

        if (index != -1) {
            highlightMoves(index);
            m_pressedItem = pieceItem;
            m_originalZ = pieceItem->zValue();
            pieceItem->setZValue(m_maxZ + 10.0);

            const hexengine::Piece piece = hexengine::get_piece(index);
            const hexengine::PieceTurn turn = hexengine::get_turn();
            const bool my_turn = (piece.side == hexengine::White && turn == hexengine::WhiteTurn) ||
                                 (piece.side == hexengine::Black && turn == hexengine::BlackTurn);

            if (my_turn) {
                m_startIndex = index;
                m_startPos = pieceItem->pos();
            }
        }
    }

    QGraphicsScene::mousePressEvent(event);
}

void HexBoard::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    clearHighlights();
    bool moved = false;
    if (m_pressedItem) {
        if (m_pressedItem->flags() & QGraphicsItem::ItemIsMovable) {
            QList<QGraphicsItem *> itemsAtPos = items(event->scenePos());
            const QGraphicsPolygonItem *hexItem = nullptr;
            for (auto *i : itemsAtPos) {
                if (const auto *poly = qgraphicsitem_cast<QGraphicsPolygonItem *>(i)) {
                    hexItem = poly;
                    break;
                }
            }

            if (hexItem && m_startIndex != -1) {
                const int targetIndex = hexItem->data(0).toInt();
                hexengine::Move move = {m_startIndex, targetIndex, hexengine::get_piece(targetIndex)};

                if (hexengine::make_move(move)) {
                    if (move.captured_piece.type != hexengine::Empty) {
                        const hexengine::HexCubeCoords &target_coords = hexengine::get_hex_qrs(targetIndex);
                        constexpr qreal size = 30.0;
                        const qreal tx = size * 1.5 * target_coords.q;
                        const qreal ty = size * 1.7320508075688773 * (target_coords.r + target_coords.q / 2.0);

                        QList<QGraphicsItem*> itemsAtTargetPos = items(QPointF(tx, ty));
                        for (auto* targetItem : itemsAtTargetPos) {
                            if (targetItem != m_pressedItem && qgraphicsitem_cast<QGraphicsSvgItem*>(targetItem)) {
                                removeItem(targetItem);
                                delete targetItem;
                            }
                        }
                    }

                    const hexengine::HexCubeCoords &hex_coords = hexengine::get_hex_qrs(targetIndex);
                    constexpr qreal size = 30.0;
                    const qreal x = size * 1.5 * hex_coords.q;
                    const qreal y = size * 1.7320508075688773 * (hex_coords.r + hex_coords.q / 2.0);

                    const QRectF pieceRect = m_pressedItem->boundingRect();
                    m_pressedItem->setPos(x - pieceRect.width() / 2.0, y - pieceRect.height() / 2.0);
                    moved = true;
                    m_pressedItem->setZValue(m_maxZ++);
                    updatePieceMovableFlags();
                    highlightKingIfChecked();
                }
            }

            if (!moved) {
                m_pressedItem->setPos(m_startPos);
                m_pressedItem->setZValue(m_originalZ);
            }
        } else {
            m_pressedItem->setZValue(m_originalZ);
        }
    }
    m_startIndex = -1;
    m_pressedItem = nullptr;
    QGraphicsScene::mouseReleaseEvent(event);
}

void HexBoard::highlightMoves(int index) {
    const auto moves = hexengine::get_legal_moves_at(hexengine::get_main_board(), index);
    for (const auto& move : moves) {
        const hexengine::HexCubeCoords &coords = hexengine::get_hex_qrs(move.to);
        constexpr qreal size = 30.0;
        const qreal x = size * 1.5 * coords.q;
        const qreal y = size * 1.7320508075688773 * (coords.r + coords.q / 2.0);

        constexpr qreal circleSize = 15.0;
        auto* circle = new QGraphicsEllipseItem(x - circleSize/2, y - circleSize/2, circleSize, circleSize);
        if (move.captured_piece.type != hexengine::Empty) {
            circle->setBrush(QBrush(Qt::red));
        } else {
            circle->setBrush(QBrush(Qt::green));
        }
        circle->setPen(Qt::NoPen);
        circle->setZValue(m_maxZ + 5.0);
        addItem(circle);
        m_highlights.append(circle);
    }
}

void HexBoard::clearHighlights() {
    for (auto* item : m_highlights) {
        removeItem(item);
        delete item;
    }
    m_highlights.clear();
}

void HexBoard::highlightKingIfChecked() {
    auto updateKingHighlight = [&](hexengine::PieceSide side, QGraphicsEllipseItem*& highlightItem) {
        bool checked = hexengine::is_king_in_check(hexengine::get_main_board(), side);
        if (checked) {
            int king_idx = -1;
            const auto& board = hexengine::get_main_board();
            for (int i=0; i<hexengine::HEXBOARD_SIZE; ++i) {
                if (board.board[i].type == hexengine::King && board.board[i].side == side) {
                    king_idx = i;
                    break;
                }
            }
            if (king_idx != -1) {
                const hexengine::HexCubeCoords &coords = hexengine::get_hex_qrs(king_idx);
                constexpr qreal size = 30.0;
                const qreal x = size * 1.5 * coords.q;
                const qreal y = size * 1.7320508075688773 * (coords.r + coords.q / 2.0);

                qreal circleSize = 50.0;
                if (!highlightItem) {
                    highlightItem = new QGraphicsEllipseItem();
                    addItem(highlightItem);
                }
                highlightItem->setRect(x - circleSize/2, y - circleSize/2, circleSize, circleSize);
                highlightItem->setBrush(Qt::NoBrush);
                highlightItem->setPen(QPen(Qt::red, 3));
                highlightItem->setZValue(m_maxZ + 2);
                highlightItem->show();
            }
        } else {
            if (highlightItem) {
                highlightItem->hide();
            }
        }
    };

    updateKingHighlight(hexengine::White, m_whiteKingCheckHighlight);
    updateKingHighlight(hexengine::Black, m_blackKingCheckHighlight);
}
