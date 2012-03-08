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


#include "communicationmanager.h"

// Protocol messages delimiters
const char COM_MSG_SEPARATOR = ';';
const char COM_MSG_ENDL = '\n';

CommunicationManager::CommunicationManager(QObject *parent) :
	QTcpSocket(parent)
{
    static bool instanciated = false;

    Q_ASSERT_X(instanciated == false, "CommunicationManager's' constructor",
               "Singleton pattern violated - "
               "CommunicationManager instanciated more than once");

    qDebug() << "CommunicationManager : initializing";

    // Init incoming messages
    initInMessageStrings();
    // Init outgoing messages
    initOutMessageStrings();
    // Init session data and communication states
    initializeSession();

    QObject::connect(this, SIGNAL(readyRead()), this,
                     SLOT(messageReceived()));
    QObject::connect(this, SIGNAL(disconnected()), this,
                     SLOT(clearDescriptions()));
    QObject::connect(this, SIGNAL(disconnected()), this,
                     SLOT(initializeSession()));

    instanciated = true;
}

CommunicationManager::~CommunicationManager()
{
    qDebug() << "CommunicationManager : terminating";

    delete _dataDescriptions;
    delete _paramDescriptions;
    delete _serverDevices;
}

void CommunicationManager::initializeSession()
{
    _dataDescriptions = new QList<DataDescription*> ();
    _paramDescriptions = new QList<ParamDescription*> ();
    _serverDevices = new QList<starpu_top_device> ;

    _serverInfoMsgCount = 0;
    _state = COM_STATE_INIT;
    _initServerInfoCompleted = _initDataCompleted = _initParamsCompleted
                                                    = _initDevCompleted = false;
}

/* -------------------------------------------------------------------------- */
/* Receive messages                                                           */
/* -------------------------------------------------------------------------- */

