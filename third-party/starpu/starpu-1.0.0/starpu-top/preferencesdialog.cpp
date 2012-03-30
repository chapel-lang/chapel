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


#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"
#include "mainwindow.h"
#include "configurationmanager.h"
#include "sessionsetupmanager.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>

const QString DEFAULT_SESSION_SETUP_NAME = "<default>";

PreferencesDialog::PreferencesDialog(MainWindow *mainWindow,
                                     ConfigurationManager *configurationManager,
                                     SessionSetupManager *sessionSetupManger,
                                     int activeTab) :
QDialog(mainWindow), ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);

    _mainWindow = mainWindow;
    _configurationManager = configurationManager;
    _sessionSetupManager = sessionSetupManger;
    _dataWidgetNames = _mainWindow->dataWidgetNames();
    _dataWidgetPossibilities = _mainWindow->dataWidgetPossibilities();
    _interactiveWidgetNames = _mainWindow->interactiveWidgetNames();
    _interactiveWidgetPossibilities
            = _mainWindow->interactiveWidgetPossibilities();

    ui->ipEdit->setText(_configurationManager->serverHost());
    ui->portEdit->setValue(_configurationManager->serverPort());
    ui->commandEdit->setText(_configurationManager->commandLine());
    ui->connectionSSHGroupBox->setChecked(_configurationManager);
    ui->antialiasingCheckBox->setChecked(_configurationManager->antialiasing());

    ui->displayModeComboBox->addItem(tr("Flying windows"),
                                     DISPLAY_FLYING_WINDOWS);
    ui->displayModeComboBox->addItem(tr("MDI, normal"), DISPLAY_MDI_NORMAL);

    ui->preferencesTab->setCurrentIndex(activeTab);

    // Synchronize display mode indexes with the main window's toolbar
    QObject::connect(_mainWindow, SIGNAL(preferencesDialogCreated(int)),
                     this, SLOT(updateDisplayModeIndex(int)));
    QObject::connect(ui->displayModeComboBox, SIGNAL(currentIndexChanged(int)),
                     _mainWindow, SLOT(updateDisplayModeIndex(int)));
    // Synchronize descriptions setup when session setup is loaded
    QObject::connect(
            _mainWindow->sessionSetupManager(),
            SIGNAL(
                    descriptionsSetupLoaded(QList<DataDescriptionSetup> ,
                                            QList<ParamDescriptionSetup> )),
            this,
            SLOT(
                    sessionDescriptionsSetupLoaded(
                            QList<DataDescriptionSetup> ,
                            QList<ParamDescriptionSetup> )));
    // Synchronize main window setup when session setup is loaded
    QObject::connect(_mainWindow->sessionSetupManager(),
                     SIGNAL(mainWindowSetupLoaded(MainWindowSetup)), this,
                     SLOT(sessionMainWindowSetupLoaded(MainWindowSetup)));

    if (_mainWindow->isConnected())
    {
        ui->connectionServerGroupBox->setEnabled(false);
    }
    if (_mainWindow->isReady() == false)
    {
        ui->displayWidgetsGroupBox->setEnabled(false);
    }

    displayWidgetPreferences();
    displaySessionSetupsList();
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PreferencesDialog::closeEvent(QCloseEvent *ce)
{
    _configurationManager->setServerHost(ui->ipEdit->text());
    _configurationManager->setServerPort(ui->portEdit->value());
    _configurationManager->setCommandLine(ui->commandEdit->text());
    _configurationManager->setAntialiasing(
            ui->antialiasingCheckBox->isChecked());
    _configurationManager->setSSH(ui->connectionSSHGroupBox->isChecked());

    ce->accept();
}

