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


#include "interactivewidget.h"
#include "mainwindow.h"
#include <QCloseEvent>
#include <QAction>
#include <QCheckBox>
#include <QSpinBox>
#include <QDial>
#include <qwt_knob.h>
#include <qwt_wheel.h>
#include <qwt_slider.h>

InteractiveWidget::InteractiveWidget(ParamDescription *paramDescription,
                                     MainWindow *mainWindow) :
QWidget(mainWindow)
{
    _mainWindow = mainWindow;
    _paramDescription = paramDescription;
    _internalWidget = 0;

    _interactiveWidgetNames = _mainWindow->interactiveWidgetNames();
    _interactiveWidgetPossibilities
            = _mainWindow->interactiveWidgetPossibilities() ->value(
                    _paramDescription->type).values();

    // Init context menu actions
    QActionGroup *actionGroup = new QActionGroup(this);
    for (int i = 0; i < _interactiveWidgetPossibilities.count(); i++)
    {
        QAction *action = new QAction(
                _interactiveWidgetNames ->value(
                        _interactiveWidgetPossibilities.at(i)), actionGroup);
        action->setCheckable(true);
        if (_interactiveWidgetPossibilities.at(i) == _paramDescription->widget)
        {
            action->setChecked(true);
        }
        QObject::connect(action, SIGNAL(triggered()), this,
                         SLOT(widgetTypeChanged()));
        actionGroup->addAction(action);
    }
    addActions(actionGroup->actions());

    // Set attributes
    setAttribute(Qt::WA_DeleteOnClose);
    setContextMenuPolicy(Qt::ActionsContextMenu);

    // Init GUI
    QString labelText = _paramDescription->descriptionString;
    if (_paramDescription->descriptionString.size() > 14)
    {
        labelText.truncate(12);
        labelText.append("...");
    }
    _label = new QLabel(labelText);
    _label->setToolTip(_paramDescription->descriptionString);
    // Set layout
    _layout = new QHBoxLayout(this);
    setLayout( _layout);
    _layout->addWidget(_label);

    // Create internal widget
    createInternalWidget();

    // Bind notifications
    QObject::connect(this, SIGNAL(paramValueChanged(int,bool)),
                     _mainWindow, SLOT(interactiveWidgetUpdated(int,bool)));
    QObject::connect(this, SIGNAL(paramValueChanged(int,int)),
                     _mainWindow, SLOT(interactiveWidgetUpdated(int,int)));
    QObject::connect(this, SIGNAL(paramValueChanged(int,double)),
                     _mainWindow, SLOT(interactiveWidgetUpdated(int,double)));
    // Setup automatic cleanup
    QObject::connect(this, SIGNAL(destroyed()), _mainWindow,
                     SLOT(removeDestroyedInteractiveWidgets()));

    qDebug() << "InteractiveWidget : initializing [desc"
            << _paramDescription->descriptionString << "; id"
            << _paramDescription->id << "; type" << _paramDescription->type
            << "; min" << _paramDescription->valMin << "; max"
            << _paramDescription->valMax << "; init bool"
            << _paramDescription->valInitBool << "; init double"
            << _paramDescription->valInitDouble << "; init enum"
            << _paramDescription->valInitEnum << "; init int"
            << _paramDescription->valInitInt << "; widget"
            << _paramDescription->widget << "]";
}

InteractiveWidget::~InteractiveWidget()
{
    qDebug() << "InteractiveWidget" << _paramDescription->id << ": terminating";
}

void InteractiveWidget::closeEvent(QCloseEvent *ce)
{
    ce->accept();
}

void InteractiveWidget::recreateInternalWidget()
{
    if (_internalWidget != 0)
    {
        _internalWidget->close();
        _internalWidget = 0;
    }

    createInternalWidget();
    adjustSize();
    updateAction(_paramDescription->widget);
}

void InteractiveWidget::updateAction(InteractiveWidgetType newWidget)
{
    for (int i = 0; i < actions().count(); i++)
    {
        if (actions().at(i)->text().compare(
                _interactiveWidgetNames->value(newWidget)) == 0)
        {
            actions().at(i)->setChecked(true);
            return;
        }
    }
}

