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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class GanttWidget;
class PreferencesDialog;
class DebugConsole;
class AbstractWidgetWindow;
class DataWidget;
class DataAggregatorWidget;
class InteractiveWidget;
class CommunicationThread;
class ConfigurationManager;
class WidgetWindowsManager;
class SessionSetupManager;
class TaskManager;

#include <QMainWindow>
#include <QMdiArea>
#include <QSplitter>
#include <QLabel>
#include <QComboBox>
#include <QPointer>
#include <QScrollArea>
#include <QAbstractSocket>
#include <QTime>
#include <QSpinBox>
#include "starpu_top_types.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // Getters
    // Get GUI components
    const QSplitter *splitter() const;
    const QDockWidget *parametersDock() const;
    // Get managers
    const WidgetWindowsManager* widgetWindowsManager() const;
    const ConfigurationManager* configurationManager() const;
    const SessionSetupManager* sessionSetupManager() const;
    const TaskManager* taskManager() const;
    // Get different descriptions for the current session
    QString serverID() const;
    const QList<DataDescription*> *dataDescriptions() const;
    const QList<ParamDescription*> *paramDescriptions() const;
    DataDescription *dataDescriptionFromId(int dataId);
    ParamDescription *paramDescriptionFromId(int interactiveId);
    const QList<starpu_top_device> *serverDevices() const;
    // Get different widgets metadata
    const QHash<DataWidgetType, QString> *dataWidgetNames() const;
    const QHash<DataType, QSet<DataWidgetType> >
            *dataWidgetPossibilities() const;
    const QHash<InteractiveWidgetType, QString> *interactiveWidgetNames() const;
    const QHash<ParamType, QSet<InteractiveWidgetType> >
            *interactiveWidgetPossibilities() const;
    // Get the actual widgets
    const QList<QPointer<DataWidget> > *dataWidgets() const;
    const QList<QPointer<DataAggregatorWidget> > *dataAggregatorWidgets() const;
    const QList<QPointer<InteractiveWidget> > *interactiveWidgets() const;
    DataWidget *dataWidgetFromDataId(int id);
    InteractiveWidget *interactiveWidgetFromParamId(int id);
    // Get widgets counts
    int nbDataWidgets() const;
    int nbDataAggregatorWidgets() const;
    int nbInteractiveWidgets() const;
    // Get GUI states
    bool isConnected() const;
    bool isReady() const;
    bool isRunning() const;
    bool isDebugEnabled() const;
    DisplayMode displayMode() const;
    // Get effective session time
    int effectiveRunningTime() const;
    // Add new widgets to the session
    DataWidget *addDataWidget(DataDescription *dataDescription);
    DataAggregatorWidget *addDataAggregatorWidget(
            QList<int> dataIds = QList<int> ());
    InteractiveWidget *addInteractiveWidget(ParamDescription *paramDescription);

protected:
    // Events
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

private:
    // GUI components
    Ui::MainWindow *ui;
    QComboBox *_widgetsDisplayModesList;
    QSplitter *_splitter;
    QMdiArea *_mdiArea;
    GanttWidget *_ganttWidget;
    QSpinBox *_ganttTimeView;
    QScrollArea *_parametersDockScrollArea;
    DebugConsole *_debugConsole;
    QLabel *_statusBarIcon;
    QLabel *_statusBarLabel;
    PreferencesDialog *_preferencesDialog;
    QList<QPointer<DataWidget> > *_dataWidgets;
    QList<QPointer<DataAggregatorWidget> > *_dataAggregatorWidgets;
    QList<QPointer<InteractiveWidget> > *_interactiveWidgets;
    // Managers
    ConfigurationManager *_configurationManager;
    CommunicationThread *_communicationThread; // Will instantiate the communication manager
    WidgetWindowsManager *_widgetWindowsManager;
    SessionSetupManager *_sessionSetupManager;
    TaskManager *_taskManager;
    // GUI states
    DisplayMode _displayMode;
    bool _connected;
    bool _ready;
    bool _running;
    bool _debug;
    // GUI actions
    QAction *_actionConnect;
    QAction *_actionLaunch;
    QAction *_actionDebug;
    QAction *_actionSaveSessionSetup;
    QAction *_actionAddDataWidget;
    QAction *_actionAddDataAggregatorWidget;
    // Widgets metadata
    QHash<DataWidgetType, QString> _dataWidgetNames;
    QSet<DataWidgetType> _dataBoolWidgets, _dataIntWidgets, _dataFloatWidgets;
    QHash<DataType, QSet<DataWidgetType> > _dataWidgetPossibilities;
    QHash<InteractiveWidgetType, QString> _interactiveWidgetNames;
    QSet<InteractiveWidgetType> _paramBoolWidgets, _paramIntWidgets,
    _paramFloatWidgets, _paramEnumWidgets;
    QHash<ParamType, QSet<InteractiveWidgetType> >
            _interactiveWidgetPossibilities;
    // Session metadata
    QString _serverID;
    qlonglong _referenceTimestamp;
    // Different descriptions
    QList<DataDescription*> *_dataDescriptions;
    QList<ParamDescription*> *_paramDescriptions;
    QList<starpu_top_device> *_serverDevices;
    int _nbDataWidgets;
    int _nbDataAggregatorWidgets;
    int _nbInteractiveWidgets;
    // Time references
    QTime _preGoTimer;
    int _preGoTime;
    QTime _runningTime;
    QTime _currentLockTime;
    int _lockedTime;
    // Get the current status bar message
    QString statusBarMessage() const;
    // Get the current session running time
    int runningTime() const;
    int currentLockTime() const;
    int lockedTime() const;

    /** SLOTS **/

