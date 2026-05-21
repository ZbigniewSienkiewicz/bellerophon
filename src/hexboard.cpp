#include "hexboard.h"
//#include <QGraphicsTextItem>
#include <QGraphicsSvgItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QFont>
#include <QBrush>
#include <QPen>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <cmath>

constexpr int COLUMN_COUNT = 11;

#define PRINT_INDEX
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
    constexpr qreal size = 30.0;
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
    const hexengine::PieceTurn turn = hexengine::get_turn(hexengine::get_main_board());
    for (int i = 0; i < hexengine::HEXBOARD_SIZE; ++i) {
        const hexengine::Piece piece = hexengine::get_piece(hexengine::get_main_board(), i);
        if (piece.type != hexengine::Empty) {
            QString path = get_piece_path(piece);
            if (!path.isEmpty()) {
                auto *pieceItem = new QGraphicsSvgItem(path);
                pieceItem->setData(1, (int)piece.side);

                const bool my_turn = (piece.side == hexengine::White && turn == hexengine::WhiteColor) ||
                                     (piece.side == hexengine::Black && turn == hexengine::BlackColor);
                pieceItem->setFlag(QGraphicsItem::ItemIsMovable, my_turn);

                addItem(pieceItem);
                placePieceAt(pieceItem, i);
            }
        }
    }
}

void HexBoard::redrawPieces() {
    QList<QGraphicsItem*> toRemove;
    for (auto* item : items()) {
        if (qgraphicsitem_cast<QGraphicsSvgItem*>(item)) {
            toRemove.append(item);
        }
    }
    for (auto* item : toRemove) {
        removeItem(item);
        delete item;
    }
    m_maxZ = 100.0;
    drawPieces();
}


void HexBoard::addHexagon(int index) {
    const hexengine::CubeCoords &hex_coords = hexengine::get_hex_qrs(index);
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

#ifdef PRINT_INDEX
    auto *textItem = new QGraphicsTextItem(QString::number(index));
#else
    auto *textItem = new QGraphicsTextItem(QString("%1,%2,%3").arg(hex_coords.q).arg(hex_coords.r).arg(hex_coords.s));
#endif
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

            const hexengine::Piece piece = hexengine::get_piece(hexengine::get_main_board(), index);
            const hexengine::PieceTurn turn = hexengine::get_turn(hexengine::get_main_board());
            const bool my_turn = (piece.side == hexengine::White && turn == hexengine::WhiteColor) ||
                                 (piece.side == hexengine::Black && turn == hexengine::BlackColor);

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

                auto legalMoves = hexengine::get_legal_moves_at(hexengine::get_main_board(), m_startIndex);
                std::vector<hexengine::Move> promotionMoves;
                for (const auto& m : legalMoves) {
                    if (m.to == targetIndex && m.promotion != hexengine::PieceType::Empty) {
                        promotionMoves.push_back(m);
                    }
                }

                hexengine::Move move{};
                bool isPromotion = !promotionMoves.empty();
                if (isPromotion) {
                    move = showPromotionMenu(promotionMoves, event->screenPos());
                    if (move.from == -1) { // Cancelled
                        m_pressedItem->setPos(m_startPos);
                        m_pressedItem->setZValue(m_originalZ);
                        m_startIndex = -1;
                        m_pressedItem = nullptr;
                        QGraphicsScene::mouseReleaseEvent(event);
                        return;
                    }
                } else {
                    move = {m_startIndex, targetIndex,
                            hexengine::get_piece(hexengine::get_main_board(), targetIndex),
                            hexengine::PieceType::Empty};
                }

                if (hexengine::make_move(move)) {
                    moved = true;
                }
            }

            if (!moved && m_pressedItem) {
                m_pressedItem->setPos(m_startPos);
                m_pressedItem->setZValue(m_originalZ);
            }
        } else if (m_pressedItem) {
            m_pressedItem->setZValue(m_originalZ);
        }
    }
    m_startIndex = -1;
    m_pressedItem = nullptr;
    QGraphicsScene::mouseReleaseEvent(event);

    if (moved) {
        redrawPieces();
        highlightKingIfChecked();
    }
}

hexengine::Move HexBoard::showPromotionMenu(const std::vector<hexengine::Move>& promotionMoves, QPoint screenPos) {
    if (promotionMoves.empty()) return {-1, -1, {hexengine::PieceType::Empty, hexengine::PieceSide::None}, hexengine::PieceType::Empty};

    QMenu menu;
    hexengine::PieceSide side = hexengine::get_piece(hexengine::get_main_board(), promotionMoves[0].from).side;

    for (const auto& move : promotionMoves) {
        hexengine::Piece piece = {move.promotion, side};
        QString path = get_piece_path(piece);
        QIcon icon(path);
        QString text;
        switch (move.promotion) {
            case hexengine::PieceType::Queen: text = "Queen"; break;
            case hexengine::PieceType::Rook: text = "Rook"; break;
            case hexengine::PieceType::Bishop: text = "Bishop"; break;
            case hexengine::PieceType::Knight: text = "Knight"; break;
            default: text = "Unknown";
        }
        QAction* action = menu.addAction(icon, text);
        action->setData(static_cast<int>(move.promotion));
    }

    QAction* selectedAction = menu.exec(screenPos);
    if (selectedAction) {
        hexengine::PieceType selectedType = static_cast<hexengine::PieceType>(selectedAction->data().toInt());
        for (const auto& move : promotionMoves) {
            if (move.promotion == selectedType) return move;
        }
    }

    return {-1, -1, {hexengine::PieceType::Empty, hexengine::PieceSide::None}, hexengine::PieceType::Empty};
}

void HexBoard::placePieceAt(QGraphicsItem* item, int index) {
    if (!item) return;
    const hexengine::CubeCoords &hex_coords = hexengine::get_hex_qrs(index);
    constexpr qreal size = 30.0;
    const qreal x = size * 1.5 * hex_coords.q;
    const qreal y = size * 1.7320508075688773 * (hex_coords.r + hex_coords.q / 2.0);

    QRectF pieceRect = item->boundingRect();
    if (pieceRect.width() <= 0 || pieceRect.height() <= 0) {
        // Fallback for QGraphicsSvgItem if its bounding rect is not yet valid
        pieceRect = QRectF(0, 0, 45, 45);
    }

    item->setPos(x - pieceRect.width() / 2.0, y - pieceRect.height() / 2.0);
    item->setZValue(m_maxZ++);
}

void HexBoard::highlightMoves(const int index) {
    const auto moves = hexengine::get_legal_moves_at(hexengine::get_main_board(), index);
    for (const auto& move : moves) {
        const hexengine::CubeCoords &coords = hexengine::get_hex_qrs(move.to);
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
        const bool checked = hexengine::is_king_in_check(hexengine::get_main_board(), side);
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
                const hexengine::CubeCoords &coords = hexengine::get_hex_qrs(king_idx);
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
