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


#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

class MainWindow;
class ConfigurationManager;
class SessionSetupManager;

#include <QMetaType>
#include <QDialog>
#include <QComboBox>
#include "starpu_top_types.h"

namespace Ui
{
    class PreferencesDialog;
}

class PreferencesDialog: public QDialog
{ /* The preferences panel of the application.
 Each type of preferences is in a different tab. */
    Q_OBJECT

public:
    explicit PreferencesDialog(MainWindow *mainWindow,
                               ConfigurationManager *configurationManager,
                               SessionSetupManager *sessionSetupManger, int activeTab = 0);
    ~PreferencesDialog();

protected:
    // Events
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *ce);

private:
    // GUI components
    Ui::PreferencesDialog *ui;
    QList<QComboBox*> _dataWidgetListComboBoxes;
    QList<QComboBox*> _interactiveWidgetListComboBoxes;
    // Components
    MainWindow *_mainWindow;
    ConfigurationManager *_configurationManager;
    SessionSetupManager *_sessionSetupManager;
    // Metadata
    const QHash<DataWidgetType, QString> *_dataWidgetNames;
    const QHash<DataType, QSet<DataWidgetType> > *_dataWidgetPossibilities;
    const QHash<InteractiveWidgetType, QString> *_interactiveWidgetNames;
    const QHash<ParamType, QSet<InteractiveWidgetType> >
            *_interactiveWidgetPossibilities;

    // Util methods
    int indexOfDataWidget(QComboBox* dataWidgetList, DataWidgetType widget);
    int indexOfInteractiveWidget(QComboBox* interactiveWidgetList,
                                 InteractiveWidgetType widget);

private slots:
    // Other methods
    void displayWidgetPreferences();
    void displaySessionSetupsList();
    void updateDisplayModeIndex(int index);
    void updateDataWidgetType(int index);
    void updateInteractiveWidgetType(int index);
    void loadSessionSetup(QString fileName);
    void loadDefaultDataWidgets();
    void loadDefaultInteractiveWidgets();
    void sessionDescriptionsSetupLoaded(
            QList<DataDescriptionSetup> dataDescriptionsSetup,
            QList<ParamDescriptionSetup> paramDescriptionsSetup);
    void sessionMainWindowSetupLoaded(MainWindowSetup mainWindowSetup);
};

// Model structs for the widget choice comboboxes
Q_DECLARE_METATYPE( DataWidgetListItemData)
Q_DECLARE_METATYPE(InteractiveWidgetListItemData)

#endif // PREFERENCESDIALOG_H
