#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace placeholders;

DrawingWidget::DrawingWidget(QWidget *parent)
    : QWidget(parent)
    , iss(nullptr)
{
    setMinimumSize(1150, 600);
}

DrawingWidget::DrawingWidget(ISS *iss, QWidget *parent)
    : QWidget(parent)
    , iss(iss)
{
    setMinimumSize(1150, 600);
}

void DrawingWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    if (!iss) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawAstronautTable(painter);
}

void DrawingWidget::drawAstronautTable(QPainter &painter)
{
    painter.fillRect(rect(), Qt::white);

    QFont titleFont = painter.font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.setPen(Qt::black);

    painter.drawText(220, 50, "МКС - УПРАВЛЕНИЕ АСТРОНАВТАМИ");

    if (iss) {
        painter.drawText(202, 82, QString("Количество загруженных астронавтов: %1").arg(iss->getAstronautsCount()));
    } else {
        painter.drawText(202, 82, "Количество загруженных астронавтов: 0");
    }

    if (!iss || iss->getAstronautsCount() == 0) {
        painter.drawText(270, 120, "Астронавтов на МКС нет.");
        return;
    }

    QFont tableFont = painter.font();
    tableFont.setPointSize(10);
    tableFont.setBold(true);
    painter.setFont(tableFont);

    int yPos = 120;

    painter.drawText(50, yPos, "Тип");
    painter.drawText(120, yPos, "Имя");
    painter.drawText(270, yPos, "Страна");
    painter.drawText(420, yPos, "Полёты");
    painter.drawText(520, yPos, "Дни");
    painter.drawText(620, yPos, "Специализация");
    painter.drawText(820, yPos, "Статус");
    painter.drawText(920, yPos, "Лицензия");
    painter.drawText(1020, yPos, "Практика");

    yPos += 30;
    painter.drawLine(50, yPos, 1100, yPos);
    yPos += 30;

    tableFont.setBold(false);
    painter.setFont(tableFont);

    int currentYPos = yPos;
    int rowHeight = 25;

    auto drawLambda = [&](const std::shared_ptr<Astronaut>& astronaut) {
        astronaut->drawInPainter(painter, currentYPos, rowHeight);
    };

    const auto& astronauts = iss->getAstronauts();
    std::for_each(astronauts.begin(), astronauts.end(), drawLambda);

    int totalHeight = currentYPos + 20;
    setMinimumHeight(totalHeight);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setlocale(LC_ALL, "rus");
    setWindowTitle("МКС - Управление астронавтами");

    ui->drawingWidget->iss = &iss;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDisplay()
{
    ui->drawingWidget->update();
}

void MainWindow::on_saveButton_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Сохранить данные МКС",
                                                    "",
                                                    "Binary Files (*.bin)");
    if (!filename.isEmpty()) {
        iss.save_in_file(filename.toStdWString());
        QMessageBox::information(this, "Успех", "Данные сохранены в файл");
    }
}

void MainWindow::on_loadButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Загрузить данные МКС",
                                                    "",
                                                    "Binary Files (*.bin)");
    if (!filename.isEmpty()) {
        iss.load_from_file(filename.toStdWString());
        updateDisplay();
        QMessageBox::information(this, "Успех", "Данные загружены из файла");
    }
}

void MainWindow::on_clearButton_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                                  "Подтверждение",
                                  "Вы уверены, что хотите удалить всех астронавтов?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        iss.delete_astronauts();
        updateDisplay();
        QMessageBox::information(this, "Успех", "Все астронавты удалены");
    }
}

void MainWindow::on_editButton_clicked()
{
    AstronautEditDialog dialog(&iss, this);
    if (dialog.exec() == QDialog::Accepted) {
        updateDisplay();
        QMessageBox::information(this, "Успех", "Изменения применены");
    }
}
