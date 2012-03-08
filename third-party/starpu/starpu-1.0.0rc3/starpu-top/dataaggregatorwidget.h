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


#ifndef DATAAGGREGATORWIDGET_H
#define DATAAGGREGATORWIDGET_H

class MainWindow;
class WidgetWindowsManager;
class QwtPlotItem;
class QwtPlotCurve;
class QwtPlot;

#include <QHash>
#include <QAction>
#include "starpu_top_types.h"
#include "abstractwidgetwindow.h"

class DataAggregatorWidget : public AbstractWidgetWindow
{ /* Aggregates data from different data sources so they can
     be easily compared. Each data source is displayed as a curve
     in a plot widget. */
    Q_OBJECT

public:
    explicit DataAggregatorWidget(
            WidgetWindowsManager *widgetWindowsManager,
            MainWindow *mainWindow,
            bool inside = false,
            QList<int> dataIds = QList<int>());
    ~DataAggregatorWidget();

    // Getters
    QSize minimumInternalWidgetSize() const;
    QList<int> aggregatedData() const;

private:
    // Metadata
    // Curve colors
    QList<QColor> _curveColors;
    QListIterator<QColor> *_colorIterator;
    // Bound data descriptions
    QHash<int, QwtPlotCurve*> _curves;
    QHash<int, CurveData> _curvesData;
    // GUI components
    QToolButton *_addDataButton;
    QToolButton *_removeDataButton;
    QwtPlot *_plot;

    // Events
    void closeEvent(QCloseEvent *ce);

public slots:
    void addData(int dataId);
    void removeData(int dataId);

private slots:
    void createInternalWidget();
    void update(int value, int dataId);
    void update(double value, int dataId);
    void curveChecked(QwtPlotItem *curve, bool checked);
    void dataAdded();
    void dataRemoved();
    void disableData(int dataId);
    void enableData(int dataId);
};

#endif // DATAAGGREGATORWIDGET_H
