#include "ExplainabilityPage.h"

#include <QFileInfo>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

ExplainabilityPage::ExplainabilityPage(
    const PatientData &patient,
    QWidget *parent
)
    : QWidget(parent),
      patient(patient)
{
    setupUI();
}

QFrame *ExplainabilityPage::createImageCard(
    const QString &title,
    const QString &description,
    const QString &imagePath,
    QLabel **imageLabel
)
{
    auto *card = new QFrame;
    auto *layout = new QVBoxLayout(card);

    layout->setContentsMargins(14, 14, 14, 14);
    layout->setSpacing(8);

    auto *titleLabel = new QLabel(title);

    titleLabel->setStyleSheet(
        "font-size: 16px;"
        "font-weight: 600;"
        "color: #34454F;"
        "border: none;"
    );

    auto *descriptionLabel = new QLabel(description);

    descriptionLabel->setWordWrap(true);

    descriptionLabel->setStyleSheet(
        "font-size: 13px;"
        "color: #687984;"
        "border: none;"
    );

    auto *image = new QLabel;

    image->setAlignment(Qt::AlignCenter);
    image->setMinimumHeight(260);

    QPixmap pixmap(imagePath);

    if (!pixmap.isNull())
    {
        image->setPixmap(
            pixmap.scaled(
                360,
                260,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            )
        );
    }
    else
    {
        image->setText("Prototype image unavailable");
    }

    image->setStyleSheet(
        "background-color: #F7F9FA;"
        "border: none;"
        "color: #8797A2;"
    );

    layout->addWidget(titleLabel);
    layout->addWidget(descriptionLabel);
    layout->addWidget(image);

    *imageLabel = image;

    return card;
}

void ExplainabilityPage::setupUI()
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
    mainLayout->setSpacing(14);

    // Header
    auto *title = new QLabel("Explainability — Grad-CAM");

    title->setStyleSheet(
        "font-size: 25px;"
        "font-weight: 600;"
        "color: #243746;"
    );

    auto *subtitle = new QLabel(
        "Visual explanation of regions contributing to the predicted DR class"
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
        "PATIENT  →  IMAGE  →  QUALITY  →  CATARACT  →  PREPROCESSING  →  ANALYSIS  →  REFERABLE DR  →  [ EXPLAINABILITY ]"
    );

    workflowLabel->setStyleSheet(
        "font-size: 14px;"
        "font-weight: 600;"
        "color: #4B8FA8;"
    );

    mainLayout->addWidget(workflowLabel);

    // Explanation
    auto *explanationCard = new QFrame;
    auto *explanationLayout = new QVBoxLayout(explanationCard);

    explanationLayout->setContentsMargins(18, 14, 18, 14);

    auto *explanation = new QLabel(
        "Grad-CAM highlights image regions that contributed to the "
        "predicted DR class. The visualization provides model-level "
        "evidence and is not a definitive lesion diagnosis."
    );

    explanation->setWordWrap(true);

    explanation->setStyleSheet(
        "font-size: 14px;"
        "color: #52636D;"
    );

    explanationLayout->addWidget(explanation);

    mainLayout->addWidget(explanationCard);

    // Three image cards
    auto *imageLayout = new QHBoxLayout;
    imageLayout->setSpacing(14);

    auto *preprocessedCard = createImageCard(
        "Preprocessed Fundus",
        "Image presented to the classification model.",
        "assets/preprocessing/prototype_preprocessed.png",
        &preprocessedImage
    );

    auto *gradcamCard = createImageCard(
        "Grad-CAM Overlay",
        "Regions contributing to the predicted DR class.",
        "assets/gradcam/prototype_gradcam.png",
        &gradcamImage
    );

    auto *attentionCard = createImageCard(
        "Attention Heatmap",
        "Model attention distribution across the retinal image.",
        "assets/attention/prototype_attention.png",
        &attentionImage
    );

    imageLayout->addWidget(preprocessedCard);
    imageLayout->addWidget(gradcamCard);
    imageLayout->addWidget(attentionCard);

    mainLayout->addLayout(imageLayout);

    // Bottom action
    auto *actionLayout = new QHBoxLayout;

    actionLayout->addStretch();

    reportButton = new QPushButton(
        "Continue to Report"
    );

    connect(
        reportButton,
        &QPushButton::clicked,
        this,
        [this]()
        {
            emit reportRequested();
        }
    );

    actionLayout->addWidget(reportButton);

    mainLayout->addLayout(actionLayout);
}
