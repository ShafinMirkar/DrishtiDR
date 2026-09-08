#include "ReportPage.h"

#include <QDate>
#include <QDir>
#include <QFileDialog>
#include <QFont>
#include <QLabel>
#include <QPainter>
#include <QPdfWriter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QPageSize>
#include <QScrollArea>

ReportPage::ReportPage(
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

void ReportPage::setupUI()
{
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);

    setStyleSheet(
        "QWidget {"
        "background-color: #FFFFFF;"
        "color: #243746;"
        "font-size: 14px;"
        "}"
        "QLabel {"
        "color: #243746;"
        "}"
        "QPushButton {"
        "background-color: #164E70;"
        "color: white;"
        "border: none;"
        "border-radius: 5px;"
        "padding: 9px 18px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #1B638D;"
        "}"
    );

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 24, 32, 0);
    mainLayout->setSpacing(10);

    // -------------------------------------------------
    // HEADER
    // -------------------------------------------------

    auto *title = new QLabel("Screening Report");
    title->setStyleSheet(
        "font-size: 23px;"
        "font-weight: bold;"
        "color: #164E70;"
    );

    auto *subtitle = new QLabel(
        "Review and generate the diabetic retinopathy screening report"
    );
    subtitle->setStyleSheet(
        "color: #607D8B;"
        "font-size: 13px;"
    );

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    patientContextLabel = new QLabel(
        "Patient: " +
        patient.patientId +
        "  |  Name: " +
        patient.name +
        "  |  Age: " +
        patient.age +
        "  |  " +
        patient.eye
    );

    patientContextLabel->setStyleSheet(
        "background-color: #F4F7F9;"
        "border: 1px solid #D7E0E5;"
        "border-radius: 5px;"
        "padding: 8px;"
        "font-weight: bold;"
        "color: #34454F;"
    );

    mainLayout->addWidget(patientContextLabel);

    workflowLabel = new QLabel(
        "Patient → Image → Quality → Analysis → Result → [ REPORT ]"
    );

    workflowLabel->setStyleSheet(
        "color: #087E8B;"
        "font-weight: bold;"
        "padding: 3px;"
    );

    mainLayout->addWidget(workflowLabel);

    // -------------------------------------------------
    // SCROLLABLE REPORT AREA
    // -------------------------------------------------

    auto *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    auto *reportContainer = new QWidget;

    auto *reportLayout = new QVBoxLayout(reportContainer);
    reportLayout->setContentsMargins(0, 4, 0, 12);
    reportLayout->setSpacing(10);

    // Report number
    auto *reportNumberFrame = new QFrame;
    reportNumberFrame->setStyleSheet(
        "QFrame {"
        "background-color: #F4F7F9;"
        "border: 1px solid #D7E0E5;"
        "border-radius: 5px;"
        "}"
    );

    auto *reportNumberLayout = new QHBoxLayout(reportNumberFrame);
    reportNumberLayout->setContentsMargins(12, 8, 12, 8);

    auto *reportNumberTitle = new QLabel("Report No.");
    reportNumberTitle->setStyleSheet(
        "font-weight: bold;"
        "color: #607D8B;"
    );

    reportNumberLabel = new QLabel(patient.reportNumber);
    reportNumberLabel->setStyleSheet(
        "font-weight: bold;"
        "color: #164E70;"
    );

    reportNumberLayout->addWidget(reportNumberTitle);
    reportNumberLayout->addWidget(reportNumberLabel);
    reportNumberLayout->addStretch();

    reportLayout->addWidget(reportNumberFrame);

    // Actual report
    reportInfo = new QLabel;

    reportInfo->setText(
        "<b>Diabetic Retinopathy Screening Report</b><br><br>"

        "<b>Patient Information</b><br>"
        "Patient ID: " + patient.patientId + "<br>"
        "Name: " + patient.name + "<br>"
        "Age: " + patient.age + "<br>"
        "Sex: " + patient.sex + "<br>"
        "Eye: " + patient.eye + "<br>"
        "Diabetes Duration: " + patient.diabetesDuration + "<br><br>"

        "<b>Image Quality</b><br>"
        "Quality: Good<br>"
        "Overall Quality: 91%<br>"
        "Focus: Good<br>"
        "Illumination: Good<br>"
        "Field of View: Good<br><br>"

        "<b>DR Assessment</b><br>"
        "Grade: Level 2<br>"
        "Severity: Moderate Non-Proliferative Diabetic Retinopathy<br>"
        "Confidence: 93%<br>"
        "Referable DR: YES<br><br>"

        "<b>Clinical Evidence</b><br>"
        "• Microaneurysms detected<br>"
        "• Retinal hemorrhages detected<br>"
        "• Hard exudates detected<br><br>"

        "<b>Explainability</b><br>"
        "• Grad-CAM attention map generated<br>"
        "• Lesion-level evidence available<br><br>"

        "<b>Recommendation</b><br>"
        "Refer patient for ophthalmological evaluation."
    );

    reportInfo->setWordWrap(true);
    reportInfo->setTextInteractionFlags(Qt::NoTextInteraction);

    reportInfo->setStyleSheet(
        "QLabel {"
        "background-color: #FFFFFF;"
        "border: 1px solid #D7E0E5;"
        "border-radius: 6px;"
        "padding: 18px;"
        "color: #243746;"
        "font-size: 14px;"
        "}"
    );

    reportInfo->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Preferred
    );

    reportLayout->addWidget(reportInfo);

    // Allow future sections to grow without moving the button
    reportLayout->addStretch();

    scrollArea->setWidget(reportContainer);

    mainLayout->addWidget(scrollArea, 1);

    // -------------------------------------------------
    // FIXED BOTTOM ACTION BAR
    // -------------------------------------------------

    auto *actionBar = new QFrame;

    actionBar->setStyleSheet(
        "QFrame {"
        "background-color: #F4F7F9;"
        "border: 1px solid #D7E0E5;"
        "border-radius: 5px;"
        "}"
    );

    actionBar->setFixedHeight(54);

    auto *actionLayout = new QHBoxLayout(actionBar);
    actionLayout->setContentsMargins(12, 7, 12, 7);

    statusLabel = new QLabel("Report ready to generate.");
    statusLabel->setStyleSheet(
        "color: #607D8B;"
        "font-size: 13px;"
    );

    generateButton = new QPushButton("Generate PDF Report");
    generateButton->setFixedHeight(36);

    connect(
        generateButton,
        &QPushButton::clicked,
        this,
        &ReportPage::generatePDF
    );

    actionLayout->addWidget(statusLabel);
    actionLayout->addStretch();
    actionLayout->addWidget(generateButton);

    mainLayout->addWidget(actionBar);
}

