#include "ReportPage.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPdfWriter>
#include <QPixmap>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

namespace
{
QString assetPath(const QString &relativePath)
{
    // Executable is inside build/, assets are one level above it.
    QString path =
        QCoreApplication::applicationDirPath()
        + "/../"
        + relativePath;

    return QDir::cleanPath(path);
}

QLabel *sectionTitle(const QString &text)
{
    auto *label = new QLabel(text);

    label->setStyleSheet(
        "font-size: 18px;"
        "font-weight: 600;"
        "color: #243746;"
        "border: none;"
    );

    return label;
}

QLabel *bodyLabel(const QString &text)
{
    auto *label = new QLabel(text);

    label->setWordWrap(true);

    label->setStyleSheet(
        "font-size: 14px;"
        "color: #52636D;"
        "border: none;"
    );

    return label;
}

QFrame *sectionCard()
{
    auto *card = new QFrame;

    card->setStyleSheet(
        "QFrame {"
        "background-color: #FFFFFF;"
        "border: 1px solid #D7E0E5;"
        "border-radius: 8px;"
        "}"
    );

    return card;
}

QLabel *imageLabel(
    const QString &path,
    int width = 260,
    int height = 200
)
{
    auto *label = new QLabel;

    label->setAlignment(Qt::AlignCenter);
    label->setMinimumSize(width, height);

    QPixmap pixmap(path);

    if (!pixmap.isNull())
    {
        label->setPixmap(
            pixmap.scaled(
                width,
                height,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            )
        );
    }
    else
    {
        label->setText("Image unavailable");
    }

    label->setStyleSheet(
        "background-color: #F7F9FA;"
        "border: none;"
        "color: #8797A2;"
    );

    return label;
}
}

ReportPage::ReportPage(
    const PatientData &patient,
    QWidget *parent
)
    : QWidget(parent),
      patient(patient)
{
    setupUI();
}