void CommunicationManager::messageReceived()
{
    while (canReadLine())
    {
        QByteArray message = readLine();
        QString messageString = QString(message).trimmed();

        switch (_state)
        {
        case COM_STATE_INIT:
            {
                if (_beginEndMessageStrings.contains(messageString))
                {
                    parseInitMessage(messageString);
                }
                else
                {
                    emit protocolError(
                            "Unexpected message received in INIT ("
                            + messageString
                            + ")");
                }
                break;
            }

        case COM_STATE_INIT_SERVERINFO:
            parseInitServerInfoMessage(messageString);
            break;

        case COM_STATE_INIT_DATA:
            parseInitDataMessage(messageString);
            break;

        case COM_STATE_INIT_PARAMS:
            parseInitParamsMessage(messageString);
            break;

        case COM_STATE_INIT_DEV:
            parseInitDevMessage(messageString);
            break;

        case COM_STATE_READY:
            parseReadyMessage(messageString);
            break;

        case COM_STATE_LOOP:
            parseLoopMessage(messageString);
            break;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Parse messages                                                             */
/* -------------------------------------------------------------------------- */

void CommunicationManager::parseInitMessage(QString messageString)
{
    switch (_inMessageStrings.value(messageString))
    {
    case COM_MSG_IN_SERVERINFO_BEGIN:
        qDebug() << "CommunicationManager : SERVERINFO BEGIN received in INIT";
        _state = COM_STATE_INIT_SERVERINFO;
        break;

    case COM_MSG_IN_DATA_BEGIN:
        qDebug() << "CommunicationManager : DATA BEGIN received in INIT";
        _state = COM_STATE_INIT_DATA;
        break;

    case COM_MSG_IN_PARAMS_BEGIN:
        qDebug() << "CommunicationManager : PARAMS BEGIN received in INIT";
        _state = COM_STATE_INIT_PARAMS;
        break;

    case COM_MSG_IN_DEV_BEGIN:
        qDebug() << "CommunicationManager : DEV BEGIN received in INIT";
        _state = COM_STATE_INIT_DEV;
        break;

    default:
        ;
    }
}

void CommunicationManager::parseInitServerInfoMessage(QString messageString)
{
    if (_beginEndMessageStrings.contains(messageString))
    {
        if (_inMessageStrings.value(messageString) == COM_MSG_IN_SERVERINFO_END)
        {
            qDebug()
                    << "CommunicationManager : "
                    "SERVERINFO END received in INIT SERVERINFO";
            _initServerInfoCompleted = true;
	    emit protoConnected();

            if (isInitCompleted())
            {
                _state = COM_STATE_READY;
            }
            else
            {
                _state = COM_STATE_INIT;
            }
        }
        else
        {
            emit protocolError(
                    "Bogus message received in INIT SERVERINFO ("
                    + messageString + ")");
        }
    }
    else if (_serverInfoMsgCount == 0)
    { // First server info : Server ID
        qDebug()
                << "CommunicationManager : "
                "SERVERINFO received in INIT SERVERINFO ("
                << messageString + ")";
        _serverID = messageString;
        _serverInfoMsgCount++;
    }
    else if (_serverInfoMsgCount == 1)
    { // Second server info : Server timestamp
        qDebug()
                << "CommunicationManager : "
                "SERVERTIMESTAMP received in INIT SERVERINFO ("
                << messageString + ")";
        QString serverTimestampString = messageString;
        bool ok = false;
        qlonglong serverTimestamp = serverTimestampString.toLongLong(&ok);

        Q_ASSERT_X(ok == true,
                   "CommunicationManager::parseInitServerInfoMessage()",
                   "Bogus SERVERTIMESTAMP received in INIT SERVERINFO");

        emit(sessionTimeSynchronized(serverTimestamp));
        _serverTimestamp = serverTimestamp;
        _serverInfoMsgCount++;
    }
}

void CommunicationManager::parseInitDataMessage(QString messageString)
{
    if (_beginEndMessageStrings.contains(messageString))
    {
        if (_inMessageStrings.value(messageString) == COM_MSG_IN_DATA_END)
        {
            qDebug() << "CommunicationManager : DATA END received in INIT DATA";
            _initDataCompleted = true;

            if (isInitCompleted())
            {
                _state = COM_STATE_READY;
            }
            else
            {
                _state = COM_STATE_INIT;
            }
        }
        else
        {
            emit protocolError(
                    "Bogus message received in INIT DATA (" + messageString
                    + ")");
        }
    }
    else
    {
        QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);
        QString typeString = messageParts.at(0);
        QString idString = messageParts.at(1);
        bool ok = false;
        int id = idString.toInt(&ok); // Data ID

        Q_ASSERT_X(ok == true, "CommunicationManager::parseInitDataMessage()",
                   "Bogus message received in INIT DATA");

        QString description = messageParts.at(2); // Data description
        DataType type;
        DataWidgetType widget; // Data widget
        double valMax = 0., valMin = 0.;
        bool active;

        Q_ASSERT_X(_typeMessageStrings.contains(typeString),
                   "CommunicationManager::parseInitDataMessage()",
                   "Bogus message received in INIT DATA");

        switch (_inMessageStrings.value(typeString))
        {
        case COM_MSG_IN_TYPE_BOOL:
            {
                Q_ASSERT_X(messageParts.count() == 4,
                           "CommunicationManager::parseInitDataMessage()",
                           "Bogus message received in INIT DATA");

                QString activeString = messageParts.at(3);

                Q_ASSERT_X(
                        (activeString.compare("0") == 0)
                        || (activeString.compare("1") == 0),
                        "CommunicationManager::parseInitDataMessage()",
                        "Bogus message received in INIT DATA");

                if (activeString.compare("1") == 0)
                {
                    active = true;
                }
                else
                {
                    active = false;
                }

                type = DATA_TYPE_BOOL;
                widget = DEFAULT_DATA_WIDGET_BOOL;

                break;
            }

        case COM_MSG_IN_TYPE_INT:
            {
                Q_ASSERT_X(messageParts.count() == 6,
                           "CommunicationManager::parseInitDataMessage()",
                           "Bogus message received in INIT DATA");

                QString activeString = messageParts.at(5);
                bool ok = false;
                int activeInt = activeString.toInt(&ok);

                Q_ASSERT_X(ok == true && (activeInt == 0 || activeInt == 1),
                           "CommunicationManager::parseInitDataMessage()",
                           "Bogus message received in INIT DATA");

                if (activeInt == 1)
                {
                    active = true;
                }
                else
                {
                    active = false;
                }

                QString valMinString = messageParts.at(3);
                valMin = valMinString.toDouble(&ok); // Data ID

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitDataMessage()",
                           "Bogus message received in INIT DATA");

                QString valMaxString = messageParts.at(4);
                valMax = valMaxString.toDouble(&ok);

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitDataMessage()",
                           "Bogus message received in INIT DATA");

                type = DATA_TYPE_INT;
                widget = DEFAULT_DATA_WIDGET_INT;

                break;
            }

        case COM_MSG_IN_TYPE_FLOAT:
            {
                Q_ASSERT_X(messageParts.count() == 6,
                           "CommunicationManager::parseInitDataMessage()",
                           "Bogus message received in INIT DATA");

                QString activeString = messageParts.at(5);
                bool ok = false;
                int activeInt = activeString.toInt(&ok);

                Q_ASSERT_X(ok == true && (activeInt == 0 || activeInt == 1),
                           "CommunicationManager::parseInitDataMessage()",
                           "Bogus message received in INIT DATA");

                if (activeInt == 1)
                {
                    active = true;
                }
                else
                {
                    active = false;
                }

                QString valMinString = messageParts.at(3);
                valMin = valMinString.toDouble(&ok);

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitDataMessage()",
                           "Bogus message received in INIT DATA");

                QString valMaxString = messageParts.at(4);
                valMax = valMaxString.toDouble(&ok);

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitDataMessage()",
                           "Bogus message received in INIT DATA");

                type = DATA_TYPE_FLOAT;
                widget = DEFAULT_DATA_WIDGET_FLOAT;

                break;
            }

        default:
            emit protocolError(
                    "Bogus message received in INIT DATA (" + messageString
                    + ")");
            return;
        }

        // Build the data description corresponding to this message
        if (active == false)
        {
            widget = DATA_WIDGET_NONE;
        }
        DataDescription *dataWidgetDescription = new DataDescription;
        dataWidgetDescription->id = id;
        dataWidgetDescription->descriptionString = description;
        dataWidgetDescription->type = type;
        dataWidgetDescription->widget = widget;
        dataWidgetDescription->valMin = valMin;
        dataWidgetDescription->valMax = valMax;
        _dataDescriptions->append(dataWidgetDescription);

        qDebug()
                << "CommunicationManager : "
                "DATA MSG successfully parsed in INIT DATA ("
                << messageString + ")";
    }
}

