#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings &getInstance();
    bool IsDisableAlertMesssages();
    bool IsDisableLogMessages();
    bool IsEnforceMinimumLength();
    bool IsSendingAlivePackages();

private:
    Settings();

    Settings(const Settings&) = delete;
    Settings(const Settings &&) = delete;
    Settings& operator=(const Settings&) = delete;

public slots:
    void disableAlertMessages(bool value);
    void disableLogMessages(bool value);
    void enforceMinimumLength(bool value);
    void sendIsAlivePackage(bool value);

private:

    bool mIsDisableAlertMessages;
    bool mIsDisableLogMessages;
    bool mEnforceMinimumLength;
    bool mSendIsAlivePackage;
};

#endif // SETTINGS_H
