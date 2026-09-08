#include "ResultPage.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

ResultPage::ResultPage(
    const PatientData &patient,
    QWidget *parent
)
    : QWidget(parent),
      patient(patient)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);

    setupUI();
}

void ResultPage::setupUI()
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

    auto *title = new QLabel("Screening Result");

    title->setStyleSheet(
        "font-size: 25px;"
        "font-weight: bold;"
        "color: #123B5D;"
    );

    mainLayout->addWidget(title);

    auto *subtitle = new QLabel(
        "AI-assisted diabetic retinopathy screening result and clinical evidence"
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
        "Patient  →  Image  →  Quality  →  Analysis  →  [ RESULT ]  →  Report"
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
    // Fundus image
    // -------------------------------------------------

    auto *imageFrame = new QFrame;

    imageFrame->setFixedSize(500, 430);

    imageFrame->setStyleSheet(
        "QFrame {"
        "background: #F8FAFB;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 7px;"
        "}"
    );

    auto *imageLayout = new QVBoxLayout(imageFrame);

    imageLayout->setContentsMargins(12, 12, 12, 12);
    imageLayout->setSpacing(8);

    auto *imageCaption = new QLabel("FUNDUS IMAGE");

    imageCaption->setAlignment(Qt::AlignCenter);

    imageCaption->setStyleSheet(
        "font-size: 13px;"
        "font-weight: bold;"
        "color: #52636D;"
    );

    imageLayout->addWidget(imageCaption);

    imageLabel = new QLabel;

    imageLabel->setAlignment(Qt::AlignCenter);

    QPixmap pixmap(patient.imagePath);

    imageLabel->setPixmap(
        pixmap.scaled(
            460,
            365,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );

    imageLayout->addWidget(imageLabel, 1);

    content->addWidget(imageFrame);

    // -------------------------------------------------
    // Result information
    // -------------------------------------------------

    auto *resultFrame = new QFrame;

    resultFrame->setStyleSheet(
        "QFrame {"
        "background: #F8FAFB;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 7px;"
        "}"
    );

    auto *resultLayout = new QVBoxLayout(resultFrame);

    resultLayout->setContentsMargins(18, 18, 18, 18);
    resultLayout->setSpacing(10);

    // Grade
    gradeLabel = new QLabel("DR GRADE: LEVEL 2");

    gradeLabel->setStyleSheet(
        "font-size: 21px;"
        "font-weight: bold;"
        "color: #B7791F;"
    );

    resultLayout->addWidget(gradeLabel);

    // Severity
    severityLabel = new QLabel(
        "Moderate Non-Proliferative Diabetic Retinopathy"
    );

    severityLabel->setWordWrap(true);

    severityLabel->setStyleSheet(
        "font-size: 16px;"
        "font-weight: bold;"
        "color: #34454F;"
    );

    resultLayout->addWidget(severityLabel);

    // Confidence
    confidenceLabel = new QLabel(
        "Confidence: 93%"
    );

    confidenceLabel->setStyleSheet(
        "font-size: 14px;"
        "color: #34454F;"
    );

    resultLayout->addWidget(confidenceLabel);

    // Referable
    referableLabel = new QLabel(
        "REFERABLE DR: YES"
    );

    referableLabel->setStyleSheet(
        "font-size: 15px;"
        "font-weight: bold;"
        "color: #B42318;"
        "padding: 5px 0;"
    );

    resultLayout->addWidget(referableLabel);

    // -------------------------------------------------
    // Clinical Evidence
    // -------------------------------------------------

    auto *evidenceFrame = new QFrame;

    evidenceFrame->setStyleSheet(
        "QFrame {"
        "background: #FFFFFF;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 5px;"
        "}"
    );

    auto *evidenceLayout = new QVBoxLayout(evidenceFrame);

    evidenceLayout->setContentsMargins(12, 10, 12, 10);

    auto *evidenceTitle = new QLabel(
        "Clinical Evidence"
    );

    evidenceTitle->setStyleSheet(
        "font-size: 15px;"
        "font-weight: bold;"
        "color: #123B5D;"
    );

    evidenceLayout->addWidget(evidenceTitle);

    evidenceLabel = new QLabel(
        "• Microaneurysms detected\n"
        "• Retinal hemorrhages detected\n"
        "• Hard exudates detected"
    );

    evidenceLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #52636D;"
        "padding-top: 4px;"
    );

    evidenceLayout->addWidget(evidenceLabel);

    resultLayout->addWidget(evidenceFrame);

    // -------------------------------------------------
    // Explainability
    // -------------------------------------------------

    auto *explainFrame = new QFrame;

    explainFrame->setStyleSheet(
        "QFrame {"
        "background: #FFFFFF;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 5px;"
        "}"
    );

    auto *explainLayout = new QVBoxLayout(explainFrame);

    explainLayout->setContentsMargins(12, 10, 12, 10);

    auto *explainTitle = new QLabel(
        "Explainability"
    );

    explainTitle->setStyleSheet(
        "font-size: 15px;"
        "font-weight: bold;"
        "color: #123B5D;"
    );

    explainLayout->addWidget(explainTitle);

    gradcamLabel = new QLabel(
        "✓ Grad-CAM attention map generated"
    );

    lesionLabel = new QLabel(
        "✓ Lesion-level evidence available"
    );

    gradcamLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #167D8D;"
        "padding-top: 3px;"
    );

    lesionLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #167D8D;"
        "padding-top: 3px;"
    );

    explainLayout->addWidget(gradcamLabel);
    explainLayout->addWidget(lesionLabel);

    resultLayout->addWidget(explainFrame);

    resultLayout->addStretch();

    content->addWidget(resultFrame, 1);

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

    reportButton = new QPushButton(
        "Generate Report"
    );

    reportButton->setMinimumSize(150, 40);
    reportButton->setCursor(Qt::PointingHandCursor);

    reportButton->setStyleSheet(
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
    );

    actions->addWidget(reportButton);

    mainLayout->addWidget(actionBar);

    connect(
        reportButton,
        &QPushButton::clicked,
        this,
        &ResultPage::reportRequested
    );
}