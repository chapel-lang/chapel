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


#include "dataaggregatorwidget.h"
#include "mainwindow.h"
#include "widgetwindowsmanager.h"
#include "datawidget.h"
#include "configurationmanager.h"
#include <qwt_plot_item.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_plot.h>
#include <QToolBar>
#include <QVBoxLayout>
#include <QCloseEvent>

DataAggregatorWidget::DataAggregatorWidget(
        WidgetWindowsManager *widgetWindowsManager, MainWindow *mainWindow,
        bool inside, QList<int> dataIds) :
	AbstractWidgetWindow(widgetWindowsManager, mainWindow, inside)
{
    // Init UI
    setWindowTitle(tr("Aggregator"));
    setWindowIcon(QIcon(":/images/widget.png"));
    // Buttons
    QToolBar *buttonsToolBar = new QToolBar();
    // In/out button
    buttonsToolBar->addWidget(_inOutButton);
    // Add data button
    _addDataButton = new QToolButton();
    _addDataButton->setIcon(QIcon(":/images/add.png"));
    _addDataButton->setPopupMode(QToolButton::InstantPopup);
    _addDataButton->setToolTip(tr("Add data source"));
    buttonsToolBar->addSeparator();
    buttonsToolBar->addWidget(_addDataButton);
    // Remove data button
    _removeDataButton = new QToolButton();
    _removeDataButton->setIcon(QIcon(":/images/remove.png"));
    _removeDataButton->setPopupMode(QToolButton::InstantPopup);
    _removeDataButton->setToolTip(tr("Remove data source"));
    buttonsToolBar->addWidget(_removeDataButton);
    // Set layout
    QVBoxLayout *layout = new QVBoxLayout();
    setLayout(layout);
    layout->addWidget(buttonsToolBar);

    // Init actions
    for (int i = 0; i < _mainWindow->dataDescriptions()->count(); i++)
    {
        if (_mainWindow->dataDescriptions()->at(i)->type != DATA_TYPE_BOOL)
        {
            QAction *addDataAction = new QAction(
                    _mainWindow->dataDescriptions()->at(i)->descriptionString,
                    _addDataButton);
            addDataAction->setData(_mainWindow->dataDescriptions()->at(i)->id);

            QObject::connect(addDataAction, SIGNAL(triggered()), this,
                             SLOT(dataAdded()));

            if (_mainWindow->dataDescriptions()->at(i)->widget
                == DATA_WIDGET_NONE)
            {
                addDataAction->setEnabled(false);
            }
            _addDataButton->addAction(addDataAction);
        }
    }

    // Init curve colors
    _curveColors.append(Qt::black);
    _curveColors.append(Qt::red);
    _curveColors.append(Qt::green);
    _curveColors.append(Qt::blue);
    _curveColors.append(Qt::magenta);
    _curveColors.append(Qt::yellow);
    _curveColors.append(Qt::gray);
    _curveColors.append(Qt::darkRed);
    _curveColors.append(Qt::darkGreen);
    _curveColors.append(Qt::darkBlue);
    _curveColors.append(Qt::darkCyan);
    _curveColors.append(Qt::darkMagenta);
    _curveColors.append(Qt::darkYellow);
    _curveColors.append(Qt::cyan);
    _colorIterator = new QListIterator<QColor> (_curveColors);

    // Create internal widget
    createInternalWidget();

    // Know when a data source is disabled / enabled
    QObject::connect(_mainWindow, SIGNAL(dataDisabled(int)),
                     this, SLOT(disableData(int)));
    QObject::connect(_mainWindow, SIGNAL(dataEnabled(int)),
                     this, SLOT(enableData(int)));

    // Init data
    if (dataIds.empty() == false)
    {
        for (int i = 0; i < dataIds.count(); i++)
        {
            addData(dataIds.at(i));
        }
    }

    // Setup automatic cleanup
    QObject::connect(this, SIGNAL(destroyed()), _mainWindow,
                     SLOT(removeDestroyedDataAggregatorWidgets()));

    qDebug() << "DataAggregatorWidget" << windowId() << ": initializing";
}

