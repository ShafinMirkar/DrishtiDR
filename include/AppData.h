#ifndef APPDATA_H
#define APPDATA_H

#include <QString>

struct PatientData
{
    // Patient
    QString patientId;
    QString name;
    QString age;
    QString sex;
    QString eye;
    QString diabetesDuration;

    // Input image
    QString imagePath;

    // Report
    QString reportNumber;

    // -------------------------------------------------
    // Image Quality Assessment
    // -------------------------------------------------

    double focusScore = 0.0009;
    double brightnessScore = 0.3938;
    double contrastScore = 0.0597;
    double fovScore = 0.7918;
    double illuminationScore = 0.0677;

    QString qualityStatus = "ACCEPTABLE";

    // -------------------------------------------------
    // Cataract Check
    // -------------------------------------------------

    QString cataractStatus = "Cataract not detected";

    // -------------------------------------------------
    // MATLAB Preprocessing
    // -------------------------------------------------

    QString preprocessedImagePath;

    // -------------------------------------------------
    // DR Classification
    // -------------------------------------------------

    int drGrade = 0;

    QString drSeverity = "No DR";

    double predictionConfidence = 99.97;

    double probabilityNoDR = 99.97;
    double probabilityMildDR = 0.02;
    double probabilityModerateDR = 0.00;
    double probabilitySevereDR = 0.00;
    double probabilityProliferativeDR = 0.01;

    // -------------------------------------------------
    // Referable DR
    // -------------------------------------------------

    double referableProbability = 0.01;

    double referableThreshold = 35.0;

    QString referableDecision = "NON-REFERABLE";

    // -------------------------------------------------
    // Explainability
    // -------------------------------------------------

    QString gradcamImagePath;
    QString attentionMapPath;
};

#endif
