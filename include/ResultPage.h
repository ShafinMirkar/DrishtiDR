#ifndef RESULTPAGE_H
#define RESULTPAGE_H

#include "AppData.h"

#include <QWidget>

class QLabel;
class QPushButton;

class ResultPage : public QWidget
{
    Q_OBJECT

public:
    explicit ResultPage(
        const PatientData &patient,
        QWidget *parent = nullptr
    );

signals:
    void reportRequested();

private:
    PatientData patient;

    QLabel *patientContextLabel;
    QLabel *workflowLabel;

    QLabel *imageLabel;
    QLabel *gradeLabel;
    QLabel *severityLabel;
    QLabel *confidenceLabel;
    QLabel *referableLabel;
    QLabel *evidenceLabel;
    QLabel *gradcamLabel;
    QLabel *lesionLabel;

    QPushButton *reportButton;

    void setupUI();
};

#endif