void CommunicationManager::parseInitParamsMessage(QString messageString)
{
    if (_beginEndMessageStrings.contains(messageString))
    {
        if (_inMessageStrings.value(messageString) == COM_MSG_IN_PARAMS_END)
        {
            qDebug()
                    << "CommunicationManager : "
                    "PARAMS END received in INIT PARAMS";
            _initParamsCompleted = true;

            if (isInitCompleted())
            {
                _state = COM_STATE_READY;
            }
            else
            {
                _state = COM_STATE_INIT;
            }
        }
        else
        {
            emit protocolError(
                    "Bogus message received in INIT PARAMS (" + messageString
                    + ")");
        }
    }
    else
    {
        QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);
        QString typeString = messageParts.at(0);
        QString idString = messageParts.at(1);
        bool ok = false;
        int id = idString.toInt(&ok); // Param ID

        Q_ASSERT_X(ok == true,
                   "CommunicationManager::parseInitParamsMessage()",
                   "Bogus message received in INIT PARAMS");

        QString description = messageParts.at(2); // Param description
        ParamType type;
        InteractiveWidgetType widget;
        bool valInitBool = false;
        int valInitInt = 0;
        double valInitDouble = 0.;
        int valInitEnum = 0;
        double valMax = 0., valMin = 0.;
        QStringList enumValues;

        Q_ASSERT_X(_typeMessageStrings.contains(typeString),
                   "CommunicationManager::parseInitParamsMessage()",
                   "Bogus message received in INIT PARAMS");

        switch (_inMessageStrings.value(typeString))
        {
        case COM_MSG_IN_TYPE_BOOL:
            {
                QString valInitString = messageParts.at(3);

                Q_ASSERT_X(
                        messageParts.count() == 4
                        && (valInitString.compare("0") == 0
                        || valInitString.compare("1") == 1),
                        "CommunicationManager::parseInitParamsMessage()",
                        "Bogus message received in INIT PARAMS");

                if (valInitString.compare("0") == 0)
                {
                    valInitBool = false;
                }
                else
                {
                    valInitBool = true;
                }

                type = PARAM_TYPE_BOOL;
                widget = DEFAULT_INTERACTIVE_WIDGET_BOOL;

                break;
            }

        case COM_MSG_IN_TYPE_INT:
            {
                Q_ASSERT_X(messageParts.count() == 6,
                           "CommunicationManager::parseInitParamsMessage()",
                           "Bogus message received in INIT PARAMS");

                QString valInitString = messageParts.at(5);
                bool ok = false;
                valInitInt = valInitString.toInt(&ok);

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitParamsMessage()",
                           "Bogus message received in INIT PARAMS");

                QString valMinString = messageParts.at(3);
                valMin = valMinString.toDouble(&ok);

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitParamsMessage()",
                           "Bogus message received in INIT PARAMS");

                QString valMaxString = messageParts.at(4);
                valMax = valMaxString.toDouble(&ok);

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitParamsMessage()",
                           "Bogus message received in INIT PARAMS");

                type = PARAM_TYPE_INT;
                widget = DEFAULT_INTERACTIVE_WIDGET_INT;

                break;
            }

        case COM_MSG_IN_TYPE_FLOAT:
            {
                Q_ASSERT_X(messageParts.count() == 6,
                           "CommunicationManager::parseInitParamsMessage()",
                           "Bogus message received in INIT PARAMS");

                QString valInitString = messageParts.at(5);
                bool ok = false;
                valInitDouble = valInitString.toDouble(&ok);

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitParamsMessage()",
                           "Bogus message received in INIT PARAMS");

                QString valMinString = messageParts.at(3);
                valMin = valMinString.toDouble(&ok);

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitParamsMessage()",
                           "Bogus message received in INIT PARAMS");

                QString valMaxString = messageParts.at(4);
                valMax = valMaxString.toDouble(&ok);

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitParamsMessage()",
                           "Bogus message received in INIT PARAMS");

                type = PARAM_TYPE_FLOAT;
                widget = DEFAULT_INTERACTIVE_WIDGET_FLOAT;

                break;
            }

        case COM_MSG_IN_TYPE_ENUM:
            {
                Q_ASSERT_X(messageParts.count() > 4,
                           "CommunicationManager::parseInitParamsMessage()",
                           "Bogus message received in INIT PARAMS");

                QString valInitString = messageParts.at(messageParts.count()-1);
                bool ok = false;
                valInitEnum = valInitString.toInt(&ok);

                Q_ASSERT_X(ok == true,
                           "CommunicationManager::parseInitParamsMessage()",
                           "Bogus message received in INIT PARAMS");

                QStringList values;
                for (int i = 3; i < messageParts.count() - 1; i++)
                {
                    values << messageParts.at(i);
                }

                valMin = 0.0;
                valMax = messageParts.count() - 3 - 1;
                enumValues = values;
                type = PARAM_TYPE_ENUM;
                widget = DEFAULT_INTERACTIVE_WIDGET_ENUM;

                break;
            }

        default:
            emit protocolError(
                    "Bogus message received in INIT PARAMS (" + messageString
                    + ")");
            return;
        }

        // Build the param description corresponding to this message
        ParamDescription *interactiveWidgetDescription = new ParamDescription;
        interactiveWidgetDescription->id = id;
        interactiveWidgetDescription->descriptionString = description;
        interactiveWidgetDescription->type = type;
        interactiveWidgetDescription->widget = widget;
        interactiveWidgetDescription->valInitBool = valInitBool;
        interactiveWidgetDescription->valInitInt = valInitInt;
        interactiveWidgetDescription->valInitDouble = valInitDouble;
        interactiveWidgetDescription->valInitEnum = valInitEnum;
        interactiveWidgetDescription->valMin = valMin;
        interactiveWidgetDescription->valMax = valMax;
        interactiveWidgetDescription->enumValues = enumValues;
        _paramDescriptions->append(interactiveWidgetDescription);

        qDebug()
                << "CommunicationManager : "
                "PARAMS MSG successfully parsed in INIT PARAMS ("
                << messageString + ")";
    }
}

