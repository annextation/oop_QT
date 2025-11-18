#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;
using namespace placeholders;

DrawingWidget::DrawingWidget(ISS *iss, QWidget *parent)
    : QWidget(parent)
    , iss(iss)
{
    setMinimumSize(1150, 600);
}

void DrawingWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawAstronautTable(painter);

}


void DrawingWidget::drawAstronautTable(QPainter &painter)
{
    painter.fillRect(rect(), Qt::black);

    QFont titleFont = painter.font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.setPen(Qt::white);

    painter.drawText(220, 50, "МКС - УПРАВЛЕНИЕ АСТРОНАВТАМИ");
    painter.drawText(202, 82, QString("Количество загруженных астронавтов: %1").arg(iss->getAstronautsCount()));

    if (iss->getAstronautsCount() == 0) {
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

    auto drawFunc = std::bind(&Astronaut::drawInPainter,
                              std::placeholders::_1,
                              std::ref(painter),
                              std::ref(currentYPos),
                              rowHeight);

    const auto& astronauts = iss->getAstronauts();
    std::for_each(astronauts.begin(), astronauts.end(), drawFunc);

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

    setupUI();
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QGroupBox *controlGroup = new QGroupBox("Управление МКС", centralWidget);
    QHBoxLayout *buttonLayout = new QHBoxLayout(controlGroup);

    saveButton = new QPushButton("Сохранить", controlGroup);
    loadButton = new QPushButton("Загрузить", controlGroup);
    clearButton = new QPushButton("Очистить", controlGroup);

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(clearButton);

    scrollArea = new QScrollArea(centralWidget);
    drawingWidget = new DrawingWidget(&iss, scrollArea);
    scrollArea->setWidget(drawingWidget);
    scrollArea->setWidgetResizable(true);

    mainLayout->addWidget(controlGroup);
    mainLayout->addWidget(scrollArea);

    connect(saveButton, &QPushButton::clicked, this, &MainWindow::on_saveButton_clicked);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::on_loadButton_clicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::on_clearButton_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateDisplay()
{
    drawingWidget->update();
    qDebug() << "Дисплей обновлен, кол-во астронавтов:" << iss.getAstronautsCount();
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
