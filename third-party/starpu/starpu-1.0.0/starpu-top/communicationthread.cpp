/*
= StarPU-Top for StarPU =

Copyright (C) 2011 
William Braik
Yann Courtois
Jean-Marie Couteyen
Anthony Roy

Copyright (C) 2011 Université de Bordeaux 1

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


#include <QMessageBox>
#include "communicationthread.h"
#include "configurationmanager.h"
#include "mainwindow.h"
#include "communicationmanager.h"
#include "starpu_top_types.h"

const int MAX_CONNECTION_ATTEMPTS = 10;

CommunicationThread::CommunicationThread(
        MainWindow *mainWindow,
        ConfigurationManager *configurationManager)
{
    static bool instanciated = false;

    Q_ASSERT_X(instanciated == false, "CommunicationThread's' constructor",
               "Singleton pattern violated - "
               "CommunicationThread instanciated more than once");

    _mainWindow = mainWindow;
    _configurationManager = configurationManager;
    _communicationManager = 0;
    _SSHProcess = NULL;

    instanciated = true;

    QObject::connect(this, SIGNAL(progress(QString)),
                   _mainWindow, SLOT(updateStatusBarMessage(QString)));
    QObject::connect(this, SIGNAL(disconnected()),
                    _mainWindow, SLOT(disconnected()));
    QObject::connect(this, SIGNAL(abort(QString)),
                   _mainWindow, SLOT(connectionAborted(QString)));

    qDebug() << "CommunicationThread : initializing";
}

CommunicationThread::~CommunicationThread()
{
    qDebug() << "CommunicationThread : terminating";

    delete _communicationManager;
}

void CommunicationThread::createNewCommunicationManager(void)
{
    emit progress(tr("Connecting..."));

    if (_communicationManager == 0)
        delete(_communicationManager );

    _communicationManager = new CommunicationManager(this);

    // Type registering is necessary for Qt "Queued Connections"
    // as we are going to send signals to a different thread
    qRegisterMetaType<QAbstractSocket::SocketError> (
            "QAbstractSocket::SocketError");

    // Connection events
    QObject::connect(_communicationManager, SIGNAL(protoConnected()),
                   _mainWindow, SLOT(connectionSucceeded()));
    QObject::connect(_communicationManager, SIGNAL(protoConnected()),
                   this, SLOT(connectionSucceeded()));
    QObject::connect(_communicationManager, SIGNAL(error(QAbstractSocket::SocketError)),
                   this, SLOT(connectionError(QAbstractSocket::SocketError)));
    QObject::connect(_communicationManager, SIGNAL(disconnected()),
                    this, SLOT(connectionDisconnected()));

    // Change communication state
    QObject::connect(_communicationManager, SIGNAL(protocolError(QString)),
                     _mainWindow, SLOT(protocolErrorCaught(QString)));
    QObject::connect(_communicationManager,
                     SIGNAL(sessionTimeSynchronized(qlonglong)),
                     _mainWindow,
                     SLOT(synchronizeSessionTime(qlonglong)));
    QObject::connect(_communicationManager,
                     SIGNAL(serverInitCompleted(QString,
                                                QList<DataDescription*>*,
                                                QList<ParamDescription*>*,
                                                QList<Starpu_TopDevice>*)),
                     _mainWindow, SLOT(initClient(
                             QString,
                             QList<DataDescription*>*,
                             QList<ParamDescription*>*,
                             QList<Starpu_TopDevice>*)));
    // Output data
    QObject::connect(_mainWindow, SIGNAL(clientLaunched()),
                     _communicationManager, SLOT(sendGoMessage()));
    QObject::connect(_mainWindow, SIGNAL(dataEnabled(int)),
                     _communicationManager, SLOT(sendDataEnableMessage(int)));
    QObject::connect(_mainWindow, SIGNAL(dataDisabled(int)),
                     _communicationManager, SLOT(sendDataDisableMessage(int)));
    QObject::connect(_mainWindow, SIGNAL(paramValueUpdated(int,bool)),
                     _communicationManager, SLOT(sendParamSetMessage(int,bool)));
    QObject::connect(_mainWindow, SIGNAL(paramValueUpdated(int,int)),
                     _communicationManager, SLOT(sendParamSetMessage(int,int)));
    QObject::connect(_mainWindow, SIGNAL(paramValueUpdated(int,double)),
                     _communicationManager, SLOT(sendParamSetMessage(int,double)));
    QObject::connect(_mainWindow, SIGNAL(debugEnabled(bool)),
                     _communicationManager, SLOT(sendDebugEnabledMessage(bool)));
    QObject::connect(_mainWindow, SIGNAL(debugStepped()),
                     _communicationManager, SLOT(sendStepMessage()));
    // Input data
    QObject::connect(_communicationManager,
                     SIGNAL(notifyDebugEnabled(bool)),
                     _mainWindow, SLOT(setDebug(bool)));
    QObject::connect(_communicationManager,
                     SIGNAL(notifyDebugMessage(QString)), _mainWindow,
                     SLOT(setDebugMessage(QString)));
    QObject::connect(_communicationManager,
                     SIGNAL(notifyDebugLock(QString)), _mainWindow,
                     SLOT(setDebugLock(QString)));
    QObject	::connect(_communicationManager,
                      SIGNAL(notifyParamUpdate(int,bool,qlonglong)),
                      _mainWindow, SLOT(updateInteractiveWidget(int,bool,qlonglong)));
    QObject::connect(_communicationManager, 
                     SIGNAL(notifyParamUpdate(int,int,qlonglong)),
                     _mainWindow, SLOT(updateInteractiveWidget(int,int,qlonglong)));
    QObject::connect(_communicationManager, 
                     SIGNAL(notifyParamUpdate(int,double,qlonglong)),
                     _mainWindow, SLOT(updateInteractiveWidget(int,double,qlonglong)));
    QObject::connect(_communicationManager, 
                     SIGNAL(notifyTaskPrevUpdate(int,int,qlonglong,qlonglong,qlonglong)),
                     _mainWindow,
                     SLOT(updateTaskPrev(int,int,qlonglong,qlonglong,qlonglong)));
    QObject::connect(_communicationManager, 
                     SIGNAL(notifyTaskStartUpdate(int,int,qlonglong)),
                     _mainWindow, SLOT(updateTaskStart(int,int,qlonglong)));
    QObject::connect(_communicationManager, 
                     SIGNAL(notifyTaskEndUpdate(int,qlonglong)),
                     _mainWindow, SLOT(updateTaskEnd(int,qlonglong)));
    QObject::connect(_communicationManager, 
                     SIGNAL(notifyDataUpdate(int,bool,qlonglong)),
                     _mainWindow, SLOT(updateDataWidget(int,bool,qlonglong)));
    QObject::connect(_communicationManager, 
                     SIGNAL(notifyDataUpdate(int,int,qlonglong)),
                     _mainWindow, SLOT(updateDataWidget(int,int,qlonglong)));
    QObject::connect(_communicationManager, 
                     SIGNAL(notifyDataUpdate(int,double,qlonglong)),
                     _mainWindow, SLOT(updateDataWidget(int,double,qlonglong)));

    _communicationManager->connectToHost(_configurationManager->serverHost(),
                                         _configurationManager->serverPort());
}

void CommunicationThread::run()
{
    if(_configurationManager->ssh())
        {
                _connectionAttemptsCount = 0;
                QString commandLine;

                #ifdef WIN32
                commandLine.append("cmd /c start ");
                #else
                commandLine.append("xterm -e ");
                #endif

                commandLine.append(_configurationManager->commandLine());

                qDebug() << "CommunicationThread : executing SSH command line : "
                        << commandLine;

                _SSHProcess = new QProcess();
                _SSHProcess->start(commandLine);
        }


    createNewCommunicationManager();

    exec();
}

void CommunicationThread::connectionError(QAbstractSocket::SocketError error)
{
    qDebug() << "CommunicationThread : connection error occured" << error;

    switch (error)
    {
    case QAbstractSocket::ConnectionRefusedError:
        if (_configurationManager->ssh()) {
            if (_connectionAttemptsCount++ <= MAX_CONNECTION_ATTEMPTS) {
                qDebug() << "refused, retrying";
                emit progress(tr("Connecting refused, retrying (") + QString::number(_connectionAttemptsCount)+(")..."));
                this->sleep(1);
                _communicationManager->connectToHost(_configurationManager->serverHost(),
                                                     _configurationManager->serverPort());
                break;
            }
        }

	qDebug() << "Too many retries, giving up";

        emit abort(tr("Connection refused by server (port : ") + QString::number(
                   _configurationManager->serverPort()) + tr(") !"));

        delete _SSHProcess;
        exit(1);
        break;

    case QAbstractSocket::RemoteHostClosedError:
        if (_connectionAttemptsCount == INT_MAX) {
            _communicationManager->abort();
	} else {
            if (_configurationManager->ssh()) {
                if (_connectionAttemptsCount++ <= MAX_CONNECTION_ATTEMPTS) {
                    qDebug() << "closed, retrying";
                    emit progress(tr("Connecting closed, retrying (") + QString::number(_connectionAttemptsCount)+(")..."));
                    _communicationManager->abort();
                    break;
                }
            }

            qDebug() << "Too many retries, giving up";
            emit abort(tr("Server closed the connection !"));
        }

        delete _SSHProcess;
        exit(1);
        break;


    case QAbstractSocket::HostNotFoundError:
        emit abort(tr("Server not found (host name : ")
                + _configurationManager->serverHost() + tr(") !"));
        delete _SSHProcess;
        exit(1);
        break;

    default:
        emit abort(tr("Connection to server is broken !"));
        delete _SSHProcess;
        exit(1);
    }
}


void CommunicationThread::connectionSucceeded()
{
    /* Connected at application level, do not try to reconnect on closure */
    _connectionAttemptsCount = INT_MAX;
}

void CommunicationThread::connectionDisconnected()
{
    if (_connectionAttemptsCount == INT_MAX) {
        /* Only raise to mainWindow if we had connected */
        emit disconnected();
    } else {
        /* Just closing for a retry, do retry */
        qDebug() << "and retrying";
        this->sleep(1);
        createNewCommunicationManager();
    }
}
