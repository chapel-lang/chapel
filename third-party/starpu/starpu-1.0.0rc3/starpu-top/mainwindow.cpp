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


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QAction>
#include <qwt_slider.h>
#include "configurationmanager.h"
#include "communicationthread.h"
#include "widgetwindowsmanager.h"
#include "sessionsetupmanager.h"
#include "taskmanager.h"
#include "debugconsole.h"
#include "ganttwidget.h"
#include "preferencesdialog.h"
#include "interactivewidget.h"
#include "datawidget.h"
#include "dataaggregatorwidget.h"
#include "aboutdialog.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Set up GUI components
    ui->setupUi(this);
    ui->mainToolBar->setWindowTitle("Toolbar");
    _mdiArea = new QMdiArea();
    _mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    _mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // Init data
    initDataWidgetLists();
    initInteractiveWidgetLists();
    _dataWidgets = new QList<QPointer<DataWidget> > ();
    _interactiveWidgets = new QList<QPointer<InteractiveWidget> > ();
    _dataAggregatorWidgets = new QList<QPointer<DataAggregatorWidget> > ();
    _dataDescriptions = new QList<DataDescription*> ();
    _paramDescriptions = new QList<ParamDescription*> ();
    _serverDevices = new QList<starpu_top_device> ();
    _nbDataWidgets = _nbInteractiveWidgets = _nbDataAggregatorWidgets = 0;

    // Init managers
    _configurationManager = new ConfigurationManager();
    _communicationThread = new CommunicationThread(this, _configurationManager);
    _widgetWindowsManager = new WidgetWindowsManager(this, _mdiArea);
    _sessionSetupManager = new SessionSetupManager(this);
    _taskManager = new TaskManager();

    // Setup status bar
    _statusBarIcon = new QLabel();
    _statusBarLabel = new QLabel();
    statusBar()->addPermanentWidget(_statusBarIcon);
    statusBar()->addWidget(_statusBarLabel);

    // Setup interactions
    // Setup main actions
    // Action connect
    _actionConnect = ui->mainToolBar->addAction(QIcon(":/images/connect.png"),
                                                tr("Connect to StarPU"));
    _actionConnect->setIconText("Connect to StarPU");
    _actionConnect->setToolTip("Connect to StarPU");
    _actionConnect->setShortcut(QKeySequence("Ctrl+C"));
    QToolButton *connectButton =
            (QToolButton*)ui->mainToolBar->widgetForAction(_actionConnect);
    connectButton->setPopupMode(QToolButton::InstantPopup);
    QObject::connect(_actionConnect, SIGNAL(triggered()), this,
                     SLOT(on_actionConnect_triggered()));
    QAction *settingsAction = new QAction(
            "Edit connection settings",
            _actionConnect);
    settingsAction->setIcon(QIcon(":/images/preferences.png"));
    QObject::connect(settingsAction, SIGNAL(triggered()), this,
                     SLOT(on_actionPreferences_triggered()));
    connectButton->addAction(settingsAction);
    ui->menu_starpu_top->addAction(_actionConnect);
    // Action launch
    _actionLaunch = ui->mainToolBar->addAction(QIcon(":/images/play.png"),
                                               tr("Launch StarPU"));
    _actionLaunch->setIconText("Launch StarPU");
    _actionLaunch->setToolTip("Launch StarPU");
    _actionLaunch->setShortcut(QKeySequence("Ctrl+L"));
    ui->menu_starpu_top->addAction(_actionLaunch);
    QObject::connect(_actionLaunch, SIGNAL(triggered()), this,
                     SLOT(on_actionLaunch_StarPU_triggered()));
    ui->mainToolBar->addSeparator();
    ui->menu_starpu_top->addSeparator();
    // Action debug
    _actionDebug = ui->mainToolBar->addAction(QIcon(":/images/debugon.png"),
                                              tr("Enable debug"));
    _actionDebug->setIconText("Enable debug");
    _actionDebug->setToolTip("Enable debug");
    _actionDebug->setShortcut(QKeySequence("Ctrl+D"));
    _actionDebug->setCheckable(true);
    ui->menu_starpu_top->addAction(_actionDebug);
    QObject::connect(_actionDebug, SIGNAL(toggled(bool)),
                     this, SLOT(on_actionDebug_triggered(bool)));
    // Action save session setup
    _actionSaveSessionSetup = ui->mainToolBar->addAction(
            QIcon(":/images/windows.png"), tr("Save session setup"));
    _actionSaveSessionSetup->setIconText("Save session setup");
    _actionSaveSessionSetup->setToolTip("Save session setup");
    _actionSaveSessionSetup->setShortcut(QKeySequence("Ctrl+S"));
    ui->menu_starpu_top->addAction(_actionSaveSessionSetup);
    QObject::connect(_actionSaveSessionSetup, SIGNAL(triggered()), this,
                     SLOT(on_actionSaveSessionSetup_triggered()));
    // Action add data aggregator widget
    _actionAddDataAggregatorWidget =
            ui->mainToolBar->addAction(QIcon(":/images/widget.png"),
                                       tr("Add data aggregator widget"));
    _actionAddDataAggregatorWidget->setIconText("Add data aggregator widget");
    _actionAddDataAggregatorWidget->setToolTip("Add data aggregator widget");
    _actionAddDataAggregatorWidget->setShortcut(QKeySequence("Ctrl+G"));
    ui->menu_starpu_top->addAction(_actionAddDataAggregatorWidget);
    QObject::connect(_actionAddDataAggregatorWidget, SIGNAL(triggered()), this,
                     SLOT(on_actionAddDataAggregatorWidget_triggered()));
    ui->mainToolBar->addSeparator();
    ui->menu_starpu_top->addSeparator();
    // Action quit
    QAction *actionQuit = ui->menu_starpu_top->addAction(
            QIcon(":/images/quit.png"), tr("Quit"));
    actionQuit->setIconText("Quit");
    actionQuit->setToolTip("Quit");
    QObject::connect(actionQuit, SIGNAL(triggered()), this,
                     SLOT(on_actionQuit_triggered()));
    actionQuit->setShortcut(QKeySequence("Ctrl+Q"));

    _widgetsDisplayModesList = new QComboBox(ui->mainToolBar);
    _widgetsDisplayModesList->addItem(tr("Flying windows"),
                                      DISPLAY_FLYING_WINDOWS);
    _widgetsDisplayModesList->addItem(tr("MDI, normal"), DISPLAY_MDI_NORMAL);
    _widgetsDisplayModesList->setCurrentIndex(1);
    QObject::connect(_widgetsDisplayModesList, SIGNAL(currentIndexChanged(int)),
                     this, SLOT(updateDataWidgetsDisplayMode(int)));
    ui->mainToolBar->addWidget(_widgetsDisplayModesList);

    // Init GUI components
    // Retrieve the central widget layout
    QGridLayout *centralLayout = (QGridLayout*) ui->centralWidget->layout();
    // Install MDI area
    _mdiArea->setMinimumSize(500, 200);
    _mdiArea->addAction(_actionAddDataAggregatorWidget);
    _mdiArea->setContextMenuPolicy(Qt::ActionsContextMenu);
    _mdiArea->setEnabled(false);
    // Install gantt widget
    _ganttWidget = new GanttWidget(this, _taskManager);
    _ganttWidget->setMinimumSize(500, 200);
    // Install parameters dock
    _parametersDockScrollArea = new QScrollArea();
    _parametersDockScrollArea->setMinimumSize(250, 250);
    _parametersDockScrollArea->setLayout(new QVBoxLayout());
    ui->parametersDock->setWidget(_parametersDockScrollArea);
    // Install the gantt widget container and the gantt controllers
    QWidget *ganttContainer = new QWidget();
    QGridLayout *ganttContainerLayout = new QGridLayout();
    ganttContainer->setLayout(ganttContainerLayout);
    // Zoom controller
    QwtSlider *ganttZoom = new QwtSlider(ganttContainer, Qt::Vertical);
    ganttZoom->setRange(100.0, 20000.0, 1.0);
    ganttZoom->setValue(10000.0);
    QObject::connect(ganttZoom, SIGNAL(valueChanged(double)),
                     _ganttWidget, SLOT(updateZoom(double)));
    // Time view controller
    _ganttTimeView = new QSpinBox();
    _ganttTimeView->setRange(0, INT_MAX);
    _ganttTimeView->setAccelerated(true);
    QObject::connect(_ganttTimeView, SIGNAL(valueChanged(int)),
                     _ganttWidget, SLOT(updateTimeView(int)));
    // Prevision line controller
    QwtSlider *ganttPrevLine = new QwtSlider(ganttContainer, Qt::Horizontal);
    ganttPrevLine->setRange(0, 100);
    ganttPrevLine->setValue(66.67);
    QObject::connect(ganttPrevLine, SIGNAL(valueChanged(double)),
                     _ganttWidget, SLOT(updatePrevLine(double)));
    // Set gantt layouts
    QHBoxLayout *ganttBottom = new QHBoxLayout();
    ganttBottom->addWidget(_ganttTimeView, 0);
    ganttBottom->addWidget(ganttPrevLine, 1);
    ganttContainerLayout->addWidget(_ganttWidget, 0, 0);
    ganttContainerLayout->addWidget(ganttZoom, 0, 1);
    ganttContainerLayout->addLayout(ganttBottom, 1, 0);
    // Install debug console
    _debugConsole = new DebugConsole(this);
    QObject::connect(_debugConsole, SIGNAL(stepped()), this, SLOT(debugStep()));
    _debugConsole->setVisible(false);

    // Install splitter
    _splitter = new QSplitter(Qt::Vertical);
    _splitter->setChildrenCollapsible(false);
    _splitter->setHandleWidth(5);
    _splitter->addWidget(_mdiArea);
    _splitter->addWidget(ganttContainer);
    _splitter->addWidget(_debugConsole);
    centralLayout->addWidget(_splitter, 0, 0);

    // Restore main window setup when a session setup is loaded
    QObject::connect(_sessionSetupManager,
                     SIGNAL(mainWindowSetupLoaded(MainWindowSetup)),
                     this,
                     SLOT(sessionMainWindowSetupLoaded(MainWindowSetup)));
    // Restore parameters dock setup when a session setup is loaded
    QObject::connect(_sessionSetupManager,
                     SIGNAL(parametersDockSetupLoaded(ParametersDockSetup)),
                     this,
                     SLOT(sessionParametersDockSetupLoaded(ParametersDockSetup)));
    // Restore widgets windows setup when a session setup is loaded
    QObject::connect(
            _sessionSetupManager,
            SIGNAL(
                    widgetWindowsSetupLoaded(
                            QList<DataWidgetSetup> ,
                            QList<DataAggregatorWidgetSetup> )),
            this,
            SLOT(
                    sessionWidgetWindowsSetupLoaded(
                            QList<DataWidgetSetup> ,
                            QList<DataAggregatorWidgetSetup> )));

    // Init UI states
    _displayMode = DISPLAY_MDI_NORMAL;
    _connected = false;
    _ready = false;
    _running = false;
    _debug = false;

    // Init main window connexion status
    setConnected(false);

    // Load previously saved configuration
    _configurationManager->syncConfiguration();
}

