#include "mainwindow.h"

#include "hexengine.h"
#include "hexboard.h"
#include "ui_mainwindow.h"
#include <QResizeEvent>
#include <QShowEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *board = new HexBoard(this);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setScene(board);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setFrameShape(QFrame::NoFrame);

    // Initial fit of the window to the board
    ui->graphicsView->setMinimumSize(board->sceneRect().size().toSize());
    adjustSize();
    ui->graphicsView->setMinimumSize(0, 0);

    connect(ui->actionRandom_vs_random, &QAction::triggered, board, &HexBoard::random_vs_random_game);
    connect(ui->actionSet_startup_position, &QAction::triggered, board, &HexBoard::setup_startup_position);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (ui->graphicsView->scene()) {
        ui->graphicsView->fitInView(ui->graphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
    }
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    if (ui->graphicsView->scene()) {
        ui->graphicsView->fitInView(ui->graphicsView->scene()->sceneRect(), Qt::KeepAspectRatio);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
