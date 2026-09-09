#ifndef PREPROCESSINGPAGE_H
#define PREPROCESSINGPAGE_H

#include "AppData.h"

#include <QWidget>

class QLabel;
class QProgressBar;
class QPushButton;
class QTimer;

class PreprocessingPage : public QWidget
{
    Q_OBJECT

public:
    explicit PreprocessingPage(
        const PatientData &patient,
        QWidget *parent = nullptr
    );

signals:
    void processingCompleted();

private slots:
    void updateProcessing();

private:
    PatientData patient;

    QLabel *patientContextLabel;
    QLabel *workflowLabel;

    QLabel *statusLabel;
    QLabel *originalImage;
    QLabel *preprocessedImage;

    QProgressBar *progressBar;
    QPushButton *nextButton;

    QTimer *timer;

    int progress;
    int step;

    void setupUI();
};

#endif
