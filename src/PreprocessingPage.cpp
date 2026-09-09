#include "PreprocessingPage.h"

#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

PreprocessingPage::PreprocessingPage(
    const PatientData &patient,
    QWidget *parent
)
    : QWidget(parent),
      patient(patient),
      progress(0),
      step(0)
{
    setupUI();

    timer = new QTimer(this);

    connect(
        timer,
        &QTimer::timeout,
        this,
        &PreprocessingPage::updateProcessing
    );

    timer->start(400);
}

void PreprocessingPage::setupUI()
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

        "QProgressBar {"
        "border: 1px solid #C7D3DA;"
        "border-radius: 5px;"
        "background: #F4F7F9;"
        "height: 12px;"
        "text-align: center;"
        "}"

        "QProgressBar::chunk {"
        "background: #4B8FA8;"
        "border-radius: 4px;"
        "}"

        "QPushButton {"
        "background-color: #4B8FA8;"
        "color: white;"
        "border: none;"
        "border-radius: 5px;"
        "padding: 10px 22px;"
        "font-size: 15px;"
        "}"

        "QPushButton:disabled {"
        "background-color: #B8C5CC;"
        "}"
    );

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 24, 32, 24);
    mainLayout->setSpacing(16);

    // Header
    auto *title = new QLabel("MATLAB Preprocessing");
    title->setStyleSheet(
        "font-size: 25px;"
        "font-weight: 600;"
        "color: #243746;"
    );

    auto *subtitle = new QLabel(
        "Preparing the retinal image for DR severity classification"
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
        "PATIENT  →  IMAGE  →  QUALITY  →  CATARACT  →  [ PREPROCESSING ]  →  ANALYSIS"
    );

    workflowLabel->setStyleSheet(
        "font-size: 14px;"
        "font-weight: 600;"
        "color: #4B8FA8;"
    );

    mainLayout->addWidget(workflowLabel);

    // Image area
    auto *imageLayout = new QHBoxLayout;
    imageLayout->setSpacing(18);

    auto *originalCard = new QFrame;
    auto *originalLayout = new QVBoxLayout(originalCard);
    originalLayout->setContentsMargins(16, 16, 16, 16);

    auto *originalTitle = new QLabel("Original Fundus Image");
    originalTitle->setStyleSheet(
        "font-size: 16px;"
        "font-weight: 600;"
        "color: #34454F;"
    );

    originalImage = new QLabel;
    originalImage->setAlignment(Qt::AlignCenter);
    originalImage->setMinimumHeight(300);

    QPixmap originalPixmap(patient.imagePath);

    if (!originalPixmap.isNull())
    {
        originalImage->setPixmap(
            originalPixmap.scaled(
                420,
                300,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            )
        );
    }
    else
    {
        originalImage->setText("Original image unavailable");
    }

    originalLayout->addWidget(originalTitle);
    originalLayout->addWidget(originalImage);

    auto *processedCard = new QFrame;
    auto *processedLayout = new QVBoxLayout(processedCard);
    processedLayout->setContentsMargins(16, 16, 16, 16);

    auto *processedTitle = new QLabel("Preprocessed Image");
    processedTitle->setStyleSheet(
        "font-size: 16px;"
        "font-weight: 600;"
        "color: #34454F;"
    );

    preprocessedImage = new QLabel;
    preprocessedImage->setAlignment(Qt::AlignCenter);
    preprocessedImage->setMinimumHeight(300);
    preprocessedImage->setText("Processing...");

    processedLayout->addWidget(processedTitle);
    processedLayout->addWidget(preprocessedImage);

    imageLayout->addWidget(originalCard);
    imageLayout->addWidget(processedCard);

    mainLayout->addLayout(imageLayout);

    // Processing status
    auto *processingCard = new QFrame;
    auto *processingLayout = new QVBoxLayout(processingCard);
    processingLayout->setContentsMargins(18, 14, 18, 14);
    processingLayout->setSpacing(10);

    statusLabel = new QLabel("Detecting retinal field...");
    statusLabel->setStyleSheet(
        "font-size: 15px;"
        "font-weight: 600;"
        "color: #34454F;"
    );

    progressBar = new QProgressBar;
    progressBar->setRange(0, 100);
    progressBar->setValue(0);
    progressBar->setTextVisible(false);

    processingLayout->addWidget(statusLabel);
    processingLayout->addWidget(progressBar);

    mainLayout->addWidget(processingCard);

    // Bottom action bar
    auto *actionLayout = new QHBoxLayout;
    actionLayout->addStretch();

    nextButton = new QPushButton("Continue to Analysis");
    nextButton->setEnabled(false);

    connect(
        nextButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            emit processingCompleted();
        }
    );

    actionLayout->addWidget(nextButton);

    mainLayout->addLayout(actionLayout);
}

void PreprocessingPage::updateProcessing()
{
    progress += 20;
    step++;

    switch (step)
    {
    case 1:
        statusLabel->setText("Detecting retinal field...");
        break;

    case 2:
        statusLabel->setText("Cropping black background...");
        break;

    case 3:
        statusLabel->setText("Correcting illumination...");
        break;

    case 4:
        statusLabel->setText("Applying CLAHE enhancement...");
        break;

    case 5:
        statusLabel->setText("Applying mild denoising...");
        break;
    }

    progressBar->setValue(progress);

    if (progress >= 100)
    {
        timer->stop();

        statusLabel->setText(
            "✓ Preprocessing completed — image resized to 224 × 224"
        );

        patient.preprocessedImagePath =
            "assets/preprocessing/prototype_preprocessed.png";

        QPixmap processedPixmap(patient.preprocessedImagePath);

        if (!processedPixmap.isNull())
        {
            preprocessedImage->setPixmap(
                processedPixmap.scaled(
                    420,
                    300,
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                )
            );
        }
        else
        {
            preprocessedImage->setText(
                "Preprocessed prototype image unavailable"
            );
        }

        nextButton->setEnabled(true);
    }
}
