#pragma once

#include "headers.h"
#include "ISS.h"
#include "astronaut_doctor.h"

class AstronautEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AstronautEditDialog(ISS* iss, QWidget *parent = nullptr);
    ~AstronautEditDialog();

    QLineEdit* getNameEdit() const { return nameEdit; }
    QLineEdit* getCountryEdit() const { return countryEdit; }
    QLineEdit* getSpecializationEdit() const { return specializationEdit; }
    QLineEdit* getSpaceflightsEdit() const { return spaceflightsEdit; }
    QLineEdit* getTotalDaysEdit() const { return totalDaysEdit; }
    QLineEdit* getMedicalLicenseEdit() const { return medicalLicenseEdit; }
    QLineEdit* getPracticeYearsEdit() const { return practiceYearsEdit; }

private slots:
    void onAddRegularAstronautClicked();
    void onAddDoctorAstronautClicked();
    void onDeleteButtonClicked();
    void onCloseButtonClicked();
    void onAstronautSelectionChanged();
    void onAstronautTypeChanged();
    void onInputFieldChanged();

private:
    ISS* iss;

    QListWidget* astronautList;
    QLineEdit* nameEdit;
    QLineEdit* countryEdit;
    QLineEdit* specializationEdit;
    QLineEdit* spaceflightsEdit;
    QLineEdit* totalDaysEdit;

    QLineEdit* medicalLicenseEdit;
    QLineEdit* practiceYearsEdit;
    QLabel* medicalLicenseLabel;
    QLabel* practiceYearsLabel;

    QPushButton* addRegularAstronautButton;
    QPushButton* addDoctorAstronautButton;
    QPushButton* deleteButton;
    QPushButton* closeButton;

    QRadioButton* regularAstronautRadio;
    QRadioButton* doctorAstronautRadio;
    QButtonGroup* astronautTypeGroup;

    void setupUI();
    void updateAstronautList();
    void clearInputFields();
    void showDoctorFields(bool show);
    void fillInputFields(const std::shared_ptr<Astronaut>& astronaut);
    std::shared_ptr<Astronaut> createAstronautFromInput();
    void updateSelectedAstronaut();
};
