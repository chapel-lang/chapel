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


#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <QSettings>

static const QString CONFIG_FILE_DIR = ".";
static const QString CONFIG_FILE_NAME = "starpu_top.cfg";

class ConfigurationManager
{ /* Contains and manages all the application settings
of the current instance, and remembers them for use
in the next instances */

public:
    ConfigurationManager();
    ~ConfigurationManager();

    // Getters
    QString serverHost() const;
    int serverPort() const;
    bool ssh() const;
    QString commandLine() const;
    bool antialiasing() const;
    // Setters
    void setServerHost(QString serverIP);
    void setServerPort(int serverPort);
    void setSSH(bool enabled);
    void setCommandLine(QString commandLine);
    void setAntialiasing(bool enabled);
    // Other methods
    void syncConfiguration();

private:
    // Data
    QSettings *_applicationSettings;
};

#endif // CONFIGURATIONMANAGER_H
