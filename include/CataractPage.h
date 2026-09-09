#ifndef CATARACTPAGE_H
#define CATARACTPAGE_H

#include "AppData.h"

#include <QWidget>

class QLabel;
class QProgressBar;
class QPushButton;
class QTimer;

class CataractPage : public QWidget
{
    Q_OBJECT

public:
    explicit CataractPage(
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
    QLabel *explanationLabel;
    QLabel *resultLabel;

    QProgressBar *progressBar;
    QPushButton *nextButton;

    QTimer *timer;

    int progress;

    void setupUI();
    void updateCheck();
};

#endif
