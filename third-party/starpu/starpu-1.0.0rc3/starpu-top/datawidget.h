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


#ifndef DATAWIDGET_H
#define DATAWIDGET_H

class MainWindow;
class WidgetWindowsManager;
class QwtPlotCurve;
class QwtPlot;

#include "starpu_top_types.h"
#include "abstractwidgetwindow.h"

class DataWidget : public AbstractWidgetWindow
{ /* Displays a single type of data in an internal widget,
     under different forms defined by the data widget specifications.
     The internal widget can be changed at any time. */
    Q_OBJECT

public:
    explicit DataWidget(
            DataDescription *dataDescription,
            WidgetWindowsManager *widgetWindowManager,
            MainWindow *mainWindow,
            bool inside = false);
    ~DataWidget();

    // Getters
    DataDescription *description() const;
    QSize minimumInternalWidgetSize() const;

private:
    // Events
    void closeEvent(QCloseEvent *ce);
    // Metadata
    DataDescription *_dataDescription;
    const QHash<DataWidgetType, QString> *_dataWidgetNames;
    QList<DataWidgetType> _dataWidgetPossibilities;
    // GUI components
    QWidget *_internalWidget;
    QwtPlotCurve *_curve;
    CurveData *_curveData;

public slots:
    // Setters
    void setValue(bool value);
    void setValue(int value);
    void setValue(double value);
    // Other methods
    void recreateInternalWidget();

private slots:
    void createInternalWidget();
    void widgetTypeChanged();
    void updateAction(DataWidgetType newWidget);

signals:
    void valueChanged(bool value, int dataId);
    void valueChanged(int value, int dataId);
    void valueChanged(double value, int dataId);
};

#endif // DATAWIDGET_H
