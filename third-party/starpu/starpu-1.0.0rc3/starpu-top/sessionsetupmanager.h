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


#ifndef SESSIONSETUPMANAGER_H
#define SESSIONSETUPMANAGER_H

class MainWindow;

#include "starpu_top_types.h"
#include <QSettings>

static const QString SESSION_SETUPS_DIR = "./sessionsetups";
static const QString SESSION_SETUPS_FILEEXT = ".ses";

class SessionSetupManager: public QObject
{ /* Saves and loads a session setup. The session setup
 includes the geometries and states of the widget windows
 and the descriptions of the data and the parameters. */
    Q_OBJECT

public:
    SessionSetupManager(MainWindow *mainWindow);
    ~SessionSetupManager();

private:
    // Data
    MainWindow *_mainWindow;
    QSettings *_sessionStateSettings;

public slots:
    bool saveSessionSetup(QString sessionSetupName);
    bool loadSessionSetup(QString sessionSetupName);

signals:
    void descriptionsSetupLoaded(
            QList<DataDescriptionSetup> dataDescriptionsSetup,
            QList<ParamDescriptionSetup> paramDescriptionsSetup);
    void mainWindowSetupLoaded(MainWindowSetup mainWindowSetup);
    void parametersDockSetupLoaded(ParametersDockSetup parametersDockSetup);
    void widgetWindowsSetupLoaded(QList<DataWidgetSetup> dataWidgetsSetup,
                                  QList<DataAggregatorWidgetSetup> dataAggregatorWidgetsSetup);
};

#endif // SESSIONSETUPMANAGER_H
