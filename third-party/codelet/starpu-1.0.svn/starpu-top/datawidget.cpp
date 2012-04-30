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


#include "datawidget.h"
#include "mainwindow.h"
#include "widgetwindowsmanager.h"
#include "configurationmanager.h"
#include <qwt_thermo.h>
#include <qwt_plot.h>
#include <qwt_dial.h>
#include <qwt_dial_needle.h>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include "qledindicator/qledindicator.h"
#include <QAction>
#include <QCloseEvent>
#include <QLCDNumber>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>

DataWidget::DataWidget(DataDescription *dataDescription,
                       WidgetWindowsManager *widgetWindowsManager,
                       MainWindow *mainWindow,
                       bool inside) :
AbstractWidgetWindow(widgetWindowsManager, mainWindow, inside)
{
    _dataDescription = dataDescription;
    _internalWidget = 0;
    // For the plot widget
    _curve = 0;
    _curveData = 0;

    _dataWidgetNames = _mainWindow->dataWidgetNames();
    _dataWidgetPossibilities = _mainWindow->dataWidgetPossibilities() ->value(
            _dataDescription->type).values();

    // Init context menu actions
    QActionGroup *actionGroup = new QActionGroup(this);
    for (int i = 0; i < _dataWidgetPossibilities.count(); i++)
    {
        QAction *action = new QAction(
                _dataWidgetNames ->value(_dataWidgetPossibilities.at(i)),
                actionGroup);
        action->setCheckable(true);
        if (_dataWidgetPossibilities.at(i) == _dataDescription->widget)
        {
            action->setChecked(true);
        }
        QObject::connect(action, SIGNAL(triggered()), this,
                         SLOT(widgetTypeChanged()));
        actionGroup->addAction(action);
    }
    addActions(actionGroup->actions());

    // Set attributes
    setContextMenuPolicy(Qt::ActionsContextMenu);

    // Init GUI
    setWindowTitle(_dataDescription->descriptionString);
    setWindowIcon(QIcon(":/images/widget.png"));
    // Set layout
    QVBoxLayout *globalLayout = new QVBoxLayout();
    setLayout(globalLayout);
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addWidget(_inOutButton);
    QString labelText = _dataDescription->descriptionString;
    if (_dataDescription->descriptionString.size() > 14)
    {
        labelText.truncate(12);
        labelText.append("...");
    }
    QLabel *dataTitle = new QLabel(labelText);
    dataTitle->setToolTip(_dataDescription->descriptionString);
    topLayout->addWidget(dataTitle, 0, Qt::AlignHCenter);
    dataTitle->setFixedHeight(dataTitle->sizeHint().height());
    globalLayout->addLayout(topLayout);

    // Create internal widget
    createInternalWidget();

    setParent(0); // Needed if it is a flying window
    // Will be updated anyway if it goes into the MDI area
    // However the object HAS to be constructed with the main window as parent
    // so it can connect to its slots.

    // Setup automatic cleanup
    QObject::connect(this, SIGNAL(destroyed()), _mainWindow,
                     SLOT(removeDestroyedDataWidgets()));

    qDebug() << "DataWidget : initializing [desc"
            << _dataDescription->descriptionString << "; id"
            << _dataDescription->id << "; type" << _dataDescription->type
            << "; min" << _dataDescription->valMin << "; max"
            << _dataDescription->valMax << "; widget"
            << _dataDescription->widget << "]";
}

DataWidget::~DataWidget()
{
    qDebug() << "DataWidget" << _dataDescription->id << ": terminating";

    delete _internalWidget;
}

void DataWidget::closeEvent(QCloseEvent *ce)
{
    if (isEnabled() == true)
    {// The widget is alive (session active)
        _mainWindow->updateDataWidgetType(_dataDescription->id,
                                          DATA_WIDGET_NONE);
    }

    ce->accept();
}

void DataWidget::recreateInternalWidget()
{
    if (_internalWidget != 0)
    {
        _internalWidget->close();
        _internalWidget = 0;
    }

    createInternalWidget();

    adjustSize();
    if (isInside() == true)
    {
        parentWidget()->resize(minimumInternalWidgetSize() + QSize(85, 85));
    }
    updateAction(_dataDescription->widget);
}

void DataWidget::updateAction(DataWidgetType newWidget)
{
    for (int i = 0; i < actions().count(); i++)
    {
        if (actions().at(i)->text().compare(_dataWidgetNames->value(newWidget))
            == 0)
            {
            actions().at(i)->setChecked(true);
            return;
        }
    }
}