MainWindow::~MainWindow()
{
    delete _statusBarLabel;
    delete _statusBarIcon;
    delete ui;
    delete _taskManager;
}

/* -------------------------------------------------------------------------- */
/* Events                                                                     */
/* -------------------------------------------------------------------------- */
void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);

    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *ce)
{
    int returned = -1;
    if (isRunning())
    {
        returned
                = QMessageBox::question(
                        this,
                        tr("Quit StarPU-Top ?"),
                        tr("StarPU-Top is currently monitoring.\n")
                        + tr(
                                "Do you really want to stop "
                                "monitoring and exit the application ?"),
                        QMessageBox::Ok | QMessageBox::Cancel);
    }
    else
    {
        _configurationManager->syncConfiguration();

        qDebug() << "MainWindow : exiting the application";

        qApp->quit(); // Exit the application
    }

    if (returned == QMessageBox::Ok)
    {
        if (_communicationThread->isRunning())
        {
            _communicationThread->exit(0);

            qDebug() << "MainWindow : stopped communication thread";
        }

        _configurationManager->syncConfiguration();

        qDebug() << "MainWindow : exiting the application";

        qApp->quit(); // exit application
    }
    else
    {
        ce->ignore(); // ignore close event
    }
}

void MainWindow::openSession()
{
    bool ok;

    // (Re)initialize the tasks database
    _taskManager->initialize();
    ok = _taskManager->connectDatabase();
    Q_ASSERT_X(ok == true,
               "MainWindow::openSession()",
               "Couldn't connect to the tasks database !");

    // Init time references
    _lockedTime = 0;

    // Possibly clear old session widgets
    clearWidgets();
    // Enable the MDI area
    _mdiArea->setEnabled(true);

    // Install fresh data widgets
    for (int i = 0; i < _dataDescriptions->count(); i++)
    {
        if (_dataDescriptions->at(i)->widget != DATA_WIDGET_NONE)
        {
            DataWidget* dataWidget = addDataWidget(_dataDescriptions->at(i));
            _widgetWindowsManager->displayWidgetWindow(dataWidget);
        }
    }
    // Install fresh interactive widgets
    for (int i = 0; i < _paramDescriptions->count(); i++)
    {
        if (_paramDescriptions->at(i)->widget != INTERACTIVE_WIDGET_NONE)
        {
            InteractiveWidget* interactiveWidget = addInteractiveWidget(
                    _paramDescriptions->at(i));
            displayInteractiveWidget(interactiveWidget);
        }
    }

    emit sessionOpened();
}

