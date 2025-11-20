#include "AstronautEditDialog.h"
#include "ui_AstronautEditDialog.h"

AstronautEditDialog::AstronautEditDialog(ISS* iss, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::AstronautEditDialog),
    iss(iss)
{
    ui->setupUi(this);
    setupConnections();
    updateAstronautList();

    ui->spaceflightsEdit->setValidator(new QIntValidator(0, 99, this));
    ui->totalDaysEdit->setValidator(new QIntValidator(0, 99, this));
    ui->practiceYearsEdit->setValidator(new QIntValidator(0, 70, this));

    showDoctorFields(false);
}

AstronautEditDialog::~AstronautEditDialog()
{
    delete ui;
}

void AstronautEditDialog::setupConnections()
{
    connect(ui->astronautList, &QListWidget::currentRowChanged,
            this, &AstronautEditDialog::onAstronautSelectionChanged);
    connect(ui->addRegularAstronautButton, &QPushButton::clicked,
            this, &AstronautEditDialog::onAddRegularAstronautClicked);
    connect(ui->addDoctorAstronautButton, &QPushButton::clicked,
            this, &AstronautEditDialog::onAddDoctorAstronautClicked);
    connect(ui->deleteButton, &QPushButton::clicked,
            this, &AstronautEditDialog::onDeleteButtonClicked);
    connect(ui->closeButton, &QPushButton::clicked,
            this, &AstronautEditDialog::onCloseButtonClicked);

    connect(ui->regularAstronautRadio, &QRadioButton::clicked,
            this, &AstronautEditDialog::onAstronautTypeChanged);
    connect(ui->doctorAstronautRadio, &QRadioButton::clicked,
            this, &AstronautEditDialog::onAstronautTypeChanged);

    connect(ui->nameEdit, &QLineEdit::editingFinished,
            this, &AstronautEditDialog::onInputFieldChanged);
    connect(ui->countryEdit, &QLineEdit::editingFinished,
            this, &AstronautEditDialog::onInputFieldChanged);
    connect(ui->specializationEdit, &QLineEdit::editingFinished,
            this, &AstronautEditDialog::onInputFieldChanged);
    connect(ui->spaceflightsEdit, &QLineEdit::editingFinished,
            this, &AstronautEditDialog::onInputFieldChanged);
    connect(ui->totalDaysEdit, &QLineEdit::editingFinished,
            this, &AstronautEditDialog::onInputFieldChanged);
    connect(ui->medicalLicenseEdit, &QLineEdit::editingFinished,
            this, &AstronautEditDialog::onInputFieldChanged);
    connect(ui->practiceYearsEdit, &QLineEdit::editingFinished,
            this, &AstronautEditDialog::onInputFieldChanged);
}

void AstronautEditDialog::updateAstronautList()
{
    ui->astronautList->clear();
    const auto& astronauts = iss->getAstronauts();

    size_t index = 0;
    auto updateLambda = [&](const std::shared_ptr<Astronaut>& astronaut) {
        QString itemText = QString("%1. %2 [%3]")
        .arg(index + 1)
            .arg(QString::fromStdWString(astronaut->get_name()))
            .arg(std::dynamic_pointer_cast<DoctorAstronaut>(astronaut) ?
                     "Астронавт-врач" : "Обычный");

        ui->astronautList->addItem(itemText);
        index++;
    };

    std::for_each(astronauts.begin(), astronauts.end(), updateLambda);
}

void AstronautEditDialog::clearInputFields()
{
    ui->nameEdit->clear();
    ui->countryEdit->clear();
    ui->specializationEdit->clear();
    ui->spaceflightsEdit->clear();
    ui->totalDaysEdit->clear();
    ui->medicalLicenseEdit->clear();
    ui->practiceYearsEdit->clear();
}

void AstronautEditDialog::showDoctorFields(bool show)
{
    ui->medicalLicenseLabel->setVisible(show);
    ui->medicalLicenseEdit->setVisible(show);
    ui->practiceYearsLabel->setVisible(show);
    ui->practiceYearsEdit->setVisible(show);
}

void AstronautEditDialog::fillInputFields(const std::shared_ptr<Astronaut>& astronaut)
{
    if (!astronaut) return;

    // Получаем базовые данные через публичные методы
    std::wstring name, country, specialization;
    int spaceflights, totalDays;
    bool status;

    astronaut->getUIFields(name, country, spaceflights, totalDays, specialization, status);

    // Заполняем UI
    ui->nameEdit->setText(QString::fromStdWString(name));
    ui->countryEdit->setText(QString::fromStdWString(country));
    ui->specializationEdit->setText(QString::fromStdWString(specialization));
    ui->spaceflightsEdit->setText(QString::number(spaceflights));
    ui->totalDaysEdit->setText(QString::number(totalDays));

    // Настраиваем тип астронавта и медицинские поля
    if (auto doctor = std::dynamic_pointer_cast<DoctorAstronaut>(astronaut)) {
        ui->doctorAstronautRadio->setChecked(true);
        showDoctorFields(true);

        std::wstring license;
        int practiceYears;
        doctor->getMedicalData(license, practiceYears);

        ui->medicalLicenseEdit->setText(QString::fromStdWString(license));
        ui->practiceYearsEdit->setText(QString::number(practiceYears));
    } else {
        ui->regularAstronautRadio->setChecked(true);
        showDoctorFields(false);
    }
}

