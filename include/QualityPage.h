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
    void enhanceImage();
    void rejectImage();

private:
    PatientData patient;

    QLabel *patientContextLabel;
    QLabel *workflowLabel;

    QLabel *originalImage;
    QLabel *enhancedImage;
    QLabel *enhancedCaption;

    QLabel *statusLabel;
    QLabel *qualityLabel;
    QLabel *focusLabel;
    QLabel *illuminationLabel;
    QLabel *fieldLabel;
    QLabel *enhancementInfo;

    QPushButton *nextButton;
    QPushButton *enhanceButton;
    QPushButton *rejectButton;
    QPushButton *uploadAnotherButton;

    void setupUI();
    void evaluateQuality();
    void applySimulatedEnhancement();
};

#endif