void ReportPage::generatePDF()
{
    QString reportsDir =
        QDir::currentPath() + "/reports";

    QDir().mkpath(reportsDir);

    QString fileName =
        "DrishtiDx_Report_" +
        patient.reportNumber +
        ".pdf";

    QString defaultPath =
        reportsDir + "/" + fileName;

    QString filePath =
        QFileDialog::getSaveFileName(
            this,
            "Save Screening Report",
            defaultPath,
            "PDF Files (*.pdf)"
        );

    if (filePath.isEmpty())
        return;

    if (!filePath.endsWith(".pdf",
                           Qt::CaseInsensitive)) {
        filePath += ".pdf";
    }

    QPdfWriter pdf(filePath);

    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(96);

    QPainter painter(&pdf);

    int y = 70;

    // -------------------------------------------------
    // Header
    // -------------------------------------------------

    painter.setFont(
        QFont("Arial", 22, QFont::Bold)
    );

    painter.drawText(
        60,
        y,
        "DrishtiDx"
    );

    y += 35;

    painter.setFont(
        QFont("Arial", 16, QFont::Bold)
    );

    painter.drawText(
        60,
        y,
        "Diabetic Retinopathy Screening Report"
    );

    y += 35;

    painter.setFont(
        QFont("Arial", 10)
    );

    painter.drawText(
        60,
        y,
        "Report No.: " + patient.reportNumber
    );

    y += 25;

    painter.drawText(
        60,
        y,
        "Date: " +
        QDate::currentDate()
            .toString("dd MMMM yyyy")
    );

    y += 40;

    // -------------------------------------------------
    // Patient Information
    // -------------------------------------------------

    painter.setFont(
        QFont("Arial", 14, QFont::Bold)
    );

    painter.drawText(
        60,
        y,
        "Patient Information"
    );

    y += 25;

    painter.setFont(
        QFont("Arial", 11)
    );

    painter.drawText(
        60,
        y,
        "Patient ID: " + patient.patientId
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Name: " + patient.name
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Age: " + patient.age +
        "     Sex: " + patient.sex
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Eye: " + patient.eye
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Diabetes Duration: " +
        patient.diabetesDuration
    );

    y += 40;

    // -------------------------------------------------
    // Image Quality
    // -------------------------------------------------

    painter.setFont(
        QFont("Arial", 14, QFont::Bold)
    );

    painter.drawText(
        60,
        y,
        "Image Quality"
    );

    y += 25;

    painter.setFont(
        QFont("Arial", 11)
    );

    painter.drawText(
        60,
        y,
        "Quality: GOOD"
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Overall Quality: 91%"
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Focus: Good"
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Illumination: Good"
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Field of View: Good"
    );

    y += 40;

    // -------------------------------------------------
    // DR Assessment
    // -------------------------------------------------

    painter.setFont(
        QFont("Arial", 14, QFont::Bold)
    );

    painter.drawText(
        60,
        y,
        "DR Assessment"
    );

    y += 25;

    painter.setFont(
        QFont("Arial", 11)
    );

    painter.drawText(
        60,
        y,
        "DR Grade: Level 2"
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Severity: Moderate Non-Proliferative "
        "Diabetic Retinopathy"
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Confidence: 93%"
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "Referable DR: YES"
    );

    y += 40;

    // -------------------------------------------------
    // Clinical Evidence
    // -------------------------------------------------

    painter.setFont(
        QFont("Arial", 14, QFont::Bold)
    );

    painter.drawText(
        60,
        y,
        "Clinical Evidence"
    );

    y += 25;

    painter.setFont(
        QFont("Arial", 11)
    );

    painter.drawText(
        60,
        y,
        "• Microaneurysms detected"
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "• Retinal hemorrhages detected"
    );

    y += 22;

    painter.drawText(
        60,
        y,
        "• Hard exudates detected"
    );

    y += 40;

    // -------------------------------------------------
    // Recommendation
    // -------------------------------------------------

    painter.setFont(
        QFont("Arial", 14, QFont::Bold)
    );

    painter.drawText(
        60,
        y,
        "Recommendation"
    );

    y += 25;

    painter.setFont(
        QFont("Arial", 11)
    );

    painter.drawText(
        60,
        y,
        "Refer patient for ophthalmological evaluation."
    );

    y += 40;

    painter.setFont(
        QFont("Arial", 9)
    );

    painter.drawText(
        60,
        y,
        "Generated by DrishtiDx screening prototype."
    );

    painter.end();

    statusLabel->setText(
        "PDF report generated successfully:\n" +
        filePath
    );
}