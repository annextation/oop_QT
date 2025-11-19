#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "headers.h"
#include "ISS.h"
#include "AstronautEditDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class DrawingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DrawingWidget(ISS *iss, QWidget *parent = nullptr);
    void updateWidgetSize();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    ISS *iss;
    void drawAstronautTable(QPainter &painter);
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_saveButton_clicked();
    void on_loadButton_clicked();
    void on_clearButton_clicked();
    void on_editButton_clicked();

private:
    Ui::MainWindow *ui;
    ISS iss;

    QWidget *centralWidget;
    DrawingWidget *drawingWidget;
    QScrollArea *scrollArea;

    QPushButton *saveButton;
    QPushButton *loadButton;
    QPushButton *clearButton;
    QPushButton *editButton;

    void updateDisplay();
    void setupUI();
};

#endif // MAINWINDOW_H