void CommunicationManager::parseInitDevMessage(QString messageString)
{
    if (_beginEndMessageStrings.contains(messageString))
    {
        if (_inMessageStrings.value(messageString) == COM_MSG_IN_DEV_END)
        {
            qDebug() << "CommunicationManager : DEV END received in INIT DEV";
            _initDevCompleted = true;

            if (isInitCompleted())
            {
                _state = COM_STATE_READY;
            }
            else
            {
                _state = COM_STATE_INIT;
            }
        }
        else
        {
            emit protocolError(
                    "Bogus message received in INIT DEV (" + messageString
                    + ")");
        }
    }
    else
    {
        QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);

        Q_ASSERT_X(messageParts.count() == 3,
                   "CommunicationManager::parseInitDevMessage()",
                   "Bogus message received in INIT DEV");

        QString deviceIdString = messageParts.at(0);
        QString deviceTypeString = messageParts.at(1);
        QString deviceNameString = messageParts.at(2);

        bool ok = false;
        int deviceId = deviceIdString.toInt(&ok);

        Q_ASSERT_X(ok == true, "CommunicationManager::parseInitDevMessage()",
                   "Bogus message received in INIT DEV");

        starpu_top_device_type deviceType;

        Q_ASSERT_X(
                deviceTypeString.compare(
                        _inMessageStrings.key(COM_MSG_IN_DEV_CPU)) == 0
                || deviceTypeString.compare(
                        _inMessageStrings.key(COM_MSG_IN_DEV_CUDA))
                == 0 || deviceTypeString.compare(
                        _inMessageStrings.key(COM_MSG_IN_DEV_OPENCL)) == 0
                || deviceTypeString.compare(
                        _inMessageStrings.key(COM_MSG_IN_DEV_GORDON))
                == 0,
                "CommunicationManager::parseInitDevMessage()",
                "Bogus message received in INIT DEV");

        if (deviceTypeString.compare(_inMessageStrings.key(COM_MSG_IN_DEV_CPU))
            == 0)
            {
            deviceType = SERVERDEVICE_CPU;
        }
        else if (deviceTypeString.compare(
                _inMessageStrings.key(COM_MSG_IN_DEV_CUDA)) == 0)
        {
            deviceType = SERVERDEVICE_CUDA;
        }
        else if (deviceTypeString.compare(
                _inMessageStrings.key(COM_MSG_IN_DEV_OPENCL)) == 0)
        {
            deviceType = SERVERDEVICE_OPENCL;
        }
        else if (deviceTypeString.compare(
                _inMessageStrings.key(COM_MSG_IN_DEV_GORDON)) == 0)
        {
            deviceType = SERVERDEVICE_GORDON;
        }

        starpu_top_device device;
        device.id = deviceId;
        device.type = deviceType;
        device.name = deviceNameString;

        _serverDevices->append(device);

        qDebug()
                << "CommunicationManager : "
                "DEV MSG successfully parsed in INIT DEV ("
                << messageString + ")";
    }
}

void CommunicationManager::parseReadyMessage(QString messageString)
{
    if (_inMessageStrings.value(messageString) == COM_MSG_IN_READY)
    {
        qDebug() << "CommunicationManager : READY received in READY ("
                << messageString + ")";

        emit serverInitCompleted(_serverID, _dataDescriptions,
                                 _paramDescriptions, _serverDevices);

        _state = COM_STATE_LOOP;
    }
    else
    {
        QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);
        QString head = messageParts.at(0);

        Q_ASSERT_X(_widgetStatusChangeMessageStrings.contains(head),
                   "CommunicationManager::parseReadyMessage()",
                   "Bogus message received in READY");

        switch (_inMessageStrings.value(head))
        {
        case COM_MSG_IN_SET:
            parseParamNotificationMessage(messageString);
            break;

        default:
            ;
        }
    }
}

void CommunicationManager::parseLoopMessage(QString messageString)
{
    QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);
    QString head = messageParts.at(0);

    if (_widgetStatusChangeMessageStrings.contains(head))
    {
        switch (_inMessageStrings.value(head))
        {
        case COM_MSG_IN_SET:
            parseParamNotificationMessage(messageString);
            break;

        default:
            ;
        }
    }
    else if (_loopMessageStrings.contains(head))
    {
        switch (_inMessageStrings.value(head))
        {
        case COM_MSG_IN_PREV:
            parseTaskPrevMessage(messageString);
            break;

        case COM_MSG_IN_SHORT_PREV:
            parseTaskPrevMessage(messageString);
            break;

        case COM_MSG_IN_START:
            parseTaskStartMessage(messageString);
            break;

        case COM_MSG_IN_SHORT_START:
            parseTaskStartMessage(messageString);
            break;

        case COM_MSG_IN_END:
            parseTaskEndMessage(messageString);
            break;

        case COM_MSG_IN_SHORT_END:
            parseTaskEndMessage(messageString);
            break;

        case COM_MSG_IN_UPDATE:
            parseDataUpdateMessage(messageString);
            break;

        case COM_MSG_IN_SHORT_UPDATE:
            parseDataUpdateMessage(messageString);
            break;

        default:
            ;
        }
    }
    else if (_debugMessageStrings.contains(head))
    {
        switch (_inMessageStrings.value(head))
        {
        case COM_MSG_IN_DEBUG:
            parseDebugEnabledMessage(messageString);
            break;

        case COM_MSG_IN_DEBUG_MESSAGE:
            parseDebugMessageMessage(messageString);
            break;

        case COM_MSG_IN_DEBUG_LOCK:
            parseDebugLockMessage(messageString);
            break;

        default:
            ;
        }
    }
    else
    {
        emit protocolError(
                "Unexpected message received in LOOP (" + messageString + ")");
    }
}

