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


#include "ganttwidget.h"
#include "mainwindow.h"
#include "taskmanager.h"
#include <math.h>
#include <QtOpenGL>
#include <QTimer>
#include <sstream>

const int MAX_CPU_NUM = 6;
const int MAX_GPU_NUM = 3;
const int MARGIN = 5;
const int DEFAULT_TIME_TOTAL = 20000;
const int HEIGHT_TIME_AXIS = 20;
const int WIDTH_GLOBAL_PU = 50; // Width of the big CPU rect
const int WIDTH_PROGRAM = 50;
const int WIDTH_PU = 70;
const int GRAD_NUM = 5; // Number of gradations between each label
const int WIDTH_PRESENT_LINE = 1;
const int SIGNIF_NUM = 100;
const int FIRST_THRESHOLD = 25;
const int SECOND_THRESHOLD = 75;
const int SHIFT_LEFT_TIME_AXIS = 3;
const int SHIFT_TOP_TIME_AXIS = 3;

const QColor GANTT_BACKGROUND_COLOR = Qt::gray;
const QFont::StyleHint GANTT_TEXT_FONT = QFont::Times;
const QColor GANTT_TEXT_COLOR = Qt::white;
const QColor GANTT_PU_COLOR = Qt::blue;
const QColor GANTT_TIMELINE_COLOR = Qt::blue;
const QColor GANTT_PRESENTLINE_COLOR = Qt::black;
const QColor GANTT_IDLE_COLOR = Qt::red;
const QColor GANTT_WORKING_COLOR = Qt::green;
const QColor GANTT_PREV_COLOR = Qt::yellow;

GanttWidget::GanttWidget(MainWindow *mainWindow, TaskManager *taskManager) :
	QGLWidget(QGLFormat(QGL::SampleBuffers), mainWindow)
{
    _mainWindow = mainWindow;
    _taskManager = taskManager;

    setAutoFillBackground(false);
    _background = QBrush(GANTT_BACKGROUND_COLOR);
    _textPen = QPen(GANTT_TEXT_COLOR);
    _textFont.setPixelSize(50);
    _textFont.setStyleHint(GANTT_TEXT_FONT);

    _coordxPresentLine = 0;
    _numPUs = 0;
    _wasRunning = false;
    _timeTotal = DEFAULT_TIME_TOTAL;
    _timeBefore = _timeTotal * 0.66;
    _timeAfter = _timeTotal * 0.34;
    _lastValPrevLine = 66;
    _timePresent = 0;
    _timeToShow = 0;
    _numCPUs = 0;
    _numGPUs = 0;
    _heightPU = 0;
    _initCompleted = false;
    _PUsByDevice = 0;
    _PUsByPos = 0;

    // Start display update timer
    _timer = new QTimer(this);
    _timer->start(35);

    // Connect events
    QObject::connect(_mainWindow, SIGNAL(sessionOpened()),
                     this, SLOT(connected()));
    QObject::connect(_mainWindow, SIGNAL(sessionClosed()),
                     this, SLOT(disconnected()));
    QObject::connect(_mainWindow, SIGNAL(debugLocked()),
                     this, SLOT(ganttLocked()));
    QObject::connect(_mainWindow, SIGNAL(debugStepped()),
                     this, SLOT(ganttUnlocked()));
    QObject::connect(_timer, SIGNAL(timeout()),
                     this, SLOT(update()));
}

GanttWidget::~GanttWidget()
{
    delete _timer;
    delete _rectPUs;
}

void GanttWidget::defaultScreen(QPainter *painter)
{
    painter->setPen(GANTT_TEXT_COLOR);
    drawPresentLine(painter);
    drawTime(painter);
}

// Called each time the window is resized
void GanttWidget::resizeGL(int width, int height)
{
    update();
}

void GanttWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(&painter, event);
    painter.end();
}

