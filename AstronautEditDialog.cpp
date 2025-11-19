#include "AstronautEditDialog.h"

AstronautEditDialog::AstronautEditDialog(ISS* iss, QWidget *parent)
    : QDialog(parent), iss(iss)
{
    setupUI();
    updateAstronautList();

    connect(astronautList, &QListWidget::currentRowChanged, this, &AstronautEditDialog::onAstronautSelectionChanged);
    connect(addRegularAstronautButton, &QPushButton::clicked, this, &AstronautEditDialog::onAddRegularAstronautClicked);
    connect(addDoctorAstronautButton, &QPushButton::clicked, this, &AstronautEditDialog::onAddDoctorAstronautClicked);
    connect(deleteButton, &QPushButton::clicked, this, &AstronautEditDialog::onDeleteButtonClicked);
    connect(closeButton, &QPushButton::clicked, this, &AstronautEditDialog::onCloseButtonClicked);
    connect(astronautTypeGroup, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &AstronautEditDialog::onAstronautTypeChanged);

    connect(nameEdit, &QLineEdit::editingFinished, this, &AstronautEditDialog::onInputFieldChanged);
    connect(countryEdit, &QLineEdit::editingFinished, this, &AstronautEditDialog::onInputFieldChanged);
    connect(specializationEdit, &QLineEdit::editingFinished, this, &AstronautEditDialog::onInputFieldChanged);
    connect(spaceflightsEdit, &QLineEdit::editingFinished, this, &AstronautEditDialog::onInputFieldChanged);
    connect(totalDaysEdit, &QLineEdit::editingFinished, this, &AstronautEditDialog::onInputFieldChanged);
    connect(medicalLicenseEdit, &QLineEdit::editingFinished, this, &AstronautEditDialog::onInputFieldChanged);
    connect(practiceYearsEdit, &QLineEdit::editingFinished, this, &AstronautEditDialog::onInputFieldChanged);
}

AstronautEditDialog::~AstronautEditDialog()
{
}

