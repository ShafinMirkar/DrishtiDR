#ifndef ANALYSISPAGE_H
#define ANALYSISPAGE_H

#include "AppData.h"

#include <QWidget>

class QLabel;
class QProgressBar;
class QPushButton;
class QTimer;

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

    QLabel *statusLabel;
    QLabel *stepLabel;
    QLabel *stepsLabel;

    QProgressBar *progressBar;
    QPushButton *resultButton;

    QTimer *timer;

    int progress;
    int currentStep;

    void setupUI();
    void updateAnalysis();
};

#endif