void CommunicationManager::parseTaskPrevMessage(QString messageString)
{
    QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);

    if (messageParts.count() == 6)
    {
        QString taskIdString = messageParts.at(1);
        QString deviceIdString = messageParts.at(2);
        QString timestampString = messageParts.at(3);
        QString timestampStartString = messageParts.at(4);
        QString timestampEndString = messageParts.at(5);

        int taskId;
        int deviceId;
        qlonglong timestamp;
        qlonglong timestampStart;
        qlonglong timestampEnd;

        bool ok = false;
        taskId = taskIdString.toInt(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Unexpected TASK PREV message received in LOOP ("
                    + messageString + ")");
            return;
        }
        deviceId = deviceIdString.toInt(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Unexpected TASK PREV message received in LOOP ("
                    + messageString + ")");
            return;
        }
        timestamp = timestampString.toLongLong(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Unexpected TASK PREV message received in LOOP ("
                    + messageString + ")");
            return;
        }
        timestampStart = timestampStartString.toLongLong(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Unexpected TASK PREV message received in LOOP ("
                    + messageString + ")");
            return;
        }
        timestampEnd = timestampEndString.toLongLong(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Unexpected TASK PREV message received in LOOP ("
                    + messageString + ")");
            return;
        }

        emit notifyTaskPrevUpdate(taskId, deviceId, timestamp, timestampStart,
                                  timestampEnd);

        qDebug()
                << "CommunicationManager : "
                "TASK PREV message successfully parsed in LOOP ("
                << messageString + ")";
    }
    else
    {
        emit protocolError(
                "Unexpected TASK PREV message received in LOOP ("
                + messageString + ")");
    }
}

void CommunicationManager::parseTaskStartMessage(QString messageString)
{
    QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);

    if (messageParts.count() == 4)
    {
        QString taskIdString = messageParts.at(1);
        QString deviceIdString = messageParts.at(2);
        QString timestampString = messageParts.at(3);

        int taskId;
        int deviceId;
        qlonglong timestamp;

        bool ok = false;
        taskId = taskIdString.toInt(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Bogus TASK START message received in LOOP ("
                    + messageString + ")");
            return;
        }
        deviceId = deviceIdString.toInt(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Bogus TASK START message received in LOOP ("
                    + messageString + ")");
            return;
        }
        timestamp = timestampString.toLongLong(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Bogus TASK START message received in LOOP ("
                    + messageString + ")");
            return;
        }

        emit notifyTaskStartUpdate(taskId, deviceId, timestamp);

        qDebug()
                << "CommunicationManager : "
                "TASK START message succesfully parsed in LOOP ("
                << messageString + ")";
    }
    else
    {
        emit protocolError(
                "Bogus TASK START message received in LOOP (" + messageString
                + ")");
    }
}

void CommunicationManager::parseTaskEndMessage(QString messageString)
{
    QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);

    if (messageParts.count() == 3)
    {
        QString taskIdString = messageParts.at(1);
        QString timestampString = messageParts.at(2);

        int taskId;
        qlonglong timestamp;

        bool ok = false;
        taskId = taskIdString.toInt(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Bogus TASK END message received in LOOP (" + messageString
                    + ")");
            return;
        }
        timestamp = timestampString.toLongLong(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Bogus TASK END message received in LOOP (" + messageString
                    + ")");
            return;
        }

        emit notifyTaskEndUpdate(taskId, timestamp);

        qDebug()
                << "CommunicationManager : "
                "TASK END message successfully parsed in LOOP ("
                << messageString + ")";
    }
    else
    {
        emit protocolError(
                "Bogus TASK END message received in LOOP (" + messageString
                + ")");
    }
}

