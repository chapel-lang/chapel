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


#include "sessionsetupmanager.h"
#include "mainwindow.h"
#include "widgetwindowsmanager.h"
#include "abstractwidgetwindow.h"
#include "datawidget.h"
#include "dataaggregatorwidget.h"
#include <QDebug>
#include <QString>
#include <QFile>
#include <QDockWidget>

SessionSetupManager::SessionSetupManager(MainWindow *mainWindow)
{
    static bool instanciated = false;

    Q_ASSERT_X(instanciated == false, "SessionSetupManager's' constructor",
               "Singleton pattern violated - "
               "SessionSetupManager instanciated more than once");

    qDebug() << "SessionSetupManager : initializing";

    _mainWindow = mainWindow;
    _sessionStateSettings = 0;

    instanciated = true;
}

SessionSetupManager::~SessionSetupManager()
{
    delete _sessionStateSettings;
}

bool SessionSetupManager::saveSessionSetup(QString sessionSetupName)
{
    QString fileName = SESSION_SETUPS_DIR + "/" + sessionSetupName
                       + SESSION_SETUPS_FILEEXT;
    if (QFile(fileName).exists() == true)
    {
        if (QFile(fileName).remove() == false)
        {
            qDebug()
                    << "SessionSetupManager : "
                    "coudln't reinitialize session setup file before saving !";

            return false;
        }
    }

    // Init settings
    delete _sessionStateSettings;
    _sessionStateSettings = new QSettings(fileName, QSettings::IniFormat);

    qDebug() << "SessionSetupManager : saving session setup to"
            << _sessionStateSettings->fileName();

    // Save data setup
    _sessionStateSettings->beginWriteArray("datadescriptions",
                                           _mainWindow->dataDescriptions()->count());
    for (int i = 0; i < _mainWindow->dataDescriptions()->count(); i++)
    {
        _sessionStateSettings->setArrayIndex(i);

        int dataId = _mainWindow->dataDescriptions()->at(i)->id;
        DataType type = _mainWindow->dataDescriptions()->at(i)->type;
        DataWidgetType widget = _mainWindow->dataDescriptions()->at(i)->widget;

        _sessionStateSettings->setValue("id", dataId);
        _sessionStateSettings->setValue("type", (int) type);
        _sessionStateSettings->setValue("widget", (int) widget);
    }
    _sessionStateSettings->endArray();

    // Save parameters setup
    _sessionStateSettings->beginWriteArray("paramdescriptions",
                                           _mainWindow->paramDescriptions()->count());
    for (int i = 0; i < _mainWindow->paramDescriptions()->count(); i++)
    {
        _sessionStateSettings->setArrayIndex(i);

        int paramId = _mainWindow->paramDescriptions()->at(i)->id;
        ParamType type = _mainWindow->paramDescriptions()->at(i)->type;
        InteractiveWidgetType widget =
                _mainWindow->paramDescriptions()->at(i)->widget;

        _sessionStateSettings->setValue("id", paramId);
        _sessionStateSettings->setValue("type", (int) type);
        _sessionStateSettings->setValue("widget", (int) widget);
    }
    _sessionStateSettings->endArray();

    // Save main window setup
    _sessionStateSettings->beginGroup("mainwindow");
    _sessionStateSettings->setValue("displaymode",
                                    (int) _mainWindow->displayMode());
    _sessionStateSettings->setValue("geometry", _mainWindow->saveGeometry());
    _sessionStateSettings->endGroup();

    // Save parameters dock setup
    _sessionStateSettings->beginGroup("parametersdock");
    _sessionStateSettings->setValue("floating",
                                    _mainWindow->parametersDock()->isFloating());
    _sessionStateSettings->setValue("geometry",
                                    _mainWindow->parametersDock()->saveGeometry());
    _sessionStateSettings->endGroup();

    // Save widget windows setup
    _sessionStateSettings->beginWriteArray("widgetwindows",
                                           _mainWindow->widgetWindowsManager()
                                           ->widgetWindows().count());
    for (int i = 0; i
         < _mainWindow->widgetWindowsManager()->widgetWindows().count(); i++)
    {
        _sessionStateSettings->setArrayIndex(i);

        // Retrive general information to save
        bool
                inside =
                _mainWindow->widgetWindowsManager()->widgetWindows().at(
                        i)->isInside();
        QByteArray windowGeometry;
        if (inside == false)
        {
            windowGeometry
                    = _mainWindow->widgetWindowsManager()
                      ->widgetWindows().at(i)->saveGeometry();
        }
        else
        { // Save the geometry of the parent subwindow, not the widget itself
            windowGeometry
                    = _mainWindow->widgetWindowsManager()
                      ->widgetWindows().at(i)->parentWidget()->saveGeometry();
        }

        DataWidget *dataWidget = qobject_cast<DataWidget*> (
                _mainWindow->widgetWindowsManager()->widgetWindows().at(i));
        if (dataWidget != 0)
        { // Data widgets
            // Retrieve specific information to save
            int dataId = dataWidget->description()->id;

            // Save information
            _sessionStateSettings->beginGroup("datawidget");
            _sessionStateSettings->setValue("inside", inside);
            _sessionStateSettings->setValue("geometry", windowGeometry);
            _sessionStateSettings->setValue("dataid", dataId);
            _sessionStateSettings->endGroup();
        }
        else
        { // Data aggregator widgets
            DataAggregatorWidget *dataAggregatorWidget = qobject_cast<
                                                         DataAggregatorWidget*> (
                                                                 _mainWindow->widgetWindowsManager()->widgetWindows().at(i));

            // Retrieve specific information to save
            QList<int> aggregatedData = dataAggregatorWidget->aggregatedData();
            QString aggregatedDataString;
            for (int i = 0; i < aggregatedData.count(); i++)
            {
                aggregatedDataString.append(
                        QString::number(aggregatedData.at(i)) + ",");
            }

            // Save information
            _sessionStateSettings->beginGroup("dataaggregatorwidget");
            _sessionStateSettings->setValue("inside", inside);
            _sessionStateSettings->setValue("geometry", windowGeometry);
            _sessionStateSettings->setValue("dataids", aggregatedDataString);
            _sessionStateSettings->endGroup();
        }
    }
    _sessionStateSettings->endArray();

    // Write the session setup
    _sessionStateSettings->sync();

    return true;
}

