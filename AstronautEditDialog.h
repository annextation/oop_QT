#pragma once

#include "headers.h"
#include "ISS.h"
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

    void setupConnections();
    void updateAstronautList();
    void clearInputFields();
    void showDoctorFields(bool show);
    void fillInputFields(const std::shared_ptr<Astronaut>& astronaut);
    std::shared_ptr<Astronaut> createAstronautFromInput();
    void updateSelectedAstronaut();
};
