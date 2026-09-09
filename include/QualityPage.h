#ifndef QUALITYPAGE_H
#define QUALITYPAGE_H

#include "AppData.h"

#include <QWidget>

class QLabel;
class QPushButton;

class QualityPage : public QWidget
{
    Q_OBJECT

public:
    explicit QualityPage(
        const PatientData &patient,
        QWidget *parent = nullptr
    );

signals:
    void nextRequested();
    void backRequested();

private slots:
    void uploadAnotherImage();
    void rejectImage();

private:
    PatientData patient;

    QLabel *patientContextLabel;
    QLabel *workflowLabel;

    QLabel *originalImage;

    QLabel *statusLabel;
    QLabel *focusLabel;
    QLabel *brightnessLabel;
    QLabel *contrastLabel;
    QLabel *fovLabel;
    QLabel *illuminationLabel;

    QLabel *focusCheckLabel;
    QLabel *brightnessCheckLabel;
    QLabel *contrastCheckLabel;
    QLabel *fovCheckLabel;
    QLabel *illuminationCheckLabel;

    QPushButton *nextButton;
    QPushButton *rejectButton;
    QPushButton *uploadAnotherButton;

    void setupUI();
};

#endif