void CommunicationManager::parseDataUpdateMessage(QString messageString)
{
    QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);

    if (messageParts.count() == 4)
    {
        QString dataIdString = messageParts.at(1);
        QString timestampString = messageParts.at(3);
        QString dataValueString = messageParts.at(2);
        int dataId;
        qlonglong timestamp;

        bool ok = false;
        dataId = dataIdString.toInt(&ok);
        if (ok == false || (dataDescriptionFromId(dataId) == 0))
        {
            emit protocolError(
                    "Bogus UPDATE message received in LOOP (" + messageString
                    + ")");
            return;
        }
        timestamp = timestampString.toLongLong(&ok);
        if (ok == false)
        {
            emit protocolError(
                    "Bogus UPDATE message received in LOOP (" + messageString
                    + ")");
            return;
        }

        switch (dataDescriptionFromId(dataId)->type)
        {
        case DATA_TYPE_BOOL:
            {
                bool dataVal;
                if (dataValueString.compare("0") == 0)
                {
                    dataVal = false;
                }
                else if (dataValueString.compare("1") == 0)
                {
                    dataVal = true;
                }
                else
                {
                    emit protocolError(
                            "Bogus UPDATE message received in LOOP ("
                            + messageString + ")");
                    return;
                }

                emit notifyDataUpdate(dataId, dataVal, timestamp);
                break;
            }

        case DATA_TYPE_INT:
            {
                int dataVal = dataValueString.toInt(&ok);
                if (ok == false)
                {
                    emit protocolError(
                            "Bogus UPDATE message received in LOOP ("
                            + messageString + ")");
                    return;
                }

                emit notifyDataUpdate(dataId, dataVal, timestamp);
                break;
            }

        case DATA_TYPE_FLOAT:
            {
                double dataVal = dataValueString.toDouble(&ok);
                if (ok == false)
                {
                    emit protocolError(
                            "Bogus UPDATE message received in LOOP ("
                            + messageString + ")");
                    return;
                }

                emit notifyDataUpdate(dataId, dataVal, timestamp);
                break;
            }

        default:
            emit protocolError(
                    "Bogus UPDATE message received in LOOP (" + messageString
                    + ")");
        }

        qDebug()
                << "CommunicationManager : "
                "UPDATE message successfully parsed in LOOP ("
                << messageString + ")";
    }
    else
    {
        emit
                protocolError(
                        "Bogus UPDATE message received in LOOP ("
                        + messageString + ")");
    }
}

void CommunicationManager::parseParamNotificationMessage(QString messageString)
{
    QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);
    if (messageParts.count() != 4)
    {
        emit
                protocolError(
                        "Bogus SET message received (" + messageString + ")");
        return;
    }

    QString paramIdString = messageParts.at(1);
    QString paramValueString = messageParts.at(2);
    QString timestampString = messageParts.at(3);
    int paramId;
    qlonglong timestamp;

    bool ok = false;
    paramId = paramIdString.toInt(&ok);
    if (ok == false || (paramDescriptionFromId(paramId) == 0))
    {
        emit
                protocolError(
                        "Bogus SET message received (" + messageString + ")");
        return;
    }
    timestamp = timestampString.toLongLong(&ok);
    if (ok == false)
    {
        emit
                protocolError(
                        "Bogus SET message received (" + messageString + ")");
        return;
    }

    switch (paramDescriptionFromId(paramId)->type)
    {
    case PARAM_TYPE_BOOL:
	{
            bool paramVal;
            if (paramValueString.compare("0") == 0)
            {
                paramVal = false;
            }
            else if (paramValueString.compare("1") == 0)
            {
                paramVal = true;
            }
            else
            {
                emit protocolError(
                        "Bogus SET message received (" + messageString + ")");
                return;
            }

            emit notifyParamUpdate(paramId, paramVal, timestamp);
            break;
	}

    case PARAM_TYPE_INT:
	{
            int paramVal = paramValueString.toInt(&ok);
            if (ok == false)
            {
                emit protocolError(
                        "Bogus SET message received (" + messageString + ")");
                return;
            }

            emit notifyParamUpdate(paramId, paramVal, timestamp);
            break;
	}

    case PARAM_TYPE_FLOAT:
	{
            double paramVal = paramValueString.toDouble(&ok);
            if (ok == false)
            {
                emit protocolError(
                        "Bogus SET message received (" + messageString + ")");
                return;
            }

            emit notifyParamUpdate(paramId, paramVal, timestamp);
            break;
	}

    case PARAM_TYPE_ENUM:
	{
            int paramVal = paramValueString.toInt(&ok);
            if (ok == false)
            {
                emit protocolError(
                        "Bogus SET message received (" + messageString + ")");
                return;
            }

            emit notifyParamUpdate(paramId, paramVal, timestamp);
            break;
	}

    default:
        emit
                protocolError(
                        "Bogus SET message received (" + messageString + ")");
    }

    qDebug() << "CommunicationManager : SET successfully parsed in LOOP ("
            << messageString + ")";
}

void CommunicationManager::parseDebugEnabledMessage(QString messageString)
{
    QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);
    if (messageParts.count() != 2)
    {
        emit protocolError(
                "Bogus DEBUG message received (" + messageString + ")");
        return;
    }

    QString debugEnabledString = messageParts.at(1);

    if (debugEnabledString.compare(_inMessageStrings.key(COM_MSG_IN_DEBUG_ON))
        == 0)
	{
        qDebug() << "CommunicationManager : DEBUG ON received ("
                << messageString + ")";

        emit notifyDebugEnabled(true);
    }
    else if (debugEnabledString.compare(
            _inMessageStrings.key(COM_MSG_IN_DEBUG_OFF)) == 0)
    {
        qDebug() << "CommunicationManager : DEBUG OFF received ("
                << messageString + ")";

        emit notifyDebugEnabled(false);
    }
    else
    {
        emit protocolError(
                "Bogus DEBUG message received (" + messageString + ")");
    }
}

void CommunicationManager::parseDebugMessageMessage(QString messageString)
{
    QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);
    if (messageParts.count() != 2)
    {
        emit protocolError(
                "Bogus DEBUG message received (" + messageString + ")");
        return;
    }

    QString debugMessageString = messageParts.at(1);

    emit notifyDebugMessage(debugMessageString);

    qDebug() << "CommunicationManager : DEBUG MESSAGE received ("
            << messageString + ")";
}

void CommunicationManager::parseDebugLockMessage(QString messageString)
{
    QStringList messageParts = messageString.split(COM_MSG_SEPARATOR);
    if (messageParts.count() != 2)
    {
        emit protocolError(
                "Bogus DEBUG message received (" + messageString + ")");
        return;
    }

    QString lockMessageString = messageParts.at(1);

    emit notifyDebugLock(lockMessageString);

    qDebug() << "CommunicationManager : DEBUG LOCK received (" << messageString
            + ")";
}

