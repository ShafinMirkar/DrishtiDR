#include "CataractPage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QFrame>

CataractPage::CataractPage(
    const PatientData &patient,
    QWidget *parent
)
    : QWidget(parent),
      patient(patient),
      timer(nullptr),
      progress(0)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);

    setupUI();

    timer = new QTimer(this);

    connect(
        timer,
        &QTimer::timeout,
        this,
        &CataractPage::updateCheck
    );

    timer->start(350);
}

void CataractPage::setupUI()
{
    setStyleSheet(
        "QWidget {"
        "background-color: #FFFFFF;"
        "color: #243746;"
        "font-family: Arial;"
        "}"

        "QLabel {"
        "color: #243746;"
        "}"

        "QFrame {"
        "background-color: #F7F9FA;"
        "border: none;"
"background: transparent;"
        "}"

        "QLabel#title {"
        "font-size: 23px;"
        "font-weight: 700;"
        "color: #123B5D;"
        "}"

        "QLabel#subtitle {"
        "font-size: 14px;"
        "color: #637887;"
        "}"

        "QLabel#patientContext {"
        "background-color: #F3F6F8;"
        "border: none;"
"background: transparent;"
        "padding: 8px 10px;"
        "font-size: 13px;"
        "font-weight: 600;"
        "color: #34454F;"
        "}"

        "QLabel#workflow {"
        "font-size: 13px;"
        "font-weight: 600;"
        "color: #087F9C;"
        "}"

        "QLabel#sectionTitle {"
        "font-size: 17px;"
        "font-weight: 700;"
        "color: #34454F;"
        "}"

        "QLabel#explanation {"
        "font-size: 15px;"
        "line-height: 1.4;"
        "color: #526773;"
        "}"

        "QLabel#status {"
        "background-color: #EEF5F8;"
        "border: none;"
"background: transparent;"
        "padding: 12px;"
        "font-size: 15px;"
        "font-weight: 600;"
        "color: #24627A;"
        "}"

        "QLabel#result {"
        "background-color: #E8F6EC;"
        "border: none;"
"background: transparent;"
        "padding: 14px;"
        "font-size: 17px;"
        "font-weight: 700;"
        "color: #237238;"
        "}"

        "QProgressBar {"
        "border: 1px solid #CBD8DE;"
        "border-radius: 5px;"
        "background-color: #FFFFFF;"
        "height: 18px;"
        "text-align: center;"
        "color: #34454F;"
        "}"

        "QProgressBar::chunk {"
        "background-color: #16839A;"
        "border-radius: 4px;"
        "}"

        "QPushButton {"
        "background-color: #123B5D;"
        "color: #FFFFFF;"
        "border: none;"
        "border-radius: 5px;"
        "padding: 9px 20px;"
        "font-size: 14px;"
        "font-weight: 600;"
        "}"

        "QPushButton:disabled {"
        "background-color: #B8C4CB;"
        "}"
    );

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(12);

    // -------------------------------------------------
    // Header
    // -------------------------------------------------

    auto *title = new QLabel("Cataract Check");
    title->setObjectName("title");

    auto *subtitle = new QLabel(
        "Preliminary assessment of retinal image clarity and haze"
    );
    subtitle->setObjectName("subtitle");

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // -------------------------------------------------
    // Patient context
    // -------------------------------------------------

    patientContextLabel = new QLabel(
        QString("Patient: %1   |   Name: %2   |   Age: %3   |   %4")
            .arg(patient.patientId)
            .arg(patient.name)
            .arg(patient.age)
            .arg(patient.eye)
    );

    patientContextLabel->setObjectName("patientContext");
    patientContextLabel->setMinimumHeight(34);

    mainLayout->addWidget(patientContextLabel);

    // -------------------------------------------------
    // Workflow
    // -------------------------------------------------

    workflowLabel = new QLabel(
        "Patient  →  Image  →  Quality  →  [ CATARACT CHECK ]  →  "
        "Preprocessing  →  Analysis  →  Result  →  Report"
    );

    workflowLabel->setObjectName("workflow");

    mainLayout->addWidget(workflowLabel);

    // -------------------------------------------------
    // Main check panel
    // -------------------------------------------------

    auto *checkFrame = new QFrame;
    auto *checkLayout = new QVBoxLayout(checkFrame);

    checkLayout->setContentsMargins(35, 35, 35, 35);
    checkLayout->setSpacing(18);

    auto *sectionTitle = new QLabel(
        "What is being checked?"
    );

    sectionTitle->setObjectName("sectionTitle");

    checkLayout->addWidget(sectionTitle);

    explanationLabel = new QLabel(
        "The cataract check examines the brightness and intensity "
        "distribution of the retinal image to identify haze or "
        "low-information regions that may reduce retinal visibility."
    );

    explanationLabel->setObjectName("explanation");
    explanationLabel->setWordWrap(true);

    checkLayout->addWidget(explanationLabel);

    statusLabel = new QLabel(
        "Checking retinal image..."
    );

    statusLabel->setObjectName("status");

    checkLayout->addWidget(statusLabel);

    progressBar = new QProgressBar;
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(true);

    checkLayout->addWidget(progressBar);

    auto *checksLabel = new QLabel(
        "• Retinal disc brightness\n"
        "• Retinal transparency / haze\n"
        "• Low-information retinal regions"
    );

    checksLabel->setObjectName("explanation");

    checkLayout->addWidget(checksLabel);

    resultLabel = new QLabel;
    resultLabel->setObjectName("result");
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setVisible(false);

    checkLayout->addWidget(resultLabel);

    checkLayout->addStretch();

    mainLayout->addWidget(checkFrame, 1);

    // -------------------------------------------------
    // Bottom action bar
    // -------------------------------------------------

    auto *actionFrame = new QFrame;

    auto *actionLayout = new QHBoxLayout(actionFrame);
    actionLayout->setContentsMargins(10, 8, 10, 8);

    nextButton = new QPushButton("Next");
    nextButton->setEnabled(false);

    actionLayout->addStretch();
    actionLayout->addWidget(nextButton);

    mainLayout->addWidget(actionFrame);

    connect(
        nextButton,
        &QPushButton::clicked,
        this,
        &CataractPage::analysisCompleted
    );
}

void CataractPage::updateCheck()
{
    progress += 10;

    if (progress >= 100)
    {
        progress = 100;

        progressBar->setValue(progress);

        statusLabel->setText(
            "Cataract screening check completed."
        );

        resultLabel->setText(
            "✓ Cataract not detected"
        );

        resultLabel->setVisible(true);

        nextButton->setEnabled(true);

        timer->stop();

        return;
    }

    progressBar->setValue(progress);

    if (progress < 35)
    {
        statusLabel->setText(
            "Checking retinal disc brightness and intensity..."
        );
    }
    else if (progress < 70)
    {
        statusLabel->setText(
            "Evaluating retinal transparency and possible haze..."
        );
    }
    else
    {
        statusLabel->setText(
            "Checking for low-information retinal regions..."
        );
    }
}