std::shared_ptr<Astronaut> AstronautEditDialog::createAstronautFromInput()
{
    // Получаем общие данные
    std::wstring name = ui->nameEdit->text().toStdWString();
    std::wstring country = ui->countryEdit->text().toStdWString();
    std::wstring specialization = ui->specializationEdit->text().toStdWString();
    int spaceflights = ui->spaceflightsEdit->text().isEmpty() ? 0 : ui->spaceflightsEdit->text().toInt();
    int totalDays = ui->totalDaysEdit->text().isEmpty() ? 0 : ui->totalDaysEdit->text().toInt();
    bool status = true;

    // Создаем астронавта нужного типа
    std::shared_ptr<Astronaut> astronaut;
    if (ui->doctorAstronautRadio->isChecked()) {
        astronaut = std::make_shared<DoctorAstronaut>()->createFromUIFields(
            name, country, spaceflights, totalDays, specialization, status);

        // Устанавливаем медицинские данные
        std::wstring license = ui->medicalLicenseEdit->text().toStdWString();
        int practiceYears = ui->practiceYearsEdit->text().isEmpty() ? 0 : ui->practiceYearsEdit->text().toInt();

        if (auto doctor = std::dynamic_pointer_cast<DoctorAstronaut>(astronaut)) {
            doctor->setMedicalData(license, practiceYears);
        }
    } else {
        astronaut = std::make_shared<Astronaut>()->createFromUIFields(
            name, country, spaceflights, totalDays, specialization, status);
    }

    return astronaut;
}

void AstronautEditDialog::updateSelectedAstronaut()
{
    int currentRow = ui->astronautList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(iss->getAstronautsCount())) {
        auto& astronauts = const_cast<std::vector<std::shared_ptr<Astronaut>>&>(iss->getAstronauts());
        auto astronaut = astronauts[currentRow];

        std::wstring name = ui->nameEdit->text().toStdWString();
        std::wstring country = ui->countryEdit->text().toStdWString();
        std::wstring specialization = ui->specializationEdit->text().toStdWString();
        int spaceflights = ui->spaceflightsEdit->text().isEmpty() ? 0 : ui->spaceflightsEdit->text().toInt();
        int totalDays = ui->totalDaysEdit->text().isEmpty() ? 0 : ui->totalDaysEdit->text().toInt();
        bool status = true;

        astronaut->updateFromUIFields(name, country, spaceflights, totalDays, specialization, status);

        if (auto doctor = std::dynamic_pointer_cast<DoctorAstronaut>(astronaut)) {
            std::wstring license = ui->medicalLicenseEdit->text().toStdWString();
            int practiceYears = ui->practiceYearsEdit->text().isEmpty() ? 0 : ui->practiceYearsEdit->text().toInt();
            doctor->setMedicalData(license, practiceYears);
        }

        updateAstronautList();
        ui->astronautList->setCurrentRow(currentRow);
    }
}

void AstronautEditDialog::onAddRegularAstronautClicked()
{
    auto newAstronaut = std::make_shared<Astronaut>()->createFromUIFields(
        L"Новый астронавт", L"", 0, 0, L"", true);
    iss->addAstronaut(newAstronaut);

    updateAstronautList();
    int newIndex = ui->astronautList->count() - 1;
    ui->astronautList->setCurrentRow(newIndex);

    clearInputFields();
    ui->regularAstronautRadio->setChecked(true);
    showDoctorFields(false);
    ui->nameEdit->setFocus();
}

void AstronautEditDialog::onAddDoctorAstronautClicked()
{
    auto newAstronaut = std::make_shared<DoctorAstronaut>()->createFromUIFields(
        L"Новый астронавт-врач", L"", 0, 0, L"", true);
    iss->addAstronaut(newAstronaut);

    updateAstronautList();
    int newIndex = ui->astronautList->count() - 1;
    ui->astronautList->setCurrentRow(newIndex);

    clearInputFields();
    ui->doctorAstronautRadio->setChecked(true);
    showDoctorFields(true);
    ui->nameEdit->setFocus();
}

void AstronautEditDialog::onDeleteButtonClicked()
{
    int currentRow = ui->astronautList->currentRow();
    if (currentRow == -1) {
        QMessageBox::warning(this, "Ошибка", "Выберите астронавта для удаления");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Подтверждение",
                                  "Вы уверены, что хотите удалить выбранного астронавта?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        auto& astronauts = const_cast<std::vector<std::shared_ptr<Astronaut>>&>(iss->getAstronauts());
        astronauts.erase(astronauts.begin() + currentRow);
        updateAstronautList();

        if (ui->astronautList->count() > 0) {
            if (currentRow >= ui->astronautList->count()) {
                currentRow = ui->astronautList->count() - 1;
            }
            ui->astronautList->setCurrentRow(currentRow);
        } else {
            clearInputFields();
        }
    }
}

void AstronautEditDialog::onCloseButtonClicked()
{
    accept();
}

void AstronautEditDialog::onAstronautSelectionChanged()
{
    int currentRow = ui->astronautList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(iss->getAstronautsCount())) {
        const auto& astronauts = iss->getAstronauts();
        fillInputFields(astronauts[currentRow]);
    } else {
        clearInputFields();
    }
}

void AstronautEditDialog::onAstronautTypeChanged()
{
    showDoctorFields(ui->doctorAstronautRadio->isChecked());

    int currentRow = ui->astronautList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(iss->getAstronautsCount())) {
        updateSelectedAstronaut();
    }
}

void AstronautEditDialog::onInputFieldChanged()
{
    int currentRow = ui->astronautList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(iss->getAstronautsCount())) {
        updateSelectedAstronaut();
    }
}
