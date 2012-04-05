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


#include "taskmanager.h"
#include <QVariant>
#include <QSqlRecord>
#include <QSqlError>

TaskManager::TaskManager()
{
    static bool instanciated = false;

    Q_ASSERT_X(instanciated == false, "TaskManager's' constructor",
               "Singleton pattern violated - "
               "TaskManager instanciated more than once");

    qDebug() << "TaskManager : initializing";

    instanciated = true;
}

TaskManager::~TaskManager()
{
    qDebug() << "TaskManager : terminating";

    _database.close();
}

void TaskManager::initialize()
{
    if (_database.isOpen())
    {
        _database.close();
    }

    _database = QSqlDatabase::addDatabase("QSQLITE");
    _database.setDatabaseName(":memory:");
}

bool TaskManager::connectDatabase()
{
    if (_database.open() == false)
    {
        qDebug() << "TaskManager : connection to tasks database failed";

        return false;
    }

    qDebug() << "TaskManager : connection to tasks database established";

    QSqlQuery createDbQuery;
    // Real tasks table
    createDbQuery.exec("create table tasks (id int primary key, "
                       "device int, start int, end int)");
    createDbQuery.exec("create index tsidx on tasks(start,end)");
    // Prev tasks table
    createDbQuery.exec("create table prevtasks (id int primary key, "
                       "device int, start int, end int)");
    createDbQuery.exec("create index prevtsidx on prevtasks(start,end)");

    // Prepare the queries
    QSqlQuery insertTaskPrevQuery, insertTaskStartQuery, updateTaskEndQuery,
    selectTasksQuery, selectPrevTasksQuery;

    insertTaskPrevQuery.prepare(
            "insert or replace into prevtasks (id, device, start, end) "
            "values (?, ?, ?, ?)");
    insertTaskStartQuery.prepare("insert into tasks (id, device, start, end) "
                                 "values (?, ?, ?, -1)");
    updateTaskEndQuery.prepare("update tasks set end = ? where id = ?");
    selectTasksQuery.prepare(
            "select * from tasks where (start between ? and ?) "
            "or (end between ? and ?) or (start < ? and end = -1)");
    selectPrevTasksQuery.prepare(
            "select * from prevtasks where (start between ? and ?) "
            "or (end between ? and ?) or (start < ? and end = -1)");

    // Optimisation
    insertTaskPrevQuery.setForwardOnly(true);
    insertTaskStartQuery.setForwardOnly(true);
    updateTaskEndQuery.setForwardOnly(true);
    selectTasksQuery.setForwardOnly(true);
    selectPrevTasksQuery.setForwardOnly(true);

    _insertTaskPrevQuery = insertTaskPrevQuery;
    _insertTaskStartQuery = insertTaskStartQuery;
    _updateTaskEndQuery = updateTaskEndQuery;
    _selectTasksQuery = selectTasksQuery;
    _selectPrevTasksQuery = selectPrevTasksQuery;

    return true;
}

void TaskManager::addTaskPrev(int taskId, int deviceId,
                              qlonglong timestampStart, qlonglong timestampEnd)
{
    _insertTaskPrevQuery.addBindValue(taskId);
    _insertTaskPrevQuery.addBindValue(deviceId);
    _insertTaskPrevQuery.addBindValue(timestampStart);
    _insertTaskPrevQuery.addBindValue(timestampEnd);

    if (_insertTaskPrevQuery.exec() == false)
    {
        qDebug() << "TaskManager : add task PREV for task" << taskId
                << "failed ! (" << _insertTaskPrevQuery.lastError() << ")";
    }
    else
    {
        qDebug() << "TaskManager : registered new task PREV with id" << taskId
                << "on device" << deviceId << ", timestampStart"
                << timestampStart << ", timestampEnd" << timestampEnd;
    }
}

void TaskManager::addTaskStart(int taskId, int deviceId,
                               qlonglong timestampStart)
{
    _insertTaskStartQuery.addBindValue(taskId);
    _insertTaskStartQuery.addBindValue(deviceId);
    _insertTaskStartQuery.addBindValue(timestampStart);

    if (_insertTaskStartQuery.exec() == false)
    {
        qDebug() << "TaskManager : add task START for task" << taskId
                << "failed !" << _insertTaskStartQuery.lastError() << ")";
    }
    else
    {
        qDebug() << "TaskManager : registered task START with id" << taskId
                << "on device" << deviceId << ", timestampStart"
                << timestampStart;
    }
}