/* -------------------------------------------------------------------------- */
/* Send messages                                                              */
/* -------------------------------------------------------------------------- */

void CommunicationManager::sendMessage(QString messageString)
{
    messageString.append(COM_MSG_ENDL);

    write(messageString.toAscii());
}

void CommunicationManager::sendGoMessage()
{
    sendMessage( buildGoMessage());
}

void CommunicationManager::sendDataEnableMessage(int dataId)
{
    sendMessage(buildDataEnableMessage(dataId));
}

void CommunicationManager::sendDataDisableMessage(int dataId)
{
    sendMessage(buildDataDisableMessage(dataId));
}

void CommunicationManager::sendParamSetMessage(int paramId, bool paramValue)
{
    sendMessage(buildParamSetMessage(paramId, paramValue));
}

void CommunicationManager::sendParamSetMessage(int paramId, int paramValue)
{
    sendMessage(buildParamSetMessage(paramId, paramValue));
}

void CommunicationManager::sendParamSetMessage(int paramId, double paramValue)
{
    sendMessage(buildParamSetMessage(paramId, paramValue));
}

void CommunicationManager::sendDebugEnabledMessage(bool enabled)
{
    sendMessage(buildDebugEnabledMessage(enabled));
}

void CommunicationManager::sendStepMessage()
{
    sendMessage( buildStepMessage());
}

/* -------------------------------------------------------------------------- */
/* Getters                                                                    */
/* -------------------------------------------------------------------------- */

CommunicationState CommunicationManager::state() const
{
    return _state;
}

bool CommunicationManager::isInitCompleted() const
{
    return (_initServerInfoCompleted && _initDataCompleted
            && _initParamsCompleted && _initDevCompleted);
}

/* -------------------------------------------------------------------------- */
/* Build messages                                                             */
/* -------------------------------------------------------------------------- */

QString CommunicationManager::buildGoMessage()
{
    QString messageString = _outMessageStrings.value(COM_MSG_OUT_GO);

    return messageString;
}

QString CommunicationManager::buildDataEnableMessage(int dataId)
{
    QString head = _outMessageStrings.value(COM_MSG_OUT_ENABLE);
    QString dataIdString = QString::number(dataId);

    QString messageString = head.append(COM_MSG_SEPARATOR).append(dataIdString);

    return messageString;
}

QString CommunicationManager::buildDataDisableMessage(int dataId)
{
    QString head = _outMessageStrings.value(COM_MSG_OUT_DISABLE);
    QString dataIdString = QString::number(dataId);

    QString messageString = head.append(COM_MSG_SEPARATOR).append(dataIdString);

    return messageString;
}

QString CommunicationManager::buildParamSetMessage(int paramId, bool paramValue)
{
    QString head = _outMessageStrings.value(COM_MSG_OUT_SET);
    QString paramIdString = QString::number(paramId);
    QString paramValueString = QString::number(paramValue ? 1 : 0);

    QString messageString =
            head.append(COM_MSG_SEPARATOR).append(paramIdString).append(
                    COM_MSG_SEPARATOR) .append(paramValueString);

    return messageString;
}

QString CommunicationManager::buildParamSetMessage(int paramId, int paramValue)
{
    QString head = _outMessageStrings.value(COM_MSG_OUT_SET);
    QString paramIdString = QString::number(paramId);
    QString paramValueString = QString::number(paramValue);

    QString messageString =
            head.append(COM_MSG_SEPARATOR).append(paramIdString).append(
                    COM_MSG_SEPARATOR) .append(paramValueString);

    return messageString;
}

QString CommunicationManager::buildParamSetMessage(int paramId,
                                                   double paramValue)
{
    QString head = _outMessageStrings.value(COM_MSG_OUT_SET);
    QString paramIdString = QString::number(paramId);
    QString paramValueString = QString::number(paramValue);

    QString messageString =
            head.append(COM_MSG_SEPARATOR).append(paramIdString).append(
                    COM_MSG_SEPARATOR) .append(paramValueString);

    return messageString;
}

QString CommunicationManager::buildDebugEnabledMessage(bool enabled)
{
    QString head = _outMessageStrings.value(COM_MSG_OUT_DEBUG);
    QString debugEnabledString = (enabled ? _outMessageStrings.value(
            COM_MSG_OUT_DEBUG_ON) : _outMessageStrings.value(
                    COM_MSG_OUT_DEBUG_OFF));

    QString messageString = head.append(COM_MSG_SEPARATOR).append(
            debugEnabledString);

    return messageString;
}

QString CommunicationManager::buildStepMessage()
{
    QString messageString = _outMessageStrings.value(COM_MSG_OUT_DEBUG_STEP);

    return messageString;
}

DataDescription *CommunicationManager::dataDescriptionFromId(int dataId) const
{
    for (int i = 0; i < _dataDescriptions->count(); i++)
    {
        if (_dataDescriptions->at(i)->id == dataId)
            return _dataDescriptions->at(i);
    }

    return 0;
}

ParamDescription 
        *CommunicationManager::paramDescriptionFromId(int paramId) const
{
    for (int i = 0; i < _paramDescriptions->count(); i++)
    {
        if (_paramDescriptions->at(i)->id == paramId)
            return _paramDescriptions->at(i);
    }

    return 0;
}