void MainWindow::closeSession()
{
    // Clear old data/params descriptions
    clearDescriptions();
    // Disable the MDI area
    _mdiArea->setEnabled(false);
    // Update gantt time controller
    _ganttTimeView->setValue(effectiveRunningTime());

    // Close the preferences dialog if open
    if (_preferencesDialog != 0)
    {
        _preferencesDialog->close();
    }
    // Stop the communication thread
    if (_communicationThread->isRunning())
    {
        _communicationThread->exit(0);

        qDebug() << "MainWindow : stopped communication thread";
    }

    QMessageBox::information(
            this,
            tr("End of the session"),
            tr("The session has just finished (time running : ")
            + QString::number(runningTime()) + "ms, time locked : "
            + QString::number(lockedTime())
            + "ms, time effectively running : " + QString::number(
                    effectiveRunningTime()) + "ms)", QMessageBox::Ok);

    emit sessionClosed();
}

void MainWindow::debugLock()
{
    qDebug() << "MainWindow : locking session";

    _currentLockTime.start();
    setStatusBarIcon(":/images/lock.png");
    setStatusBarMessage("Locked");

    emit debugLocked();
}

void MainWindow::debugStep()
{
    qDebug() << "MainWindow : unlocking session";

    _lockedTime += _currentLockTime.elapsed();
    _currentLockTime = QTime();
    _actionDebug->setEnabled(true);
    setStatusBarIcon("");
    setStatusBarMessage("Running");

    emit debugStepped();
}

void MainWindow::sessionMainWindowSetupLoaded(MainWindowSetup mainWindowSetup)
{
    qDebug() << "MainWindow : restoring main window setup from another session";

    // Load geometry
    if (restoreGeometry(mainWindowSetup.geometry) == false)
    {
        qDebug() << "MainWindow : failed to restore main window geometry";
    }
    // Load display mode
    setDisplayMode(mainWindowSetup.displayMode);
}

void MainWindow::sessionParametersDockSetupLoaded(
        ParametersDockSetup parametersDockSetup)
{
    qDebug() << "MainWindow : "
            "restoring parameters dock setup from another session";
}

void MainWindow::sessionWidgetWindowsSetupLoaded(
        QList<DataWidgetSetup> dataWidgetsSetup,
        QList<DataAggregatorWidgetSetup> dataAggregatorWidgetsSetup)
{
    qDebug()
            << "MainWindow : "
            "restoring widget windows setup from another session";

    // Restore data widgets setup
    for (int i = 0; i < dataWidgetsSetup.count(); i++)
    {
        DataWidget *dataWidget = dataWidgetFromDataId(
                dataWidgetsSetup.at(i).dataId);
        if (dataWidget->isInside() != dataWidgetsSetup.at(i).inside)
        {
            dataWidget->setInside(dataWidgetsSetup.at(i).inside);
            if (dataWidget->isInside() == true)
            {
                _widgetWindowsManager->flyingWindowToMdi(dataWidget);
            }
            else
            {
                _widgetWindowsManager->mdiToFlyingWindow(dataWidget);
            }
        }

        if (dataWidget->isInside() == false)
        {
            dataWidget->restoreGeometry(dataWidgetsSetup.at(i).geometry);
        }
        else
        {
            dataWidget->parentWidget()->restoreGeometry(
                    dataWidgetsSetup.at(i).geometry);
        }
    }
    // Restore data aggregator widgets setup
    for (int i = 0; i < dataAggregatorWidgetsSetup.count(); i++)
    {
        DataAggregatorWidget *dataAggregatorWidget = addDataAggregatorWidget(
                dataAggregatorWidgetsSetup.at(i).dataIds);
        _widgetWindowsManager->displayWidgetWindow(dataAggregatorWidget);

        if (dataAggregatorWidget->isInside()
            != dataAggregatorWidgetsSetup.at(i).inside)
            {
            dataAggregatorWidget->setInside(
                    dataAggregatorWidgetsSetup.at(i).inside);
            if (dataAggregatorWidget->isInside() == true)
            {
                _widgetWindowsManager->flyingWindowToMdi(dataAggregatorWidget);
            }
            else
            {
                _widgetWindowsManager->mdiToFlyingWindow(dataAggregatorWidget);
            }
        }

        if (dataAggregatorWidget->isInside() == false)
        {
            dataAggregatorWidget ->restoreGeometry(
                    dataAggregatorWidgetsSetup.at(i).geometry);
        }
        else
        {
            dataAggregatorWidget->parentWidget() ->restoreGeometry(
                    dataAggregatorWidgetsSetup.at(i).geometry);
        }
    }
}

