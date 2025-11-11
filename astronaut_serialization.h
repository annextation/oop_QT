#pragma once

#include "astronaut.h"
#include "astronaut_doctor.h"

// ВСЕ BOOST ИНКЛЮДЫ ТОЛЬКО ЗДЕСЬ
#include <boost/serialization/access.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>


// СЕРИАЛИЗАЦИЯ ASTRONAUT
class Astronaut
{
    // Дружественный класс для сериализации
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & name;
        ar & country;
        ar & spaceflights_count;
        ar & total_days_in_space;
        ar & specialization;
        ar & current_status;
    }
};

// СЕРИАЛИЗАЦИЯ DOCTOR ASTRONAUT
class DoctorAstronaut
{
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar & boost::serialization::base_object<Astronaut>(*this);
        ar & medical_license_number;
        ar & years_of_medical_practice;
    }
};

BOOST_CLASS_EXPORT_KEY(Astronaut)
BOOST_CLASS_EXPORT_KEY(DoctorAstronaut)
