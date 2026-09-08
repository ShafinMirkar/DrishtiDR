#include "AnalysisPage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

AnalysisPage::AnalysisPage(
    const PatientData &patient,
    QWidget *parent
)
    : QWidget(parent),
      patient(patient),
      progress(0),
      currentStep(0)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);

    setupUI();

    timer = new QTimer(this);

    connect(
        timer,
        &QTimer::timeout,
        this,
        &AnalysisPage::updateAnalysis
    );

    timer->start(350);
}

void AnalysisPage::setupUI()
{
    setStyleSheet(
        "QWidget {"
        "background: #FFFFFF;"
        "color: #243746;"
        "font-family: Sans Serif;"
        "}"
        "QLabel {"
        "background: transparent;"
        "color: #243746;"
        "}"
        "QPushButton {"
        "font-size: 14px;"
        "font-weight: 600;"
        "}"
    );

    auto *mainLayout = new QVBoxLayout(this);

    mainLayout->setContentsMargins(36, 24, 36, 24);
    mainLayout->setSpacing(12);

    // -------------------------------------------------
    // Title
    // -------------------------------------------------

    auto *title = new QLabel("Retinal Analysis");

    title->setStyleSheet(
        "font-size: 25px;"
        "font-weight: bold;"
        "color: #123B5D;"
    );

    mainLayout->addWidget(title);

    auto *subtitle = new QLabel(
        "Automated retinal structure, lesion and severity assessment"
    );

    subtitle->setStyleSheet(
        "font-size: 14px;"
        "color: #60717D;"
    );

    mainLayout->addWidget(subtitle);

    // -------------------------------------------------
    // Patient context
    // -------------------------------------------------

    patientContextLabel = new QLabel;

    patientContextLabel->setText(
        "Patient: " + patient.patientId +
        "   |   Name: " + patient.name +
        "   |   Age: " + patient.age +
        "   |   " + patient.eye
    );

    patientContextLabel->setStyleSheet(
        "background: #F4F7F9;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 5px;"
        "padding: 9px 12px;"
        "font-size: 13px;"
        "font-weight: 600;"
        "color: #34454F;"
    );

    mainLayout->addWidget(patientContextLabel);

    // -------------------------------------------------
    // Workflow
    // -------------------------------------------------

    workflowLabel = new QLabel(
        "Patient  →  Image  →  Quality  →  [ ANALYSIS ]  →  Result  →  Report"
    );

    workflowLabel->setStyleSheet(
        "font-size: 13px;"
        "font-weight: bold;"
        "color: #167D8D;"
        "padding: 4px 0;"
    );

    mainLayout->addWidget(workflowLabel);

    // -------------------------------------------------
    // Main content
    // -------------------------------------------------

    auto *content = new QHBoxLayout;
    content->setSpacing(20);

    // -------------------------------------------------
    // Image panel
    // -------------------------------------------------

    auto *imageFrame = new QFrame;

    imageFrame->setFixedSize(500, 400);

    imageFrame->setStyleSheet(
        "QFrame {"
        "background: #F8FAFB;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 7px;"
        "}"
    );

    auto *imageLayout = new QVBoxLayout(imageFrame);
    imageLayout->setContentsMargins(12, 12, 12, 12);

    auto *imageCaption = new QLabel("FUNDUS IMAGE");

    imageCaption->setAlignment(Qt::AlignCenter);
    imageCaption->setStyleSheet(
        "font-size: 13px;"
        "font-weight: bold;"
        "color: #52636D;"
    );

    imageLayout->addWidget(imageCaption);

    auto *image = new QLabel;

    image->setAlignment(Qt::AlignCenter);

    QPixmap pixmap(patient.imagePath);

    image->setPixmap(
        pixmap.scaled(
            460,
            345,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );

    imageLayout->addWidget(image, 1);

    content->addWidget(imageFrame);

    // -------------------------------------------------
    // Analysis panel
    // -------------------------------------------------

    auto *analysisFrame = new QFrame;

    analysisFrame->setStyleSheet(
        "QFrame {"
        "background: #F8FAFB;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 7px;"
        "}"
    );

    auto *panel = new QVBoxLayout(analysisFrame);

    panel->setContentsMargins(18, 18, 18, 18);
    panel->setSpacing(10);

    statusLabel = new QLabel("ANALYSIS IN PROGRESS");

    statusLabel->setStyleSheet(
        "font-size: 20px;"
        "font-weight: bold;"
        "color: #167D8D;"
    );

    panel->addWidget(statusLabel);

    stepLabel = new QLabel("Initializing analysis...");

    stepLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #34454F;"
        "padding: 4px 0;"
    );

    panel->addWidget(stepLabel);

    // -------------------------------------------------
    // Progress
    // -------------------------------------------------

    progressBar = new QProgressBar;

    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setMinimumHeight(28);

    progressBar->setStyleSheet(
        "QProgressBar {"
        "border: 1px solid #C7D3DA;"
        "border-radius: 5px;"
        "background: #FFFFFF;"
        "text-align: center;"
        "color: #34454F;"
        "}"
        "QProgressBar::chunk {"
        "background: #167D8D;"
        "border-radius: 4px;"
        "}"
    );

    panel->addWidget(progressBar);

    // -------------------------------------------------
    // Processing steps
    // -------------------------------------------------

    stepsLabel = new QLabel(
        "○ Image preprocessing\n"
        "○ Optic disc localization\n"
        "○ Vessel analysis\n"
        "○ Lesion detection\n"
        "○ DR severity assessment\n"
        "○ Explainability generation"
    );

    stepsLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #52636D;"
        "padding-top: 8px;"
        "line-height: 1.7;"
    );

    panel->addWidget(stepsLabel);

    panel->addStretch();

    content->addWidget(analysisFrame, 1);

    mainLayout->addLayout(content);

    // -------------------------------------------------
    // Bottom action bar
    // -------------------------------------------------

    auto *actionBar = new QFrame;

    actionBar->setStyleSheet(
        "QFrame {"
        "background: #F4F7F9;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 7px;"
        "}"
    );

    auto *actions = new QHBoxLayout(actionBar);

    actions->setContentsMargins(12, 10, 12, 10);

    actions->addStretch();

    resultButton = new QPushButton("View Result");

    resultButton->setMinimumSize(130, 40);
    resultButton->setEnabled(false);
    resultButton->setCursor(Qt::PointingHandCursor);

    resultButton->setStyleSheet(
        "QPushButton {"
        "background: #123B5D;"
        "color: white;"
        "border: none;"
        "border-radius: 5px;"
        "padding: 8px 24px;"
        "}"
        "QPushButton:hover {"
        "background: #0E304A;"
        "}"
        "QPushButton:disabled {"
        "background: #CBD5DA;"
        "color: #7A878E;"
        "}"
    );

    actions->addWidget(resultButton);

    mainLayout->addWidget(actionBar);

    connect(
        resultButton,
        &QPushButton::clicked,
        this,
        &AnalysisPage::analysisCompleted
    );
}