void MainWindow::protocolErrorCaught(QString errorMessage)
{
    qDebug() << "MainWindow : protocol error caught (" + errorMessage + ")";
}

void MainWindow::synchronizeSessionTime(qlonglong serverTimestamp)
{
    qDebug() << "Synchronized with server reference timestamp ("
            << serverTimestamp << ")";

    _referenceTimestamp = serverTimestamp;

    if (_preGoTimer.isNull())
    {
        _preGoTimer.start();
    }
    else
    {
        _preGoTimer.restart();
    }
}

void MainWindow::initClient(QString serverID,
                            QList<DataDescription*> *dataDescriptions,
                            QList<ParamDescription*> *paramDescriptions,
                            QList<starpu_top_device> *serverDevices)
{
    _serverID = serverID;
    _dataDescriptions = dataDescriptions;
    _paramDescriptions = paramDescriptions;
    _serverDevices = serverDevices;

    setReady(true);

    // Display widget choices list in preferences dialog
    _preferencesDialog = new PreferencesDialog(this, _configurationManager,
                                               _sessionSetupManager, 1);
    emit preferencesDialogCreated(_widgetsDisplayModesList->currentIndex());
    _preferencesDialog->setVisible(true);
}

/* -------------------------------------------------------------------------- */
/* Actions                                                                    */
/* -------------------------------------------------------------------------- */

void MainWindow::connectionSucceeded()
{
    qDebug() << "MainWindow : connected to server";

    setConnected(true);
}

void MainWindow::connectionAborted(QString message)
{
    QMessageBox::critical(this, "StarPU-Top", message, QMessageBox::Ok);

    setConnected(false);
}

void MainWindow::disconnected()
{
    if (isRunning())
    {
        setRunning(false);
    }
    if (isReady())
    {
        setReady(false);
    }
    setConnected(false);

    qDebug() << "MainWindow : disconnected from server";
}

void MainWindow::on_actionConnect_triggered()
{
    setStatusBarMessage("Awaiting connection...");
    _actionConnect->setEnabled(false);

    // Initiate connection to server
    _communicationThread->start(/*QThread::NormalPriority*/);
    _communicationThread->moveToThread(_communicationThread);
}

void MainWindow::on_actionLaunch_StarPU_triggered()
{
    // Tell server to launch the session
    setRunning(true);

    emit clientLaunched();
}

void MainWindow::on_actionDebug_triggered(bool enabled)
{
    _actionDebug->setEnabled(false);
    _actionDebug->setToolTip("Waiting for the server's acknowledgement...");
    _actionDebug->setIconText("Waiting for the server's acknowledgement...");

    emit debugEnabled(enabled);
}

void MainWindow::on_actionSaveSessionSetup_triggered()
{
    if (_sessionSetupManager->saveSessionSetup(_serverID) == true)
    {
        qDebug() << "MainWindow : session setup successfully saved";

        statusBar()->showMessage(tr("Session setup successfully saved"),3000);
    }
    else
    {
        statusBar()->showMessage(tr("Couldn't save the session setup"),3000);
    }
}

void MainWindow::on_actionQuit_triggered()
{
    close(); // Calls CloseEvent()
}

void MainWindow::on_actionAddDataAggregatorWidget_triggered()
{
    AbstractWidgetWindow *widgetWindow =
            (AbstractWidgetWindow*) addDataAggregatorWidget();
    _widgetWindowsManager->displayWidgetWindow(widgetWindow);
}

void MainWindow::on_actionPreferences_triggered()
{
    int tabindex = (_connected ? 1 : 0);

    // Open preferences dialog
    _preferencesDialog = new PreferencesDialog(this, _configurationManager,
                                               _sessionSetupManager, tabindex);
    _preferencesDialog->setVisible(true);

    emit preferencesDialogCreated(_widgetsDisplayModesList->currentIndex());
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->setVisible(true);
}

/* -------------------------------------------------------------------------- */
/* Getters                                                                    */
/* -------------------------------------------------------------------------- */

const QSplitter *MainWindow::splitter() const
{
    return _splitter;
}

const QDockWidget *MainWindow::parametersDock() const
{
    return ui->parametersDock;
}

const WidgetWindowsManager* MainWindow::widgetWindowsManager() const
{
    return _widgetWindowsManager;
}

const ConfigurationManager* MainWindow::configurationManager() const
{
    return _configurationManager;
}

const SessionSetupManager* MainWindow::sessionSetupManager() const
{
    return _sessionSetupManager;
}

const TaskManager* MainWindow::taskManager() const
{
    return _taskManager;
}

const QHash<DataWidgetType, QString> *MainWindow::dataWidgetNames() const
{
    return &_dataWidgetNames;
}

const QHash<DataType, QSet<DataWidgetType> > 
        *MainWindow::dataWidgetPossibilities() const
{
    return &_dataWidgetPossibilities;
}

const QHash<InteractiveWidgetType, QString> 
        *MainWindow::interactiveWidgetNames() const
{
    return &_interactiveWidgetNames;
}

const QHash<ParamType, QSet<InteractiveWidgetType> > 
        *MainWindow::interactiveWidgetPossibilities() const
{
    return &_interactiveWidgetPossibilities;
}

int MainWindow::nbDataWidgets() const
{
    return _nbDataWidgets;
}

int MainWindow::nbInteractiveWidgets() const
{
    return _nbInteractiveWidgets;
}

int MainWindow::nbDataAggregatorWidgets() const
{
    return _nbDataAggregatorWidgets;
}

QString MainWindow::serverID() const
{
    return _serverID;
}

const QList<DataDescription*> *MainWindow::dataDescriptions() const
{
    return _dataDescriptions;
}

const QList<ParamDescription*> *MainWindow::paramDescriptions() const
{
    return _paramDescriptions;
}

bool MainWindow::isConnected() const
{
    return _connected;
}

bool MainWindow::isReady() const
{
    return _ready;
}

bool MainWindow::isRunning() const
{
    return _running;
}

bool MainWindow::isDebugEnabled() const
{
    return _debug;
}

