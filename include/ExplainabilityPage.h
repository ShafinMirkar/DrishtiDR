#ifndef EXPLAINABILITYPAGE_H
#define EXPLAINABILITYPAGE_H

#include "AppData.h"

#include <QWidget>

class QLabel;
class QFrame;
class QPushButton;

class ExplainabilityPage : public QWidget
{
    Q_OBJECT

public:
    explicit ExplainabilityPage(
        const PatientData &patient,
        QWidget *parent = nullptr
    );

signals:
    void reportRequested();

private:
    PatientData patient;

    QLabel *patientContextLabel;
    QLabel *workflowLabel;

    QLabel *preprocessedImage;
    QLabel *gradcamImage;
    QLabel *attentionImage;

    QPushButton *reportButton;

    void setupUI();
    QFrame *createImageCard(
        const QString &title,
        const QString &description,
        const QString &imagePath,
        QLabel **imageLabel
    );
};

#endif
