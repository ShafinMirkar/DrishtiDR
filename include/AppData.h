#ifndef APPDATA_H
#define APPDATA_H

#include <QString>

struct PatientData
{
    QString patientId;
    QString name;
    QString age;
    QString sex;
    QString eye;
    QString diabetesDuration;
    QString imagePath;

    QString reportNumber;
};

#endif