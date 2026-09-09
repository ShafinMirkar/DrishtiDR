#include "MainWindow.h"
#include "QualityPage.h"
#include "AnalysisPage.h"
#include "ResultPage.h"
#include "ReportPage.h"
#include "CataractPage.h"
#include "PreprocessingPage.h"
#include "ReferablePage.h"
#include "ExplainabilityPage.h"

#include <QFileInfo>
#include <QComboBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QImage>
#include <QImageReader>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <QDir>
#include <QDesktopServices>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTabWidget>
#include <QUrl>

#include <QDateTime>
#include <QUuid>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("DrishtiDx");
    resize(1200, 750);

    setupUI();
}

PatientData MainWindow::collectPatientData()
{
    PatientData patient;

    patient.patientId = patientIdEdit->text().trimmed();
    patient.name = nameEdit->text().trimmed();
    patient.age = ageEdit->text().trimmed();
    patient.sex = sexCombo->currentText();
    patient.eye = eyeCombo->currentText();
    patient.diabetesDuration =
        diabetesDurationEdit->text().trimmed();
    patient.imagePath = selectedImagePath;

    return patient;
}   

void MainWindow::setupUI()
{
    auto *centralWidget = new QWidget(this);

    centralWidget->setAttribute(Qt::WA_StyledBackground, true);

    centralWidget->setStyleSheet(
        "QWidget {"
        "background-color: #FFFFFF;"
        "color: #243746;"
        "}"
    );

    // =========================================================
    // Main Tabs
    // =========================================================

    mainTabs = new QTabWidget;

    mainTabs->setStyleSheet(
        "QTabWidget::pane {"
        "border: none;"
        "background: #FFFFFF;"
        "}"

        "QTabBar::tab {"
        "background: #F4F7F9;"
        "color: #52636D;"
        "padding: 10px 22px;"
        "font-size: 14px;"
        "border: 1px solid #D7E0E5;"
        "}"

        "QTabBar::tab:selected {"
        "background: #FFFFFF;"
        "color: #4B8FA8;"
        "font-weight: 600;"
        "}"
    );

    // =========================================================
    // New Screening Tab
    // =========================================================

    auto *screeningTab = new QWidget;

    auto *mainLayout = new QVBoxLayout(screeningTab);

    mainLayout->setContentsMargins(40, 30, 40, 30);
    mainLayout->setSpacing(20);

    // ---------------------------------------------------------
    // Header
    // ---------------------------------------------------------

    auto *title = new QLabel("DrishtiDx");

    title->setStyleSheet(
        "font-size: 30px;"
        "font-weight: bold;"
        "color: #123B5D;"
    );

    auto *subtitle = new QLabel(
        "Diabetic Retinopathy Screening"
    );

    subtitle->setStyleSheet(
        "font-size: 15px;"
        "color: #60717D;"
    );

    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // ---------------------------------------------------------
    // Workflow
    // ---------------------------------------------------------

    auto *workflow = new QLabel(
        "Patient  →  Image  →  Quality  →  Analysis  →  Result  →  Report"
    );

    workflow->setStyleSheet(
        "font-size: 14px;"
        "font-weight: bold;"
        "color: #167D8D;"
        "padding: 10px 0;"
    );

    mainLayout->addWidget(workflow);

    // ---------------------------------------------------------
    // Content
    // ---------------------------------------------------------

    auto *contentLayout = new QHBoxLayout;

    contentLayout->setSpacing(30);

    // =========================================================
    // Patient Information
    // =========================================================

    auto *patientFrame = new QFrame;

    patientFrame->setFrameShape(QFrame::StyledPanel);

    patientFrame->setStyleSheet(
        "QFrame {"
"background-color: transparent;"
"border: none;"
"}"

        "QLabel {"
        "border: none;"
        "}"

        "QLineEdit, QComboBox {"
        "border: 1px solid #C7D3DA;"
        "border-radius: 5px;"
        "padding: 8px;"
        "background: #FFFFFF;"
        "color: #243746;"
        "font-size: 14px;"
        "}"

        "QLineEdit::placeholder {"
        "color: #8797A2;"
        "}"

        "QComboBox QAbstractItemView {"
        "background: #FFFFFF;"
        "color: #243746;"
        "}"
    );

    auto *patientLayout =
        new QVBoxLayout(patientFrame);

    patientLayout->setContentsMargins(25, 25, 25, 25);

    auto *patientTitle =
        new QLabel("Patient Information");

    patientTitle->setStyleSheet(
        "font-size: 19px;"
        "font-weight: bold;"
        "color: #123B5D;"
        "border: none;"
    );

    patientLayout->addWidget(patientTitle);

    auto *form = new QFormLayout;

    form->setSpacing(14);

    patientIdEdit = new QLineEdit;
    patientIdEdit->setPlaceholderText(
        "Enter patient ID"
    );

    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText(
        "Enter full name"
    );

    ageEdit = new QLineEdit;
    ageEdit->setPlaceholderText(
        "Age"
    );

    sexCombo = new QComboBox;
    sexCombo->addItems({
        "Select",
        "Male",
        "Female",
        "Other"
    });

    eyeCombo = new QComboBox;
    eyeCombo->addItems({
        "Select",
        "Left Eye",
        "Right Eye"
    });

    diabetesDurationEdit = new QLineEdit;
    diabetesDurationEdit->setPlaceholderText(
        "Years"
    );

    form->addRow(
        "Patient ID",
        patientIdEdit
    );

    form->addRow(
        "Name",
        nameEdit
    );

    form->addRow(
        "Age",
        ageEdit
    );

    form->addRow(
        "Sex",
        sexCombo
    );

    form->addRow(
        "Eye",
        eyeCombo
    );

    form->addRow(
        "Diabetes Duration",
        diabetesDurationEdit
    );

    patientLayout->addLayout(form);
    patientLayout->addStretch();

    contentLayout->addWidget(
        patientFrame,
        1
    );

    // =========================================================
    // Image Upload
    // =========================================================

    auto *imageFrame = new QFrame;

    imageFrame->setFrameShape(
        QFrame::StyledPanel
    );

    imageFrame->setStyleSheet(
        "QFrame {"
"background-color: transparent;"
"border: none;"
"}"

        "QLabel {"
        "border: none;"
        "}"
    );

    auto *imageLayout =
        new QVBoxLayout(imageFrame);

    imageLayout->setContentsMargins(
        25,
        25,
        25,
        25
    );

    imageLayout->setSpacing(15);

    auto *imageTitle =
        new QLabel("Fundus Image");

    imageTitle->setStyleSheet(
        "font-size: 19px;"
        "font-weight: bold;"
        "color: #123B5D;"
        "border: none;"
    );

    imageLayout->addWidget(imageTitle);

    imagePreview = new QLabel;

    imagePreview->setMinimumSize(
        450,
        350
    );

    imagePreview->setAlignment(
        Qt::AlignCenter
    );

    imagePreview->setText(
        "No fundus image selected\n\n"
        "Upload an image from this workstation"
    );

    imagePreview->setStyleSheet(
        "background: #F4F7F9;"
        "border: 2px dashed #B8C8D1;"
        "border-radius: 6px;"
        "color: #71818B;"
        "font-size: 15px;"
    );

    imageLayout->addWidget(
        imagePreview,
        1
    );

    imageNameLabel =
        new QLabel("No image selected");

    imageNameLabel->setStyleSheet(
        "color: #60717D;"
        "font-size: 13px;"
        "border: none;"
    );

    imageLayout->addWidget(
        imageNameLabel
    );

    auto *uploadButton =
        new QPushButton(
            "Upload Fundus Image"
        );

    uploadButton->setMinimumHeight(42);

    uploadButton->setStyleSheet(
        "QPushButton {"
        "background: #167D8D;"
        "color: white;"
        "border: none;"
        "border-radius: 5px;"
        "font-size: 15px;"
        "font-weight: bold;"
        "padding: 8px 16px;"
        "}"

        "QPushButton:hover {"
        "background: #126B78;"
        "}"
    );

    connect(
        uploadButton,
        &QPushButton::clicked,
        this,
        &MainWindow::uploadImage
    );

    imageLayout->addWidget(
        uploadButton
    );

    contentLayout->addWidget(
        imageFrame,
        1
    );

    mainLayout->addLayout(
        contentLayout,
        1
    );

    // =========================================================
    // Bottom Buttons
    // =========================================================

    auto *bottomLayout =
        new QHBoxLayout;

    auto *clearButton =
        new QPushButton("Clear");

    clearButton->setMinimumSize(
        100,
        40
    );

    clearButton->setStyleSheet(
        "QPushButton {"
        "background: white;"
        "color: #52636D;"
        "border: 1px solid #BFCBD2;"
        "border-radius: 5px;"
        "padding: 8px 18px;"
        "}"
    );

    connect(
        clearButton,
        &QPushButton::clicked,
        [this]()
        {
            patientIdEdit->clear();
            nameEdit->clear();
            ageEdit->clear();
            diabetesDurationEdit->clear();

            sexCombo->setCurrentIndex(0);
            eyeCombo->setCurrentIndex(0);

            selectedImagePath.clear();

            imagePreview->setPixmap(
                QPixmap()
            );

            imagePreview->setText(
                "No fundus image selected\n\n"
                "Upload an image from this workstation"
            );

            imageNameLabel->setText(
                "No image selected"
            );

            nextButton->setEnabled(false);
        }
    );

    nextButton =
        new QPushButton("Next");

    nextButton->setMinimumSize(
        120,
        40
    );

    nextButton->setEnabled(false);

    nextButton->setStyleSheet(
        "QPushButton {"
        "background: #123B5D;"
        "color: white;"
        "border: none;"
        "border-radius: 5px;"
        "font-weight: bold;"
        "padding: 8px 20px;"
        "}"

        "QPushButton:disabled {"
        "background: #CBD5DA;"
        "color: #7A878E;"
        "}"
    );

    connect(
        nextButton,
        &QPushButton::clicked,
        this,
        &MainWindow::continueToQuality
    );

    bottomLayout->addWidget(
        clearButton
    );

    bottomLayout->addStretch();

    bottomLayout->addWidget(
        nextButton
    );

    mainLayout->addLayout(
        bottomLayout
    );

    // =========================================================
    // Add New Screening Tab
    // =========================================================

    mainTabs->addTab(
        screeningTab,
        "New Screening"
    );

    // =========================================================
    // Generated Reports Tab
    // =========================================================

    setupReportsTab();

    // =========================================================
    // Main Window Layout
    // =========================================================

    auto *centralLayout =
        new QVBoxLayout(centralWidget);

    centralLayout->setContentsMargins(
        0,
        0,
        0,
        0
    );

    centralLayout->addWidget(
        mainTabs
    );

    setCentralWidget(
        centralWidget
    );
}