/* Draw CPU and GPU rects */
void GanttWidget::drawPUs(QPainter *painter)
{
    QFont f;
    QPen penRect;
    QPen penText;
    QPointF coordTxt;
    QString txt;
    QStaticText name;

    f.setFamily("utf8");
    f.setWeight(QFont::Bold);
    f.setPixelSize(12);
    painter->setFont(f);
    penRect.setColor(GANTT_PU_COLOR);
    penText.setColor(GANTT_TEXT_COLOR);
    painter->setPen(penRect);
    _rectPUs = new QRectF[_numCPUs + _numGPUs];
    int height_allowed = size().height() - HEIGHT_TIME_AXIS - MARGIN;
    // height of each processor unit rectangles
    int height = height_allowed / (_numCPUs + _numGPUs);
    _heightPU = height;
    int height_cpublock = height * _numCPUs;
    int height_gpublock = height * _numGPUs;

    /* The CPUs rect */
    if (_numCPUs > 0)
    {
        _rectCPUblock = QRectF(WIDTH_PROGRAM + MARGIN,
                               HEIGHT_TIME_AXIS + MARGIN, WIDTH_GLOBAL_PU,
                               height_cpublock - MARGIN);
        painter->drawRect(_rectCPUblock);
        painter->fillRect(_rectCPUblock, GANTT_PU_COLOR);
        painter->setPen(penText);
        coordTxt = QPointF(
                _rectCPUblock.center().rx() - (WIDTH_GLOBAL_PU / 2) + 3,
                _rectCPUblock.center().ry() - f.pixelSize());
        txt = QString("CPU(s)");
        name = QStaticText(txt);
        name.setPerformanceHint(QStaticText::AggressiveCaching);

        painter->drawStaticText(coordTxt, name);
    }

    f.setWeight(QFont::Light);
    f.setPixelSize(10);
    painter->setFont(f);
    painter->setPen(penRect);

    /* devices */
    for (int i = 0; i < _numCPUs + _numGPUs; i++)
    {
        painter->setPen(penRect);
        _rectPUs[i] = QRectF(WIDTH_PROGRAM + MARGIN + WIDTH_GLOBAL_PU + MARGIN,
                             HEIGHT_TIME_AXIS + MARGIN + i * height, WIDTH_PU,
                             height - MARGIN);
        painter->drawRect(_rectPUs[i]);
        painter->fillRect(_rectPUs[i], Qt::blue);
        painter->setPen(penText);
        if (_heightPU >= f.pixelSize() * 2)
        {
            painter->drawText(_rectPUs[i], Qt::AlignVCenter, _PUsByPos[i].name);
        }
    }

    f.setFamily("utf8");
    f.setWeight(QFont::Bold);
    f.setPixelSize(12);
    painter->setFont(f);
    painter->setPen(penRect);
    /* The GPUs rect */
    if (_numGPUs > 0)
    {
        _rectGPUblock = QRectF(WIDTH_PROGRAM + MARGIN,
                               HEIGHT_TIME_AXIS + MARGIN + height_cpublock, WIDTH_GLOBAL_PU,
                               height_gpublock - MARGIN);
        painter->drawRect(_rectGPUblock);
        painter->fillRect(_rectGPUblock, Qt::blue);
        coordTxt = QPointF(
                _rectGPUblock.center().rx() - (WIDTH_GLOBAL_PU / 2) + 3,
                _rectGPUblock.center().ry() - f.pixelSize());
        txt = QString("GPU(s)");
        name = QStaticText(txt);
        name.setPerformanceHint(QStaticText::AggressiveCaching);
        painter->setPen(penText);
        painter->drawStaticText(coordTxt, name);
    }
}

/* calculate abscissa on the screen for a given time (in ms) if it's displayable
 return -1 if timestampMs is outside time displayable because of scale*/
qreal GanttWidget::coordxForTime(qlonglong timestampMs)
{
    qreal startx = (qreal) WIDTH_PROGRAM + MARGIN + WIDTH_GLOBAL_PU + MARGIN
                   + WIDTH_PU + MARGIN;
    qreal widthAllowed = (qreal) size().width() - startx;
    qreal scale = (qreal) widthAllowed / _timeTotal;
    qreal coordx = -1;

    if (timestampMs >= 0)
    {
        if ((timestampMs <= _timePresent) && (timestampMs >= (_timePresent
                                                              - _timeBefore)))
        {
            coordx = (qreal) startx + (qreal)(
                    (_timeBefore - (_timePresent - timestampMs)) * scale);
        }
        else if ((timestampMs > _timePresent) && timestampMs <= (_timeAfter
                                                                 + _timePresent))
        {
            coordx = (qreal) _coordxPresentLine + (qreal)(
                    (timestampMs - _timePresent) * scale);
        }
    }

    return coordx;
}