DataAggregatorWidget::~DataAggregatorWidget()
{
    delete _colorIterator;
    delete _addDataButton;
    delete _removeDataButton;
    delete _plot;

    qDebug() << "DataAggregatorWidget" << windowId() << ": terminating";
}

void DataAggregatorWidget::closeEvent(QCloseEvent *ce)
{
    ce->accept();
}

void DataAggregatorWidget::createInternalWidget()
{
    _plot = new QwtPlot(QwtText(tr("Aggregator")));
    QwtLegend *legend = new QwtLegend;
    legend->setItemMode(QwtLegend::CheckableItem);
    _plot->insertLegend(legend, QwtPlot::BottomLegend);
    _plot->setMinimumSize(300, 200);
    _plot->setAttribute(Qt::WA_DeleteOnClose);
    layout()->addWidget(_plot);

    QObject::connect(_plot, SIGNAL(legendChecked(QwtPlotItem *, bool)),
                     SLOT(curveChecked(QwtPlotItem *, bool)));
}

void DataAggregatorWidget::addData(int dataId)
{
    DataWidget *dataWidget = _mainWindow->dataWidgetFromDataId(dataId);
    if (dataWidget != 0)
    {
        qDebug() << "Aggregator" << _windowId << ": adding new data of id"
                << dataId;

        switch (dataWidget->description()->type)
        {
        case DATA_TYPE_INT:
            {
                QObject::connect(dataWidget, SIGNAL(valueChanged(int,int)),
                                 this, SLOT(update(int,int)));
                break;
            }

        case DATA_TYPE_FLOAT:
            {
                QObject::connect(dataWidget, SIGNAL(valueChanged(double,int)),
                                 this, SLOT(update(double,int)));
                break;
            }

        default:
            qDebug() << "Aggregator" << _windowId
                    << ": attempt to add data of unsupported type (id :"
                    << dataId << ")";
        }

        QwtPlotCurve *curve = new QwtPlotCurve(
                _mainWindow->dataDescriptionFromId(dataId)->descriptionString);
        if (_mainWindow->configurationManager()->antialiasing() == true)
        {
            curve->setRenderHint(QwtPlotItem::RenderAntialiased);
        }
        QColor c;
        if (_colorIterator->hasNext())
        {
            c = _colorIterator->next();
            c.setAlpha(150);
            curve->setPen(c);
        }
        _curves.insert(dataId, curve);
        CurveData curveData;
        curveData.xData = new QVector<double> ();
        curveData.yData = new QVector<double> ();
        _curvesData.insert(dataId, curveData);
        curve->attach(_plot);
    }
    else
    {
        qDebug() << "Aggregator" << _windowId << ": failed to add data of id"
                << dataId;
    }
}

void DataAggregatorWidget::removeData(int dataId)
{
    DataWidget *dataWidget = _mainWindow->dataWidgetFromDataId(dataId);
    if (dataWidget != 0)
    {
        qDebug() << "Aggregator" << _windowId << ": removing data of id"
                << dataId;

        switch (dataWidget->description()->type)
        {
        case DATA_TYPE_INT:
            QObject::disconnect(dataWidget, SIGNAL(valueChanged(int,int)),
                                this, SLOT(update(int,int)));
            break;

        case DATA_TYPE_FLOAT:
            QObject::disconnect(dataWidget, SIGNAL(valueChanged(double,int)),
                                this, SLOT(update(double,int)));
            break;

        default:
            ;
        }

        _curves.value(dataId)->detach();
        _curvesData.remove(dataId);
        _curves.remove(dataId);
    }
    else
    {
        qDebug() << "Aggregator" << _windowId
                << ": failed to remove data of id" << dataId;
    }
}