void MainWindow::uploadImage()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        "Select Fundus Image",
        QString(),
        "Images (*.png *.jpg *.jpeg *.bmp *.tif *.tiff)"
    );

    if (filePath.isEmpty())
        return;

    QImageReader reader(filePath);

    if (!reader.canRead()) {
        QMessageBox::warning(
            this,
            "Invalid Image",
            "The selected file could not be read."
        );
        return;
    }

    QImage image = reader.read();

    if (image.isNull()) {
        QMessageBox::warning(
            this,
            "Invalid Image",
            "The selected image could not be loaded."
        );
        return;
    }

    selectedImagePath = filePath;

    QPixmap pixmap = QPixmap::fromImage(image);

    imagePreview->setPixmap(
        pixmap.scaled(
            imagePreview->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation
        )
    );

    imagePreview->setText("");

    imageNameLabel->setText(
        "Selected: " + QFileInfo(filePath).fileName()
    );

    nextButton->setEnabled(true);
}
void MainWindow::continueToQuality()
{
    if (selectedImagePath.isEmpty())
        return;

    PatientData patient = collectPatientData();

    patient.reportNumber =
    "DRX-" +
    QDateTime::currentDateTime().toString("yyyyMMdd-HHmmss") +
    "-" +
    QUuid::createUuid()
        .toString(QUuid::WithoutBraces)
        .left(4)
        .toUpper();

    auto *qualityPage = new QualityPage(patient);

    connect(
    qualityPage,
    &QualityPage::nextRequested,
    this,
    [this, qualityPage, patient]()
    {
        auto *cataractPage =
            new CataractPage(patient);

        connect(
    cataractPage,
    &CataractPage::analysisCompleted,
    this,
    [this, cataractPage, patient]()
    {
        auto *preprocessingPage =
            new PreprocessingPage(patient);

        connect(
            preprocessingPage,
            &PreprocessingPage::processingCompleted,
            this,
            [this, preprocessingPage, patient]()
            {
                auto *analysisPage =
                    new AnalysisPage(patient);

                connect(
                    analysisPage,
                    &AnalysisPage::analysisCompleted,
                    this,
                    [this, analysisPage, patient]()
                    {
                        auto *referablePage =
                            new ReferablePage(patient);
                    
                        connect(
                            referablePage,
                            &ReferablePage::assessmentCompleted,
                            this,
                            [this, referablePage, patient]()
                               {
        auto *explainabilityPage =
            new ExplainabilityPage(patient);

        connect(
            explainabilityPage,
            &ExplainabilityPage::reportRequested,
            this,
            [this, explainabilityPage, patient]()
            {
                auto *reportPage =
                    new ReportPage(patient);

                connect(
                    reportPage,
                    &ReportPage::backToMainRequested,
                    this,
                    [this, reportPage]()
                    {
                        reportPage->deleteLater();
                        setupUI();
                    }
                );

                setCentralWidget(reportPage);
                explainabilityPage->deleteLater();
            }
        );

        setCentralWidget(explainabilityPage);
        referablePage->deleteLater();
    }
                        );
                    
                        setCentralWidget(referablePage);
                        analysisPage->deleteLater();
                    }
                );
                    

                setCentralWidget(analysisPage);
                preprocessingPage->deleteLater();
            }
        );

        setCentralWidget(preprocessingPage);
        cataractPage->deleteLater();
    }
);

                       

        setCentralWidget(cataractPage);
        qualityPage->deleteLater();
    }
);

    connect(
        qualityPage,
        &QualityPage::backRequested,
        this,
        [this, qualityPage]()
        {
            qualityPage->deleteLater();
            setupUI();
        }
    );

    setCentralWidget(qualityPage);
}