DisplayMode MainWindow::displayMode() const
{
    return _displayMode;
}

int MainWindow::effectiveRunningTime() const
{
    if (_runningTime.isNull())
    {
        return 0;
    }

    return _runningTime.elapsed() - (_lockedTime + currentLockTime());
}

int MainWindow::runningTime() const
{
    if (_runningTime.isNull())
    {
        return 0;
    }

    return _runningTime.elapsed();
}

int MainWindow::currentLockTime() const
{
    if (_currentLockTime.isNull())
    {
        return 0;
    }

    return _currentLockTime.elapsed();
}

int MainWindow::lockedTime() const
{
    return _lockedTime;
}

/* -------------------------------------------------------------------------- */
/* Setters                                                                    */
/* -------------------------------------------------------------------------- */

void MainWindow::setStatusBarMessage(QString message)
{
    if (_statusBarLabel != 0)
    {
        _statusBarLabel->setText(message);
    }
}

void MainWindow::updateStatusBarMessage(QString message)
{
    setStatusBarMessage(message);
}

void MainWindow::setStatusBarIcon(QString iconFile)
{
    if (_statusBarIcon != 0)
    {
        _statusBarIcon->setPixmap(QPixmap(iconFile));
    }
}

void MainWindow::setConnected(bool connected)
{
    _connected = connected;

    if (_connected == false)
    {
        Q_ASSERT_X(_ready == false && _running == false,
                   "MainWindow::setConnected()",
                   "MainWindow states hierarchy violated");

        _actionConnect->setEnabled(true);
        _actionLaunch->setEnabled(false);
        _actionDebug->setEnabled(false);
        _actionSaveSessionSetup->setEnabled(false);
        _actionAddDataAggregatorWidget->setEnabled(false);
        setStatusBarMessage(tr("Disconnected"));
    }
    else
    {
        _actionConnect->setEnabled(false);
        setStatusBarMessage(
                tr("Connected (") + _configurationManager->serverHost() + ":"
                + QString::number(_configurationManager->serverPort())
                + ")");
    }
}

void MainWindow::setReady(bool ready)
{
    _ready = ready;

    if (_ready == true)
    {
        Q_ASSERT_X(_connected == true, "MainWindow::setReady()",
                   "MainWindow states hierarchy violated");

        _actionLaunch->setEnabled(true);
        _actionDebug->setEnabled(true);
        _actionAddDataAggregatorWidget->setEnabled(true);
        setStatusBarMessage(
                tr("Connected and ready (") + _serverID + tr(" on ")
                + _configurationManager->serverHost() + ":"
                + QString::number(_configurationManager->serverPort())
                + ")");

        openSession();

        _actionSaveSessionSetup->setEnabled(true);
    }
    else
    {
        Q_ASSERT_X(_running == false, "MainWindow::setReady()",
                   "MainWindow states hierarchy violated");

        _actionConnect->setEnabled(false);
        _actionLaunch->setEnabled(false);
        _actionDebug->setEnabled(false);
        _actionAddDataAggregatorWidget->setEnabled(false);
        setStatusBarMessage(
                tr("Connected (") + _configurationManager->serverHost() + ":"
                + QString::number(_configurationManager->serverPort())
                + ")");

        closeSession();

        _actionSaveSessionSetup->setEnabled(false);
    }
}

void MainWindow::setRunning(bool running)
{
    _running = running;

    if (running == true)
    {
        Q_ASSERT_X(_connected == true && _ready == true,
                   "MainWindow::setRunning()",
                   "MainWindow states hierarchy violated");

        // Stop pre-running time and start running time
        _preGoTime = _preGoTimer.elapsed() + _referenceTimestamp;
        qDebug() << "Pre-Go time :" << _preGoTime;

        if (_runningTime.isNull())
        {
            _runningTime.start();
        }
        else
        {
            _runningTime.restart();
        }

        _actionLaunch->setEnabled(false);
        setStatusBarMessage(tr("Running"));
        setWindowTitle(
                windowTitle() + " (" + _serverID + tr(" on ")
                + _configurationManager->serverHost() + ":"
                + QString::number(_configurationManager->serverPort())
                + ")");
    }
    else
    {
        _actionLaunch->setEnabled(true);
        setStatusBarMessage(
                tr("Connected and ready (") + _serverID + tr(" on ")
                + _configurationManager->serverHost() + ":"
                + QString::number(_configurationManager->serverPort())
                + ")");
        setWindowTitle(tr("StarPU-Top"));
    }
}

void MainWindow::setDebug(bool enabled)
{
    _debug = enabled;
    _actionDebug->setEnabled(true);

    if (_debug == true)
    {
        qDebug() << "MainWindow : enabling debug mode";

        _actionDebug->setIcon(QIcon(":/images/debugoff.png"));
        _actionDebug->setIconText("Disable debug");
        _actionDebug->setText("Disable debug");
        _actionDebug->setToolTip("Disable debug");
        _debugConsole->setVisible(true);
    }
    else
    {
        qDebug() << "MainWindow : disabling debug mode";

        _actionDebug->setIcon(QIcon(":/images/debugon.png"));
        _actionDebug->setIconText("Enable debug");
        _actionDebug->setText("Enable debug");
        _actionDebug->setToolTip("Enable debug");
        _debugConsole->setVisible(false);
    }
}

void MainWindow::setDisplayMode(DisplayMode displayMode)
{
    _displayMode = displayMode;

    switch (_displayMode)
    {
    case DISPLAY_FLYING_WINDOWS:
        _widgetWindowsManager->mdiToFlyingWindows();
        break;
    case DISPLAY_MDI_NORMAL:
        _widgetWindowsManager->flyingWindowsToMdi();
        break;
    }
}

void MainWindow::setDebugMessage(QString debugMessage)
{
    if (isDebugEnabled() == true)
    {
        _debugConsole->appendDebugLogMessage(debugMessage);
    }
    else
    {
        qDebug() << "Debug message ignored (received while not in debug mode)";
    }
}