void PreferencesDialog::displayWidgetPreferences()
{
    // Read data descriptions
    for (int i = 0; i < _mainWindow->dataDescriptions()->count(); i++)
    {
        DataDescription *dataDescription = _mainWindow->dataDescriptions()->at(
                i);

        // Init UI
        QGridLayout *layout = (QGridLayout*) ui->dataWidgetsGroupBox->layout();
        layout->addWidget(new QLabel(dataDescription->descriptionString), i, 0,
                          Qt::AlignHCenter);
        QComboBox *dataWidgetList = new QComboBox();
        dataWidgetList->setMinimumWidth(100);

        // Init list
        int dataId = dataDescription->id;
        DataType dataType = dataDescription->type;
        DataWidgetListItemData dataStruct;
        dataStruct.id = dataId;
        QVariant data;
        QList < DataWidgetType > dataWidgetPossibilities
                = _dataWidgetPossibilities->value(dataType).values();

        int currentDataWidgetIndex;
        for (int j = 0; j < dataWidgetPossibilities.count(); j++)
        {
            DataWidgetType widget = dataWidgetPossibilities.at(j);
            if (widget == dataDescription->widget)
            {
                currentDataWidgetIndex = j;
            }
            dataStruct.widget = widget;
            data.setValue(dataStruct);
            dataWidgetList->addItem(_dataWidgetNames->value(widget), data);
        }
        // Last item : MUST be DATA_WIDGET_NONE
        dataStruct.widget = DATA_WIDGET_NONE;
        data.setValue(dataStruct);
        dataWidgetList->addItem(
                _dataWidgetNames->value(DATA_WIDGET_NONE), data);

        // Init index
        if (dataDescription->widget == DATA_WIDGET_NONE)
        { // NONE is the last item
            dataWidgetList->setCurrentIndex(dataWidgetList->count() - 1);
        }
        else
        {
            dataWidgetList->setCurrentIndex(currentDataWidgetIndex);
        }

        QObject::connect(dataWidgetList, SIGNAL(currentIndexChanged(int)),
                         this, SLOT(updateDataWidgetType(int)));
        _dataWidgetListComboBoxes.append(dataWidgetList);
        layout->addWidget(dataWidgetList, i, 1, Qt::AlignHCenter);
    }

    // Param descriptions
    for (int i = 0; i < _mainWindow->paramDescriptions()->count(); i++)
    {
        ParamDescription *paramDescription =
                _mainWindow->paramDescriptions()->at(i);

        QGridLayout *layout =
                (QGridLayout*) ui->interactiveWidgetsGroupBox->layout();
        layout->addWidget(new QLabel(paramDescription->descriptionString), i,
                          0, Qt::AlignHCenter);
        QComboBox *interactiveWidgetList = new QComboBox();
        interactiveWidgetList->setMinimumWidth(100);

        // Init list
        int paramId = paramDescription->id;
        ParamType paramType = paramDescription->type;
        InteractiveWidgetListItemData dataStruct;
        dataStruct.id = paramId;
        QVariant data;
        QList < InteractiveWidgetType > interactiveWidgetPossibilities
                = _interactiveWidgetPossibilities->value(paramType).values();

        int currentInteractiveWidgetIndex;
        for (int j = 0; j < interactiveWidgetPossibilities.count(); j++)
        {
            InteractiveWidgetType widget = interactiveWidgetPossibilities.at(j);
            if (widget == paramDescription->widget)
            {
                currentInteractiveWidgetIndex = j;
            }
            dataStruct.widget = widget;
            data.setValue(dataStruct);
            interactiveWidgetList->addItem(
                    _interactiveWidgetNames->value(widget), data);
        }

        // Init index
        interactiveWidgetList->setCurrentIndex(currentInteractiveWidgetIndex);

        QObject::connect(interactiveWidgetList,
                         SIGNAL(currentIndexChanged(int)),
                         this, SLOT(updateInteractiveWidgetType(int)));
        _interactiveWidgetListComboBoxes.append(interactiveWidgetList);
        layout->addWidget(interactiveWidgetList, i, 1, Qt::AlignHCenter);
    }
}

void PreferencesDialog::displaySessionSetupsList()
{
    // Default session setup must be the first index
    ui->loadSessionSetupComboBox->addItem(DEFAULT_SESSION_SETUP_NAME);

    QDir sessionSetupsDir = QDir(SESSION_SETUPS_DIR);
    QStringList filter;
    filter << "*" + SESSION_SETUPS_FILEEXT;
    sessionSetupsDir.setNameFilters(filter);
    sessionSetupsDir.setSorting(QDir::Time);
    QStringList sessionSetupFiles = sessionSetupsDir.entryList();

    QStringList sessionSetupNames;
    for (int i = 0; i < sessionSetupFiles.count(); i++)
    {
        QString sessionSetupFile = sessionSetupFiles.at(i);
        QString sessionSetupName = sessionSetupFile;
        sessionSetupName.chop(SESSION_SETUPS_FILEEXT.size());
        sessionSetupNames.append(sessionSetupName);
    }
    ui->loadSessionSetupComboBox->addItems(sessionSetupNames);

    ui->loadSessionSetupComboBox->setCurrentIndex(0);
    QObject::connect(ui->loadSessionSetupComboBox,
                     SIGNAL(currentIndexChanged(QString)), this,
                     SLOT(loadSessionSetup(QString)));
}

