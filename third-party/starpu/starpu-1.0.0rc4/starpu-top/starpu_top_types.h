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


#ifndef STARPU_TOP_TYPES_H
#define STARPU_TOP_TYPES_H

#include <QString>
#include <QStringList>
#include <QVector>

/* -------------------------------------------------------------------------- */
/* Display modes                                                              */
/* -------------------------------------------------------------------------- */

enum DisplayMode
{
    DISPLAY_FLYING_WINDOWS = 0, DISPLAY_MDI_NORMAL = 1,
};

/* -------------------------------------------------------------------------- */
/* Widgets                                                                    */
/* -------------------------------------------------------------------------- */

// Types
enum DataWidgetType
{
    // Data widgets
    /** You must pass any modification to the following functions :
	 MainWindow::initDataWidgetLists(),
	 DataWidget::DataWidget(),
	 DataWidget::updateValue() **/

    DATA_WIDGET_NONE = -1,

    DATA_WIDGET_LCD = 0,
    DATA_WIDGET_PLOT = 1,
    DATA_WIDGET_LEVEL = 2,
    DATA_WIDGET_LED = 3,
    DATA_WIDGET_DIAL = 4,
};

enum InteractiveWidgetType
{
    // Interactive widgets
    /** You must pass any modification to the following functions :
	 MainWindow::initInteractiveWidgetLists(),
	 InteractiveWidget::InteractiveWidget() **/
    INTERACTIVE_WIDGET_NONE = -1,

    INTERACTIVE_WIDGET_SLIDER = 0,
    INTERACTIVE_WIDGET_KNOB = 1,
    INTERACTIVE_WIDGET_WHEEL = 2,
    INTERACTIVE_WIDGET_CHECKBOX = 3,
    INTERACTIVE_WIDGET_SPINBOX = 4,
    INTERACTIVE_WIDGET_DOUBLESPINBOX = 5,
    INTERACTIVE_WIDGET_DIAL = 6,
    INTERACTIVE_WIDGET_COMBOBOX = 7,
};

static const DataWidgetType DEFAULT_DATA_WIDGET_BOOL = DATA_WIDGET_LED;
static const DataWidgetType DEFAULT_DATA_WIDGET_INT = DATA_WIDGET_LCD;
static const DataWidgetType DEFAULT_DATA_WIDGET_FLOAT = DATA_WIDGET_LCD;
static const InteractiveWidgetType DEFAULT_INTERACTIVE_WIDGET_BOOL =
        INTERACTIVE_WIDGET_CHECKBOX;
static const InteractiveWidgetType DEFAULT_INTERACTIVE_WIDGET_INT =
        INTERACTIVE_WIDGET_SPINBOX;
static const InteractiveWidgetType DEFAULT_INTERACTIVE_WIDGET_FLOAT =
        INTERACTIVE_WIDGET_DOUBLESPINBOX;
static const InteractiveWidgetType DEFAULT_INTERACTIVE_WIDGET_ENUM =
        INTERACTIVE_WIDGET_COMBOBOX;

enum DataType
{
    // Data
    /** If types were to be added the following functions should be updated :
	 PreferencesDialog::initDataWidgetLists() **/
    DATA_TYPE_BOOL = 0,
    DATA_TYPE_INT = 1,
    DATA_TYPE_FLOAT = 2,
};

enum ParamType
{
    // Params
    /** If types were to be added the following functions should be updated :
	 PreferencesDialog::initInteractiveWidgetLists() **/
    PARAM_TYPE_BOOL = 0,
    PARAM_TYPE_INT = 1,
    PARAM_TYPE_FLOAT = 2,
    PARAM_TYPE_ENUM = 4,
};

enum starpu_top_device_type
{
    SERVERDEVICE_CPU = 0,
    SERVERDEVICE_CUDA = 1,
    SERVERDEVICE_OPENCL = 2,
    SERVERDEVICE_GORDON = 3,
};

// Server devices
typedef struct
{
    int id;
    starpu_top_device_type type;
    QString name;
} starpu_top_device;

// Server tasks
typedef struct
{
    int taskId;
    int deviceId;
    qlonglong timestampStart;
    qlonglong timestampEnd;
} starpu_top_task;

// Descriptions
typedef struct
{ // Certain fields must be ignored depending on type
    int id;
    QString descriptionString;
    DataWidgetType widget;
    DataType type;
    double valMax;
    double valMin;
} DataDescription;

typedef struct
{ // Certain fields must be ignored depending on type
    int id;
    QString descriptionString;
    InteractiveWidgetType widget;
    ParamType type;
    bool valInitBool;
    int valInitInt;
    double valInitDouble;
    int valInitEnum;
    double valMax;
    double valMin;
    QStringList enumValues;
} ParamDescription;