void DataWidget::createInternalWidget()
{
    qDebug() << "Creating the data widget for data id" << _dataDescription->id;

    switch (_dataDescription->widget)
    {
    case DATA_WIDGET_LCD:
	{
            _internalWidget = new QLCDNumber(this);
            _internalWidget->setMinimumSize(50, 50);
            break;
	}

    case DATA_WIDGET_PLOT:
	{
            _internalWidget = new QwtPlot(this);
            _curve = new QwtPlotCurve(_dataDescription->descriptionString);
            if (_mainWindow->configurationManager()->antialiasing() == true)
            {
                _curve->setRenderHint(QwtPlotItem::RenderAntialiased);
            }
            _curve->attach((QwtPlot*) _internalWidget);
            _curveData = new CurveData;
            _curveData->xData = new QVector<double> ();
            _curveData->yData = new QVector<double> ();
            _internalWidget->setMinimumSize(300, 200);
            break;
	}

    case DATA_WIDGET_LEVEL:
	{
            _internalWidget = new QwtThermo(this);

            QwtThermo *widget = (QwtThermo*) _internalWidget;
            widget->setRange(_dataDescription->valMin, _dataDescription->valMax);
            _internalWidget->setMinimumSize(100, 200);
            break;
	}

    case DATA_WIDGET_LED:
	{
            _internalWidget = new QLedIndicator(this);
            _internalWidget->setMinimumSize(50, 50);
            break;
	}

    case DATA_WIDGET_DIAL:
	{
            _internalWidget = new QwtDial(this);
            _internalWidget->setMinimumSize(250, 250);
            QwtDial *widget = (QwtDial*) _internalWidget;
            widget->setReadOnly(true);
            widget->setWrapping(false);
            widget->setOrigin(135.0);
            widget->setRange(_dataDescription->valMin, _dataDescription->valMax);
            widget->setScaleArc(0.0, 270.0);
            widget->scaleDraw()->setSpacing(8);
            QwtDialSimpleNeedle *needle = new QwtDialSimpleNeedle(
                    QwtDialSimpleNeedle::Arrow, true, Qt::red,
                    QColor(Qt::gray).light(130));
            widget->setNeedle(needle);
            widget->setScaleOptions(QwtDial::ScaleTicks | QwtDial::ScaleLabel);
            widget->setScaleTicks(0, 4, 8);
	}

    default:
        ;
    }

    _internalWidget->setAttribute(Qt::WA_DeleteOnClose);
    layout()->addWidget(_internalWidget);
}

/* -------------------------------------------------------------------------- */
/* Getters                                                                    */
/* -------------------------------------------------------------------------- */

DataDescription *DataWidget::description() const
{
    return _dataDescription;
}

QSize DataWidget::minimumInternalWidgetSize() const
{
    return _internalWidget->minimumSize();
}

/* -------------------------------------------------------------------------- */
/* Setters                                                                    */
/* -------------------------------------------------------------------------- */

void DataWidget::setValue(bool value)
{
    switch (_dataDescription->widget)
    {
    case DATA_WIDGET_LED:
	{
            QLedIndicator *widget = qobject_cast<QLedIndicator*> (_internalWidget);
            if (widget != 0)
            {
                widget->setChecked(value);
            }
            else
            {
                qDebug() << "Bad data widget cast (DataWidget::setValue()) !";
            }
            break;
	}

    default:
        ;
    }

    emit valueChanged(value, _dataDescription->id);
}

void DataWidget::setValue(int value)
{
    switch (_dataDescription->widget)
    {
    case DATA_WIDGET_PLOT:
	{
            QwtPlot *widget = qobject_cast<QwtPlot*> (_internalWidget);
            if (widget != 0)
            {
                _curveData->xData->append(_mainWindow->effectiveRunningTime());
                _curveData->yData->append(value);
#if QWT_VERSION >= 0x060000
                _curve->setRawSamples(_curveData->xData->data(),
                                      _curveData->yData->data(), _curveData->xData->size());
#else
#  warning Old version of qwt being used, data aggregator will not work.
#endif
                widget->replot();
            }
            else
            {
                qDebug() << "Bad data widget cast (DataWidget::setValue()) !";
            }
            break;
	}

    case DATA_WIDGET_LCD:
	{
            QLCDNumber *widget = qobject_cast<QLCDNumber*> (_internalWidget);
            if (widget != 0)
            {
                widget->display(value);
            }
            else
            {
                qDebug() << "Bad data widget cast (DataWidget::setValue()) !";
            }
            break;
	}

    case DATA_WIDGET_LEVEL:
	{
            QwtThermo *widget = qobject_cast<QwtThermo*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug() << "Bad data widget cast (DataWidget::setValue()) !";
            }
            break;
	}

    case DATA_WIDGET_DIAL:
	{
            QwtDial *widget = qobject_cast<QwtDial*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug() << "Bad data widget cast (DataWidget::setValue()) !";
            }
            break;
	}

    default:
        ;
    }

    emit valueChanged(value, _dataDescription->id);
}

void DataWidget::setValue(double value)
{
    switch (_dataDescription->widget)
    {
    case DATA_WIDGET_PLOT:
	{
            QwtPlot *widget = qobject_cast<QwtPlot*> (_internalWidget);
            if (widget != 0)
            {
                _curveData->xData->append(_mainWindow->effectiveRunningTime());
                _curveData->yData->append(value);
#if QWT_VERSION >= 0x060000
                _curve->setRawSamples(_curveData->xData->data(),
                                      _curveData->yData->data(), _curveData->xData->size());
#else
#  warning Old version of qwt being used, data aggregator will not work.
#endif
                widget->replot();
            }
            else
            {
                qDebug() << "Bad data widget cast (DataWidget::setValue()) !";
            }
            break;
	}

    case DATA_WIDGET_LCD:
	{
            QLCDNumber *widget = qobject_cast<QLCDNumber*> (_internalWidget);
            if (widget != 0)
            {
                widget->display(value);
            }
            else
            {
                qDebug() << "Bad data widget cast (DataWidget::setValue()) !";
            }
            break;
	}

    case DATA_WIDGET_LEVEL:
	{
            QwtThermo *widget = qobject_cast<QwtThermo*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug() << "Bad data widget cast (DataWidget::setValue()) !";
            }
            break;
	}

    case DATA_WIDGET_DIAL:
	{
            QwtDial *widget = qobject_cast<QwtDial*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug() << "Bad data widget cast (DataWidget::setValue()) !";
            }
            break;
	}

    default:
        ;
    }

    emit valueChanged(value, _dataDescription->id);
}

void DataWidget::widgetTypeChanged()
{
    QAction *action = (QAction*) QObject::sender();

    if(_dataDescription->widget != _dataWidgetNames->key(action->text()))
    {
        _mainWindow->updateDataWidgetType(_dataDescription->id,
                                          _dataWidgetNames->key(action->text()));
    }
}