void GanttWidget::drawTime(QPainter *painter)
{
    QFont f;
    QPen penRect;
    QPen penText;
    penRect.setColor(GANTT_TIMELINE_COLOR);
    penText.setColor(GANTT_TEXT_COLOR);
    painter->setPen(penRect);
    f.setFamily("utf8");
    f.setWeight(QFont::DemiBold);
    f.setPixelSize(10);
    painter->setFont(f);

    _rectTime = QRectF(0, 0, size().width(), 20);
    painter->drawRect(_rectTime);
    painter->fillRect(_rectTime, Qt::blue);
    QString titre = QString("Time (ms)");
    QStaticText titreTxt = QStaticText(titre);
    titreTxt.setPerformanceHint(QStaticText::AggressiveCaching);
    QPointF coordRect = QPointF(_rectTime.left() + SHIFT_LEFT_TIME_AXIS,
                                _rectTime.center().ry() - f.pixelSize() + SHIFT_TOP_TIME_AXIS);
    painter->setPen(penText);
    painter->drawStaticText(coordRect, titreTxt);
    f.setWeight(QFont::Light);
    f.setPixelSize(10);
    painter->setFont(f);

    qreal ourStartx = WIDTH_PROGRAM + MARGIN + WIDTH_GLOBAL_PU + MARGIN
                      + WIDTH_PU + MARGIN;
    qreal widthAvailable = ((qreal)(size().width() - ourStartx));
    qreal scale = ((qreal)(_timeBefore + _timeAfter) / widthAvailable);
    qreal scale_time_px = (qreal) widthAvailable / (_timeBefore + _timeAfter);
    int timeInterval = computeTimeInterval(_timeTotal);
    qreal sizeInterval = ((qreal)(timeInterval * scale_time_px));
    qreal step_x = (qreal)(sizeInterval / GRAD_NUM);

    //we calculate the location of the next gradation in the future part
    qlonglong first = _timePresent - _timePresent % ((qlonglong) timeInterval);
    qlonglong cur_t = first;

    //gradations in the past
    while (cur_t > _timePresent - _timeBefore)
    {
        if (cur_t >= (qlonglong) 0)
        {
            qreal cur_x =
                    ((qreal)((_timeBefore - (_timePresent - cur_t)) / scale)
                     + ourStartx);
            QString buffer;
            buffer.append(QString("%1").arg(cur_t));
            QStaticText gradTxt = QStaticText(buffer);
            gradTxt.setPerformanceHint(QStaticText::AggressiveCaching);
            //calculate the fading
            int fading = 3 * (cur_x - ourStartx);
            if (fading < 0)
            {
                fading = 0;
            }
            if (fading > 255)
            {
                fading = 255;
            }
            _textPen.setColor(QColor(255, 255, 255, fading));
            painter->setPen(_textPen);
            painter->drawLine(
                    QLineF(cur_x, 0, cur_x, HEIGHT_TIME_AXIS - MARGIN));
            if (widthAvailable > 500)
            {
                painter->drawStaticText(
                        QPointF(cur_x,
                                HEIGHT_TIME_AXIS - MARGIN - f.pixelSize()),
                        gradTxt);
            }
            for (int k = 1; k < GRAD_NUM; k++)
            { // k=1 ,because of spans
                painter->drawLine(
                        QLineF(cur_x + step_x * k, 0, cur_x + step_x * k,
                               HEIGHT_TIME_AXIS - MARGIN * 3));
            }
        }
        cur_t -= timeInterval;
    }

    //in the future
    cur_t = first + timeInterval;
    while (cur_t < _timePresent + _timeAfter)
    {
        qreal cur_x = ((qreal)((_timeBefore - (_timePresent - cur_t)) / scale)
                       + ourStartx);
        QString buffer;
        buffer.append(QString("%1").arg(cur_t));
        QStaticText gradTxt = QStaticText(buffer);
        gradTxt.setPerformanceHint(QStaticText::AggressiveCaching);
        //calculate the fading
        int fading = 3 * (size().width() - cur_x);
        if (fading > 255)
        {
            fading = 255;
        }
        _textPen.setColor(QColor(255, 255, 255, fading));
        painter->setPen(_textPen);
        painter->drawLine(QLineF(cur_x, 0, cur_x, HEIGHT_TIME_AXIS - MARGIN));
        if (widthAvailable > 500)
        {
            painter->drawStaticText(
                    QPointF(cur_x, HEIGHT_TIME_AXIS - MARGIN - f.pixelSize()),
                    gradTxt);
        }
        for (int k = 1; k < GRAD_NUM; k++)
        {
            painter->drawLine(
                    QLineF(cur_x + step_x * k, 0, cur_x + step_x * k,
                           HEIGHT_TIME_AXIS - MARGIN * 3));
        }
        cur_t += timeInterval;
    }
    _textPen.setColor(QColor(255, 255, 255));
    painter->setPen(_textPen);
}

