#include "settings.h"


bool Settings::loadSettings(const QString &filename, QJsonObject &settings)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (doc.isNull()) {
        return false;
    }
    
    settings = doc.object();
    return true;
}

bool Settings::saveSettings(const QString &filename, const QJsonObject &settings)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    QJsonDocument doc(settings);
    file.write(doc.toJson(QJsonDocument::Indented));
    return true;
}

QStringList Settings::loadSounds(const QString &folderPath)
{
    QStringList sounds;
    QDir dir(folderPath);
    
    if (!dir.exists()) {
        return sounds;
    }
    
    QStringList filters;
    filters << "*.mp3";
    QFileInfoList files = dir.entryInfoList(filters, QDir::Files);
    
    for (const QFileInfo &file : files) {
        sounds << file.fileName();
    }
    
    // If no sounds found, add default ones
    if (sounds.isEmpty()) {
        sounds << "Happy bells notification.mp3";
        sounds << "Bell notification.mp3";
        sounds << "alarm-clock.mp3";
    }
    
    return sounds;
} 