/* --------------------------------------------------------------------------- */
/* Widget lists items data                                                     */
/* --------------------------------------------------------------------------- */

typedef struct
{
    int id;
    DataWidgetType widget;
} DataWidgetListItemData;

typedef struct
{
    int id;
    InteractiveWidgetType widget;
} InteractiveWidgetListItemData;

/* --------------------------------------------------------------------------- */
/* Plot curves data                                                            */
/* --------------------------------------------------------------------------- */

typedef struct
{
    QVector<double> *xData;
    QVector<double> *yData;
} CurveData;

/* --------------------------------------------------------------------------- */
/* Communication Manager states                                                */
/* --------------------------------------------------------------------------- */

enum CommunicationState
{
    COM_STATE_INIT = 0,
    COM_STATE_INIT_SERVERINFO = 1,
    COM_STATE_INIT_DATA = 2,
    COM_STATE_INIT_PARAMS = 3,
    COM_STATE_INIT_DEV = 4,

    COM_STATE_READY = 8,

    COM_STATE_LOOP = 10,
};

/* --------------------------------------------------------------------------- */
/* Communication protocol messages                                             */
/* --------------------------------------------------------------------------- */

enum CommunicationInMessageType
{
    /* Complete commands */
    COM_MSG_IN_TYPE_BOOL = 0,
    COM_MSG_IN_TYPE_INT = 1,
    COM_MSG_IN_TYPE_FLOAT = 2,
    COM_MSG_IN_TYPE_ENUM = 3,

    COM_MSG_IN_SERVERINFO_BEGIN = 5,
    COM_MSG_IN_SERVERINFO_END = 6,

    COM_MSG_IN_DATA_BEGIN = 10,
    COM_MSG_IN_DATA_END = 11,

    COM_MSG_IN_PARAMS_BEGIN = 20,
    COM_MSG_IN_PARAMS_END = 21,

    COM_MSG_IN_DEV_BEGIN = 30,
    COM_MSG_IN_DEV_END = 31,
    COM_MSG_IN_DEV_CPU = 32,
    COM_MSG_IN_DEV_CUDA = 33,
    COM_MSG_IN_DEV_OPENCL = 34,
    COM_MSG_IN_DEV_GORDON = 35,

    COM_MSG_IN_READY = 40,

    COM_MSG_IN_SET = 50,

    COM_MSG_IN_PREV = 60,
    COM_MSG_IN_START = 61,
    COM_MSG_IN_END = 62,

    COM_MSG_IN_UPDATE = 70,

    COM_MSG_IN_DEBUG = 80,
    COM_MSG_IN_DEBUG_ON = 81,
    COM_MSG_IN_DEBUG_OFF = 82,
    COM_MSG_IN_DEBUG_MESSAGE = 83,
    COM_MSG_IN_DEBUG_LOCK = 84,

    /* Shortened commands */
    COM_MSG_IN_SHORT_UPDATE = 100,

    COM_MSG_IN_SHORT_PREV = 110,
    COM_MSG_IN_SHORT_START = 111,
    COM_MSG_IN_SHORT_END = 112,
};

enum CommunicationOutMessageType
{
    /* Complete commands */
    COM_MSG_OUT_GO = 0,

    COM_MSG_OUT_ENABLE = 10,
    COM_MSG_OUT_DISABLE = 11,

    COM_MSG_OUT_SET = 20,

    COM_MSG_OUT_DEBUG = 30,
    COM_MSG_OUT_DEBUG_ON = 31,
    COM_MSG_OUT_DEBUG_OFF = 32,
    COM_MSG_OUT_DEBUG_STEP = 33,
};

/* --------------------------------------------------------------------------- */
/* Session setup data for loading session setups                               */
/* --------------------------------------------------------------------------- */

typedef struct
{
    int id;
    DataType type;
    DataWidgetType widget;
} DataDescriptionSetup;

typedef struct
{
    int id;
    ParamType type;
    InteractiveWidgetType widget;
} ParamDescriptionSetup;

typedef struct
{
    QByteArray geometry;
    DisplayMode displayMode;
} MainWindowSetup;

typedef struct
{
    bool floating;
    QByteArray geometry;
} ParametersDockSetup;

typedef struct
{
    bool inside;
    QByteArray geometry;
    int dataId;
} DataWidgetSetup;

typedef struct
{
    bool inside;
    QByteArray geometry;
    QList<int> dataIds;
} DataAggregatorWidgetSetup;

#endif // STARPU_TOP_TYPES_H
