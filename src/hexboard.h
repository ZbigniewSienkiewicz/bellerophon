#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <QGraphicsScene>
#include "hexengine.h"

class HexBoard : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit HexBoard(QObject *parent = nullptr);

private:
    void drawBoard();
    void addHexagon(int index);
};

#endif // HEXBOARD_H
