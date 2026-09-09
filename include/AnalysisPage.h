#ifndef ANALYSISPAGE_H
#define ANALYSISPAGE_H

#include "AppData.h"

#include <QWidget>

class QLabel;
class QPushButton;

class AnalysisPage : public QWidget
{
    Q_OBJECT

public:
    explicit AnalysisPage(
        const PatientData &patient,
        QWidget *parent = nullptr
    );

signals:
    void analysisCompleted();

private:
    PatientData patient;

    QLabel *patientContextLabel;
    QLabel *workflowLabel;

    QLabel *gradeLabel;
    QLabel *severityLabel;
    QLabel *confidenceLabel;

    QLabel *noDrProbabilityLabel;
    QLabel *mildProbabilityLabel;
    QLabel *moderateProbabilityLabel;
    QLabel *severeProbabilityLabel;
    QLabel *proliferativeProbabilityLabel;

    QPushButton *nextButton;

    void setupUI();
};

#endif