//calculate un lapse of time to get at least 20 gradations if 1000 <= timeTotal
//and <=24 * 10^X
int GanttWidget::computeTimeInterval(int timeTotal)
{
    int coef = 1;
    while (timeTotal >= SIGNIF_NUM)
    {
        timeTotal = timeTotal / 10;
        coef *= 10;
    }
    //we have a number between 1 and 99
    if (timeTotal < FIRST_THRESHOLD)
    {
        return coef;
    }
    else if (timeTotal < SECOND_THRESHOLD)
    {
        return coef * 5;
    }
    else
    {
        return coef * 10;
    }
}

void GanttWidget::drawProgram(QPainter *painter)
{
    QFont f;
    QPen penRect;
    QPen penText;
    penRect.setColor(GANTT_PU_COLOR);
    penText.setColor(GANTT_TEXT_COLOR);
    f.setFamily("utf8");
    f.setWeight(QFont::Black);
    f.setPixelSize(10);
    painter->setFont(f);
    painter->setPen(penRect);

    _rectProg = QRectF(0, HEIGHT_TIME_AXIS + MARGIN, WIDTH_PROGRAM,
                       size().height() - HEIGHT_TIME_AXIS - MARGIN * 2);
    painter->drawRect(_rectProg);
    painter->fillRect(_rectProg, Qt::blue);
    QPointF coordTxt = QPointF(
            _rectProg.center().rx() - (WIDTH_PROGRAM / 2) + 3,
            _rectProg.center().ry() - f.pixelSize());
    QString txt = QString("Program");
    QStaticText name = QStaticText(txt);
    name.setPerformanceHint(QStaticText::AggressiveCaching);
    painter->setPen(penText);
    painter->drawStaticText(coordTxt, name);
}

void GanttWidget::drawPresentLine(QPainter *painter)
{
    QPen pen;
    pen.setColor(GANTT_PRESENTLINE_COLOR);
    painter->setPen(pen);
    qreal widthAllowed = (qreal)(
            size().width() - WIDTH_PROGRAM - MARGIN - WIDTH_GLOBAL_PU - MARGIN
            - WIDTH_PU - MARGIN);
    int timeTotal = _timeBefore + _timeAfter;
    int x, y, w, h;
    x = WIDTH_PROGRAM + MARGIN + WIDTH_GLOBAL_PU + MARGIN + WIDTH_PU + MARGIN;
    y = (HEIGHT_TIME_AXIS + MARGIN);
    w = WIDTH_PRESENT_LINE;
    h = size().height() - (HEIGHT_TIME_AXIS + MARGIN);
    qreal scale = (qreal)(widthAllowed / timeTotal);
    qreal gap_x = (qreal)(scale * _timeBefore);

    _linePresent = QRectF(x + gap_x, y, w, h);
    _coordxPresentLine = (qreal)(x + gap_x);
    painter->drawRect(_linePresent);
    painter->fillRect(_linePresent, Qt::black);
}

/* Draw whole gantt diagram depending on time showing line present location.
 Useful to draw the diagram after the complete execution. */