void PreferencesDialog::updateDisplayModeIndex(int index)
{
    ui->displayModeComboBox->setCurrentIndex(index);
}

void PreferencesDialog::updateDataWidgetType(int index)
{
    QComboBox *dataWidgetList = (QComboBox*) QObject::sender();
    QVariant data = dataWidgetList->itemData(index, Qt::UserRole);
    DataWidgetListItemData dataStruct;

    Q_ASSERT_X(data.canConvert<DataWidgetListItemData> () == true,
               "PreferencesDialog::updateDataWidgetType()",
               "Data extraction from QVariant failed !");

    dataStruct = data.value<DataWidgetListItemData> ();
    int dataId = dataStruct.id;
    DataWidgetType newWidget = dataStruct.widget;

    _mainWindow->updateDataWidgetType(dataId, newWidget);
}

void PreferencesDialog::updateInteractiveWidgetType(int index)
{
    QComboBox *interactiveWidgetList = (QComboBox*) QObject::sender();
    QVariant data = interactiveWidgetList->itemData(index, Qt::UserRole);
    InteractiveWidgetListItemData dataStruct;

    Q_ASSERT_X(data.canConvert<InteractiveWidgetListItemData> () == true,
               "PreferencesDialog::updateInteractiveWidgetType()",
               "Data extraction from QVariant failed !");

    dataStruct = data.value<InteractiveWidgetListItemData> ();
    int paramId = dataStruct.id;

    InteractiveWidgetType newWidget = dataStruct.widget;
    _mainWindow->updateInteractiveWidgetType(paramId, newWidget);
}

void PreferencesDialog::loadSessionSetup(QString sessionSetupName)
{
    if (sessionSetupName.compare(DEFAULT_SESSION_SETUP_NAME) == 0)
    {
        qDebug() << "PreferencesDialog : loading default session setup";

        // Load default data widgets
        loadDefaultDataWidgets();

        // Load default interactive widgets
        loadDefaultInteractiveWidgets();
    }
    else
    {
        qDebug() << "PreferencesDialog : loading session setup"
                << sessionSetupName;

        if (_sessionSetupManager->loadSessionSetup(sessionSetupName) == true)
        {
            qDebug() << "PreferencesDialog : loaded session setup successfully";
        }
        else
        {
            QMessageBox::warning(this, tr("StarPU-Top"),
                                 tr("Couldn't load the session setup !"),
                                 QMessageBox::Ok);

            QComboBox *comboBox = (QComboBox*) QObject::sender();
            int indexToRemove = comboBox->currentIndex();
            comboBox->setCurrentIndex(0);
            comboBox->removeItem(indexToRemove);
        }
    }
}

void PreferencesDialog::loadDefaultDataWidgets()
{
    for (int i = 0; i < _dataWidgetListComboBoxes.count(); i++)
    {
        for (int j = 0; _dataWidgetListComboBoxes.at(i)->count(); j++)
        {
            Q_ASSERT_X(
                    _dataWidgetListComboBoxes.at(i)->itemData(j). canConvert<
                    DataWidgetListItemData> () == true,
                    "PreferencesDialog::loadSessionSetup()",
                    "Data extraction from QVariant failed !");

            DataWidgetListItemData dataStruct =
                    _dataWidgetListComboBoxes.at(i)->itemData(j).value<
                    DataWidgetListItemData> ();
            if (dataStruct.widget == DEFAULT_DATA_WIDGET_BOOL
                || dataStruct.widget == DEFAULT_DATA_WIDGET_INT
                || dataStruct.widget == DEFAULT_DATA_WIDGET_FLOAT)
            {
                _dataWidgetListComboBoxes.at(i)->setCurrentIndex(j);
                break;
            }
        }
    }
}

