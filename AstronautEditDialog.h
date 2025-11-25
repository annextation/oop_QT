#pragma once

#include "headers.h"
#include "ISS.h"
#include "astronaut.h"
#include "astronaut_doctor.h"

namespace Ui {
class AstronautEditDialog;
}

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
    QRadioButton* getRegularRadio() const { return regularAstronautRadio; }
    QRadioButton* getDoctorRadio() const { return doctorAstronautRadio; }

private slots:
    void onAddRegularAstronautClicked();
    void onAddDoctorAstronautClicked();
    void onDeleteButtonClicked();
    void onCloseButtonClicked();
    void onAstronautSelectionChanged();
    void onAstronautTypeChanged();
    void onInputFieldChanged();

private:
    Ui::AstronautEditDialog *ui;
    ISS* iss;

    QLineEdit* nameEdit;
    QLineEdit* countryEdit;
    QLineEdit* specializationEdit;
    QLineEdit* spaceflightsEdit;
    QLineEdit* totalDaysEdit;
    QLineEdit* medicalLicenseEdit;
    QLineEdit* practiceYearsEdit;
    QRadioButton* regularAstronautRadio;
    QRadioButton* doctorAstronautRadio;

    void setupConnections();
    void updateAstronautList();
    void clearInputFields();
    void showDoctorFields(bool show);
};
