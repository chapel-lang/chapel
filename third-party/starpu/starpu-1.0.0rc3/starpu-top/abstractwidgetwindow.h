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


#ifndef ABSTRACTWIDGETWINDOW_H
#define ABSTRACTWIDGETWINDOW_H

class WidgetWindowsManager;
class MainWindow;

#include <QWidget>
#include <QSizeGrip>
#include <QToolButton>
#include <QMouseEvent>

class AbstractWidgetWindow : public QWidget
{ /* Abstract class representing a widget window.
     It can be put "outside" (flying window) or "inside" (MDI) at any moment.
     The widget window is managed by the widget windows manager. */
    Q_OBJECT

public:
    explicit AbstractWidgetWindow(
            WidgetWindowsManager *widgetWindowsManager,
            MainWindow *mainWindow,
            bool inside = false);
    virtual ~AbstractWidgetWindow();

    // Getters
    int windowId() const;
    bool isInside() const;
    // Setters
    void setInside(bool inside);

protected:
    // Components
    WidgetWindowsManager *_widgetWindowsManager;
    MainWindow *_mainWindow;
    QPoint dragPosition;
    // GUI components
    QSizeGrip *_sizeGrip;
    QToolButton *_inOutButton;
    // Metadata
    int _windowId;
    bool _inside;

    // Window id generation
    int generateWindowId();
    // Events
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);
    virtual void closeEvent(QCloseEvent *ce) = 0;

private slots:
    // GUI interactions
    void on_inOutButton_clicked();
};

#endif // ABSTRACTWIDGETWINDOW_H
