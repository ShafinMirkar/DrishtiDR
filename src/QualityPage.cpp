#include "QualityPage.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>

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
}

void QualityPage::setupUI()
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
        "border:none;"
        "}"

        "QPushButton {"
        "background-color: #123B5D;"
        "color: #FFFFFF;"
        "border: none;"
        "border-radius: 5px;"
        "padding: 9px 18px;"
        "font-size: 14px;"
        "font-weight: 600;"
        "}"

        "QPushButton:hover {"
        "background-color: #174C73;"
        "}"

        "QPushButton:disabled {"
        "background-color: #B8C4CB;"
        "color: #FFFFFF;"
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
        "border: none"
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
        "font-size: 15px;"
        "font-weight: 700;"
        "color: #34454F;"
        "}"

        "QLabel#imageCaption {"
        "font-size: 13px;"
        "font-weight: 700;"
        "color: #526773;"
        "}"

        "QLabel#metric {"
        "background-color: #FFFFFF;"
        "border: none;"
"background: transparent;"
        "padding: 9px;"
        "font-size: 14px;"
        "}"

        "QLabel#check {"
        "font-size: 14px;"
        "color: #2F6F3E;"
        "}"

        "QLabel#status {"
        "background-color: #E8F6EC;"
        "border: 1px solid #B8DEC2;"
        "border-radius: 5px;"
        "padding: 10px;"
        "font-size: 14px;"
        "font-weight: 600;"
        "color: #237238;"
        "}"
    );

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 24, 32, 20);
    mainLayout->setSpacing(12);

    // -------------------------------------------------
    // Header
    // -------------------------------------------------

    auto *title = new QLabel("Image Quality Assessment");
    title->setObjectName("title");

    auto *subtitle = new QLabel(
        "Assessment of focus, illumination and retinal field of view"
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
        "Patient  →  Image  →  [ QUALITY ]  →  Analysis  →  Result  →  Report"
    );

    workflowLabel->setObjectName("workflow");

    mainLayout->addWidget(workflowLabel);

    // -------------------------------------------------
    // Main content
    // -------------------------------------------------

    auto *contentLayout = new QHBoxLayout;
    contentLayout->setSpacing(18);

    // -------------------------------------------------
    // Original image panel
    // -------------------------------------------------

    auto *imageFrame = new QFrame;
    imageFrame->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
    );

    auto *imageLayout = new QVBoxLayout(imageFrame);
    imageLayout->setContentsMargins(14, 14, 14, 14);
    imageLayout->setSpacing(8);

    auto *imageCaption = new QLabel("ORIGINAL FUNDUS IMAGE");
    imageCaption->setObjectName("imageCaption");

    imageLayout->addWidget(imageCaption);

    originalImage = new QLabel;
    originalImage->setAlignment(Qt::AlignCenter);
    originalImage->setMinimumSize(450, 360);
    originalImage->setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding
    );
    originalImage->setStyleSheet(
        "QLabel {"
        "background-color: #FFFFFF;"
        "border: none;"
"background: transparent;"
        "}"
    );

    QImage image(patient.imagePath);

    if (!image.isNull())
    {
        QPixmap pixmap = QPixmap::fromImage(image);

        originalImage->setPixmap(
            pixmap.scaled(
                520,
                400,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            )
        );
    }
    else
    {
        originalImage->setText("Unable to load image");
    }

    imageLayout->addWidget(originalImage, 1);

    // -------------------------------------------------
    // Quality information panel
    // -------------------------------------------------

    auto *qualityFrame = new QFrame;
    qualityFrame->setMinimumWidth(430);
    qualityFrame->setMaximumWidth(520);

    auto *qualityLayout = new QVBoxLayout(qualityFrame);
    qualityLayout->setContentsMargins(16, 14, 16, 14);
    qualityLayout->setSpacing(8);

    statusLabel = new QLabel("IMAGE QUALITY: ACCEPTABLE");
    statusLabel->setObjectName("status");
    statusLabel->setStyleSheet(
    "font-size: 17px;"
    "font-weight: 700;"
    "color: #167D8D;"
    "border: none;"
    "background: transparent;"
);

    qualityLayout->addWidget(statusLabel);

    auto *qualityTitle = new QLabel("Quality Metrics");
    qualityTitle->setObjectName("sectionTitle");

    qualityLayout->addWidget(qualityTitle);

    focusLabel = new QLabel(
        QString("Focus: %1")
            .arg(patient.focusScore, 0, 'f', 4)
    );

    brightnessLabel = new QLabel(
        QString("Brightness: %1")
            .arg(patient.brightnessScore, 0, 'f', 4)
    );

    contrastLabel = new QLabel(
        QString("Contrast: %1")
            .arg(patient.contrastScore, 0, 'f', 4)
    );

    fovLabel = new QLabel(
        QString("FOV: %1")
            .arg(patient.fovScore, 0, 'f', 4)
    );

    illuminationLabel = new QLabel(
        QString("Illumination: %1")
            .arg(patient.illuminationScore, 0, 'f', 4)
    );

    QLabel *metrics[] = {
        focusLabel,
        brightnessLabel,
        contrastLabel,
        fovLabel,
        illuminationLabel
    };

    for (auto *label : metrics)
    {
        label->setObjectName("metric");
        qualityLayout->addWidget(label);
    }

    auto *checksTitle = new QLabel("Quality Checks");
    checksTitle->setObjectName("sectionTitle");

    qualityLayout->addSpacing(5);
    qualityLayout->addWidget(checksTitle);

    focusCheckLabel = new QLabel("✓ Focus");
    brightnessCheckLabel = new QLabel("✓ Brightness");
    contrastCheckLabel = new QLabel("✓ Contrast");
    fovCheckLabel = new QLabel("✓ FOV");
    illuminationCheckLabel = new QLabel("✓ Illumination");

    QLabel *checks[] = {
        focusCheckLabel,
        brightnessCheckLabel,
        contrastCheckLabel,
        fovCheckLabel,
        illuminationCheckLabel
    };

    for (auto *label : checks)
    {
        label->setObjectName("check");
        qualityLayout->addWidget(label);
    }

    qualityLayout->addStretch();

    contentLayout->addWidget(imageFrame, 3);
    contentLayout->addWidget(qualityFrame, 2);

    mainLayout->addLayout(contentLayout, 1);

    // -------------------------------------------------
    // Bottom action bar
    // -------------------------------------------------

    auto *actionFrame = new QFrame;
    actionFrame->setSizePolicy(
        QSizePolicy::Preferred,
        QSizePolicy::Fixed
    );

    auto *actionLayout = new QHBoxLayout(actionFrame);
    actionLayout->setContentsMargins(10, 8, 10, 8);

    uploadAnotherButton =
        new QPushButton("Upload Another Image");

    rejectButton =
        new QPushButton("Reject");

    nextButton =
        new QPushButton("Next");

    rejectButton->setStyleSheet(
        "QPushButton {"
        "background-color: #FFFFFF;"
        "color: #B23A3A;"
        "border: 1px solid #D99A9A;"
        "border-radius: 5px;"
        "padding: 9px 18px;"
        "font-size: 14px;"
        "font-weight: 600;"
        "}"
        "QPushButton:hover {"
        "background-color: #FFF5F5;"
        "}"
    );

    actionLayout->addWidget(uploadAnotherButton);
    actionLayout->addWidget(rejectButton);
    actionLayout->addStretch();
    actionLayout->addWidget(nextButton);

    mainLayout->addWidget(actionFrame);

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
        rejectButton,
        &QPushButton::clicked,
        this,
        &QualityPage::rejectImage
    );

    connect(
        uploadAnotherButton,
        &QPushButton::clicked,
        this,
        &QualityPage::uploadAnotherImage
    );
}

void QualityPage::uploadAnotherImage()
{
    QString path = QFileDialog::getOpenFileName(
        this,
        "Select Fundus Image",
        QString(),
        "Images (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"
    );

    if (path.isEmpty())
        return;

    QImage image(path);

    if (image.isNull())
        return;

    patient.imagePath = path;

    originalImage->setPixmap(
        QPixmap::fromImage(image).scaled(
            520,
            400,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );

    nextButton->setEnabled(true);
}

void QualityPage::rejectImage()
{
    emit backRequested();
}