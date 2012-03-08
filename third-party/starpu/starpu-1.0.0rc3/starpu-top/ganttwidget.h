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


#ifndef GANTTWIDGET_H
#define GANTTWIDGET_H

class MainWindow;
class TaskManager;

#include <QGLWidget>
#include <QPainter>
#include "starpu_top_types.h"

class GanttWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit GanttWidget(
            MainWindow *mainWindow,
            TaskManager *taskManager);
    ~GanttWidget();

public slots:
    void updateZoom(double value);
    void updatePrevLine(double value);
    void updateTimeView(int time);
    void connected();
    void disconnected();
    void ganttLocked();
    void ganttUnlocked();

protected:
    void paintEvent(QPaintEvent *event);
    void paint(QPainter *painter, QPaintEvent *event);
    void drawPUs(QPainter *painter);
    void drawTime(QPainter *painter);
    void drawProgram(QPainter *painter);
    void resizeGL (int width,int height);
    void drawWorkPU(QPainter *painter, starpu_top_task t);
    void drawIdlePU(QPainter *painter);
    void drawPrevWorkPU(QPainter *painter, starpu_top_task t);
    void defaultScreen(QPainter *painter);
    void drawPresentLine(QPainter *painter);
    int computeTimeInterval(int timeTotal);
    qreal coordxForTime(qlonglong timestamp_ms);
    void drawFromTime(QPainter *painter, qlonglong timestamp);
    void countPUs();

private:
    MainWindow *_mainWindow;
    TaskManager *_taskManager;

    QBrush _background;
    QFont _textFont;
    QPen _textPen;
    QRectF _rectTime;
    QRectF _rectProg;
    QRectF _rectCPUblock;
    QRectF _rectGPUblock;
    QRectF *_rectPUs;
    QRectF _linePresent;
    qreal _coordxPresentLine;
    int _numPUs;
    bool _wasRunning;
    QList<starpu_top_task> _tasks;
    int _timeTotal;
    int _timeAfter;
    int _timeBefore;
    double _lastValPrevLine;
    QTimer *_timer;
    qlonglong _timePresent;
    qlonglong _timeToShow;
    starpu_top_device *_PUsByDevice;
    starpu_top_device *_PUsByPos;
    int _numCPUs;
    int _numGPUs;
    bool _initCompleted;
    int _heightPU; // Height of each CPU or GPU rectangles
};

#endif // GANTTWIDGET_H
