#include "AstronautEditDialog.h"
#include "ui_AstronautEditDialog.h"

AstronautEditDialog::AstronautEditDialog(ISS* iss, QWidget *parent)
    : QDialog(parent),
    ui(new Ui::AstronautEditDialog),
    iss(iss)
{
    ui->setupUi(this);

    nameEdit = ui->nameEdit;
    countryEdit = ui->countryEdit;
    specializationEdit = ui->specializationEdit;
    spaceflightsEdit = ui->spaceflightsEdit;
    totalDaysEdit = ui->totalDaysEdit;
    medicalLicenseEdit = ui->medicalLicenseEdit;
    practiceYearsEdit = ui->practiceYearsEdit;
    regularAstronautRadio = ui->regularAstronautRadio;
    doctorAstronautRadio = ui->doctorAstronautRadio;

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
    for (const auto& astronaut : astronauts) {
        QString itemText = QString("%1. %2 [%3]")
        .arg(index + 1)
            .arg(QString::fromStdWString(astronaut->get_name()))
            .arg(astronaut->isDoctor() ? "Астронавт-врач" : "Обычный");

        ui->astronautList->addItem(itemText);
        index++;
    }
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

void AstronautEditDialog::onAddRegularAstronautClicked()
{
    auto newAstronaut = std::make_shared<Astronaut>(
        L"Новый астронавт", L"", 0, 0, L"", true);
    iss->addAstronaut(newAstronaut);

    updateAstronautList();
    int newIndex = ui->astronautList->count() - 1;
    ui->astronautList->setCurrentRow(newIndex);
}

void AstronautEditDialog::onAddDoctorAstronautClicked()
{
    auto newAstronaut = std::make_shared<DoctorAstronaut>(
        L"Новый астронавт-врач", L"", 0, 0, L"", true, L"", 0);
    iss->addAstronaut(newAstronaut);

    updateAstronautList();
    int newIndex = ui->astronautList->count() - 1;
    ui->astronautList->setCurrentRow(newIndex);
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
            showDoctorFields(false);
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
        auto astronaut = astronauts[currentRow];

        astronaut->updateDialogFields(this);
        showDoctorFields(astronaut->isDoctor());
    } else {
        clearInputFields();
        showDoctorFields(false);
    }
}

void AstronautEditDialog::onAstronautTypeChanged()
{
    showDoctorFields(ui->doctorAstronautRadio->isChecked());
}

void AstronautEditDialog::onInputFieldChanged()
{
    int currentRow = ui->astronautList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(iss->getAstronautsCount())) {
        auto& astronauts = const_cast<std::vector<std::shared_ptr<Astronaut>>&>(iss->getAstronauts());
        auto astronaut = astronauts[currentRow];

        astronaut->updateFromDialogFields(this);

        updateAstronautList();
        ui->astronautList->setCurrentRow(currentRow);
    }
}
