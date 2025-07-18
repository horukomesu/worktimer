#pragma once

#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#include <QStringList>

class Settings
{
public:
    static bool loadSettings(const QString &filename, QJsonObject &settings);
    static bool saveSettings(const QString &filename, const QJsonObject &settings);
    static QStringList loadSounds(const QString &folderPath);
}; 