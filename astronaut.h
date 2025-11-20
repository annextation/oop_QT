#pragma once

#include "headers.h"

class Astronaut
{
private:
    std::wstring name;
    std::wstring country;
    int spaceflights_count;
    int total_days_in_space;
    std::wstring specialization;
    bool current_status;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & name;
        ar & country;
        ar & spaceflights_count;
        ar & total_days_in_space;
        ar & specialization;
        ar & current_status;
    }

protected:


public:
    Astronaut();
    Astronaut(std::wistream &wis);
    Astronaut(const std::wstring &name,
              const std::wstring &country,
              int spaceflights,
              int total_days,
              const std::wstring &specialization,
              bool status);
    virtual ~Astronaut();

    std::wstring get_name() const { return name; }
    std::wstring get_country() const { return country; }
    int get_spaceflights_count() const { return spaceflights_count; }
    int get_total_days_in_space() const { return total_days_in_space; }
    std::wstring get_specialization() const { return specialization; }
    bool get_current_status() const { return current_status; }

    virtual void writetoconsole() const;

    virtual QSize getTextBlockSize(QPainter &painter) const
    {
        QFontMetrics metrics(painter.font());
        return QSize(1100, metrics.height() * 7);
    }

    virtual void drawInPainter(QPainter &painter, int &yPos, int rowHeight) const;

    virtual std::shared_ptr<Astronaut> createFromUIFields(
        const std::wstring& name,
        const std::wstring& country,
        int spaceflights,
        int totalDays,
        const std::wstring& specialization,
        bool status) const;

    virtual void updateFromUIFields(
        const std::wstring& name,
        const std::wstring& country,
        int spaceflights,
        int totalDays,
        const std::wstring& specialization,
        bool status);

    virtual void getUIFields(std::wstring& name,
                             std::wstring& country,
                             int& spaceflights,
                             int& totalDays,
                             std::wstring& specialization,
                             bool& status) const;

    virtual void fillUIInputs(QLineEdit* nameEdit,
                              QLineEdit* countryEdit,
                              QLineEdit* specializationEdit,
                              QLineEdit* spaceflightsEdit,
                              QLineEdit* totalDaysEdit) const;

    virtual void setupUIFields(QRadioButton* regularRadio,
                               QRadioButton* doctorRadio,
                               QLabel* medicalLicenseLabel,
                               QLineEdit* medicalLicenseEdit,
                               QLabel* practiceYearsLabel,
                               QLineEdit* practiceYearsEdit) const;
};

BOOST_CLASS_EXPORT_KEY(Astronaut)