void PreferencesDialog::loadDefaultInteractiveWidgets()
{
    for (int i = 0; i < _interactiveWidgetListComboBoxes.count(); i++)
    {
        for (int j = 0; _interactiveWidgetListComboBoxes.at(i)->count(); j++)
        {
            Q_ASSERT_X(
                    _interactiveWidgetListComboBoxes.
                    at(i)->itemData(j). canConvert<
                    InteractiveWidgetListItemData> () == true,
                    "PreferencesDialog::loadSessionSetup()",
                    "Data extraction from QVariant failed !");

            InteractiveWidgetListItemData dataStruct =
                    _interactiveWidgetListComboBoxes.at(i) ->itemData(j).value<
                    InteractiveWidgetListItemData> ();
            if (dataStruct.widget == DEFAULT_INTERACTIVE_WIDGET_BOOL
                || dataStruct.widget == DEFAULT_INTERACTIVE_WIDGET_INT
                || dataStruct.widget == DEFAULT_INTERACTIVE_WIDGET_FLOAT
                || dataStruct.widget == DEFAULT_INTERACTIVE_WIDGET_ENUM)
            {
                _interactiveWidgetListComboBoxes.at(i)->setCurrentIndex(j);
                break;
            }
        }
    }
}

void PreferencesDialog::sessionDescriptionsSetupLoaded(
        QList<DataDescriptionSetup> dataDescriptionsSetup,
        QList<ParamDescriptionSetup> paramDescriptionsSetup)
{
    qDebug()
            << "PreferencesDialog : "
            "restoring descriptions setup from another session";

    // Load data descriptions setup
    for (int i = 0; i < dataDescriptionsSetup.count(); i++)
    {
        // The comboboxes are created following the order of the descriptions
        int newIndex = indexOfDataWidget(_dataWidgetListComboBoxes.at(i),
                                         dataDescriptionsSetup.at(i).widget);
        Q_ASSERT_X(newIndex != -1, "PreferencesDialog::sessionSetupLoaded()",
                   "Couldn't find loaded data widget index in the list !");

        _dataWidgetListComboBoxes.at(i)->setCurrentIndex(newIndex);
    }
    // Load param descriptions setup
    for (int i = 0; i < paramDescriptionsSetup.count(); i++)
    {
        // The comboboxes are created following the order of the descriptions
        int newIndex = indexOfInteractiveWidget(
                _interactiveWidgetListComboBoxes.at(i),
                paramDescriptionsSetup.at(i).widget);
        Q_ASSERT_X(newIndex != -1, "PreferencesDialog::sessionSetupLoaded()",
                   "Couldn't find loaded interactive widget index in the list !");

        _interactiveWidgetListComboBoxes.at(i)->setCurrentIndex(newIndex);
    }
}

void PreferencesDialog::sessionMainWindowSetupLoaded(
        MainWindowSetup mainWindowSetup)
{
    if (_mainWindow->displayMode() != mainWindowSetup.displayMode)
    {
        for (int i = 0; i < ui->displayModeComboBox->count(); i++)
        {
            DisplayMode displayMode =
                    (DisplayMode) ui->displayModeComboBox ->itemData(i).toInt();
            if (displayMode == mainWindowSetup.displayMode)
            {
                ui->displayModeComboBox->setCurrentIndex(i);
                return;
            }
        }
    }
}

// Util methods
int PreferencesDialog::indexOfDataWidget(QComboBox* dataWidgetList,
                                         DataWidgetType widget)
{
    for (int i = 0; dataWidgetList->count(); i++)
    {
        Q_ASSERT_X(
                dataWidgetList->itemData(i)
                . canConvert<DataWidgetListItemData> ()
                == true, "PreferencesDialog::indexOfDataWidget()",
                "Data extraction from QVariant failed !");

        DataWidgetListItemData dataStruct = dataWidgetList->itemData(i).value<
                                            DataWidgetListItemData> ();
        if (dataStruct.widget == widget)
        {
            return i;
        }
    }

    return -1;
}

int PreferencesDialog::indexOfInteractiveWidget(
        QComboBox* interactiveWidgetList, InteractiveWidgetType widget)
{
    for (int i = 0; interactiveWidgetList->count(); i++)
    {
        Q_ASSERT_X(
                interactiveWidgetList->itemData(i). canConvert<
                InteractiveWidgetListItemData> () == true,
                "PreferencesDialog::indexOfInteractiveWidget()",
                "Data extraction from QVariant failed !");

        InteractiveWidgetListItemData dataStruct =
                interactiveWidgetList->itemData(i).value<
                InteractiveWidgetListItemData> ();
        if (dataStruct.widget == widget)
        {
            return i;
        }
    }

    return -1;
}
