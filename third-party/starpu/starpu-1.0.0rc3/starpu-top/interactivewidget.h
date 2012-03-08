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


#ifndef INTERACTIVEWIDGET_H
#define INTERACTIVEWIDGET_H

#include <QWidget>
#include <QCloseEvent>
#include <QLabel>
#include <QHBoxLayout>
#include "starpu_top_types.h"

class MainWindow;

class InteractiveWidget: public QWidget
{ /* Widget which represents a single parameter on the server
 and which takes place in the parameters dock of
 the main window. Can display the parameter under
 different forms defined in the interactive widget
 specifications. The internal widget can be changed at any time. */
    Q_OBJECT

public:
    explicit InteractiveWidget(ParamDescription *paramDescription,
                               MainWindow *mainWindow);
    ~InteractiveWidget();

    // Getters
    ParamDescription *description() const;
    QSize minimumInternalWidgetSize() const;

private:
    // Events
    void closeEvent(QCloseEvent *ce);
    // UI components
    MainWindow *_mainWindow;
    QLabel *_label;
    QWidget *_internalWidget;
    QHBoxLayout *_layout;
    // Data
    ParamDescription *_paramDescription;
    const QHash<InteractiveWidgetType, QString> *_interactiveWidgetNames;
    QList<InteractiveWidgetType> _interactiveWidgetPossibilities;
    // Create the internal widget
    void createInternalWidget();

    /** SLOTS **/

public slots:
    // Setters
    void setValue(bool value);
    void setValue(int value);
    void setValue(double value);
    // Other methods
    void recreateInternalWidget();

private slots:
    // Other methods
    void sliderValueChanged();
    void wheelValueChanged();
    void knobValueChanged();
    void dialValueChanged();
    void notifyValueChanged(bool value);
    void notifyValueChanged(int value);
    void notifyValueChanged(double value);
    void widgetTypeChanged();
    void updateAction(InteractiveWidgetType newWidget);

signals:
    void paramValueChanged(int id, bool value);
    void paramValueChanged(int id, int value);
    void paramValueChanged(int id, double value);
};

#endif // INTERACTIVEWIDGET_H