void TaskManager::addTaskEnd(int taskId, qlonglong timestampEnd)
{
    _updateTaskEndQuery.addBindValue(timestampEnd);
    _updateTaskEndQuery.addBindValue(taskId);

    if (_updateTaskEndQuery.exec() == false)
    {
        qDebug() << "TaskManager : add task END for task" << taskId
                << "failed !" << _updateTaskEndQuery.lastError() << ")";
    }
    else
    {
        qDebug() << "TaskManager : registered task END with id" << taskId
                << ", timestampEnd" << timestampEnd;
    }
}

QList<starpu_top_task> TaskManager::tasks(qlonglong timestampStart,
					  qlonglong timestampEnd)
{
    QList < starpu_top_task > tasks;

    _selectTasksQuery.addBindValue(timestampStart);
    _selectTasksQuery.addBindValue(timestampEnd);
    _selectTasksQuery.addBindValue(timestampStart);
    _selectTasksQuery.addBindValue(timestampEnd);
    _selectTasksQuery.addBindValue(timestampStart);

    if (_selectTasksQuery.exec() == false)
    {
        qDebug() << "TaskManager : failed to select tasks ! ("
                << _selectTasksQuery.lastError() << ")";
    }
    else
    {
        qDebug() << "TaskManager : fetching tasks between" << timestampStart
                << "and" << timestampEnd;

        int idField = _selectTasksQuery.record().indexOf("id");
        int deviceField = _selectTasksQuery.record().indexOf("device");
        int startField = _selectTasksQuery.record().indexOf("start");
        int endField = _selectTasksQuery.record().indexOf("end");

        while (_selectTasksQuery.next())
        {
            int taskId = _selectTasksQuery.value(idField).toInt();
            int deviceId = _selectTasksQuery.value(deviceField).toInt();
            qlonglong timestampStart =
                    _selectTasksQuery.value(startField).toLongLong();
            qlonglong timestampEnd =
                    _selectTasksQuery.value(endField).toLongLong();

            starpu_top_task task;
            task.taskId = taskId;
            task.deviceId = deviceId;
            task.timestampStart = timestampStart;
            task.timestampEnd = timestampEnd;
            tasks.append(task);
        }

        qDebug() << "TaskManager : selected" << tasks.count() << "tasks";
    }

    return tasks;
}

QList<starpu_top_task> TaskManager::prevTasks(qlonglong timestampStart,
                                            qlonglong timestampEnd)
{
    QList < starpu_top_task > prevTasks;

    _selectPrevTasksQuery.addBindValue(timestampStart);
    _selectPrevTasksQuery.addBindValue(timestampEnd);
    _selectPrevTasksQuery.addBindValue(timestampStart);
    _selectPrevTasksQuery.addBindValue(timestampEnd);
    _selectPrevTasksQuery.addBindValue(timestampStart);

    if (_selectPrevTasksQuery.exec() == false)
    {
        qDebug() << "TaskManager : failed to select prev tasks ! ("
                << _selectPrevTasksQuery.lastError() << ")";
    }
    else
    {
        qDebug() << "TaskManager : fetching prev tasks between"
                << timestampStart << "and" << timestampEnd;

        int idField = _selectPrevTasksQuery.record().indexOf("id");
        int deviceField = _selectPrevTasksQuery.record().indexOf("device");
        int startField = _selectPrevTasksQuery.record().indexOf("start");
        int endField = _selectPrevTasksQuery.record().indexOf("end");

        while (_selectPrevTasksQuery.next())
        {
            int taskId = _selectPrevTasksQuery.value(idField).toInt();
            int deviceId = _selectPrevTasksQuery.value(deviceField).toInt();
            qlonglong timestampStart =
                    _selectPrevTasksQuery.value(startField).toLongLong();
            qlonglong timestampEnd =
                    _selectPrevTasksQuery.value(endField).toLongLong();

            starpu_top_task prevTask;
            prevTask.taskId = taskId;
            prevTask.deviceId = deviceId;
            prevTask.timestampStart = timestampStart;
            prevTask.timestampEnd = timestampEnd;
            prevTasks.append(prevTask);
        }

        qDebug() << "TaskManager : selected" << prevTasks.count()
                << "prev tasks";
    }

    return prevTasks;
}
