#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AppData.h"

#include <QMainWindow>

class QLineEdit;
class QComboBox;
class QLabel;
class QPushButton;

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

    QString selectedImagePath;

    void setupUI();
    PatientData collectPatientData();
};

#endif