void GanttWidget::drawFromTime(QPainter *painter, qlonglong timestamp)
{
    Q_ASSERT_X(timestamp >= 0, "GanttWidget::drawFromTime",
               "Negative timestamp !");

    /* program rect */
    drawProgram(painter);

    _timePresent = timestamp;

    /* time line */
    drawTime(painter);

    drawPUs(painter);

    /* Tasks */
    drawIdlePU(painter);

    qlonglong borneBefore = _timePresent - _timeBefore;
    if (borneBefore < 0)
    {
        borneBefore = 0;
    }
    _tasks = _taskManager->tasks(borneBefore, _timePresent);
    foreach(starpu_top_task t, _tasks)
    {
	drawWorkPU(painter,t);
    }
}

/* draw idle time for each processor */
void GanttWidget::drawIdlePU(QPainter *painter)
{
    int starty = HEIGHT_TIME_AXIS + MARGIN;
    int widthAllowed = size().width() - WIDTH_PROGRAM - MARGIN
                       - WIDTH_GLOBAL_PU - MARGIN - WIDTH_PU - MARGIN;
    int timeTotal = _timeBefore + _timeAfter;
    qreal posBlockx = 0;
    qreal placeTime = 0;
    bool isStartx = false;
    qreal scale = (qreal) widthAllowed / timeTotal;

    QRectF rectIdle;

    qlonglong timestampS = 0;
    if (_timePresent >= _timeBefore)
    {
        timestampS = _timePresent - _timeBefore;
    }

    /* drawIdlePu */
    for (qlonglong j = timestampS; j < _timePresent; j++)
    {
        if (!isStartx)
        {
            isStartx = true;
            posBlockx = coordxForTime(j);
        }
        placeTime++;
    }

    QFont f;
    QPen pen;

    f.setFamily("utf8");
    f.setWeight(QFont::DemiBold);
    f.setPixelSize(10);
    painter->setFont(f);
    pen.setColor(GANTT_IDLE_COLOR.lighter(110));
    painter->setPen(pen);

    for (int i = 0; i < _numPUs; i++)
    {
        rectIdle = QRectF(posBlockx, starty + (_heightPU) * i,
                          placeTime * scale, (_heightPU - MARGIN));
        painter->drawRect(rectIdle);
        painter->fillRect(rectIdle, GANTT_IDLE_COLOR);
    }
}

/* draw forecasted working time for each processor */
void GanttWidget::drawPrevWorkPU(QPainter *painter, starpu_top_task t)
{
    int starty = HEIGHT_TIME_AXIS + MARGIN;
    int widthAllowed = size().width() - WIDTH_PROGRAM - MARGIN
                       - WIDTH_GLOBAL_PU - MARGIN - WIDTH_PU - MARGIN;
    qreal posBlockx = 0;
    qreal placeTime = 0;
    bool isStartx = false;
    qreal scale = (qreal) widthAllowed / _timeTotal;
    QRectF rectForecast;

    //either the beginning is in the Future Part
    //( timeAfter ) or either in the Past Part (timeBefore)
    qlonglong timestampS = (t.timestampStart - _timePresent);
    if (timestampS > 0)
    {
        // the beginning is in the Future part (displayable here)
        timestampS = t.timestampStart;
    }
    else
    {
        // the beginning is in the Past part (not displayable here)
        timestampS = _timePresent;
        // +1 just to be sure we don't draw over the line of Present
        posBlockx = _coordxPresentLine + WIDTH_PRESENT_LINE + 1;
        isStartx = true;
        placeTime -= WIDTH_PRESENT_LINE + 1;
    }

    qlonglong timestampE = (t.timestampEnd - _timePresent);
    if (timestampE > 0)
    {
        if (timestampE <= _timeAfter)
        {
            timestampE = t.timestampEnd; // the end is displayable
        }
        else
        {
            // the end is not displayable because it's in the future
            // more than timePresent + timeAfter
            timestampE = _timePresent + _timeAfter;
        }
    }
    else
    {
        // the end is not displayable because it's in the past
        timestampE = _timePresent;
        posBlockx = _coordxPresentLine + WIDTH_PRESENT_LINE + 1;
        isStartx = true;
        placeTime -= WIDTH_PRESENT_LINE + 1;
    }

    /* Future */
    for (qlonglong i = timestampS; i <= timestampE; i++)
    {
        if (!isStartx)
        {
            isStartx = true;
            posBlockx = coordxForTime(i);
        }
        placeTime++;
    }

    int pos = _PUsByDevice[t.deviceId].id;
    QFont f;
    QPen pen;
    pen.setColor(GANTT_TEXT_COLOR);
    painter->setPen(pen);
    f.setFamily("utf8");
    f.setWeight(QFont::DemiBold);
    f.setPixelSize(10);
    painter->setFont(f);

    QLinearGradient gradient(posBlockx,
                             starty + (_heightPU) * pos + (_heightPU - MARGIN) / 2,
                             posBlockx + placeTime * scale,
                             starty + (_heightPU) * pos + (_heightPU - MARGIN) / 2);
    QColor prevColor = GANTT_PREV_COLOR;
    gradient.setColorAt(0.2, prevColor.lighter(110));
    gradient.setColorAt(0.3, Qt::yellow);
    gradient.setColorAt(0.80, prevColor.darker(270));

    QBrush brush(gradient);
    pen.setBrush(brush);
    painter->setPen(pen);
    rectForecast = QRectF(posBlockx, starty + (_heightPU) * pos,
                          placeTime * scale, (_heightPU - MARGIN));
    painter->drawRect(rectForecast);
    painter->fillRect(rectForecast, brush);
}

