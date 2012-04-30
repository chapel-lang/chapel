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


#ifndef TASKMANAGER_H
#define TASKMANAGER_H

#include "starpu_top_types.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class TaskManager
{ /* Manages the tasks received from the server
     in a memory SQLITE database. */
public:
    TaskManager();
    ~TaskManager();

    void initialize();
    bool connectDatabase();
    // Add new tasks
    void addTaskPrev(int taskId, int deviceId, qlonglong timestampStart,
                     qlonglong timestampEnd);
    void addTaskStart(int taskId, int deviceId, qlonglong timestampStart);
    void addTaskEnd(int taskId, qlonglong timestampEnd);
    // Getters
    QList<starpu_top_task> tasks(qlonglong timestampStart,
                               qlonglong timestampEnd);
    QList<starpu_top_task> prevTasks(qlonglong timestampStart,
                                   qlonglong timestampEnd);

private:
    // Metadata
    // Database
    QSqlDatabase _database;
    // Queries
    QSqlQuery _insertTaskPrevQuery;
    QSqlQuery _insertTaskStartQuery;
    QSqlQuery _updateTaskEndQuery;
    QSqlQuery _selectTasksQuery;
    QSqlQuery _selectPrevTasksQuery;
};

#endif // TASKMANAGER_H
