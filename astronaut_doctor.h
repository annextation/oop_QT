#pragma once

#include "astronaut.h"

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

    void drawInPainter(QPainter &painter, int &yPos, int rowHeight) const override
    {
        painter.drawText(50, yPos, "Доктор");
        painter.drawText(120, yPos, QString::fromStdWString(get_name()));
        painter.drawText(270, yPos, QString::fromStdWString(get_country()));
        painter.drawText(420, yPos, QString::number(get_spaceflights_count()));
        painter.drawText(520, yPos, QString::number(get_total_days_in_space()));
        painter.drawText(620, yPos, QString::fromStdWString(get_specialization()));
        painter.drawText(820, yPos, get_current_status() ? "Активный" : "Неактивный");
        painter.drawText(920, yPos, QString::fromStdWString(medical_license_number));
        painter.drawText(1020, yPos, QString::number(years_of_medical_practice));

        yPos += rowHeight;
    }

};

BOOST_CLASS_EXPORT_KEY(DoctorAstronaut)