void DataAggregatorWidget::update(int value, int dataId)
{
    qDebug() << "Aggregator" << _windowId << ": updating data of id" << dataId
            << "with value" << value;

    _curvesData.value(dataId).xData->append(_mainWindow
                                            ->effectiveRunningTime());
    _curvesData.value(dataId).yData->append(value);
#if QWT_VERSION >= 0x060000
    _curves.value(dataId)->setRawSamples(
            _curvesData.value(dataId).xData->data(),
            _curvesData.value(dataId).yData->data(),
            _curvesData.value(dataId).xData->size());
#else
#  warning Old version of qwt being used, data aggregator will not work.
#endif
    _plot->replot();
}

void DataAggregatorWidget::update(double value, int dataId)
{
    qDebug() << "Aggregator" << _windowId << ": updating data of id" << dataId
            << "with value" << value;

    _curvesData.value(dataId).xData->append(_mainWindow
                                            ->effectiveRunningTime());
    _curvesData.value(dataId).yData->append(value);
#if QWT_VERSION >= 0x060000
    _curves.value(dataId)->setRawSamples(
            _curvesData.value(dataId).xData->data(),
            _curvesData.value(dataId).yData->data(),
            _curvesData.value(dataId).xData->size());
#else
#  warning Old version of qwt being used, data aggregator will not work.
#endif
    _plot->replot();
}

QSize DataAggregatorWidget::minimumInternalWidgetSize() const
{
    return _plot->minimumSize();
}

QList<int> DataAggregatorWidget::aggregatedData() const
{
    QList<int> addedData;

    for (int i = 0; i < _curves.values().count(); i++)
    {
        addedData.append(_curves.key(_curves.values().at(i)));
    }

    return addedData;
}

void DataAggregatorWidget::curveChecked(QwtPlotItem *curve, bool checked)
{
    curve->setVisible(!checked);
}

void DataAggregatorWidget::dataAdded()
{
    QAction *action = (QAction*) QObject::sender();
    int dataId = action->data().toInt();

    _addDataButton->removeAction(action);
    addData(dataId);

    QAction *removeDataAction = new QAction(
            _mainWindow->dataDescriptionFromId(dataId)->descriptionString,
            _removeDataButton);
    removeDataAction->setData(dataId);
    QObject::connect(removeDataAction, SIGNAL(triggered()), this,
                     SLOT(dataRemoved()));
    _removeDataButton->addAction(removeDataAction);
}

void DataAggregatorWidget::dataRemoved()
{
    QAction *action = (QAction*) QObject::sender();
    int dataId = action->data().toInt();

    _removeDataButton->removeAction(action);
    removeData(dataId);

    QAction *addDataAction = new QAction(
            _mainWindow->dataDescriptionFromId(dataId)->descriptionString,
            _addDataButton);
    addDataAction->setData(dataId);
    QObject::connect(addDataAction, SIGNAL(triggered()), this,
                     SLOT(dataAdded()));
    _addDataButton->addAction(addDataAction);
}

void DataAggregatorWidget::disableData(int dataId)
{
    // Search the action on the add button
    for (int i = 0; i < _addDataButton->actions().count(); i++)
    {
        if (_addDataButton->actions().at(i)->data().toInt() == dataId)
        {
            _addDataButton->actions().at(i)->setEnabled(false);

            return;
        }
    }
    // If not found, it has already been added
    for (int i = 0; i < _removeDataButton->actions().count(); i++)
    {
        if (_removeDataButton->actions().at(i)->data().toInt() == dataId)
        {
            _removeDataButton->removeAction(_removeDataButton->actions().at(i));
            removeData(dataId);

            QAction
                    *action =
                    new QAction(
                            _mainWindow->dataDescriptionFromId(dataId)
                            ->descriptionString,
                            _addDataButton);
            action->setData(dataId);
            QObject::connect(action, SIGNAL(triggered()), this,
                             SLOT(dataAdded()));
            action->setEnabled(false);
            _addDataButton->addAction(action);

            return;
        }
    }
}

void DataAggregatorWidget::enableData(int dataId)
{
    // Search the action on the add button
    for (int i = 0; i < _addDataButton->actions().count(); i++)
    {
        if (_addDataButton->actions().at(i)->data().toInt() == dataId)
        {
            _addDataButton->actions().at(i)->setEnabled(true);

            return;
        }
    }
}
