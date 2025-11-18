#include "ISS.h"

using namespace std;

ISS::ISS() {}

ISS::~ISS()
{
    this->delete_astronauts();
}

void ISS::view_all_astronauts() const
{
    if (!this->astronauts.size()) {
        wcout << L"Нет астронавтов на МКС" << endl;
    } else {
        wcout << endl << endl;
        for (const auto &astronaut : this->astronauts) {
            astronaut->writetoconsole();
        }
    }
}

void ISS::delete_astronauts()
{
    this->astronauts.clear();
    wcout << L"Все астронавты удалены" << endl;
}

void ISS::save_in_file(const wstring &file_name) const
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string filename_str = converter.to_bytes(file_name);

    ofstream file(filename_str, ios::binary);

    if (file.is_open()) {
        boost::archive::binary_oarchive ofile(file);
        ofile << *this;
        wcout << L"Все астронавты сохранены!" << endl;
        file.close();
    } else {
        wcout << L"Не удалось открыть файл!" << endl;
    }
}

void ISS::load_from_file(const std::wstring &file_name)
{
    wstring_convert<codecvt_utf8<wchar_t>> converter;
    string filename_str = converter.to_bytes(file_name);

    ifstream file(filename_str, ios::binary);

    if (file.is_open()) {
        boost::archive::binary_iarchive ifile(file);
        ifile >> *this;
        wcout << L"Данные загружены!" << endl;
    } else {
        wcout << L"Файл не открыт!" << endl;
    }
}

void ISS::addAstronaut(shared_ptr<Astronaut> astronaut)
{
    astronauts.push_back(astronaut);
}

void ISS::addAstronaut(const std::wstring &name,
                       const std::wstring &country,
                       int spaceflights,
                       int total_days,
                       const std::wstring &specialization,
                       bool status)
{
    auto astronaut
        = make_shared<Astronaut>(name, country, spaceflights, total_days, specialization, status);
    astronauts.push_back(astronaut);
}

void ISS::addDoctorAstronaut(const std::wstring &name,
                             const std::wstring &country,
                             int spaceflights,
                             int total_days,
                             const std::wstring &specialization,
                             bool status,
                             const std::wstring &license,
                             int practice_years)
{
    auto astronaut = make_shared<DoctorAstronaut>(name,
                                                  country,
                                                  spaceflights,
                                                  total_days,
                                                  specialization,
                                                  status,
                                                  license,
                                                  practice_years);
    astronauts.push_back(astronaut);
}