/* Draw working time for each processor. Working PUs.
 We don't mind about task values because the taskmanager gives us only the tasks
 between timeBefore and timePresent so
 we haven't to test if they are displayable or not. We just have to calculate
 which part of time is displayable.
 The task t has its begin or its end between time Before and timePresent */
void GanttWidget::drawWorkPU(QPainter *painter, starpu_top_task t)
{
    int starty = HEIGHT_TIME_AXIS + MARGIN;
    int widthAllowed = size().width() - WIDTH_PROGRAM - MARGIN
                       - WIDTH_GLOBAL_PU - MARGIN - WIDTH_PU - MARGIN;
    qreal posBlockx = 0;
    qreal placeTime = 0;
    bool isStartx = false;
    qreal scale = (qreal) widthAllowed / _timeTotal;
    QRectF rectWorking;

    qlonglong timestampS = (_timePresent - t.timestampStart);
    qlonglong timestampE = -1;
    if (timestampS > _timeBefore)
    {
        // Begin time of task t is not displayable
        timestampS = _timePresent - _timeBefore;
    }
    else
    {
        timestampS = t.timestampStart; // Begin time of task t is displayable
    }
    if (t.timestampEnd == -1)
    {
        for (qlonglong i = timestampS; i <= _timePresent; i++)
        {
            if (!isStartx)
            {
                isStartx = true;
                posBlockx = coordxForTime(i);
            }
            placeTime++;
        }
    }
    else
    {
        timestampE = t.timestampEnd;
        if (timestampE > _timePresent)
        {
            timestampE = _timePresent;
        }
        for (qlonglong i = timestampS; i <= timestampE; i++)
        {
            if (!isStartx)
            {
                isStartx = true;
                posBlockx = coordxForTime(i);
            }
            placeTime++;
        }
    }

    int pos = _PUsByDevice[t.deviceId].id;

    QFont f;
    QPen pen;

    f.setFamily("utf8");
    f.setWeight(QFont::DemiBold);
    f.setPixelSize(10);
    painter->setFont(f);

    rectWorking = QRectF(posBlockx, starty + (_heightPU) * pos,
                         placeTime * scale, (_heightPU - MARGIN));
    painter->drawRect(rectWorking);

    QLinearGradient gradient(posBlockx,
                             starty + (_heightPU) * pos + (_heightPU - MARGIN) / 2,
                             posBlockx + placeTime * scale,
                             starty + (_heightPU) * pos + (_heightPU - MARGIN) / 2);
    QColor workingColor = GANTT_WORKING_COLOR;
    gradient.setColorAt(0.2, workingColor.lighter(110));
    gradient.setColorAt(0.3, Qt::green);
    gradient.setColorAt(0.8, workingColor.darker(270));
    gradient.setSpread(QGradient::PadSpread);

    QBrush brush(gradient);
    pen.setBrush(brush);
    painter->setPen(pen);
    painter->fillRect(rectWorking, brush);
}

void GanttWidget::updateZoom(double value)
{
    qDebug() << "VALEUR === " << value;
    //   if(!(value == 0.0))
    // {
    _timeTotal = value;
    updatePrevLine( _lastValPrevLine);
    //}
}