void MainWindow::setDebugLock(QString debugLockMessage)
{
    if (isDebugEnabled() == true)
    {
        debugLock();

        _debugConsole->appendDebugLockMessage(debugLockMessage);
    }
    else
    {
        qDebug() << "Debug lock ignored (received while not in debug mode)";
    }
}

/* -------------------------------------------------------------------------- */
/* Other methods                                                              */
/* -------------------------------------------------------------------------- */

void MainWindow::clearDescriptions()
{
    clearDataDescriptions();
    clearParamDescriptions();
}

DataWidget* MainWindow::addDataWidget(DataDescription *dataDescription)
{
    // Create new data widget following the description
    bool inside = (_displayMode == DISPLAY_FLYING_WINDOWS ? false : true);
    DataWidget *dataWidget = new DataWidget(dataDescription,
                                            _widgetWindowsManager,
                                            this, inside);

    _dataWidgets->append(QPointer<DataWidget> (dataWidget));
    _nbDataWidgets++;

    return dataWidget;
}

void MainWindow::clearWidgets()
{
    clearDataWidgets();
    clearInteractiveWidgets();
}

void MainWindow::clearDataWidgets()
{
    _widgetWindowsManager->closeWidgetWindows();
    _dataWidgets->clear();
}

void MainWindow::removeDestroyedDataWidgets()
{
    // Clear all null data widget pointers in the data widgets list
    for (int i = 0; i < _dataWidgets->count(); i++)
    {
        if (_dataWidgets->at(i).isNull())
        {
            _dataWidgets->removeAt(i);
            _nbDataWidgets--;
        }
    }
}

void MainWindow::clearDataDescriptions()
{
    _dataDescriptions->clear();
}

InteractiveWidget *MainWindow::addInteractiveWidget(
        ParamDescription *paramDescription)
{
    // Add new interactive widget following the description
    InteractiveWidget *interactiveWidget = new InteractiveWidget(
            paramDescription, this);

    _interactiveWidgets->append(
            QPointer<InteractiveWidget> (interactiveWidget));
    _nbInteractiveWidgets++;

    return interactiveWidget;
}

DataAggregatorWidget *MainWindow::addDataAggregatorWidget(QList<int> dataIds)
{
    bool inside = (_displayMode == DISPLAY_FLYING_WINDOWS ? false : true);
    DataAggregatorWidget *dataAggregatorWidget = new DataAggregatorWidget(
            _widgetWindowsManager, this, inside, dataIds);

    _dataAggregatorWidgets->append(
            QPointer<DataAggregatorWidget> (dataAggregatorWidget));
    _nbDataAggregatorWidgets++;

    return dataAggregatorWidget;
}

void MainWindow::displayInteractiveWidget(InteractiveWidget *interactiveWidget)
{
    // Display interactive widget in the dock widget
    QVBoxLayout* layout = (QVBoxLayout*) _parametersDockScrollArea->layout();
    layout->addWidget(interactiveWidget);
}

void MainWindow::clearInteractiveWidgets()
{
    qDebug() << "Closing all interactive widgets";

    for (int i = 0; i < _interactiveWidgets->count(); i++)
    {
        _interactiveWidgets->at(i).data()->close();
    }
    _interactiveWidgets->clear();
}

void MainWindow::removeDestroyedInteractiveWidgets()
{
    // Clear all null data widget pointers in the data widgets list
    for (int i = 0; i < _interactiveWidgets->count(); i++)
    {
        if (_interactiveWidgets->at(i).isNull())
        {
            _interactiveWidgets->removeAt(i);
            _paramDescriptions->at(i)->widget = INTERACTIVE_WIDGET_NONE;
            _nbInteractiveWidgets--;
        }
    }
}

void MainWindow::clearParamDescriptions()
{
    _paramDescriptions->clear();
}

