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


#include "abstractwidgetwindow.h"
#include "widgetwindowsmanager.h"
#include "mainwindow.h"
#include <QVBoxLayout>

AbstractWidgetWindow::AbstractWidgetWindow(
        WidgetWindowsManager *widgetWindowsManager,
        MainWindow *mainWindow,
        bool inside)
            : QWidget(mainWindow)
{
    _widgetWindowsManager = widgetWindowsManager;
    _mainWindow = mainWindow;
    _inside = inside;

    // Init id
    _windowId = generateWindowId();

    // Init GUI
    _sizeGrip = new QSizeGrip(this);
    _sizeGrip->move(width()-32,height()-32);
    _sizeGrip->resize(32,32);
    _inOutButton = new QToolButton();
    QObject::connect(_inOutButton, SIGNAL(clicked()),
                     this, SLOT(on_inOutButton_clicked()));
    _inside ? _inOutButton->setToolTip(tr("To flying window"))
        : _inOutButton->setToolTip(tr("To MDI"));
    _inside ? _inOutButton->setIcon(QIcon(":/images/outside.png"))
        : _inOutButton->setIcon(QIcon(":/images/inside.png"));

    // Set attributes
    setAttribute(Qt::WA_DeleteOnClose);
}

AbstractWidgetWindow::~AbstractWidgetWindow()
{
    delete _sizeGrip;
    delete _inOutButton;
}

/* -------------------------------------------------------------------------- */
/* Events                                                                     */
/* -------------------------------------------------------------------------- */

void AbstractWidgetWindow::mousePressEvent(QMouseEvent *event)
{
    if ((isInside() == false) && (event->button() == Qt::LeftButton))
    {
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void AbstractWidgetWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((isInside() == false) && (event->buttons() & Qt::LeftButton))
    {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void AbstractWidgetWindow::resizeEvent(QResizeEvent *event)
{
    _sizeGrip->move(width()-32,height()-32);
    _sizeGrip->resize(32,32);
}

/* -------------------------------------------------------------------------- */
/* Getters                                                                    */
/* -------------------------------------------------------------------------- */

int AbstractWidgetWindow::windowId() const
{
    return _windowId;
}

bool AbstractWidgetWindow::isInside() const
{
    return _inside;
}

/* -------------------------------------------------------------------------- */
/* Setters                                                                    */
/* -------------------------------------------------------------------------- */

void AbstractWidgetWindow::setInside(bool inside)
{
    _inside = inside;

    if(isInside() == false)
    {
        _inOutButton->setToolTip(tr("To MDI"));
        _inOutButton->setIcon(QIcon(":/images/inside.png"));
    }
    else
    {
        _inOutButton->setToolTip(tr("To flying window"));
        _inOutButton->setIcon(QIcon(":/images/outside.png"));
    }
}

/* -------------------------------------------------------------------------- */
/* GUI interactions                                                           */
/* -------------------------------------------------------------------------- */

void AbstractWidgetWindow::on_inOutButton_clicked()
{
    setInside(!isInside());

    if(_inside)
    {
        _widgetWindowsManager->flyingWindowToMdi(this);
    }
    else
    {
        _widgetWindowsManager->mdiToFlyingWindow(this);
    }
}

/* -------------------------------------------------------------------------- */
/* Window ID generation                                                       */
/* -------------------------------------------------------------------------- */

int AbstractWidgetWindow::generateWindowId()
{
    static int windowId = 0;

    return windowId++;
}
