#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include "AppData.h"

#include <QWidget>

class QLabel;
class QPushButton;

class ReportPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReportPage(
        const PatientData &patient,
        QWidget *parent = nullptr
    );

private slots:
    void generatePDF();

private:
    PatientData patient;

    QLabel *patientContextLabel;
    QLabel *workflowLabel;
    QLabel *reportNumberLabel;
    QLabel *reportInfo;
    QLabel *statusLabel;

    QPushButton *generateButton;

    void setupUI();
};

#endif