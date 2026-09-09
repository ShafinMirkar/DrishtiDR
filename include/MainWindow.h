#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AppData.h"

#include <QMainWindow>

class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;
class QTabWidget;
class QListWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void uploadImage();
    void continueToQuality();

private:
    QLineEdit *patientIdEdit;
    QLineEdit *nameEdit;
    QLineEdit *ageEdit;
    QComboBox *sexCombo;
    QComboBox *eyeCombo;
    QLineEdit *diabetesDurationEdit;

    QLabel *imagePreview;
    QLabel *imageNameLabel;
    QPushButton *nextButton;

    QTabWidget *mainTabs;
    QListWidget *reportsList;

    QString selectedImagePath;

    void setupUI();
    void setupReportsTab();
    void refreshReports();
    void openSelectedReport();

    PatientData collectPatientData();
};

#endif