private slots:
    // Actions on GUI
    void on_actionConnect_triggered();
    void on_actionLaunch_StarPU_triggered();
    void on_actionSaveSessionSetup_triggered();
    void on_actionDebug_triggered(bool enabled);
    void on_actionQuit_triggered();
    void on_actionPreferences_triggered();
    void on_actionAbout_triggered();
    void on_actionAddDataAggregatorWidget_triggered();
    // Setters
    // Set GUI states
    void setConnected(bool connected);
    void setReady(bool ready);
    void setRunning(bool running);
    void setDebug(bool enabled);
    void setDisplayMode(DisplayMode displayMode);
    // Other methods
    void initDataWidgetLists();
    void initInteractiveWidgetLists();
    void displayInteractiveWidget(InteractiveWidget *interactiveWidget);
    void removeDestroyedDataWidgets();
    void removeDestroyedInteractiveWidgets();
    void removeDestroyedDataAggregatorWidgets();
    void clearDescriptions();
    void clearDataDescriptions();
    void clearParamDescriptions();
    void clearWidgets();
    void clearDataWidgets();
    void clearInteractiveWidgets();
    void openSession();
    void closeSession();
    void setStatusBarIcon(QString iconFile);
    void setStatusBarMessage(QString message);
    void debugLock();
    void debugStep();
    void sessionMainWindowSetupLoaded(MainWindowSetup mainWindowSetup);
    void sessionParametersDockSetupLoaded(
            ParametersDockSetup parametersDockSetup);
    void sessionWidgetWindowsSetupLoaded(
            QList<DataWidgetSetup> dataWidgetsSetup,
            QList<DataAggregatorWidgetSetup> dataAggregatorWidgetsSetup);

public slots:
    void synchronizeSessionTime(qlonglong serverTimestamp);
    void initClient(QString serverID,
                    QList<DataDescription*> *dataDescriptions,
                    QList<ParamDescription*> *paramDescriptions,
                    QList<starpu_top_device> *serverDevices);
    // Connection events handlers
    void connectionSucceeded();
    void connectionAborted(QString message);
    void disconnected();
    // GUI updates
    void updateDataWidgetsDisplayMode(int index);
    void updateDisplayModeIndex(int index);
    void updateDataWidgetType(int dataId, DataWidgetType newWidget);
    void updateInteractiveWidgetType(int paramId,
                                     InteractiveWidgetType newWidget);
    void updateStatusBarMessage(QString message);
    // Responses to protocol messages
    void protocolErrorCaught(QString errorMessage);
    // Debug
    void setDebugMessage(QString debugMessage);
    void setDebugLock(QString debugLockMessage);
    // Incoming widget update messages
    void updateDataWidget(int dataWidgetId, bool value, qlonglong timestamp);
    void updateDataWidget(int dataWidgetId, int value, qlonglong timestamp);
    void updateDataWidget(int dataWidgetId, double value, qlonglong timestamp);
    void updateInteractiveWidget(int interactiveWidgetId, bool value,
                                 qlonglong timestamp);
    void updateInteractiveWidget(int interactiveWidgetId, int value,
                                 qlonglong timestamp);
    void updateInteractiveWidget(int interactiveWidgetId, double value,
                                 qlonglong timestamp);
    // Incoming task update messages
    void updateTaskPrev(int taskId, int deviceId, qlonglong timestamp,
			qlonglong timestampStart, qlonglong timestampEnd);
    void updateTaskStart(int taskId, int deviceId, qlonglong timestamp);
    void updateTaskEnd(int taskId, qlonglong timestamp);
    // Outgoing widget update messages
    void interactiveWidgetUpdated(int interactiveWidgetId, bool value);
    void interactiveWidgetUpdated(int interactiveWidgetId, int value);
    void interactiveWidgetUpdated(int interactiveWidgetId, double value);

    /** SIGNALS **/

signals:
    void preferencesDialogCreated(int);
    void sessionOpened();
    void sessionClosed();
    void clientLaunched();
    void dataEnabled(int dataId);
    void dataDisabled(int dataId);
    void paramValueUpdated(int paramId, bool paramValue);
    void paramValueUpdated(int paramId, int paramValue);
    void paramValueUpdated(int paramId, double paramValue);
    void debugEnabled(bool enabled);
    void debugStepped();
    void debugLocked();
};

#endif // MAINWINDOW_H
