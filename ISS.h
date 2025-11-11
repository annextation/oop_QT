#pragma once

#include "astronaut_doctor.h"

class ISS
{
private:
    std::vector<std::shared_ptr<Astronaut>> astronauts;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & astronauts;
    }

public:
    ISS();
    ~ISS();

    void view_all_astronauts() const;
    void delete_astronauts();
    void save_in_file(const std::wstring& file_name) const;
    void load_from_file();

    void addAstronaut(std::shared_ptr<Astronaut> astronaut);
    void addAstronaut(const std::wstring& name, const std::wstring& country,
                      int spaceflights, int total_days, const std::wstring& specialization,
                      bool status);
    void addDoctorAstronaut(const std::wstring& name, const std::wstring& country,
                            int spaceflights, int total_days, const std::wstring& specialization,
                            bool status, const std::wstring& license, int practice_years);

    size_t getAstronautsCount() const { return astronauts.size(); }
    const std::vector<std::shared_ptr<Astronaut>>& getAstronauts() const { return astronauts; }

    void forEachAstronaut(const std::function<void(const std::shared_ptr<Astronaut>&)>& func) const {
        std::for_each(astronauts.begin(), astronauts.end(), func);
    }
};
