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


#ifndef COMMUNICATIONMANAGER_H
#define COMMUNICATIONMANAGER_H

#include <QTcpSocket>
#include "starpu_top_types.h"

class CommunicationManager : public QTcpSocket
{ /* Receives protocol messages from server, parses them
     and notifies the GUI appropriately using signals.
     Sends protocol messages to server when the GUI notifies. */
    Q_OBJECT

public:
    CommunicationManager(QObject *parent = 0);
    ~CommunicationManager();

private:
    // Protocol message strings
    QHash<QString, CommunicationInMessageType> _inMessageStrings;
    QHash<CommunicationOutMessageType, QString> _outMessageStrings;
    QSet<QString> _typeMessageStrings;
    QSet<QString> _beginEndMessageStrings;
    QSet<QString> _widgetStatusChangeMessageStrings;
    QSet<QString> _debugMessageStrings;
    QSet<QString> _loopMessageStrings;
    // Metadata
    int _serverInfoMsgCount;
    QString _serverID;
    qlonglong _serverTimestamp;
    QList<DataDescription*> *_dataDescriptions;
    QList<ParamDescription*> *_paramDescriptions;
    QList<starpu_top_device> *_serverDevices;
    // Communication states
    CommunicationState _state;
    bool _initServerInfoCompleted;
    bool _initDataCompleted;
    bool _initParamsCompleted;
    bool _initDevCompleted;
    // Getters
    // Get communication states
    CommunicationState state() const;
    bool isInitCompleted() const;
    // Get session descriptions
    DataDescription *dataDescriptionFromId(int dataId) const;
    ParamDescription *paramDescriptionFromId(int paramId) const;

    // Init protocol message strings
    void initInMessageStrings();
    void initOutMessageStrings();

private slots:
    // Initialize a new session
    void initializeSession();
    // Receive protocol messages
    void messageReceived();
    // Parse different protocol messages
    void parseInitMessage(QString messageString);
    void parseInitServerInfoMessage(QString messageString);
    void parseInitDataMessage(QString messageString);
    void parseInitParamsMessage(QString messageString);
    void parseInitDevMessage(QString messageString);
    void parseReadyMessage(QString messageString);
    void parseLoopMessage(QString messageString);
    void parseTaskPrevMessage(QString messageString);
    void parseTaskStartMessage(QString messageString);
    void parseTaskEndMessage(QString messageString);
    void parseDataUpdateMessage(QString messageString);
    void parseParamNotificationMessage(QString messageString);
    void parseDebugEnabledMessage(QString messageString);
    void parseDebugMessageMessage(QString messageString);
    void parseDebugLockMessage(QString messageString);

public slots:
    // Build different protocol messages
    QString buildGoMessage();
    QString buildDataEnableMessage(int dataId);
    QString buildDataDisableMessage(int dataId);
    QString buildParamSetMessage(int paramId, bool paramValue);
    QString buildParamSetMessage(int paramId, int paramValue);
    QString buildParamSetMessage(int paramId, double paramValue);
    QString buildDebugEnabledMessage(bool enabled);
    QString buildStepMessage();
    // Send different protocol messages
    void sendMessage(QString messageString);
    void sendGoMessage();
    void sendDataEnableMessage(int dataId);
    void sendDataDisableMessage(int dataId);
    void sendParamSetMessage(int paramId, bool paramValue);
    void sendParamSetMessage(int paramId, int paramValue);
    void sendParamSetMessage(int paramId, double paramValue);
    void sendDebugEnabledMessage(bool enabled);
    void sendStepMessage();
    // Clear session descriptions
    void clearDescriptions();

signals:
    // Reference time received from server
    void sessionTimeSynchronized(qlonglong serverTimestamp);
    // Send init session data to GUI
    void serverInitCompleted(QString serverID,
                             QList<DataDescription*> *dataDescriptions,
                             QList<ParamDescription*> *paramDescriptions,
                             QList<starpu_top_device> *serverDevices);
    // Notify GUI with a protocol message
    // Protocol error
    void protocolError(QString errorMessage);
    // Debug protocol messages
    void notifyDebugEnabled(bool enabled);
    void notifyDebugMessage(QString debugMessage);
    void notifyDebugLock(QString lockMessage);
    // Data update protocol messages
    void notifyDataUpdate(int dataId, bool dataValue, qlonglong timestamp);
    void notifyDataUpdate(int dataId, int dataValue, qlonglong timestamp);
    void notifyDataUpdate(int dataId, double dataValue, qlonglong timestamp);
    // Parameter update protocol messages
    void notifyParamUpdate(int paramId, bool paramValue, qlonglong timestamp);
    void notifyParamUpdate(int paramId, int paramValue, qlonglong timestamp);
    void notifyParamUpdate(int paramId, double paramValue, qlonglong timestamp);
    // Task update protocol messages
    void notifyTaskPrevUpdate(int taskId, int deviceId,  qlonglong timestamp,
                              qlonglong timestampStart, qlonglong timestampEnd);
    void notifyTaskStartUpdate(int taskId, int deviceId,  qlonglong timestamp);
    void notifyTaskEndUpdate(int taskId, qlonglong timestamp);
    void protoConnected();
};

#endif // COMMUNICATIONMANAGER_H
