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


#ifndef COMMUNICATIONTHREAD_H
#define COMMUNICATIONTHREAD_H

class MainWindow;
class ConfigurationManager;
class CommunicationManager;

#include <QThread>
#include <QProcess>
#include <QTcpSocket>

class CommunicationThread : public QThread
{
    Q_OBJECT
public:
    CommunicationThread(
            MainWindow *mainWindow,
            ConfigurationManager *configurationManager);
    ~CommunicationThread();

    // Thread execution loop
    void run();

private:
    // Linked components
    MainWindow *_mainWindow;
    ConfigurationManager *_configurationManager;
    CommunicationManager *_communicationManager;

    QProcess * _SSHProcess;
    int _connectionAttemptsCount;

    void createNewCommunicationManager(void);

private slots:
    void connectionError(QAbstractSocket::SocketError error);
    void connectionSucceeded();
    void connectionDisconnected();

signals:
    void SocketError(QAbstractSocket::SocketError);
    void progress(QString message);
    void abort(QString message);
    void disconnected();
};

#endif // COMMUNICATIONTHREAD_H