void AstronautEditDialog::setupUI()
{
    setWindowTitle("Редактирование списка астронавтов");
    setMinimumSize(900, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QGroupBox *listGroup = new QGroupBox("Список астронавтов", this);
    QVBoxLayout *listLayout = new QVBoxLayout(listGroup);
    astronautList = new QListWidget(listGroup);
    astronautList->setMinimumHeight(180);
    listLayout->addWidget(astronautList);

    QHBoxLayout *bottomLayout = new QHBoxLayout();

    QGroupBox *inputGroup = new QGroupBox("Данные астронавта", this);
    inputGroup->setMinimumWidth(400);
    QGridLayout *inputLayout = new QGridLayout(inputGroup);

    inputLayout->addWidget(new QLabel("Имя:"), 0, 0);
    nameEdit = new QLineEdit(inputGroup);
    inputLayout->addWidget(nameEdit, 0, 1);

    inputLayout->addWidget(new QLabel("Страна:"), 1, 0);
    countryEdit = new QLineEdit(inputGroup);
    inputLayout->addWidget(countryEdit, 1, 1);

    inputLayout->addWidget(new QLabel("Специализация:"), 2, 0);
    specializationEdit = new QLineEdit(inputGroup);
    inputLayout->addWidget(specializationEdit, 2, 1);

    inputLayout->addWidget(new QLabel("Кол-во полетов:"), 3, 0);
    spaceflightsEdit = new QLineEdit(inputGroup);
    spaceflightsEdit->setValidator(new QIntValidator(0, 99, this));
    inputLayout->addWidget(spaceflightsEdit, 3, 1);

    inputLayout->addWidget(new QLabel("Дней в космосе:"), 4, 0);
    totalDaysEdit = new QLineEdit(inputGroup);
    totalDaysEdit->setValidator(new QIntValidator(0, 99, this));
    inputLayout->addWidget(totalDaysEdit, 4, 1);

    medicalLicenseLabel = new QLabel("Мед. лицензия:", inputGroup);
    inputLayout->addWidget(medicalLicenseLabel, 5, 0);
    medicalLicenseEdit = new QLineEdit(inputGroup);
    inputLayout->addWidget(medicalLicenseEdit, 5, 1);

    practiceYearsLabel = new QLabel("Лет практики:", inputGroup);
    inputLayout->addWidget(practiceYearsLabel, 6, 0);
    practiceYearsEdit = new QLineEdit(inputGroup);
    practiceYearsEdit->setValidator(new QIntValidator(0, 70, this));
    inputLayout->addWidget(practiceYearsEdit, 6, 1);

    showDoctorFields(false);

    QGroupBox *actionGroup = new QGroupBox("Действия", this);
    actionGroup->setMinimumWidth(250);
    QVBoxLayout *actionLayout = new QVBoxLayout(actionGroup);

    QGroupBox *typeGroup = new QGroupBox("Тип астронавта", actionGroup);
    QVBoxLayout *typeLayout = new QVBoxLayout(typeGroup);

    regularAstronautRadio = new QRadioButton("Обычный астронавт", typeGroup);
    doctorAstronautRadio = new QRadioButton("Астронавт-врач", typeGroup);
    regularAstronautRadio->setChecked(true);

    astronautTypeGroup = new QButtonGroup(typeGroup);
    astronautTypeGroup->addButton(regularAstronautRadio);
    astronautTypeGroup->addButton(doctorAstronautRadio);

    typeLayout->addWidget(regularAstronautRadio);
    typeLayout->addWidget(doctorAstronautRadio);

    QVBoxLayout *buttonLayout = new QVBoxLayout();

    addRegularAstronautButton = new QPushButton("Добавить обычного", actionGroup);
    addDoctorAstronautButton = new QPushButton("Добавить врача", actionGroup);
    deleteButton = new QPushButton("Удалить", actionGroup);
    closeButton = new QPushButton("Закрыть", actionGroup);

    buttonLayout->addWidget(addRegularAstronautButton);
    buttonLayout->addWidget(addDoctorAstronautButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(closeButton);
    buttonLayout->addStretch();

    actionLayout->addWidget(typeGroup);
    actionLayout->addLayout(buttonLayout);

    bottomLayout->addWidget(inputGroup, 2);
    bottomLayout->addWidget(actionGroup, 1);

    mainLayout->addWidget(listGroup, 1);
    mainLayout->addLayout(bottomLayout, 2);
}

void AstronautEditDialog::updateAstronautList()
{
    astronautList->clear();

    const auto& astronauts = iss->getAstronauts();

    int index = 0;
    std::for_each(astronauts.begin(), astronauts.end(), [this, &index](const std::shared_ptr<Astronaut>& astronaut) {
        QString itemText = QString("%1. %2")
        .arg(index + 1)
            .arg(QString::fromStdWString(astronaut->get_name()));

        if (auto doctorAstronaut = std::dynamic_pointer_cast<DoctorAstronaut>(astronaut)) {
            itemText += " [Астронавт-врач]";
        } else {
            itemText += " [Обычный]";
        }

        astronautList->addItem(itemText);
        index++;
    });
}

void AstronautEditDialog::clearInputFields()
{
    nameEdit->clear();
    countryEdit->clear();
    specializationEdit->clear();
    spaceflightsEdit->clear();
    totalDaysEdit->clear();
    medicalLicenseEdit->clear();
    practiceYearsEdit->clear();
}

void AstronautEditDialog::showDoctorFields(bool show)
{
    medicalLicenseLabel->setVisible(show);
    medicalLicenseEdit->setVisible(show);
    practiceYearsLabel->setVisible(show);
    practiceYearsEdit->setVisible(show);
}

void AstronautEditDialog::fillInputFields(const std::shared_ptr<Astronaut>& astronaut)
{
    if (!astronaut) return;

    nameEdit->setText(QString::fromStdWString(astronaut->get_name()));
    countryEdit->setText(QString::fromStdWString(astronaut->get_country()));
    specializationEdit->setText(QString::fromStdWString(astronaut->get_specialization()));
    spaceflightsEdit->setText(QString::number(astronaut->get_spaceflights_count()));
    totalDaysEdit->setText(QString::number(astronaut->get_total_days_in_space()));

    if (auto doctorAstronaut = std::dynamic_pointer_cast<DoctorAstronaut>(astronaut)) {
        doctorAstronautRadio->setChecked(true);
        showDoctorFields(true);
        medicalLicenseEdit->setText(QString::fromStdWString(doctorAstronaut->getMedicalLicense()));
        practiceYearsEdit->setText(QString::number(doctorAstronaut->getPracticeYears()));
    } else {
        regularAstronautRadio->setChecked(true);
        showDoctorFields(false);
    }
}

std::shared_ptr<Astronaut> AstronautEditDialog::createAstronautFromInput()
{
    std::wstring name = nameEdit->text().toStdWString();
    std::wstring country = countryEdit->text().toStdWString();
    std::wstring specialization = specializationEdit->text().toStdWString();
    int spaceflights = spaceflightsEdit->text().isEmpty() ? 0 : spaceflightsEdit->text().toInt();
    int totalDays = totalDaysEdit->text().isEmpty() ? 0 : totalDaysEdit->text().toInt();
    bool status = true;

    if (doctorAstronautRadio->isChecked()) {
        std::wstring license = medicalLicenseEdit->text().toStdWString();
        int practiceYears = practiceYearsEdit->text().isEmpty() ? 0 : practiceYearsEdit->text().toInt();
        return std::make_shared<DoctorAstronaut>(name, country, spaceflights, totalDays, specialization, status, license, practiceYears);
    } else {
        return std::make_shared<Astronaut>(name, country, spaceflights, totalDays, specialization, status);
    }
}

void AstronautEditDialog::updateSelectedAstronaut()
{
    int currentRow = astronautList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(iss->getAstronautsCount())) {
        auto& astronauts = const_cast<std::vector<std::shared_ptr<Astronaut>>&>(iss->getAstronauts());
        astronauts[currentRow] = createAstronautFromInput();
        updateAstronautList();
        astronautList->setCurrentRow(currentRow);
    }
}

void AstronautEditDialog::onAddRegularAstronautClicked()
{
    auto newAstronaut = std::make_shared<Astronaut>(L"Новый астронавт", L"", 0, 0, L"", true);
    iss->addAstronaut(newAstronaut);

    updateAstronautList();
    int newIndex = astronautList->count() - 1;
    astronautList->setCurrentRow(newIndex);

    clearInputFields();
    regularAstronautRadio->setChecked(true);
    showDoctorFields(false);
    nameEdit->setFocus();
}

void AstronautEditDialog::onAddDoctorAstronautClicked()
{
    auto newAstronaut = std::make_shared<DoctorAstronaut>(L"Новый астронавт-врач", L"", 0, 0, L"", true, L"", 0);
    iss->addAstronaut(newAstronaut);

    updateAstronautList();
    int newIndex = astronautList->count() - 1;
    astronautList->setCurrentRow(newIndex);

    clearInputFields();
    doctorAstronautRadio->setChecked(true);
    showDoctorFields(true);
    nameEdit->setFocus();
}

void AstronautEditDialog::onDeleteButtonClicked()
{
    int currentRow = astronautList->currentRow();
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

        if (astronautList->count() > 0) {
            if (currentRow >= astronautList->count()) {
                currentRow = astronautList->count() - 1;
            }
            astronautList->setCurrentRow(currentRow);
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
    int currentRow = astronautList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(iss->getAstronautsCount())) {
        const auto& astronauts = iss->getAstronauts();
        fillInputFields(astronauts[currentRow]);
    } else {
        clearInputFields();
    }
}

void AstronautEditDialog::onAstronautTypeChanged()
{
    showDoctorFields(doctorAstronautRadio->isChecked());

    int currentRow = astronautList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(iss->getAstronautsCount())) {
        updateSelectedAstronaut();
    }
}

void AstronautEditDialog::onInputFieldChanged()
{
    int currentRow = astronautList->currentRow();
    if (currentRow >= 0 && currentRow < static_cast<int>(iss->getAstronautsCount())) {
        updateSelectedAstronaut();
    }
}
