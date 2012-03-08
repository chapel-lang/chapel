/*
= StarPU-Top for StarPU =

Copyright (C) 2011 
William Braik
Yann Courtois
Jean-Marie Couteyen
Anthony Roy

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "configurationmanager.h"

#include <QDebug>

ConfigurationManager::ConfigurationManager()
{
    static bool instanciated = false;

    Q_ASSERT_X(instanciated == false, "ConfigurationManager's' constructor",
               "Singleton pattern violated - "
               "ConfigurationManager instanciated more than once");

    qDebug() << "ConfigurationManager : initializing";

    // Init settings
    _applicationSettings = new QSettings(CONFIG_FILE_DIR+"/"+CONFIG_FILE_NAME,
                                         QSettings::IniFormat);

    instanciated = true;
}

ConfigurationManager::~ConfigurationManager()
{
    qDebug() << "ConfigurationManager : terminating";

    delete _applicationSettings;
}

// Getters
QString ConfigurationManager::serverHost() const
{
    return _applicationSettings->value("connection/serverhost",
                                       QString("localhost")).toString();
}

int ConfigurationManager::serverPort() const
{
    return _applicationSettings->value("connection/serverport", 2011).toInt();
}

bool ConfigurationManager::ssh() const
{
    return _applicationSettings->value("connection/ssh", false).toBool();
}

QString ConfigurationManager::commandLine() const
{
    return _applicationSettings->value("connection/commandline",
                                       QString("ssh -L")).toString();
}

bool ConfigurationManager::antialiasing() const
{
    return _applicationSettings->value("display/antialiasing", true).toBool();
}

// Setters
void ConfigurationManager::setServerHost(QString serverIP)
{
    _applicationSettings->setValue("connection/serverhost", serverIP);
}

void ConfigurationManager::setServerPort(int serverPort)
{
    _applicationSettings->setValue("connection/serverport", serverPort);
}

void ConfigurationManager::setSSH(bool enabled)
{
    _applicationSettings->setValue("connection/ssh", enabled);
}

void ConfigurationManager::setCommandLine(QString commandLine)
{
    _applicationSettings->setValue("connection/commandline", commandLine);
}

void ConfigurationManager::setAntialiasing(bool enabled)
{
    _applicationSettings->setValue("display/antialiasing", enabled);
}

// Other methods
void ConfigurationManager::syncConfiguration()
{
    qDebug() << "ConfigurationManager : synchronizing application settings";

    _applicationSettings->sync();
}
