#include "astronaut.h"
#include "utils.h"

using namespace std;

Astronaut::Astronaut() {
    name = L"Неизвестно";
    country = L"Неизвестно";
    spaceflights_count = 0;
    total_days_in_space = 0;
    specialization = L"Неизвестно";
    current_status = true;
}

Astronaut::Astronaut(std::wistream& wis) {
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

Astronaut::Astronaut(const std::wstring& name, const std::wstring& country,
                      int spaceflights, int total_days, const std::wstring& specialization,
                      bool status) {
     this->name = name;
     this->country = country;
     this->spaceflights_count = spaceflights;
     this->total_days_in_space = total_days;
     this->specialization = specialization;
     this->current_status = status;
 }

Astronaut::~Astronaut() {}

void Astronaut::writetoconsole() const {
    wcout << L"Имя: " << this->name << endl <<
        L"Страна: " << this->country << endl <<
        L"Количество полетов: " << this->spaceflights_count << endl <<
        L"Общее кол-во дней в космосе: " << this->total_days_in_space << endl <<
        L"Специализация: " << this->specialization << endl <<
        L"Текущий статус: " << (this->current_status ? L"Активный" : L"Неактивный") << endl << endl;
}