void InteractiveWidget::createInternalWidget()
{
    qDebug() << "Creating the interactive widget for param id"
            << _paramDescription->id;

    switch (_paramDescription->widget)
    {
    case INTERACTIVE_WIDGET_SLIDER:
	{
            _internalWidget = new QwtSlider(this);
            _internalWidget->setMinimumWidth(100);

            QwtSlider *widget = (QwtSlider*) _internalWidget;
            widget->setRange(_paramDescription->valMin, _paramDescription->valMax);
            widget->setScalePosition(QwtSlider::BottomScale);
            widget->setValue(_paramDescription->valInitInt);
            QObject::connect(widget, SIGNAL(sliderReleased()), this,
                             SLOT(sliderValueChanged()));
            break;
	}

    case INTERACTIVE_WIDGET_KNOB:
	{
            _internalWidget = new QwtKnob(this);
            _internalWidget->setMinimumSize(50, 50);

            QwtKnob *widget = (QwtKnob*) _internalWidget;
            widget->setRange(_paramDescription->valMin, _paramDescription->valMax);
            widget->setValue(_paramDescription->valInitDouble);
            QObject::connect(widget, SIGNAL(sliderReleased()), this,
                             SLOT(knobValueChanged()));
            break;
	}

    case INTERACTIVE_WIDGET_WHEEL:
	{
            _internalWidget = new QwtWheel(this);
            _internalWidget->setMinimumSize(50, 30);

            QwtWheel *widget = (QwtWheel*) _internalWidget;
            widget->setRange(_paramDescription->valMin, _paramDescription->valMax);
            widget->setValue(_paramDescription->valInitDouble);
            QObject::connect(widget, SIGNAL(sliderReleased()), this,
                             SLOT(wheelValueChanged()));
            break;
	}

    case INTERACTIVE_WIDGET_CHECKBOX:
	{
            _internalWidget = new QCheckBox(this);
            _internalWidget->setMinimumSize(25, 25);

            QCheckBox *widget = (QCheckBox*) _internalWidget;
            widget->setChecked(_paramDescription->valInitBool);
            QObject::connect(widget, SIGNAL(clicked(bool)),
                             this, SLOT(notifyValueChanged(bool)));
            break;
	}

    case INTERACTIVE_WIDGET_SPINBOX:
	{
            _internalWidget = new QSpinBox(this);
            _internalWidget->setMinimumSize(75, 30);

            QSpinBox *widget = (QSpinBox*) _internalWidget;
            widget->setRange(_paramDescription->valMin, _paramDescription->valMax);
            widget->setValue(_paramDescription->valInitInt);
            QObject::connect(widget, SIGNAL(valueChanged(int)),
                             this, SLOT(notifyValueChanged(int)));
            break;
	}

    case INTERACTIVE_WIDGET_DOUBLESPINBOX:
	{
            _internalWidget = new QDoubleSpinBox(this);
            _internalWidget->setMinimumSize(75, 30);

            QDoubleSpinBox *widget = (QDoubleSpinBox*) _internalWidget;
            widget->setRange(_paramDescription->valMin, _paramDescription->valMax);
            widget->setValue(_paramDescription->valInitDouble);
            QObject::connect(widget, SIGNAL(valueChanged(double)),
                             this, SLOT(notifyValueChanged(double)));
            break;
	}

    case INTERACTIVE_WIDGET_DIAL:
	{
            _internalWidget = new QDial(this);
            _internalWidget->setMinimumSize(80, 40);

            QDial *widget = (QDial*) _internalWidget;
            widget->setRange(_paramDescription->valMin, _paramDescription->valMax);
            widget->setValue(_paramDescription->valInitInt);
            QObject::connect(widget, SIGNAL(sliderReleased()), this,
                             SLOT(dialValueChanged()));
            break;
	}

    case INTERACTIVE_WIDGET_COMBOBOX:
	{
            _internalWidget = new QComboBox(this);
            _internalWidget->setMinimumSize(75, 25);

            QComboBox *widget = (QComboBox*) _internalWidget;
            widget->addItems(_paramDescription->enumValues);
            widget->setCurrentIndex(_paramDescription->valInitEnum);
            QObject::connect(widget, SIGNAL(currentIndexChanged(int)),
                             this, SLOT(notifyValueChanged(int)));
            break;
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

ParamDescription *InteractiveWidget::description() const
{
    return _paramDescription;
}

QSize InteractiveWidget::minimumInternalWidgetSize() const
{
    return _internalWidget->minimumSize();
}

/* -------------------------------------------------------------------------- */
/* Setters                                                                    */
/* -------------------------------------------------------------------------- */

void InteractiveWidget::setValue(bool value)
{
    switch (_paramDescription->widget)
    {
    case INTERACTIVE_WIDGET_CHECKBOX:
	{
            QCheckBox *widget = qobject_cast<QCheckBox*> (_internalWidget);
            if (widget != 0)
            {
                widget->setChecked(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    default:
        ;
    }
}

void InteractiveWidget::setValue(int value)
{
    switch (_paramDescription->widget)
    {
    case INTERACTIVE_WIDGET_SLIDER:
	{
            QwtSlider *widget = qobject_cast<QwtSlider*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    case INTERACTIVE_WIDGET_KNOB:
	{
            QwtKnob *widget = qobject_cast<QwtKnob*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    case INTERACTIVE_WIDGET_WHEEL:
	{
            QwtWheel *widget = qobject_cast<QwtWheel*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    case INTERACTIVE_WIDGET_SPINBOX:
	{
            QSpinBox *widget = qobject_cast<QSpinBox*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    case INTERACTIVE_WIDGET_DOUBLESPINBOX:
	{
            QDoubleSpinBox *widget =
                    qobject_cast<QDoubleSpinBox*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    case INTERACTIVE_WIDGET_DIAL:
	{
            QDial *widget = qobject_cast<QDial*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    case INTERACTIVE_WIDGET_COMBOBOX:
	{
            QComboBox *widget = qobject_cast<QComboBox*> (_internalWidget);
            if (widget != 0)
            {
                if (value >= _paramDescription->valMin && value
                    <= _paramDescription->valMax)
                {
                    widget->setCurrentIndex(value);
                }
                else
                {
                    qDebug()
                            << "Trying to set out of range enum value for enum widget "
                            << _paramDescription->id << "value" << value;
                }
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    default:
        ;
    }
}

void InteractiveWidget::setValue(double value)
{
    switch (_paramDescription->widget)
    {
    case INTERACTIVE_WIDGET_SLIDER:
	{
            QwtSlider *widget = qobject_cast<QwtSlider*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    case INTERACTIVE_WIDGET_KNOB:
	{
            QwtKnob *widget = qobject_cast<QwtKnob*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    case INTERACTIVE_WIDGET_WHEEL:
	{
            QwtWheel *widget = qobject_cast<QwtWheel*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    case INTERACTIVE_WIDGET_DOUBLESPINBOX:
	{
            QDoubleSpinBox *widget =
                    qobject_cast<QDoubleSpinBox*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    case INTERACTIVE_WIDGET_DIAL:
	{
            QDial *widget = qobject_cast<QDial*> (_internalWidget);
            if (widget != 0)
            {
                widget->setValue(value);
            }
            else
            {
                qDebug()
                        << "Bad interactive widget cast "
                        "(InteractiveWidget::setValue()) !";
            }
            break;
	}

    default:
        ;
    }
}

/* -------------------------------------------------------------------------- */
/* Other methods                                                              */
/* -------------------------------------------------------------------------- */

void InteractiveWidget::sliderValueChanged()
{
    QwtSlider *widget = (QwtSlider*) QObject::sender();

    if (_paramDescription->type == PARAM_TYPE_INT)
    {
        int valueInt = (int) widget->value();
        notifyValueChanged(valueInt);
    }
    else
    { // Param type is double
        double valueDouble = (double) widget->value();
        notifyValueChanged(valueDouble);
    }
}

void InteractiveWidget::wheelValueChanged()
{
    QwtWheel *widget = (QwtWheel*) QObject::sender();

    if (_paramDescription->type == PARAM_TYPE_INT)
    {
        int valueInt = (int) widget->value();
        notifyValueChanged(valueInt);
    }
    else
    { // Param type is double
        double valueDouble = (double) widget->value();
        notifyValueChanged(valueDouble);
    }
}

void InteractiveWidget::knobValueChanged()
{
    QwtKnob *widget = (QwtKnob*) QObject::sender();

    if (_paramDescription->type == PARAM_TYPE_INT)
    {
        int valueInt = (int) widget->value();
        notifyValueChanged(valueInt);
    }
    else
    { // Param type is double
        double valueDouble = (double) widget->value();
        notifyValueChanged(valueDouble);
    }
}

void InteractiveWidget::dialValueChanged()
{
    QDial *widget = (QDial*) QObject::sender();

    if (_paramDescription->type == PARAM_TYPE_INT)
    {
        int valueInt = (int) widget->value();
        notifyValueChanged(valueInt);
    }
    else
    { // Param type is double
        double valueDouble = (double) widget->value();
        notifyValueChanged(valueDouble);
    }
}

void InteractiveWidget::notifyValueChanged(bool value)
{
    emit paramValueChanged(_paramDescription->id, value);
}

void InteractiveWidget::notifyValueChanged(int value)
{
    emit paramValueChanged(_paramDescription->id, value);
}

void InteractiveWidget::notifyValueChanged(double value)
{
    emit paramValueChanged(_paramDescription->id, value);
}

void InteractiveWidget::widgetTypeChanged()
{
    QAction *action = (QAction*) QObject::sender();

    if(_paramDescription->widget != _interactiveWidgetNames->key(action->text()))
    {
        _mainWindow->updateInteractiveWidgetType
                (_paramDescription->id,
                 _interactiveWidgetNames->key(action->text()));
    }
}
