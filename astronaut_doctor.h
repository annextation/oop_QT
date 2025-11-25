#pragma once

#include "headers.h"
#include "astronaut.h"

class AstronautEditDialog;

class DoctorAstronaut : public Astronaut
{
private:
    std::wstring medical_license_number;
    int years_of_medical_practice;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar &boost::serialization::base_object<Astronaut>(*this);
        ar & medical_license_number;
        ar & years_of_medical_practice;
    }

public:
    DoctorAstronaut();
    DoctorAstronaut(std::wistream &is);
    DoctorAstronaut(const std::wstring &name,
                    const std::wstring &country,
                    int spaceflights,
                    int total_days,
                    const std::wstring &specialization,
                    bool status,
                    const std::wstring &license,
                    int practice_years);

    void writetoconsole() const override;

    std::wstring getMedicalLicense() const { return medical_license_number; }
    int getPracticeYears() const { return years_of_medical_practice; }

    QSize getTextBlockSize(QPainter &painter) const override
    {
        QFontMetrics metrics(painter.font());
        return QSize(1100, metrics.height() * 9);
    }

    void drawInPainter(QPainter &painter, int &yPos, int rowHeight) const override;

    void updateDialogFields(AstronautEditDialog* dialog) const override;
    void updateFromDialogFields(AstronautEditDialog* dialog) override;

    bool isDoctor() const override { return true; }
};

BOOST_CLASS_EXPORT_KEY(DoctorAstronaut)
