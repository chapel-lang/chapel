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


#include "debugconsole.h"
#include "ui_debugconsole.h"

DebugConsole::DebugConsole(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::DebugConsole)
{
    ui->setupUi(this);

    ui->stepButton->setEnabled(false);
    QObject::connect(ui->stepButton, SIGNAL(clicked()),
                     this, SLOT(step()));
}

DebugConsole::~DebugConsole()
{
    delete ui;
}

void DebugConsole::appendDebugLogMessage(QString debugMessage)
{
    ui->console->append("INFO : " + debugMessage);
}

void DebugConsole::appendDebugLockMessage(QString lockMessage)
{
    ui->stepButton->setEnabled(true);

    ui->console->append("LOCK : " + lockMessage);
}

void DebugConsole::clearConsole()
{
    ui->console->clear();
}

void DebugConsole::step()
{
    ui->stepButton->setEnabled(false);

    emit stepped();
}