void CommunicationManager::clearDescriptions()
{
    _dataDescriptions->clear();
    _paramDescriptions->clear();
}

/* -------------------------------------------------------------------------- */
/* Init message strings                                                       */
/* -------------------------------------------------------------------------- */

void CommunicationManager::initInMessageStrings()
{
    // Types
    _inMessageStrings.insert("BOOL", COM_MSG_IN_TYPE_BOOL);
    _inMessageStrings.insert("INT", COM_MSG_IN_TYPE_INT);
    _inMessageStrings.insert("FLOAT", COM_MSG_IN_TYPE_FLOAT);
    _inMessageStrings.insert("ENUM", COM_MSG_IN_TYPE_ENUM);
    _typeMessageStrings.insert("BOOL");
    _typeMessageStrings.insert("INT");
    _typeMessageStrings.insert("FLOAT");
    _typeMessageStrings.insert("ENUM");
    // Init server ID
    _inMessageStrings.insert("SERVERINFO", COM_MSG_IN_SERVERINFO_BEGIN);
    _inMessageStrings.insert("/SERVERINFO", COM_MSG_IN_SERVERINFO_END);
    _beginEndMessageStrings.insert("SERVERINFO");
    _beginEndMessageStrings.insert("/SERVERINFO");
    // Init data
    _inMessageStrings.insert("DATA", COM_MSG_IN_DATA_BEGIN);
    _inMessageStrings.insert("/DATA", COM_MSG_IN_DATA_END);
    _beginEndMessageStrings.insert("DATA");
    _beginEndMessageStrings.insert("/DATA");
    // Init parameters
    _inMessageStrings.insert("PARAMS", COM_MSG_IN_PARAMS_BEGIN);
    _inMessageStrings.insert("/PARAMS", COM_MSG_IN_PARAMS_END);
    _beginEndMessageStrings.insert("PARAMS");
    _beginEndMessageStrings.insert("/PARAMS");
    // Init devices
    _inMessageStrings.insert("DEV", COM_MSG_IN_DEV_BEGIN);
    _inMessageStrings.insert("/DEV", COM_MSG_IN_DEV_END);
    _inMessageStrings.insert("CPU", COM_MSG_IN_DEV_CPU);
    _inMessageStrings.insert("GPU", COM_MSG_IN_DEV_CUDA);
    _inMessageStrings.insert("OPENCL", COM_MSG_IN_DEV_OPENCL);
    _inMessageStrings.insert("GORDON", COM_MSG_IN_DEV_GORDON);
    _beginEndMessageStrings.insert("DEV");
    _beginEndMessageStrings.insert("/DEV");
    // Server ready
    _inMessageStrings.insert("READY", COM_MSG_IN_READY);
    // Widget status
    _inMessageStrings.insert("SET", COM_MSG_IN_SET);
    _widgetStatusChangeMessageStrings.insert("SET");
    // Loop messages
    // Complete versions
    _inMessageStrings.insert("PREV", COM_MSG_IN_PREV);
    _inMessageStrings.insert("START", COM_MSG_IN_START);
    _inMessageStrings.insert("END", COM_MSG_IN_END);
    _inMessageStrings.insert("UPDATE", COM_MSG_IN_UPDATE);
    // Short versions
    _inMessageStrings.insert("P", COM_MSG_IN_SHORT_PREV);
    _inMessageStrings.insert("S", COM_MSG_IN_SHORT_START);
    _inMessageStrings.insert("E", COM_MSG_IN_SHORT_END);
    _inMessageStrings.insert("U", COM_MSG_IN_SHORT_UPDATE);
    // Complete versions
    _loopMessageStrings.insert("PREV");
    _loopMessageStrings.insert("START");
    _loopMessageStrings.insert("END");
    _loopMessageStrings.insert("UPDATE");
    // Short versions
    _loopMessageStrings.insert("P");
    _loopMessageStrings.insert("S");
    _loopMessageStrings.insert("E");
    _loopMessageStrings.insert("U");
    // Debug messages
    _inMessageStrings.insert("DEBUG", COM_MSG_IN_DEBUG);
    _inMessageStrings.insert("ON", COM_MSG_IN_DEBUG_ON);
    _inMessageStrings.insert("OFF", COM_MSG_IN_DEBUG_OFF);
    _inMessageStrings.insert("MESSAGE", COM_MSG_IN_DEBUG_MESSAGE);
    _inMessageStrings.insert("LOCK", COM_MSG_IN_DEBUG_LOCK);
    _debugMessageStrings.insert("DEBUG");
    _debugMessageStrings.insert("MESSAGE");
    _debugMessageStrings.insert("LOCK");
}

void CommunicationManager::initOutMessageStrings()
{
    // Launch session on server
    _outMessageStrings.insert(COM_MSG_OUT_GO, "GO");
    // Widget status change
    _outMessageStrings.insert(COM_MSG_OUT_ENABLE, "ENABLE");
    _outMessageStrings.insert(COM_MSG_OUT_DISABLE, "DISABLE");
    _outMessageStrings.insert(COM_MSG_OUT_SET, "SET");
    // Debug mode
    _outMessageStrings.insert(COM_MSG_OUT_DEBUG, "DEBUG");
    _outMessageStrings.insert(COM_MSG_OUT_DEBUG_ON, "ON");
    _outMessageStrings.insert(COM_MSG_OUT_DEBUG_OFF, "OFF");
    _outMessageStrings.insert(COM_MSG_OUT_DEBUG_STEP, "STEP");
}
