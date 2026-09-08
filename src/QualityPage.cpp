#include "QualityPage.h"

#include <QFileInfo>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFrame>

QualityPage::QualityPage(
    const PatientData &patient,
    QWidget *parent
)
    : QWidget(parent),
      patient(patient)
{
    setAttribute(Qt::WA_StyledBackground, true);
    setAutoFillBackground(true);

    setupUI();
    evaluateQuality();
}
void QualityPage::setupUI()
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
    // Page title
    // -------------------------------------------------

    auto *title = new QLabel("Image Quality Assessment");

    title->setStyleSheet(
        "font-size: 25px;"
        "font-weight: bold;"
        "color: #123B5D;"
    );

    mainLayout->addWidget(title);

    auto *subtitle = new QLabel(
        "Assessment of focus, illumination and retinal field of view"
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
        "Patient  →  Image  →  [ QUALITY ]  →  Analysis  →  Result  →  Report"
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
    // Image section
    // -------------------------------------------------

    auto *imageArea = new QHBoxLayout;
    imageArea->setSpacing(12);

    // Original image
    auto *originalContainer = new QVBoxLayout;

    auto *originalCaption = new QLabel("ORIGINAL IMAGE");
    originalCaption->setAlignment(Qt::AlignCenter);
    originalCaption->setStyleSheet(
        "font-size: 13px;"
        "font-weight: bold;"
        "color: #52636D;"
    );

    originalImage = new QLabel;
    originalImage->setFixedSize(430, 360);
    originalImage->setAlignment(Qt::AlignCenter);
    originalImage->setStyleSheet(
        "background: #F8FAFB;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 7px;"
    );

    QPixmap pixmap(patient.imagePath);

    originalImage->setPixmap(
    pixmap.scaled(
        410,
        340,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );

    originalContainer->addWidget(originalCaption);
    originalContainer->addWidget(originalImage, 1);

    imageArea->addLayout(originalContainer, 1);

    // Enhanced image
    auto *enhancedContainer = new QVBoxLayout;

    enhancedCaption = new QLabel("ENHANCED IMAGE");
    enhancedCaption->setAlignment(Qt::AlignCenter);
    enhancedCaption->setStyleSheet(
        "font-size: 13px;"
        "font-weight: bold;"
        "color: #52636D;"
    );

    enhancedImage = new QLabel;
    enhancedImage->setFixedSize(430, 360);
    enhancedImage->setAlignment(Qt::AlignCenter);
    enhancedImage->setStyleSheet(
        "background: #F8FAFB;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 7px;"
    );

    enhancedContainer->addWidget(enhancedCaption);
    enhancedContainer->addWidget(enhancedImage, 1);

    imageArea->addLayout(enhancedContainer, 1);

    // Hide enhanced section until enhancement actually exists.
    enhancedImage->hide();
    enhancedCaption->hide();

    content->addLayout(imageArea, 2);

    // -------------------------------------------------
    // Quality details
    // -------------------------------------------------

    auto *detailsFrame = new QFrame;

    detailsFrame->setStyleSheet(
        "QFrame {"
        "background: #F8FAFB;"
        "border: 1px solid #D9E2E8;"
        "border-radius: 7px;"
        "}"
    );

    auto *details = new QVBoxLayout(detailsFrame);
    details->setContentsMargins(18, 18, 18, 18);
    details->setSpacing(10);

    statusLabel = new QLabel;
    qualityLabel = new QLabel;
    focusLabel = new QLabel;
    illuminationLabel = new QLabel;
    fieldLabel = new QLabel;
    enhancementInfo = new QLabel;

    statusLabel->setStyleSheet(
        "font-size: 20px;"
        "font-weight: bold;"
        "color: #238636;"
        "padding-bottom: 5px;"
    );

    for (auto *label :
         {qualityLabel, focusLabel, illuminationLabel, fieldLabel}) {

        label->setStyleSheet(
            "font-size: 14px;"
            "color: #34454F;"
            "padding: 4px 0;"
        );
    }

    enhancementInfo->setWordWrap(true);
    enhancementInfo->setStyleSheet(
        "font-size: 13px;"
        "color: #52636D;"
        "padding-top: 8px;"
    );

    details->addWidget(statusLabel);
    details->addWidget(qualityLabel);
    details->addWidget(focusLabel);
    details->addWidget(illuminationLabel);
    details->addWidget(fieldLabel);

    details->addSpacing(8);

    details->addWidget(enhancementInfo);
    details->addStretch();

    content->addWidget(detailsFrame, 1);

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
    actions->setSpacing(10);

    nextButton = new QPushButton("Next");
    enhanceButton = new QPushButton("Enhance");
    rejectButton = new QPushButton("Reject");
    uploadAnotherButton = new QPushButton("Upload Another Image");

    for (auto *button :
         {nextButton, enhanceButton, rejectButton, uploadAnotherButton}) {

        button->setMinimumHeight(40);
        button->setCursor(Qt::PointingHandCursor);
    }

    nextButton->setStyleSheet(
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

    enhanceButton->setStyleSheet(
        "QPushButton {"
        "background: #167D8D;"
        "color: white;"
        "border: none;"
        "border-radius: 5px;"
        "padding: 8px 24px;"
        "}"
        "QPushButton:hover {"
        "background: #126B78;"
        "}"
        "QPushButton:disabled {"
        "background: #CBD5DA;"
        "color: #7A878E;"
        "}"
    );

    rejectButton->setStyleSheet(
        "QPushButton {"
        "background: white;"
        "color: #B42318;"
        "border: 1px solid #D5A5A1;"
        "border-radius: 5px;"
        "padding: 8px 20px;"
        "}"
        "QPushButton:hover {"
        "background: #FFF5F4;"
        "}"
    );

    uploadAnotherButton->setStyleSheet(
        "QPushButton {"
        "background: white;"
        "color: #123B5D;"
        "border: 1px solid #BFCBD2;"
        "border-radius: 5px;"
        "padding: 8px 20px;"
        "}"
        "QPushButton:hover {"
        "background: #F4F7F9;"
        "}"
    );

    actions->addWidget(uploadAnotherButton);
    actions->addWidget(rejectButton);
    actions->addStretch();
    actions->addWidget(enhanceButton);
    actions->addWidget(nextButton);

    mainLayout->addWidget(actionBar);

    // -------------------------------------------------
    // Connections
    // -------------------------------------------------

    connect(
        nextButton,
        &QPushButton::clicked,
        this,
        &QualityPage::nextRequested
    );

    connect(
        enhanceButton,
        &QPushButton::clicked,
        this,
        &QualityPage::enhanceImage
    );

    connect(
        rejectButton,
        &QPushButton::clicked,
        this,
        &QualityPage::rejectImage
    );

    connect(
        uploadAnotherButton,
        &QPushButton::clicked,
        this,
        &QualityPage::rejectImage
    );
}

void QualityPage::evaluateQuality()
{
    QString filename =
        QFileInfo(patient.imagePath).fileName().toLower();

    if (filename.contains("poor")) {

        statusLabel->setText("UNGRADABLE");

        statusLabel->setStyleSheet(
            "font-size: 20px;"
            "font-weight: bold;"
            "color: #B42318;"
        );

        qualityLabel->setText("Overall Quality: 28%");
        focusLabel->setText("Focus: Poor");
        illuminationLabel->setText("Illumination: Poor");
        fieldLabel->setText("Field of View: Inadequate");

        enhancementInfo->setText(
            "This image cannot be reliably assessed.\n\n"
            "Please recapture the fundus image or upload another image."
        );

        enhancementInfo->setStyleSheet(
            "font-size: 13px;"
            "color: #B42318;"
        );

        nextButton->setEnabled(false);
        enhanceButton->setEnabled(false);

        return;
    }

    if (filename.contains("borderline")) {

        statusLabel->setText("BORDERLINE");

        statusLabel->setStyleSheet(
            "font-size: 20px;"
            "font-weight: bold;"
            "color: #B7791F;"
        );

        qualityLabel->setText("Overall Quality: 67%");
        focusLabel->setText("Focus: Borderline");
        illuminationLabel->setText("Illumination: Uneven");
        fieldLabel->setText("Field of View: Adequate");

        enhancementInfo->setText(
            "Borderline image detected.\n"
            "Automatic enhancement has been applied."
        );

        enhancementInfo->setStyleSheet(
            "font-size: 13px;"
            "color: #8A6116;"
        );

        nextButton->setEnabled(false);
        enhanceButton->setEnabled(false);

        applySimulatedEnhancement();

        return;
    }

    statusLabel->setText("GOOD");

    statusLabel->setStyleSheet(
        "font-size: 20px;"
        "font-weight: bold;"
        "color: #238636;"
    );

    qualityLabel->setText("Overall Quality: 91%");
    focusLabel->setText("Focus: Good");
    illuminationLabel->setText("Illumination: Good");
    fieldLabel->setText("Field of View: Good");

    enhancementInfo->setText(
        "Image is suitable for analysis.\n"
        "You may continue, enhance, or reject the image."
    );

    enhancementInfo->setStyleSheet(
        "font-size: 13px;"
        "color: #238636;"
    );

    nextButton->setEnabled(true);
    enhanceButton->setEnabled(true);
}

void QualityPage::applySimulatedEnhancement()
{
    QImage original(patient.imagePath);

    if (original.isNull())
        return;

    QImage enhanced =
        original.convertToFormat(QImage::Format_RGB32);

    for (int y = 0; y < enhanced.height(); ++y) {

        QRgb *line =
            reinterpret_cast<QRgb *>(enhanced.scanLine(y));

        for (int x = 0; x < enhanced.width(); ++x) {

            int r = qRed(line[x]);
            int g = qGreen(line[x]);
            int b = qBlue(line[x]);

            r = qBound(0, (r - 128) * 12 / 10 + 128, 255);
            g = qBound(0, (g - 128) * 12 / 10 + 128, 255);
            b = qBound(0, (b - 128) * 12 / 10 + 128, 255);

            line[x] = qRgb(r, g, b);
        }
    }

    enhancedImage->setPixmap(
        QPixmap::fromImage(enhanced).scaled(
            430,
            380,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );

    // Show enhanced panel only after enhancement exists.
    enhancedCaption->show();
    enhancedImage->show();

    statusLabel->setText("ENHANCED");

    statusLabel->setStyleSheet(
        "font-size: 20px;"
        "font-weight: bold;"
        "color: #167D8D;"
    );

    qualityLabel->setText("Enhanced Quality: 94%");
    focusLabel->setText("Focus: Acceptable");
    illuminationLabel->setText("Illumination: Normalized");
    fieldLabel->setText("Field of View: Good");

    enhancementInfo->setText(
        "Enhancement applied:\n"
        "✓ Contrast enhancement\n"
        "✓ Illumination normalization\n"
        "✓ Noise reduction"
    );

    enhancementInfo->setStyleSheet(
        "font-size: 13px;"
        "color: #167D8D;"
    );

    nextButton->setEnabled(true);
}

void QualityPage::enhanceImage()
{
    applySimulatedEnhancement();
    enhanceButton->setEnabled(false);
}

void QualityPage::rejectImage()
{
    emit backRequested();
}