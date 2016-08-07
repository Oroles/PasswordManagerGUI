#include "settings.h"

Settings& Settings::getInstance()
{
    static Settings instance;
    return instance;
}

bool Settings::IsDisableAlertMesssages()
{
    return mIsDisableAlertMessages;
}

bool Settings::IsDisableLogMessages()
{
    return mIsDisableLogMessages;
}

bool Settings::IsEnforceMinimumLength()
{
    return mEnforceMinimumLength;
}

bool Settings::IsSendingAlivePackages()
{
    return mSendIsAlivePackage;
}

Settings::Settings()
    : mIsDisableAlertMessages(false),
      mIsDisableLogMessages(false)
{

}

void Settings::disableAlertMessages(bool value)
{
    mIsDisableAlertMessages = value;
}

void Settings::disableLogMessages(bool value)
{
    mIsDisableLogMessages = value;
}

void Settings::enforceMinimumLength(bool value)
{
    mEnforceMinimumLength = value;
}

void Settings::sendIsAlivePackage(bool value)
{
    mSendIsAlivePackage = value;
}

