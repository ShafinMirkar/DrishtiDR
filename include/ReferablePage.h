#ifndef REFERABLEPAGE_H
#define REFERABLEPAGE_H

#include "AppData.h"

#include <QWidget>

class QLabel;
class QPushButton;

class ReferablePage : public QWidget
{
    Q_OBJECT

public:
    explicit ReferablePage(
        const PatientData &patient,
        QWidget *parent = nullptr
    );

signals:
    void assessmentCompleted();

private:
    PatientData patient;

    QLabel *patientContextLabel;
    QLabel *workflowLabel;

    QLabel *probabilityLabel;
    QLabel *thresholdLabel;
    QLabel *decisionLabel;
    QLabel *statusLabel;
    QLabel *recommendationLabel;

    QPushButton *nextButton;

    void setupUI();
};

#endif