bool SessionSetupManager::loadSessionSetup(QString sessionName)
{
    QString fileName = SESSION_SETUPS_DIR + "/" + sessionName
                       + SESSION_SETUPS_FILEEXT;
    if (QFile(fileName).exists() == false)
    {
        qDebug() << "SessionSetupManager : session setup file doesn't exist !";

        return false;
    }

    delete _sessionStateSettings;
    _sessionStateSettings = new QSettings(fileName, QSettings::IniFormat);

    qDebug() << "SessionSetupManager : loading session setup from"
            << _sessionStateSettings->fileName();

    // Load data descriptions setup
    QList < DataDescriptionSetup > dataDescriptionsSetup;
    int size = _sessionStateSettings->beginReadArray("datadescriptions");
    for (int i = 0; i < size; i++)
    {
        _sessionStateSettings->setArrayIndex(i);
        DataDescriptionSetup dataDescriptionSetup;
        bool ok = false;
        dataDescriptionSetup.id = _sessionStateSettings->value("id").toInt(&ok);
        if (ok == false)
        {
            qDebug()
                    << "SessionSetupManager : "
                    "attempted to load incorrect data widget !";

            return false;
        }
        dataDescriptionSetup.type = (DataType) _sessionStateSettings->value(
                "type").toInt(&ok);
        if (ok == false)
        {
            qDebug()
                    << "SessionSetupManager : "
                    "attempted to load incorrect data widget !";

            return false;
        }
        if (dataDescriptionSetup.type != _mainWindow->dataDescriptionFromId(
                dataDescriptionSetup.id)->type)
        {
            qDebug()
                    << "SessionSetupManager : "
                    "incompatible data types between the sessions !";

            return false;
        }
        dataDescriptionSetup.widget
                = (DataWidgetType) _sessionStateSettings->value("widget").toInt(
                        &ok);
        if (ok == false)
        {
            qDebug()
                    << "SessionSetupManager : "
                    "attempted to load incorrect data widget !";

            return false;
        }
        dataDescriptionsSetup.append(dataDescriptionSetup);
    }
    _sessionStateSettings->endArray();

    // Load param descriptions setup
    QList < ParamDescriptionSetup > paramDescriptionsSetup;
    size = _sessionStateSettings->beginReadArray("paramdescriptions");
    for (int i = 0; i < size; i++)
    {
        _sessionStateSettings->setArrayIndex(i);
        ParamDescriptionSetup paramDescriptionSetup;
        bool ok = false;
        paramDescriptionSetup.id
                = _sessionStateSettings->value("id").toInt(&ok);
        if (ok == false)
        {
            qDebug()
                    << "SessionSetupManager : "
                    "attempted to load incorrect param id !";

            return false;
        }
        paramDescriptionSetup.type = (ParamType) _sessionStateSettings->value(
                "type").toInt(&ok);
        if (ok == false)
        {
            qDebug()
                    << "SessionSetupManager : "
                    "attempted to load incorrect param type !";

            return false;
        }
        if (paramDescriptionSetup.type != _mainWindow->paramDescriptionFromId(
                paramDescriptionSetup.id)->type)
        {
            qDebug()
                    << "SessionSetupManager : "
                    "incompatible data types between the sessions !";

            return false;
        }
        paramDescriptionSetup.widget
                = (InteractiveWidgetType) _sessionStateSettings
                  ->value("widget").toInt(&ok);
        if (ok == false)
        {
            qDebug()
                    << "SessionSetupManager : "
                    "attempted to load incorrect param widget !";

            return false;
        }
        paramDescriptionsSetup.append(paramDescriptionSetup);
    }
    _sessionStateSettings->endArray();

    emit descriptionsSetupLoaded(dataDescriptionsSetup, paramDescriptionsSetup);

    // Load main window setup
    MainWindowSetup mainWindowSetup;
    _sessionStateSettings->beginGroup("mainwindow");
    mainWindowSetup.geometry
            = _sessionStateSettings->value("geometry").toByteArray();
    bool ok = false;
    mainWindowSetup.displayMode = (DisplayMode) _sessionStateSettings->value(
            "displaymode").toInt(&ok);
    if (ok == false)
    {
        qDebug()
                << "SessionSetupManager : "
                "attempted to load incorrect main window display mode !";

        return false;
    }
    _sessionStateSettings->endGroup();

    emit mainWindowSetupLoaded(mainWindowSetup);

    // Load main window setup
    ParametersDockSetup parametersDockSetup;
    _sessionStateSettings->beginGroup("mainwindow");
    parametersDockSetup.floating
            = _sessionStateSettings->value("floating").toBool();
    parametersDockSetup.geometry
            = _sessionStateSettings->value("geometry").toByteArray();
    _sessionStateSettings->endGroup();

    emit parametersDockSetupLoaded(parametersDockSetup);

    // Load widget windows setup
    QList < DataWidgetSetup > dataWidgetsSetup;
    QList < DataAggregatorWidgetSetup > dataAggregatorWidgetsSetup;
    size = _sessionStateSettings->beginReadArray("widgetwindows");
    for (int i = 0; i < size; i++)
    {
        _sessionStateSettings->setArrayIndex(i);
        QString currentGroup = _sessionStateSettings->childGroups().at(0);
        if (currentGroup.compare("datawidget") == 0)
        {
            _sessionStateSettings->beginGroup("datawidget");
            DataWidgetSetup dataWidgetSetup;
            dataWidgetSetup.inside
                    = _sessionStateSettings->value("inside").toBool();
            dataWidgetSetup.geometry
                    = _sessionStateSettings->value("geometry").toByteArray();
            bool ok = false;
            dataWidgetSetup.dataId
                    = _sessionStateSettings->value("dataid").toInt(&ok);
            if (ok == false)
            {
                qDebug()
                        << "SessionSetupManager : "
                        "attempted to load incorrect data id for the data widget !";

                return false;
            }
            dataWidgetsSetup.append(dataWidgetSetup);
            _sessionStateSettings->endGroup();
        }
        else if (currentGroup.compare("dataaggregatorwidget") == 0)
        {
            _sessionStateSettings->beginGroup("dataaggregatorwidget");
            DataAggregatorWidgetSetup dataAggregatorWidgetSetup;
            dataAggregatorWidgetSetup.inside = _sessionStateSettings->value(
                    "inside").toBool();
            dataAggregatorWidgetSetup.geometry = _sessionStateSettings->value(
                    "geometry").toByteArray();
            QString dataIdsString =
                    _sessionStateSettings->value("dataids").toString();
            QStringList dataIdsStringParts = dataIdsString.split(",",
                                                                 QString::SkipEmptyParts);
            QList<int> dataIds;
            for (int i = 0; i < dataIdsStringParts.count(); i++)
            {
                QString dataIdsStringPart = dataIdsStringParts.at(i);
                bool ok = false;
                dataIds.append(dataIdsStringPart.toInt(&ok));
                if (ok == false)
                {
                    qDebug()
                            << "SessionSetupManager : "
                            "attempted to load incorrect data id for data aggregator widget !";

                    return false;
                }
            }
            dataAggregatorWidgetSetup.dataIds = dataIds;
            dataAggregatorWidgetsSetup.append(dataAggregatorWidgetSetup);
            _sessionStateSettings->endGroup();
        }
        else
        {
            qDebug()
                    << "SessionSetupManager : "
                    "attempted to load unknow window widget type !";

            return false;
        }
    }
    _sessionStateSettings->endArray();

    emit widgetWindowsSetupLoaded(dataWidgetsSetup, dataAggregatorWidgetsSetup);

    return true;
}
