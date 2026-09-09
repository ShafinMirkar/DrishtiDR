#include "ReferablePage.h"

#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

ReferablePage::ReferablePage(
    const PatientData &patient,
    QWidget *parent
)
    : QWidget(parent),
      patient(patient)
{
    setupUI();
}

void ReferablePage::setupUI()
{
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(
        "QWidget {"
        "background-color: #FFFFFF;"
        "color: #243746;"
        "}"

        "QFrame {"
        "background-color: #FFFFFF;"
        "border: 1px solid #D7E0E5;"
        "border-radius: 8px;"
        "}"

        "QLabel {"
        "border: none;"
        "}"

        "QPushButton {"
        "background-color: #4B8FA8;"
        "color: white;"
        "border: none;"
        "border-radius: 5px;"
        "padding: 10px 22px;"
        "font-size: 15px;"
        "}"
    );

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 24, 32, 24);
    mainLayout->setSpacing(16);

    // Header
    auto *title = new QLabel("Referable DR Assessment");
    title->setStyleSheet(
        "font-size: 25px;"
        "font-weight: 600;"
        "color: #243746;"
    );

    auto *subtitle = new QLabel(
        "Assessment of whether the predicted DR severity requires referral"
    );
    subtitle->setStyleSheet(
        "font-size: 14px;"
        "color: #687984;"
    );

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // Patient context
    patientContextLabel = new QLabel(
        QString("Patient: %1   |   Eye: %2   |   Image: %3")
            .arg(patient.patientId)
            .arg(patient.eye)
            .arg(QFileInfo(patient.imagePath).fileName())
    );

    patientContextLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #52636D;"
    );

    mainLayout->addWidget(patientContextLabel);

    // Workflow
    workflowLabel = new QLabel(
        "PATIENT  →  IMAGE  →  QUALITY  →  CATARACT  →  PREPROCESSING  →  ANALYSIS  →  [ REFERABLE DR ]"
    );

    workflowLabel->setStyleSheet(
        "font-size: 14px;"
        "font-weight: 600;"
        "color: #4B8FA8;"
    );

    mainLayout->addWidget(workflowLabel);

    // Assessment card
    auto *assessmentCard = new QFrame;
    auto *assessmentLayout = new QVBoxLayout(assessmentCard);
    assessmentLayout->setContentsMargins(24, 22, 24, 22);
    assessmentLayout->setSpacing(14);

    auto *assessmentTitle = new QLabel("Referable DR Assessment");
    assessmentTitle->setStyleSheet(
        "font-size: 18px;"
        "font-weight: 600;"
        "color: #34454F;"
    );

    assessmentLayout->addWidget(assessmentTitle);

    // Probability
    auto *probabilityTitle = new QLabel("Referable Probability");
    probabilityTitle->setStyleSheet(
        "font-size: 14px;"
        "font-weight: 600;"
        "color: #687984;"
    );

    probabilityLabel = new QLabel(
        QString("%1%")
            .arg(patient.referableProbability, 0, 'f', 2)
    );

    probabilityLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 700;"
        "color: #243746;"
    );

    assessmentLayout->addWidget(probabilityTitle);
    assessmentLayout->addWidget(probabilityLabel);

    // Threshold
    auto *thresholdTitle = new QLabel("Decision Threshold");
    thresholdTitle->setStyleSheet(
        "font-size: 14px;"
        "font-weight: 600;"
        "color: #687984;"
    );

    thresholdLabel = new QLabel(
        QString("%1%")
            .arg(patient.referableThreshold, 0, 'f', 0)
    );

    thresholdLabel->setStyleSheet(
        "font-size: 22px;"
        "font-weight: 600;"
        "color: #34454F;"
    );

    assessmentLayout->addWidget(thresholdTitle);
    assessmentLayout->addWidget(thresholdLabel);

    // Decision
    auto *decisionTitle = new QLabel("Decision");
    decisionTitle->setStyleSheet(
        "font-size: 14px;"
        "font-weight: 600;"
        "color: #687984;"
    );

    decisionLabel = new QLabel(patient.referableDecision);
    decisionLabel->setStyleSheet(
        "font-size: 22px;"
        "font-weight: 700;"
        "color: #4B8FA8;"
    );

    assessmentLayout->addWidget(decisionTitle);
    assessmentLayout->addWidget(decisionLabel);

    mainLayout->addWidget(assessmentCard);

    // Status card
    auto *statusCard = new QFrame;
    auto *statusLayout = new QVBoxLayout(statusCard);
    statusLayout->setContentsMargins(24, 20, 24, 20);
    statusLayout->setSpacing(8);

    statusLabel = new QLabel(
        "✓ NON-REFERABLE DR"
    );

    statusLabel->setStyleSheet(
        "font-size: 22px;"
        "font-weight: 700;"
        "color: #4B8FA8;"
    );

    recommendationLabel = new QLabel(
        "Routine monitoring recommended."
    );

    recommendationLabel->setStyleSheet(
        "font-size: 15px;"
        "color: #52636D;"
    );

    statusLayout->addWidget(statusLabel);
    statusLayout->addWidget(recommendationLabel);

    mainLayout->addWidget(statusCard);

    mainLayout->addStretch();

    // Bottom action bar
    auto *actionLayout = new QHBoxLayout;
    actionLayout->addStretch();

    nextButton = new QPushButton("Continue to Explainability");

    connect(
        nextButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            emit assessmentCompleted();
        }
    );

    actionLayout->addWidget(nextButton);

    mainLayout->addLayout(actionLayout);
}
