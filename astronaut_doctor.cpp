#include "astronaut_doctor.h"
#include "utils.h"

using namespace std;

DoctorAstronaut::DoctorAstronaut()
    : Astronaut()
{}

DoctorAstronaut::DoctorAstronaut(wistream &wis)
    : Astronaut(wis)
{
    wcout << L"Медицинский номер лицензии: ";
    getline(wis >> std::ws, this->medical_license_number);
    wcout << L"Кол-во лет медицинской практики: ";
    this->years_of_medical_practice = GetCorrectNumber<int>(0, 70);
    wis.ignore();
}

DoctorAstronaut::DoctorAstronaut(const std::wstring &name,
                                 const std::wstring &country,
                                 int spaceflights,
                                 int total_days,
                                 const std::wstring &specialization,
                                 bool status,
                                 const std::wstring &license,
                                 int practice_years)
    : Astronaut(name, country, spaceflights, total_days, specialization, status)
{
    this->medical_license_number = license;
    this->years_of_medical_practice = practice_years;
}

void DoctorAstronaut::drawInPainter(QPainter &painter, int &yPos, int rowHeight) const
{
    int startY = yPos;
    Astronaut::drawInPainter(painter, yPos, rowHeight);
    painter.drawText(920, startY, QString::fromStdWString(medical_license_number));
    painter.drawText(1020, startY, QString::number(years_of_medical_practice));
}

void DoctorAstronaut::writetoconsole() const
{
    Astronaut::writetoconsole();
    wcout << L"Медицинский номер лицензии: " << this->medical_license_number << endl;
    wcout << L"Кол-во лет медицинской практики: " << this->years_of_medical_practice << endl;
}

std::shared_ptr<Astronaut> DoctorAstronaut::createFromUIFields(
    const std::wstring& name,
    const std::wstring& country,
    int spaceflights,
    int totalDays,
    const std::wstring& specialization,
    bool status) const
{
    // Создаем с пустыми медицинскими данными
    return std::make_shared<DoctorAstronaut>(name, country, spaceflights, totalDays,
                                             specialization, status, L"", 0);
}

void DoctorAstronaut::updateFromUIFields(
    const std::wstring& name,
    const std::wstring& country,
    int spaceflights,
    int totalDays,
    const std::wstring& specialization,
    bool status)
{
    // Вызываем базовый метод - он обновит private поля Astronaut
    Astronaut::updateFromUIFields(name, country, spaceflights, totalDays, specialization, status);
    // НЕ пытаемся напрямую обращаться к this->name, this->country и т.д.
}

void DoctorAstronaut::setMedicalData(const std::wstring& license, int practiceYears)
{
    this->medical_license_number = license;
    this->years_of_medical_practice = practiceYears;
}

void DoctorAstronaut::getMedicalData(std::wstring& license, int& practiceYears) const
{
    license = this->medical_license_number;
    practiceYears = this->years_of_medical_practice;
}

void DoctorAstronaut::fillUIInputs(QLineEdit* nameEdit,
                                   QLineEdit* countryEdit,
                                   QLineEdit* specializationEdit,
                                   QLineEdit* spaceflightsEdit,
                                   QLineEdit* totalDaysEdit) const
{
    Astronaut::fillUIInputs(nameEdit, countryEdit, specializationEdit, spaceflightsEdit, totalDaysEdit);
}

void DoctorAstronaut::setupUIFields(QRadioButton* regularRadio,
                                    QRadioButton* doctorRadio,
                                    QLabel* medicalLicenseLabel,
                                    QLineEdit* medicalLicenseEdit,
                                    QLabel* practiceYearsLabel,
                                    QLineEdit* practiceYearsEdit) const
{
    if (regularRadio) regularRadio->setChecked(false);
    if (doctorRadio) doctorRadio->setChecked(true);

    if (medicalLicenseLabel) medicalLicenseLabel->setVisible(true);
    if (medicalLicenseEdit) {
        medicalLicenseEdit->setVisible(true);
        medicalLicenseEdit->setText(QString::fromStdWString(getMedicalLicense()));
    }
    if (practiceYearsLabel) practiceYearsLabel->setVisible(true);
    if (practiceYearsEdit) {
        practiceYearsEdit->setVisible(true);
        practiceYearsEdit->setText(QString::number(getPracticeYears()));
    }
}
