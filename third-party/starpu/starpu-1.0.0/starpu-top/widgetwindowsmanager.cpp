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


#include "widgetwindowsmanager.h"
#include "mainwindow.h"
#include "abstractwidgetwindow.h"
#include "datawidget.h"
#include "dataaggregatorwidget.h"
#include <QDebug>

WidgetWindowsManager::WidgetWindowsManager(MainWindow *mainWindow,
                                           QMdiArea *mdiArea)
{
    static bool instanciated = false;

    Q_ASSERT_X(instanciated == false, "WidgetWindowsManager's' constructor",
               "Singleton pattern violated - "
               "WidgetWindowsManager instanciated more than once");

    qDebug() << "WindowsWidgetManager : initializing";

    _mainWindow = mainWindow;
    _mdiArea = mdiArea;

    instanciated = true;
}

WidgetWindowsManager::~WidgetWindowsManager()
{
    qDebug() << "WindowsWidgetManager : terminating";
}

void WidgetWindowsManager::displayWidgetWindow(
        AbstractWidgetWindow *widgetWindow) const
{
    qDebug() << "WidgetWindowsManager : displaying widget window";

    if (widgetWindow->isInside() == true)
    { // MDI mode
        _mdiArea->addSubWindow(widgetWindow);
    }

    widgetWindow->setVisible(true);
}

void WidgetWindowsManager::mdiToFlyingWindows() const
{
    qDebug() << "WidgetWindowsManager : putting all widget windows outside";

    _mdiArea->setVisible(false);

    // Data widgets
    for (int i = 0; i < _mainWindow->dataWidgets()->count(); i++)
    {
        AbstractWidgetWindow *widgetWindow = _mainWindow->dataWidgets()->at(i);
        if (widgetWindow != 0)
        {
            if (widgetWindow->isInside())
            {
                _mdiArea->removeSubWindow(widgetWindow);
                widgetWindow->setInside(false);
                displayWidgetWindow(widgetWindow);
            }
        }
    }
    // Data aggregator widgets
    for (int i = 0; i < _mainWindow->dataAggregatorWidgets()->count(); i++)
    {
        AbstractWidgetWindow
                *widgetWindow =
                (AbstractWidgetWindow*) _mainWindow
                ->dataAggregatorWidgets()->at(i).data();
        if (widgetWindow != 0)
        {
            if (widgetWindow->isInside())
            {
                _mdiArea->removeSubWindow(widgetWindow);
                widgetWindow->setInside(false);
                displayWidgetWindow(widgetWindow);
            }
        }
    }

    _mdiArea->closeAllSubWindows();
}

void WidgetWindowsManager::flyingWindowsToMdi() const
{
    qDebug() << "WidgetWindowsManager : putting all widget windows inside";

    _mdiArea->setVisible(true);

    // Data widgets
    for (int i = 0; i < _mainWindow->dataWidgets()->count(); i++)
    {
        AbstractWidgetWindow *widgetWindow = _mainWindow->dataWidgets()->at(i);
        if (widgetWindow != 0)
        {
            if (widgetWindow->isInside() == false)
            {
                widgetWindow->setInside(true);
                displayWidgetWindow(widgetWindow);
            }
        }
    }
    // Data aggregator widgets
    for (int i = 0; i < _mainWindow->dataAggregatorWidgets()->count(); i++)
    {
        AbstractWidgetWindow
                *widgetWindow =
                (AbstractWidgetWindow*) _mainWindow
                ->dataAggregatorWidgets()->at(i).data();
        if (widgetWindow != 0)
        {
            if (widgetWindow->isInside() == false)
            {
                widgetWindow->setInside(true);
                displayWidgetWindow(widgetWindow);
            }
        }
    }
}

void WidgetWindowsManager::mdiToFlyingWindow(
        AbstractWidgetWindow *widgetWindow) const
{
    QWidget* parentWindow = widgetWindow->parentWidget();
    _mdiArea->removeSubWindow(widgetWindow);
    _mdiArea->removeSubWindow(parentWindow);

    displayWidgetWindow(widgetWindow);
}

void WidgetWindowsManager::flyingWindowToMdi(
        AbstractWidgetWindow *widgetWindow) const
{
    if (_mdiArea->isVisible() == false)
        _mdiArea->setVisible(true);

    displayWidgetWindow(widgetWindow);
}

void WidgetWindowsManager::closeWidgetWindow(
        AbstractWidgetWindow *widgetWindow) const
{
    if (widgetWindow->isInside())
    {
        QWidget* parentWindow = widgetWindow->parentWidget();
        _mdiArea->removeSubWindow(widgetWindow);
        _mdiArea->removeSubWindow(parentWindow);
    }

    widgetWindow->close();
}

void WidgetWindowsManager::closeWidgetWindows() const
{
    qDebug() << "WidgetWindowsManager : closing all widget windows";

    // Data widgets
    for (int i = 0; i < _mainWindow->dataWidgets()->count(); i++)
    {
        AbstractWidgetWindow* widgetWindow =
                _mainWindow->dataWidgets()->at(i).data();
        if (widgetWindow != 0)
        {
            closeWidgetWindow(widgetWindow);
        }
    }
    // Data aggregator widgets
    for (int i = 0; i < _mainWindow->dataAggregatorWidgets()->count(); i++)
    {
        AbstractWidgetWindow
                * widgetWindow =
                (AbstractWidgetWindow*) _mainWindow
                ->dataAggregatorWidgets()->at(i).data();
        if (widgetWindow != 0)
        {
            closeWidgetWindow(widgetWindow);
        }
    }
}

const QList<AbstractWidgetWindow*> WidgetWindowsManager::widgetWindows() const
{
    QList<AbstractWidgetWindow*> widgetWindows;

    // Get data widget windows
    for (int i = 0; i < _mainWindow->dataWidgets()->count(); i++)
    {
        widgetWindows.append(_mainWindow->dataWidgets()->at(i));
    }
    // Get data aggregator widget windows
    for (int i = 0; i < _mainWindow->dataAggregatorWidgets()->count(); i++)
    {
        widgetWindows.append(_mainWindow->dataAggregatorWidgets()->at(i));
    }

    return widgetWindows;
}