void AnalysisPage::updateAnalysis()
{
    progress += 10;

    progressBar->setValue(progress);

    if (progress < 20) {

        currentStep = 0;

        stepLabel->setText(
            "Image preprocessing..."
        );

        stepsLabel->setText(
            "◉ Image preprocessing\n"
            "○ Optic disc localization\n"
            "○ Vessel analysis\n"
            "○ Lesion detection\n"
            "○ DR severity assessment\n"
            "○ Explainability generation"
        );
    }
    else if (progress < 35) {

        currentStep = 1;

        stepLabel->setText(
            "Optic disc localization..."
        );

        stepsLabel->setText(
            "✓ Image preprocessing\n"
            "◉ Optic disc localization\n"
            "○ Vessel analysis\n"
            "○ Lesion detection\n"
            "○ DR severity assessment\n"
            "○ Explainability generation"
        );
    }
    else if (progress < 50) {

        currentStep = 2;

        stepLabel->setText(
            "Analyzing retinal vessels..."
        );

        stepsLabel->setText(
            "✓ Image preprocessing\n"
            "✓ Optic disc localization\n"
            "◉ Vessel analysis\n"
            "○ Lesion detection\n"
            "○ DR severity assessment\n"
            "○ Explainability generation"
        );
    }
    else if (progress < 70) {

        currentStep = 3;

        stepLabel->setText(
            "Detecting retinal lesions..."
        );

        stepsLabel->setText(
            "✓ Image preprocessing\n"
            "✓ Optic disc localization\n"
            "✓ Vessel analysis\n"
            "◉ Lesion detection\n"
            "○ DR severity assessment\n"
            "○ Explainability generation"
        );
    }
    else if (progress < 90) {

        currentStep = 4;

        stepLabel->setText(
            "Assessing DR severity..."
        );

        stepsLabel->setText(
            "✓ Image preprocessing\n"
            "✓ Optic disc localization\n"
            "✓ Vessel analysis\n"
            "✓ Lesion detection\n"
            "◉ DR severity assessment\n"
            "○ Explainability generation"
        );
    }
    else if (progress < 100) {

        currentStep = 5;

        stepLabel->setText(
            "Generating explainability results..."
        );

        stepsLabel->setText(
            "✓ Image preprocessing\n"
            "✓ Optic disc localization\n"
            "✓ Vessel analysis\n"
            "✓ Lesion detection\n"
            "✓ DR severity assessment\n"
            "◉ Explainability generation"
        );
    }
    else {

        timer->stop();

        currentStep = 6;

        stepLabel->setText(
            "Analysis completed successfully."
        );

        statusLabel->setText(
            "ANALYSIS COMPLETE"
        );

        statusLabel->setStyleSheet(
            "font-size: 20px;"
            "font-weight: bold;"
            "color: #238636;"
        );

        stepsLabel->setText(
            "✓ Image preprocessing\n"
            "✓ Optic disc localization\n"
            "✓ Vessel analysis\n"
            "✓ Lesion detection\n"
            "✓ DR severity assessment\n"
            "✓ Explainability generation"
        );

        resultButton->setEnabled(true);
    }
}