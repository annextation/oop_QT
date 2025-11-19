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
