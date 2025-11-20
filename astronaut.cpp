#include "astronaut.h"
#include "utils.h"

using namespace std;

Astronaut::Astronaut()
{
    name = L"Неизвестно";
    country = L"Неизвестно";
    spaceflights_count = 0;
    total_days_in_space = 0;
    specialization = L"Неизвестно";
    current_status = true;
}

Astronaut::Astronaut(std::wistream &wis)
{
    wcout << L"Имя: ";
    std::getline(wis >> std::ws, this->name);
    wcout << L"Страна: ";
    std::getline(wis >> std::ws, this->country);
    wcout << L"Количество полетов: ";
    this->spaceflights_count = GetCorrectNumber<int>(0, 99);
    wcout << L"Общее кол-во дней в космосе: ";
    this->total_days_in_space = GetCorrectNumber<int>(0, 99);
    wcout << L"Специализация: ";
    std::getline(wis >> std::ws, this->specialization);
    wcout << L"Текущий статус(1 - активный, 0 - неактивный): ";
    this->current_status = GetCorrectNumber<bool>(0, 1);
}

Astronaut::Astronaut(const std::wstring &name,
                     const std::wstring &country,
                     int spaceflights,
                     int total_days,
                     const std::wstring &specialization,
                     bool status)
{
    this->name = name;
    this->country = country;
    this->spaceflights_count = spaceflights;
    this->total_days_in_space = total_days;
    this->specialization = specialization;
    this->current_status = status;
}

Astronaut::~Astronaut() {}

void Astronaut::writetoconsole() const
{
    wcout << L"Имя: " << this->name << endl
          << L"Страна: " << this->country << endl
          << L"Количество полетов: " << this->spaceflights_count << endl
          << L"Общее кол-во дней в космосе: " << this->total_days_in_space << endl
          << L"Специализация: " << this->specialization << endl
          << L"Текущий статус: " << (this->current_status ? L"Активный" : L"Неактивный") << endl
          << endl;
}

void Astronaut::drawInPainter(QPainter &painter, int &yPos, int rowHeight) const
{
    painter.drawText(50, yPos, "Обычный");
    painter.drawText(120, yPos, QString::fromStdWString(name));
    painter.drawText(270, yPos, QString::fromStdWString(country));
    painter.drawText(420, yPos, QString::number(spaceflights_count));
    painter.drawText(520, yPos, QString::number(total_days_in_space));
    painter.drawText(620, yPos, QString::fromStdWString(specialization));
    painter.drawText(820, yPos, get_current_status() ? "Активный" : "Неактивный");

    yPos += rowHeight;
}

std::shared_ptr<Astronaut> Astronaut::createFromUIFields(
    const std::wstring& name,
    const std::wstring& country,
    int spaceflights,
    int totalDays,
    const std::wstring& specialization,
    bool status) const
{
    return std::make_shared<Astronaut>(name, country, spaceflights, totalDays, specialization, status);
}

void Astronaut::updateFromUIFields(
    const std::wstring& name,
    const std::wstring& country,
    int spaceflights,
    int totalDays,
    const std::wstring& specialization,
    bool status)
{
    // Этот метод внутри класса Astronaut, поэтому имеет доступ к private полям
    this->name = name;
    this->country = country;
    this->spaceflights_count = spaceflights;
    this->total_days_in_space = totalDays;
    this->specialization = specialization;
    this->current_status = status;
}

void Astronaut::getUIFields(std::wstring& name,
                            std::wstring& country,
                            int& spaceflights,
                            int& totalDays,
                            std::wstring& specialization,
                            bool& status) const
{
    name = this->name;
    country = this->country;
    spaceflights = this->spaceflights_count;
    totalDays = this->total_days_in_space;
    specialization = this->specialization;
    status = this->current_status;
}

void Astronaut::fillUIInputs(QLineEdit* nameEdit,
                             QLineEdit* countryEdit,
                             QLineEdit* specializationEdit,
                             QLineEdit* spaceflightsEdit,
                             QLineEdit* totalDaysEdit) const
{
    if (nameEdit) nameEdit->setText(QString::fromStdWString(get_name()));
    if (countryEdit) countryEdit->setText(QString::fromStdWString(get_country()));
    if (specializationEdit) specializationEdit->setText(QString::fromStdWString(get_specialization()));
    if (spaceflightsEdit) spaceflightsEdit->setText(QString::number(get_spaceflights_count()));
    if (totalDaysEdit) totalDaysEdit->setText(QString::number(get_total_days_in_space()));
}

void Astronaut::setupUIFields(QRadioButton* regularRadio,
                              QRadioButton* doctorRadio,
                              QLabel* medicalLicenseLabel,
                              QLineEdit* medicalLicenseEdit,
                              QLabel* practiceYearsLabel,
                              QLineEdit* practiceYearsEdit) const
{
    if (regularRadio) regularRadio->setChecked(true);
    if (doctorRadio) doctorRadio->setChecked(false);

    if (medicalLicenseLabel) medicalLicenseLabel->setVisible(false);
    if (medicalLicenseEdit) {
        medicalLicenseEdit->setVisible(false);
        medicalLicenseEdit->clear();
    }
    if (practiceYearsLabel) practiceYearsLabel->setVisible(false);
    if (practiceYearsEdit) {
        practiceYearsEdit->setVisible(false);
        practiceYearsEdit->clear();
    }
}

