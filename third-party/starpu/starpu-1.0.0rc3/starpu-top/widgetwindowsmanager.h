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


#ifndef WIDGETWINDOWSMANAGER_H
#define WIDGETWINDOWSMANAGER_H

#include <QMdiArea>
#include <QMdiSubWindow>
#include <QPointer>
#include <QSettings>

class MainWindow;
class AbstractWidgetWindow;

class WidgetWindowsManager
{ /* Manages the widget windows of the current session and can save
their states and geometries for use in another session */

public:
    WidgetWindowsManager(
            MainWindow *mainWindow,
            QMdiArea *mdiArea);
    ~WidgetWindowsManager();

    void displayWidgetWindow(AbstractWidgetWindow *widgetWindow) const;
    void flyingWindowsToMdi() const;
    void mdiToFlyingWindows() const;
    void flyingWindowToMdi(AbstractWidgetWindow *widgetWindow) const;
    void mdiToFlyingWindow(AbstractWidgetWindow *widgetWindow) const;
    void closeWidgetWindow(AbstractWidgetWindow *widgetWindow) const;
    void closeWidgetWindows() const;
    const QList<AbstractWidgetWindow*> widgetWindows() const;

private:
    // Data
    MainWindow *_mainWindow;
    QMdiArea *_mdiArea;
};

#endif // WIDGETWINDOWSMANAGER_H