void ReportPage::setupUI()
{
    setAttribute(Qt::WA_StyledBackground, true);

    setStyleSheet(
        "QWidget {"
        "background-color: #FFFFFF;"
        "color: #243746;"
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

    mainLayout->setContentsMargins(32, 24, 32, 16);
    mainLayout->setSpacing(14);

    // ---------------------------------------------------------
    // Header
    // ---------------------------------------------------------

    auto *title = new QLabel(
        "DIABETIC RETINOPATHY SCREENING REPORT"
    );

    title->setStyleSheet(
        "font-size: 25px;"
        "font-weight: 700;"
        "color: #243746;"
    );

    auto *subtitle = new QLabel(
        "Explainable AI Screening Prototype"
    );

    subtitle->setStyleSheet(
        "font-size: 14px;"
        "color: #687984;"
    );

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    auto *patientContext = new QLabel(
        QString(
            "Patient: %1   |   Eye: %2   |   Image: %3   |   Report: %4"
        )
        .arg(patient.patientId)
        .arg(patient.eye)
        .arg(QFileInfo(patient.imagePath).fileName())
        .arg(patient.reportNumber)
    );

    patientContext->setStyleSheet(
        "font-size: 14px;"
        "color: #52636D;"
    );

    mainLayout->addWidget(patientContext);

    // ---------------------------------------------------------
    // Scrollable report
    // ---------------------------------------------------------

    auto *scrollArea = new QScrollArea;

    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto *content = new QWidget;

    auto *contentLayout = new QVBoxLayout(content);

    contentLayout->setContentsMargins(0, 4, 8, 20);
    contentLayout->setSpacing(14);

    // =========================================================
    // 1. IMAGE QUALITY
    // =========================================================

    auto *qualityCard = sectionCard();
    auto *qualityLayout = new QVBoxLayout(qualityCard);

    qualityLayout->setContentsMargins(20, 18, 20, 18);
    qualityLayout->setSpacing(8);

    qualityLayout->addWidget(
        sectionTitle("1. Image Quality Assessment")
    );

    auto *qualityStatus = bodyLabel(
        "IMAGE QUALITY: " + patient.qualityStatus
    );

    qualityStatus->setStyleSheet(
        "font-size: 16px;"
        "font-weight: 600;"
        "color: #4B8FA8;"
        "border: none;"
    );

    qualityLayout->addWidget(qualityStatus);

    qualityLayout->addWidget(
        bodyLabel(
            QString("Focus: %1").arg(patient.focusScore, 0, 'f', 4)
        )
    );

    qualityLayout->addWidget(
        bodyLabel(
            QString("Brightness: %1")
                .arg(patient.brightnessScore, 0, 'f', 4)
        )
    );

    qualityLayout->addWidget(
        bodyLabel(
            QString("Contrast: %1")
                .arg(patient.contrastScore, 0, 'f', 4)
        )
    );

    qualityLayout->addWidget(
        bodyLabel(
            QString("FOV Ratio: %1")
                .arg(patient.fovScore, 0, 'f', 4)
        )
    );

    qualityLayout->addWidget(
        bodyLabel(
            QString("Illumination: %1")
                .arg(patient.illuminationScore, 0, 'f', 4)
        )
    );

    contentLayout->addWidget(qualityCard);

    // =========================================================
    // 2. DR CLASSIFICATION
    // =========================================================

    auto *classificationCard = sectionCard();
    auto *classificationLayout =
        new QVBoxLayout(classificationCard);

    classificationLayout->setContentsMargins(20, 18, 20, 18);
    classificationLayout->setSpacing(8);

    classificationLayout->addWidget(
        sectionTitle("2. DR Classification")
    );

    classificationLayout->addWidget(
        bodyLabel(
            QString("Predicted Grade: Grade %1")
                .arg(patient.drGrade)
        )
    );

    classificationLayout->addWidget(
        bodyLabel(
            "Severity: " + patient.drSeverity
        )
    );

    classificationLayout->addWidget(
        bodyLabel(
            QString("Model Confidence: %1%")
                .arg(patient.predictionConfidence, 0, 'f', 2)
        )
    );

    contentLayout->addWidget(classificationCard);

    // =========================================================
    // 3. CLASS PROBABILITIES
    // =========================================================

    auto *probabilityCard = sectionCard();
    auto *probabilityLayout =
        new QVBoxLayout(probabilityCard);

    probabilityLayout->setContentsMargins(20, 18, 20, 18);
    probabilityLayout->setSpacing(7);

    probabilityLayout->addWidget(
        sectionTitle("3. Class Probabilities")
    );

    probabilityLayout->addWidget(
        bodyLabel(
            QString("No DR: %1%")
                .arg(patient.probabilityNoDR, 0, 'f', 2)
        )
    );

    probabilityLayout->addWidget(
        bodyLabel(
            QString("Mild DR: %1%")
                .arg(patient.probabilityMildDR, 0, 'f', 2)
        )
    );

    probabilityLayout->addWidget(
        bodyLabel(
            QString("Moderate DR: %1%")
                .arg(patient.probabilityModerateDR, 0, 'f', 2)
        )
    );

    probabilityLayout->addWidget(
        bodyLabel(
            QString("Severe DR: %1%")
                .arg(patient.probabilitySevereDR, 0, 'f', 2)
        )
    );

    probabilityLayout->addWidget(
        bodyLabel(
            QString("Proliferative DR: %1%")
                .arg(patient.probabilityProliferativeDR, 0, 'f', 2)
        )
    );

    contentLayout->addWidget(probabilityCard);

    // =========================================================
    // 4. REFERABLE DR
    // =========================================================

    auto *referableCard = sectionCard();
    auto *referableLayout =
        new QVBoxLayout(referableCard);

    referableLayout->setContentsMargins(20, 18, 20, 18);
    referableLayout->setSpacing(8);

    referableLayout->addWidget(
        sectionTitle("4. Referable DR Assessment")
    );

    referableLayout->addWidget(
        bodyLabel(
            QString("Referable Probability: %1%")
                .arg(patient.referableProbability, 0, 'f', 2)
        )
    );

    referableLayout->addWidget(
        bodyLabel(
            QString("Decision Threshold: %1%")
                .arg(patient.referableThreshold, 0, 'f', 0)
        )
    );

    referableLayout->addWidget(
        bodyLabel(
            "Decision: " + patient.referableDecision
        )
    );

    auto *referableStatus = bodyLabel(
        "✓ NON-REFERABLE DR"
    );

    referableStatus->setStyleSheet(
        "font-size: 17px;"
        "font-weight: 700;"
        "color: #4B8FA8;"
        "border: none;"
    );

    referableLayout->addWidget(referableStatus);

    referableLayout->addWidget(
        bodyLabel(
            "Routine monitoring recommended."
        )
    );

    contentLayout->addWidget(referableCard);

    // =========================================================
    // 5. EXPLAINABILITY
    // =========================================================

    auto *explainabilityCard = sectionCard();
    auto *explainabilityLayout =
        new QVBoxLayout(explainabilityCard);

    explainabilityLayout->setContentsMargins(20, 18, 20, 18);
    explainabilityLayout->setSpacing(10);

    explainabilityLayout->addWidget(
        sectionTitle("5. Explainability — Grad-CAM")
    );

    explainabilityLayout->addWidget(
        bodyLabel(
            "Grad-CAM highlights image regions that contributed "
            "to the predicted DR class. The visualization provides "
            "model-level evidence and is not a definitive lesion diagnosis."
        )
    );

    auto *imagesLayout = new QHBoxLayout;

    auto *fundusColumn = new QVBoxLayout;

    auto *fundusTitle = bodyLabel(
        "Preprocessed Fundus"
    );

    fundusTitle->setStyleSheet(
        "font-size: 15px;"
        "font-weight: 600;"
        "color: #34454F;"
        "border: none;"
    );

    fundusColumn->addWidget(fundusTitle);

    fundusColumn->addWidget(
        imageLabel(
            assetPath(
                "assets/preprocessing/prototype_preprocessed.png"
            )
        )
    );

    auto *gradcamColumn = new QVBoxLayout;

    auto *gradcamTitle = bodyLabel(
        "Grad-CAM Overlay"
    );

    gradcamTitle->setStyleSheet(
        "font-size: 15px;"
        "font-weight: 600;"
        "color: #34454F;"
        "border: none;"
    );

    gradcamColumn->addWidget(gradcamTitle);

    gradcamColumn->addWidget(
        imageLabel(
            assetPath(
                "assets/gradcam/prototype_gradcam.png"
            )
        )
    );

    auto *attentionColumn = new QVBoxLayout;

    auto *attentionTitle = bodyLabel(
        "Attention Heatmap"
    );

    attentionTitle->setStyleSheet(
        "font-size: 15px;"
        "font-weight: 600;"
        "color: #34454F;"
        "border: none;"
    );

    attentionColumn->addWidget(attentionTitle);

    attentionColumn->addWidget(
        imageLabel(
            assetPath(
                "assets/attention/prototype_attention.png"
            )
        )
    );

    imagesLayout->addLayout(fundusColumn);
    imagesLayout->addLayout(gradcamColumn);
    imagesLayout->addLayout(attentionColumn);

    explainabilityLayout->addLayout(imagesLayout);

    contentLayout->addWidget(explainabilityCard);

    // =========================================================
    // 6. PREPROCESSING
    // =========================================================

    auto *preprocessingCard = sectionCard();
    auto *preprocessingLayout =
        new QVBoxLayout(preprocessingCard);

    preprocessingLayout->setContentsMargins(20, 18, 20, 18);
    preprocessingLayout->setSpacing(7);

    preprocessingLayout->addWidget(
        sectionTitle("6. MATLAB Preprocessing")
    );

    preprocessingLayout->addWidget(
        bodyLabel("✓ Retinal field detection and cropping")
    );

    preprocessingLayout->addWidget(
        bodyLabel("✓ Illumination correction")
    );

    preprocessingLayout->addWidget(
        bodyLabel("✓ CLAHE enhancement")
    );

    preprocessingLayout->addWidget(
        bodyLabel("✓ Mild denoising")
    );

    preprocessingLayout->addWidget(
        bodyLabel("✓ Resizing to 224 × 224")
    );

    contentLayout->addWidget(preprocessingCard);

    // =========================================================
    // 7. SCREENING SUMMARY
    // =========================================================

    auto *summaryCard = sectionCard();
    auto *summaryLayout =
        new QVBoxLayout(summaryCard);

    summaryLayout->setContentsMargins(20, 18, 20, 18);
    summaryLayout->setSpacing(7);

    summaryLayout->addWidget(
        sectionTitle("7. Screening Summary")
    );

    summaryLayout->addWidget(
        bodyLabel(
            QString("Predicted Grade: Grade %1")
                .arg(patient.drGrade)
        )
    );

    summaryLayout->addWidget(
        bodyLabel(
            "Severity: " + patient.drSeverity
        )
    );

    summaryLayout->addWidget(
        bodyLabel(
            QString("Model Confidence: %1%")
                .arg(patient.predictionConfidence, 0, 'f', 2)
        )
    );

    summaryLayout->addWidget(
        bodyLabel(
            QString("Referable Probability: %1%")
                .arg(patient.referableProbability, 0, 'f', 2)
        )
    );

    summaryLayout->addWidget(
        bodyLabel(
            "Decision: " + patient.referableDecision
        )
    );

    summaryLayout->addWidget(
        bodyLabel(
            "Recommendation: Routine monitoring recommended."
        )
    );

    contentLayout->addWidget(summaryCard);

    // =========================================================
    // Disclaimer
    // =========================================================

    auto *disclaimer = bodyLabel(
        "Research Prototype — Not a Final Clinical Diagnosis\n"
        "Analysis completed successfully."
    );

    disclaimer->setStyleSheet(
        "font-size: 14px;"
        "font-weight: 600;"
        "color: #687984;"
        "border: none;"
    );
contentLayout->addWidget(disclaimer);

contentLayout->addStretch();

scrollArea->setWidget(content);

mainLayout->addWidget(scrollArea, 1);

// Status message shown after PDF generation
statusLabel = new QLabel;

statusLabel->setStyleSheet(
    "font-size: 14px;"
    "color: #4B8FA8;"
    "border: none;"
);

statusLabel->setAlignment(Qt::AlignCenter);

mainLayout->addWidget(statusLabel);

// ---------------------------------------------------------
// Bottom action bar
// ---------------------------------------------------------

auto *actionLayout = new QHBoxLayout;

backButton = new QPushButton(
    "Back to Main Window"
);
backButton->setStyleSheet(
    "QPushButton {"
    "background-color: #FFFFFF;"
    "color: #4B8FA8;"
    "border: 1px solid #4B8FA8;"
    "border-radius: 5px;"
    "padding: 10px 22px;"
    "font-size: 15px;"
    "}"
    "QPushButton:hover {"
    "background-color: #F2F7F9;"
    "}"
);

connect(
    backButton,
    &QPushButton::clicked,
    this,
    &ReportPage::backToMainRequested
);

generateButton = new QPushButton(
    "Generate PDF Report"
);

connect(
    generateButton,
    &QPushButton::clicked,
    this,
    &ReportPage::generatePDF
);

actionLayout->addWidget(backButton);
actionLayout->addStretch();
actionLayout->addWidget(generateButton);

mainLayout->addLayout(actionLayout);
}

void ReportPage::generatePDF()  
{
    QString reportsDir =
        QCoreApplication::applicationDirPath() + "/../reports";

    QDir().mkpath(reportsDir);

    QString fileName =
        "DrishtiDx_Report_" +
        patient.reportNumber +
        ".pdf";

    QString filePath =
        QDir::cleanPath(
            reportsDir + "/" + fileName
        );

    QPdfWriter pdf(filePath);

    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(150);

    QPainter painter(&pdf);

    painter.setRenderHint(QPainter::Antialiasing);

    const int pageWidth = pdf.width();
    const int margin = 70;

    int y = margin;

    QFont titleFont("Arial", 18, QFont::Bold);
    QFont headingFont("Arial", 13, QFont::Bold);
    QFont normalFont("Arial", 10);

    painter.setFont(titleFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        40,
        Qt::AlignLeft,
        "DIABETIC RETINOPATHY SCREENING REPORT"
    );

    y += 45;

    painter.setFont(normalFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "Explainable AI Screening Prototype"
    );

    y += 40;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "Patient ID: " + patient.patientId
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "Name: " + patient.name
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "Age: " + patient.age +
        "    Sex: " + patient.sex +
        "    Eye: " + patient.eye
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "Report Number: " + patient.reportNumber
    );

    y += 40;

    // ---------------------------------------------------------
// Input Fundus Image
// ---------------------------------------------------------

painter.setFont(headingFont);

painter.drawText(
    margin,
    y,
    pageWidth - (2 * margin),
    25,
    Qt::AlignLeft,
    "Input Fundus Image"
);

y += 30;

QPixmap originalFundus(patient.imagePath);

if (!originalFundus.isNull())
{
    const int originalWidth = 300;
    const int originalHeight = 230;

    painter.drawPixmap(
        QRect(
            margin,
            y,
            originalWidth,
            originalHeight
        ),
        originalFundus
    );

    painter.setFont(normalFont);

    painter.drawText(
        margin,
        y + originalHeight + 20,
        originalWidth,
        25,
        Qt::AlignCenter,
        QFileInfo(patient.imagePath).fileName()
    );

    y += originalHeight + 50;
}
else
{
    painter.setFont(normalFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "Original fundus image unavailable."
    );

    y += 35;
}

    // ---------------------------------------------------------
    // 1. Image Quality
    // ---------------------------------------------------------

    painter.setFont(headingFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "1. Image Quality Assessment"
    );

    y += 28;

    painter.setFont(normalFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        "IMAGE QUALITY: " + patient.qualityStatus
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Focus: %1")
            .arg(patient.focusScore, 0, 'f', 4)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Brightness: %1")
            .arg(patient.brightnessScore, 0, 'f', 4)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Contrast: %1")
            .arg(patient.contrastScore, 0, 'f', 4)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("FOV Ratio: %1")
            .arg(patient.fovScore, 0, 'f', 4)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Illumination: %1")
            .arg(patient.illuminationScore, 0, 'f', 4)
    );

    y += 35;

    // ---------------------------------------------------------
    // 2. DR Classification
    // ---------------------------------------------------------

    painter.setFont(headingFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "2. DR Classification"
    );

    y += 28;

    painter.setFont(normalFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Predicted Grade: Grade %1")
            .arg(patient.drGrade)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        "Severity: " + patient.drSeverity
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Model Confidence: %1%")
            .arg(patient.predictionConfidence, 0, 'f', 2)
    );

    y += 35;

    // ---------------------------------------------------------
    // 3. Class Probabilities
    // ---------------------------------------------------------

    painter.setFont(headingFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "3. Class Probabilities"
    );

    y += 28;

    painter.setFont(normalFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("No DR: %1%")
            .arg(patient.probabilityNoDR, 0, 'f', 2)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Mild DR: %1%")
            .arg(patient.probabilityMildDR, 0, 'f', 2)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Moderate DR: %1%")
            .arg(patient.probabilityModerateDR, 0, 'f', 2)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Severe DR: %1%")
            .arg(patient.probabilitySevereDR, 0, 'f', 2)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Proliferative DR: %1%")
            .arg(patient.probabilityProliferativeDR, 0, 'f', 2)
    );

    y += 35;

    // ---------------------------------------------------------
    // 4. Referable DR
    // ---------------------------------------------------------

    painter.setFont(headingFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "4. Referable DR Assessment"
    );

    y += 28;

    painter.setFont(normalFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Referable Probability: %1%")
            .arg(patient.referableProbability, 0, 'f', 2)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Decision Threshold: %1%")
            .arg(patient.referableThreshold, 0, 'f', 0)
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        "Decision: " + patient.referableDecision
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        "NON-REFERABLE DR"
    );

    y += 22;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        "Routine monitoring recommended."
    );

    y += 35;

    // ---------------------------------------------------------
    // 5. Explainability
    // ---------------------------------------------------------

    painter.setFont(headingFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "5. Explainability - Grad-CAM"
    );

    y += 28;

    painter.setFont(normalFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        40,
        Qt::TextWordWrap,
        "Grad-CAM highlights image regions that contributed to "
        "the predicted DR class. The visualization provides "
        "model-level evidence and is not a definitive lesion diagnosis."
    );

    y += 55;

    const int imageWidth = 180;
    const int imageHeight = 150;
    const int imageGap = 25;

    QString preprocessedPath =
        assetPath(
            "assets/preprocessing/prototype_preprocessed.png"
        );

    QString gradcamPath =
        assetPath(
            "assets/gradcam/prototype_gradcam.png"
        );

    QString attentionPath =
        assetPath(
            "assets/attention/prototype_attention.png"
        );

    QPixmap preprocessed(preprocessedPath);
    QPixmap gradcam(gradcamPath);
    QPixmap attention(attentionPath);

    int x1 = margin;
    int x2 = margin + imageWidth + imageGap;
    int x3 = margin + (imageWidth + imageGap) * 2;

    if (!preprocessed.isNull())
    {
        painter.drawPixmap(
            QRect(x1, y, imageWidth, imageHeight),
            preprocessed
        );
    }

    if (!gradcam.isNull())
    {
        painter.drawPixmap(
            QRect(x2, y, imageWidth, imageHeight),
            gradcam
        );
    }

    if (!attention.isNull())
    {
        painter.drawPixmap(
            QRect(x3, y, imageWidth, imageHeight),
            attention
        );
    }

    y += imageHeight + 30;

    painter.setFont(normalFont);

    painter.drawText(
        QRect(x1, y, imageWidth, 25),
        Qt::AlignCenter,
        "Preprocessed Fundus"
    );

    painter.drawText(
        QRect(x2, y, imageWidth, 25),
        Qt::AlignCenter,
        "Grad-CAM Overlay"
    );

    painter.drawText(
        QRect(x3, y, imageWidth, 25),
        Qt::AlignCenter,
        "Attention Heatmap"
    );

    // ---------------------------------------------------------
    // New page for remaining sections
    // ---------------------------------------------------------

    pdf.newPage();

    y = margin;

    // ---------------------------------------------------------
    // 6. MATLAB Preprocessing
    // ---------------------------------------------------------

    painter.setFont(headingFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "6. MATLAB Preprocessing"
    );

    y += 35;

    painter.setFont(normalFont);

    QStringList preprocessingSteps = {
        "Retinal field detection and cropping",
        "Illumination correction",
        "CLAHE enhancement",
        "Mild denoising",
        "Resizing to 224 x 224"
    };

    for (const QString &step : preprocessingSteps)
    {
        painter.drawText(
            margin,
            y,
            pageWidth - (2 * margin),
            22,
            Qt::AlignLeft,
            "✓ " + step
        );

        y += 24;
    }

    y += 25;

    // ---------------------------------------------------------
    // 7. Screening Summary
    // ---------------------------------------------------------

    painter.setFont(headingFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "7. Screening Summary"
    );

    y += 35;

    painter.setFont(normalFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Predicted Grade: Grade %1")
            .arg(patient.drGrade)
    );

    y += 24;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        "Severity: " + patient.drSeverity
    );

    y += 24;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Model Confidence: %1%")
            .arg(patient.predictionConfidence, 0, 'f', 2)
    );

    y += 24;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        QString("Referable Probability: %1%")
            .arg(patient.referableProbability, 0, 'f', 2)
    );

    y += 24;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        22,
        Qt::AlignLeft,
        "Decision: " + patient.referableDecision
    );

    y += 24;

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        40,
        Qt::TextWordWrap,
        "Recommendation: Routine monitoring recommended."
    );

    y += 60;

    painter.setFont(headingFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        30,
        Qt::AlignLeft,
        "Research Prototype - Not a Final Clinical Diagnosis"
    );

    y += 30;

    painter.setFont(normalFont);

    painter.drawText(
        margin,
        y,
        pageWidth - (2 * margin),
        25,
        Qt::AlignLeft,
        "Analysis completed successfully."
    );

    painter.end();

    statusLabel->setText(
        "✓ PDF report generated: " + fileName
    );
}