void MainWindow::removeDestroyedDataAggregatorWidgets()
{
    // Clear all null data aggregator widget pointers in list
    for (int i = 0; i < _dataAggregatorWidgets->count(); i++)
    {
        if (_dataAggregatorWidgets->at(i).isNull())
        {
            _dataAggregatorWidgets->removeAt(i);
            _nbDataAggregatorWidgets--;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Getters                                                                    */
/* -------------------------------------------------------------------------- */

QString MainWindow::statusBarMessage() const
{
    return _statusBarLabel->text();
}

const QList<QPointer<DataWidget> > *MainWindow::dataWidgets() const
{
    return _dataWidgets;
}

const QList<QPointer<InteractiveWidget> > 
        *MainWindow::interactiveWidgets() const
{
    return _interactiveWidgets;
}

const QList<QPointer<DataAggregatorWidget> > 
        *MainWindow::dataAggregatorWidgets() const
{
    return _dataAggregatorWidgets;
}

DataWidget *MainWindow::dataWidgetFromDataId(int dataId)
{
    for (int i = 0; i < _dataWidgets->count(); i++)
    {
        if (_dataWidgets->at(i)->description()->id == dataId)
            return _dataWidgets->at(i);
    }

    return 0;
}

InteractiveWidget *MainWindow::interactiveWidgetFromParamId(int paramId)
{
    for (int i = 0; i < _interactiveWidgets->count(); i++)
    {
        if (_interactiveWidgets->at(i)->description()->id == paramId)
            return _interactiveWidgets->at(i);
    }

    return 0;
}

DataDescription *MainWindow::dataDescriptionFromId(int dataId)
{
    for (int i = 0; i < _dataDescriptions->count(); i++)
    {
        if (_dataDescriptions->at(i)->id == dataId)
            return _dataDescriptions->at(i);
    }

    return 0;
}

ParamDescription *MainWindow::paramDescriptionFromId(int paramId)
{
    for (int i = 0; i < _paramDescriptions->count(); i++)
    {
        if (_paramDescriptions->at(i)->id == paramId)
            return _paramDescriptions->at(i);
    }

    return 0;
}

const QList<starpu_top_device> *MainWindow::serverDevices() const
{
    return _serverDevices;
}

/* -------------------------------------------------------------------------- */
/* Other methods                                                              */
/* -------------------------------------------------------------------------- */

void MainWindow::updateDataWidgetsDisplayMode(int index)
{
    // Update the display mode
    setDisplayMode(
            (DisplayMode) _widgetsDisplayModesList->itemData(index).toInt());
}

void MainWindow::updateDisplayModeIndex(int index)
{
    // Update display mode choice index in the toolbar
    _widgetsDisplayModesList->setCurrentIndex(index);
}

void MainWindow::updateDataWidgetType(int dataId, DataWidgetType newWidget)
{
    // Update the data widget description with type selected by user
    DataDescription *dataDescription = dataDescriptionFromId(dataId);

    if (dataDescription != 0)
    {
        DataWidgetType oldWidget = dataDescription->widget;
        dataDescription->widget = newWidget;

        qDebug() << "Data of id" << dataId << "changed widget type (old :"
                << oldWidget << " new:" << newWidget << ")";

        DataWidget *dataWidget = dataWidgetFromDataId(dataId);

        if (newWidget == DATA_WIDGET_NONE && oldWidget != DATA_WIDGET_NONE)
        { // We have disabled this data
            emit dataDisabled(dataId);
            qDebug() << "Data of id has been" << dataId << "DISABLED";

            _widgetWindowsManager->closeWidgetWindow(dataWidget);
        }
        else if (newWidget != DATA_WIDGET_NONE && oldWidget == DATA_WIDGET_NONE)
        { // We have enabled this data
            // The entire widget needs to be created
            _widgetWindowsManager->displayWidgetWindow(
                    addDataWidget(dataDescriptionFromId(dataId)));

            emit dataEnabled(dataId);
            qDebug() << "Data of id has been" << dataId << "ENABLED";
        }
        else if (newWidget != DATA_WIDGET_NONE && oldWidget != DATA_WIDGET_NONE)
        { // Neither disable nor enabled, just change of the widget
            // The widget just needs to redraw the internal data widget
            dataWidget->recreateInternalWidget();
        }
    }
}

void MainWindow::updateInteractiveWidgetType(int paramId,
                                             InteractiveWidgetType newWidget)
{
    ParamDescription *paramDescription = paramDescriptionFromId(paramId);

    if (paramDescription != 0)
    {
        InteractiveWidgetType oldWidget = paramDescription->widget;
        paramDescription->widget = newWidget;

        qDebug() << "Param of id" << paramId << "changed widget type (old :"
                << oldWidget << " new:" << newWidget << ")";

        // The widget just needs to redraw the internal interactive widget
        InteractiveWidget *interactiveWidget = interactiveWidgetFromParamId(
                paramId);
        interactiveWidget->recreateInternalWidget();
    }
}

/* -------------------------------------------------------------------------- */
/* Slots                                                                      */
/* -------------------------------------------------------------------------- */

void MainWindow::updateInteractiveWidget(int interactiveWidgetId, bool value,
                                         qlonglong timestamp)
{
    InteractiveWidget *interactiveWidget = interactiveWidgetFromParamId(
            interactiveWidgetId);
    if (interactiveWidget != 0)
    {
        qDebug() << "Updating interactive widget of id" << interactiveWidgetId
                << "updated with value" << value << "and timestamp"
                << timestamp;

        interactiveWidget->setValue(value);
    }
    else
    {
        qDebug() << "Trying to update inexistant interactive widget ! (id"
                << interactiveWidgetId << ")";
    }
}

void MainWindow::updateInteractiveWidget(int interactiveWidgetId, int value,
                                         qlonglong timestamp)
{
    InteractiveWidget *interactiveWidget = interactiveWidgetFromParamId(
            interactiveWidgetId);
    if (interactiveWidget != 0)
    {
        qDebug() << "Updating interactive widget of id" << interactiveWidgetId
                << "updated with value" << value << "and timestamp"
                << timestamp;

        interactiveWidget->setValue(value);
    }
    else
    {
        qDebug() << "Trying to update inexistant interactive widget ! (id"
                << interactiveWidgetId << ")";
    }
}

void MainWindow::updateInteractiveWidget(int interactiveWidgetId, double value,
                                         qlonglong timestamp)
{
    InteractiveWidget *interactiveWidget = interactiveWidgetFromParamId(
            interactiveWidgetId);
    if (interactiveWidget != 0)
    {
        qDebug() << "Updating interactive widget of id" << interactiveWidgetId
                << "updated with value" << value << "and timestamp"
                << timestamp;

        interactiveWidget->setValue(value);
    }
    else
    {
        qDebug() << "Trying to update inexistant interactive widget ! (id"
                << interactiveWidgetId << ")";
    }
}

void MainWindow::updateTaskPrev(int taskId, int deviceId, qlonglong timestamp,
                                qlonglong timestampStart,
                                qlonglong timestampEnd)
{
    _taskManager->addTaskPrev(taskId, deviceId, timestampStart - _preGoTime,
                              timestampEnd - _preGoTime);
}

void MainWindow::updateTaskStart(int taskId, int deviceId, qlonglong timestamp)
{
    _taskManager->addTaskStart(taskId, deviceId, timestamp - _preGoTime);
}

void MainWindow::updateTaskEnd(int taskId, qlonglong timestamp)
{
    _taskManager->addTaskEnd(taskId, timestamp - _preGoTime);
}

void MainWindow::updateDataWidget(int dataWidgetId, bool value,
                                  qlonglong timestamp)
{
    DataWidget *dataWidget = dataWidgetFromDataId(dataWidgetId);
    if (dataWidget != 0)
    {
        qDebug() << "MainWindow : updating data widget of id" << dataWidgetId
                << "with value" << value << "and timestamp" << timestamp;

        dataWidget->setValue(value);
    }
    else
    {
        qDebug()
                << "MainWindow : trying to update inexistant data widget ! (id"
                << dataWidgetId << ")";
    }
}

void MainWindow::updateDataWidget(int dataWidgetId, int value,
                                  qlonglong timestamp)
{
    DataWidget *dataWidget = dataWidgetFromDataId(dataWidgetId);
    if (dataWidget != 0)
    {
        qDebug() << "MainWindow : updating data widget of id" << dataWidgetId
                << "with value" << value << "and timestamp" << timestamp;

        dataWidget->setValue(value);
    }
    else
    {
        qDebug()
                << "MainWindow : trying to update inexistant data widget ! (id"
                << dataWidgetId << ")";
    }
}

void MainWindow::updateDataWidget(int dataWidgetId, double value,
                                  qlonglong timestamp)
{
    DataWidget *dataWidget = dataWidgetFromDataId(dataWidgetId);
    if (dataWidget != 0)
    {
        qDebug() << "MainWindow : updating data widget of id" << dataWidgetId
                << "with value" << value << "and timestamp" << timestamp;

        dataWidget->setValue(value);
    }
    else
    {
        qDebug()
                << "MainWindow : trying to update inexistant data widget ! (id"
                << dataWidgetId << ")";
    }
}

void MainWindow::interactiveWidgetUpdated(int interactiveWidgetId, bool value)
{
    qDebug() << "MainWindow : interactive widget of id" << interactiveWidgetId
            << "updated with value" << value;

    emit paramValueUpdated(interactiveWidgetId, value);
}

void MainWindow::interactiveWidgetUpdated(int interactiveWidgetId, int value)
{
    qDebug() << "MainWindow : interactive widget of id" << interactiveWidgetId
            << "updated with value" << value;

    emit paramValueUpdated(interactiveWidgetId, value);
}

void MainWindow::interactiveWidgetUpdated(int interactiveWidgetId, double value)
{
    qDebug() << "MainWindow : interactive widget of id" << interactiveWidgetId
            << "updated with value" << value;

    emit paramValueUpdated(interactiveWidgetId, value);
}

void MainWindow::initDataWidgetLists()
{
    //Names
    _dataWidgetNames.insert(DATA_WIDGET_LCD, tr("Lcd"));
    _dataWidgetNames.insert(DATA_WIDGET_PLOT, tr("Plot"));
    _dataWidgetNames.insert(DATA_WIDGET_LEVEL, tr("Level"));
    _dataWidgetNames.insert(DATA_WIDGET_LED, tr("Led"));
    _dataWidgetNames.insert(DATA_WIDGET_DIAL, tr("Dial"));
    _dataWidgetNames.insert(DATA_WIDGET_NONE, tr("<none>"));

    // Sets of widgets for each type
    // Bool
    _dataBoolWidgets.insert(DATA_WIDGET_LED);
    // Int
    _dataIntWidgets.insert(DATA_WIDGET_LCD);
    _dataIntWidgets.insert(DATA_WIDGET_PLOT);
    _dataIntWidgets.insert(DATA_WIDGET_LEVEL);
    _dataIntWidgets.insert(DATA_WIDGET_DIAL);
    // Float
    _dataFloatWidgets.insert(DATA_WIDGET_LCD);
    _dataFloatWidgets.insert(DATA_WIDGET_PLOT);
    _dataFloatWidgets.insert(DATA_WIDGET_LEVEL);
    _dataFloatWidgets.insert(DATA_WIDGET_DIAL);

    // Associate a type to widget possibilities
    _dataWidgetPossibilities.insert(DATA_TYPE_BOOL, _dataBoolWidgets);
    _dataWidgetPossibilities.insert(DATA_TYPE_INT, _dataIntWidgets);
    _dataWidgetPossibilities.insert(DATA_TYPE_FLOAT, _dataFloatWidgets);
}

void MainWindow::initInteractiveWidgetLists()
{
    _interactiveWidgetNames.insert(INTERACTIVE_WIDGET_SLIDER, tr("Slider"));
    _interactiveWidgetNames.insert(INTERACTIVE_WIDGET_KNOB, tr("Knob"));
    _interactiveWidgetNames.insert(INTERACTIVE_WIDGET_WHEEL, tr("Wheel"));
    _interactiveWidgetNames.insert(INTERACTIVE_WIDGET_CHECKBOX, tr("Checkbox"));
    _interactiveWidgetNames.insert(INTERACTIVE_WIDGET_SPINBOX, tr("Spinbox"));
    _interactiveWidgetNames.insert(INTERACTIVE_WIDGET_DOUBLESPINBOX,
                                   tr("Double Spinbox"));
    _interactiveWidgetNames.insert(INTERACTIVE_WIDGET_DIAL, tr("Dial"));
    _interactiveWidgetNames.insert(INTERACTIVE_WIDGET_COMBOBOX, tr("Combobox"));
    _interactiveWidgetNames.insert(INTERACTIVE_WIDGET_NONE, tr("<none>"));

    // Sets of widgets for each type
    // Bool
    _paramBoolWidgets.insert(INTERACTIVE_WIDGET_CHECKBOX);
    // Int
    _paramIntWidgets.insert(INTERACTIVE_WIDGET_SLIDER);
    _paramIntWidgets.insert(INTERACTIVE_WIDGET_KNOB);
    _paramIntWidgets.insert(INTERACTIVE_WIDGET_WHEEL);
    _paramIntWidgets.insert(INTERACTIVE_WIDGET_SPINBOX);
    _paramIntWidgets.insert(INTERACTIVE_WIDGET_DIAL);
    // Float
    _paramFloatWidgets.insert(INTERACTIVE_WIDGET_SLIDER);
    _paramFloatWidgets.insert(INTERACTIVE_WIDGET_KNOB);
    _paramFloatWidgets.insert(INTERACTIVE_WIDGET_WHEEL);
    _paramFloatWidgets.insert(INTERACTIVE_WIDGET_DOUBLESPINBOX);
    _paramFloatWidgets.insert(INTERACTIVE_WIDGET_DIAL);
    // Enum
    _paramEnumWidgets.insert(INTERACTIVE_WIDGET_COMBOBOX);

    // Associate a type to widget possibilities
    _interactiveWidgetPossibilities.insert(PARAM_TYPE_BOOL, _paramBoolWidgets);
    _interactiveWidgetPossibilities.insert(PARAM_TYPE_INT, _paramIntWidgets);
    _interactiveWidgetPossibilities.insert(PARAM_TYPE_FLOAT,
                                           _paramFloatWidgets);
    _interactiveWidgetPossibilities.insert(PARAM_TYPE_ENUM, _paramEnumWidgets);
}