void GanttWidget::updatePrevLine(double value)
{
    _timeBefore = _timeTotal * (value / 100);
    _timeAfter = _timeTotal * ((100 - value) / 100);
    _lastValPrevLine = value;
    if (!(_timer->isActive()))
    {
        update();
    }
}

void GanttWidget::updateTimeView(int time)
{
    qlonglong newTimeToShow = time;
    if (!(_timer->isActive()))
    {
        _timeToShow = newTimeToShow;
        update();
    }
}

void GanttWidget::connected()
{
    _wasRunning = false;
    _initCompleted = false;
    _timePresent = 0;
    if (!(_timer->isActive()))
    {
        _timer->start(35);
    }
}

void GanttWidget::disconnected()
{
    _wasRunning = true;
    if (_timer->isActive())
    {
        _timer->stop();
    }
}

void GanttWidget::ganttLocked()
{
    update(); // a last drawing update before to freeze the drawing
    if (_timer->isActive())
    {
        _timer->stop(); // freeze the drawing
    }
}

void GanttWidget::ganttUnlocked()
{
    if (!(_timer->isActive()))
    {
        _timer->start(35); // start the timer again
    }
}

void GanttWidget::countPUs()
{
    int length;
    _numCPUs = 0;
    _numGPUs = 0;

    length = _mainWindow->serverDevices()->length();
    _numPUs = length;
    delete _PUsByDevice;
    delete _PUsByPos;
    _PUsByDevice = new starpu_top_device[length];
    _PUsByPos = new starpu_top_device[length];
    int pos = 0;

    /* CPUs */
    foreach(starpu_top_device sD,*_mainWindow->serverDevices())
    {
	if(sD.type == 0)
	{
            _PUsByDevice[sD.id].name = sD.name;
            _PUsByDevice[sD.id].type = sD.type;
            _PUsByDevice[sD.id].id = pos; // actual location

            _PUsByPos[pos].id = sD.id; // the reak id
            _PUsByPos[pos].name = sD.name;
            _PUsByPos[pos].type = sD.type;

            _numCPUs++;
            pos++;
	}
    }

    /* GPUs */
    foreach (starpu_top_device sD , *_mainWindow->serverDevices())
    {
	if(sD.type == 1 || sD.type == 2)
	{
            _PUsByDevice[sD.id].name = sD.name;
            _PUsByDevice[sD.id].type = sD.type;
            _PUsByDevice[sD.id].id = pos; // actually location

            _PUsByPos[pos].id = sD.id; // the real id
            _PUsByPos[pos].name = sD.name;
            _PUsByPos[pos].type = sD.type;

            pos++;
            _numGPUs++;
	}
    }
}

void GanttWidget::paint(QPainter *painter, QPaintEvent *event)
{
    painter->fillRect(event->rect(), _background);
    painter->setPen(_textPen);

    if (_mainWindow->isReady())
    {
        if (!_initCompleted)
        { // do not call countPUs more than once per execution
            countPUs();
            _initCompleted = true;
        }
        drawProgram(painter);
        drawPUs(painter);

        if (_mainWindow->isRunning())
        {
            _timePresent = _mainWindow->effectiveRunningTime();
            _timeToShow = _timePresent; // saved the time of end
            drawTime(painter);
            drawPresentLine(painter);
            drawIdlePU(painter);

            /* Past part */
            qlonglong borneBefore = _timePresent - _timeBefore;
            if (borneBefore < 0)
            {
                borneBefore = 0;
            }

            _tasks = _taskManager->tasks(borneBefore, _timePresent);
            foreach (starpu_top_task t, _tasks)
            {
                drawWorkPU(painter,t);
            }

            /* Future past */
            qlonglong borneAfter = _timePresent + _timeAfter;
            _tasks = _taskManager->prevTasks(_timePresent, borneAfter);
            foreach		(starpu_top_task t, _tasks)
            {
                drawPrevWorkPU(painter,t);
            }
	}
	else if (!_wasRunning)
	{
            defaultScreen(painter);
	}
    }
    else
    {
	if (_wasRunning)
	{
            // display wanted part of execution (only if it's finished)
            drawFromTime(painter,_timeToShow);
	}
    }
}