void MainWindow::setupReportsTab()
{
    auto *reportsTab = new QWidget;

    auto *layout = new QVBoxLayout(reportsTab);

    layout->setContentsMargins(32, 24, 32, 24);
    layout->setSpacing(14);

    auto *title = new QLabel("Generated Reports");

    title->setStyleSheet(
        "font-size: 25px;"
        "font-weight: 600;"
        "color: #243746;"
        "border: none;"
    );

    auto *subtitle = new QLabel(
        "Previously generated DrishtiDx screening reports"
    );

    subtitle->setStyleSheet(
        "font-size: 14px;"
        "color: #687984;"
        "border: none;"
    );

    layout->addWidget(title);
    layout->addWidget(subtitle);

    reportsList = new QListWidget;

    reportsList->setStyleSheet(
        "QListWidget {"
        "background: #FFFFFF;"
        "border: 1px solid #D7E0E5;"
        "border-radius: 8px;"
        "padding: 6px;"
        "font-size: 15px;"
        "color: #34454F;"
        "}"

        "QListWidget::item {"
        "padding: 12px;"
        "border-bottom: 1px solid #E5EBEE;"
        "}"

        "QListWidget::item:selected {"
        "background: #EEF5F7;"
        "color: #243746;"
        "}"
    );

    layout->addWidget(reportsList, 1);

    auto *openButton =
        new QPushButton("Open Selected Report");

    openButton->setStyleSheet(
        "QPushButton {"
        "background-color: #4B8FA8;"
        "color: white;"
        "border: none;"
        "border-radius: 5px;"
        "padding: 10px 22px;"
        "font-size: 15px;"
        "}"
    );

    connect(
        openButton,
        &QPushButton::clicked,
        this,
        &MainWindow::openSelectedReport
    );

    layout->addWidget(openButton);

    connect(
        reportsList,
        &QListWidget::itemDoubleClicked,
        this,
        [this](QListWidgetItem *)
        {
            openSelectedReport();
        }
    );

    refreshReports();

    // Store the tab widget temporarily through the list's parent.
    reportsList->setProperty(
        "reportsTab",
        QVariant::fromValue(
            static_cast<QObject *>(reportsTab)
        )
    );

    // We will use this directly in setupUI.
    mainTabs->addTab(
        reportsTab,
        "Generated Reports"
    );
}
void MainWindow::refreshReports()
{
    if (!reportsList)
        return;

    reportsList->clear();

    QDir reportsDir(
        QCoreApplication::applicationDirPath()
        + "/../reports"
    );

    QStringList files =
        reportsDir.entryList(
            QStringList() << "*.pdf",
            QDir::Files,
            QDir::Time
        );

    if (files.isEmpty())
    {
        auto *item =
            new QListWidgetItem(
                "No generated reports yet."
            );

        item->setFlags(
            item->flags() & ~Qt::ItemIsSelectable
        );

        reportsList->addItem(item);
        return;
    }

    for (const QString &file : files)
    {
        QFileInfo info(
            reportsDir.absoluteFilePath(file)
        );

        QString displayText =
            QString("%1\nGenerated: %2")
                .arg(info.fileName())
                .arg(
                    info.lastModified()
                        .toString("dd/MM/yyyy  hh:mm")
                );

        auto *item =
            new QListWidgetItem(displayText);

        item->setData(
            Qt::UserRole,
            info.absoluteFilePath()
        );

        reportsList->addItem(item);
    }
}
void MainWindow::openSelectedReport()
{
    auto *item = reportsList->currentItem();

    if (!item)
        return;

    QString filePath =
        item->data(Qt::UserRole).toString();

    if (filePath.isEmpty())
        return;

    QDesktopServices::openUrl(
        QUrl::fromLocalFile(filePath)
    );
}