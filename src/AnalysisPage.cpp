#include "AnalysisPage.h"

#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

AnalysisPage::AnalysisPage(
    const PatientData &patient,
    QWidget *parent
)
    : QWidget(parent),
      patient(patient)
{
    setupUI();
}

void AnalysisPage::setupUI()
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
    auto *title = new QLabel("DR Severity Classification");
    title->setStyleSheet(
        "font-size: 25px;"
        "font-weight: 600;"
        "color: #243746;"
    );

    auto *subtitle = new QLabel(
        "Classification of diabetic retinopathy severity from the retinal image"
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
        "PATIENT  →  IMAGE  →  QUALITY  →  CATARACT  →  PREPROCESSING  →  [ ANALYSIS ]"
    );

    workflowLabel->setStyleSheet(
        "font-size: 14px;"
        "font-weight: 600;"
        "color: #4B8FA8;"
    );

    mainLayout->addWidget(workflowLabel);

    // Main result card
    auto *resultCard = new QFrame;
    auto *resultLayout = new QVBoxLayout(resultCard);
    resultLayout->setContentsMargins(24, 22, 24, 22);
    resultLayout->setSpacing(12);

    auto *resultTitle = new QLabel("Predicted DR Grade");
    resultTitle->setStyleSheet(
        "font-size: 16px;"
        "font-weight: 600;"
        "color: #52636D;"
    );

    gradeLabel = new QLabel(
        QString("Grade %1").arg(patient.drGrade)
    );

    gradeLabel->setStyleSheet(
        "font-size: 30px;"
        "font-weight: 700;"
        "color: #243746;"
    );

    severityLabel = new QLabel(
        patient.drSeverity
    );

    severityLabel->setStyleSheet(
        "font-size: 21px;"
        "font-weight: 600;"
        "color: #4B8FA8;"
    );

    resultLayout->addWidget(resultTitle);
    resultLayout->addWidget(gradeLabel);
    resultLayout->addWidget(severityLabel);

    mainLayout->addWidget(resultCard);

    // Confidence + probabilities
    auto *detailsLayout = new QHBoxLayout;
    detailsLayout->setSpacing(18);

    // Confidence card
    auto *confidenceCard = new QFrame;
    auto *confidenceLayout = new QVBoxLayout(confidenceCard);
    confidenceLayout->setContentsMargins(20, 20, 20, 20);

    auto *confidenceTitle = new QLabel("Prediction Confidence");
    confidenceTitle->setStyleSheet(
        "font-size: 16px;"
        "font-weight: 600;"
        "color: #52636D;"
    );

    confidenceLabel = new QLabel(
        QString("%1%").arg(patient.predictionConfidence, 0, 'f', 2)
    );

    confidenceLabel->setStyleSheet(
        "font-size: 28px;"
        "font-weight: 700;"
        "color: #243746;"
    );

    confidenceLayout->addWidget(confidenceTitle);
    confidenceLayout->addWidget(confidenceLabel);
    confidenceLayout->addStretch();

    // Probability card
    auto *probabilityCard = new QFrame;
    auto *probabilityLayout = new QVBoxLayout(probabilityCard);
    probabilityLayout->setContentsMargins(20, 20, 20, 20);
    probabilityLayout->setSpacing(9);

    auto *probabilityTitle = new QLabel("Class Probabilities");
    probabilityTitle->setStyleSheet(
        "font-size: 16px;"
        "font-weight: 600;"
        "color: #52636D;"
    );

    probabilityLayout->addWidget(probabilityTitle);

    auto addProbability =
        [&](const QString &name,
            double probability,
            QLabel *&label)
    {
        label = new QLabel(
            QString("%1   %2%")
                .arg(name)
                .arg(probability, 0, 'f', 2)
        );

        label->setStyleSheet(
            "font-size: 15px;"
            "color: #34454F;"
        );

        probabilityLayout->addWidget(label);
    };

    addProbability(
        "No DR",
        patient.probabilityNoDR,
        noDrProbabilityLabel
    );

    addProbability(
        "Mild DR",
        patient.probabilityMildDR,
        mildProbabilityLabel
    );

    addProbability(
        "Moderate DR",
        patient.probabilityModerateDR,
        moderateProbabilityLabel
    );

    addProbability(
        "Severe DR",
        patient.probabilitySevereDR,
        severeProbabilityLabel
    );

    addProbability(
        "Proliferative DR",
        patient.probabilityProliferativeDR,
        proliferativeProbabilityLabel
    );

    detailsLayout->addWidget(confidenceCard, 1);
    detailsLayout->addWidget(probabilityCard, 2);

    mainLayout->addLayout(detailsLayout);

    // Bottom action bar
    auto *actionLayout = new QHBoxLayout;
    actionLayout->addStretch();

    nextButton = new QPushButton("Continue to Referable DR");

    connect(
        nextButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            emit analysisCompleted();
        }
    );

    actionLayout->addWidget(nextButton);

    mainLayout->addLayout(actionLayout);
}