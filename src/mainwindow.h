#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QResizeEvent;
class QShowEvent;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
