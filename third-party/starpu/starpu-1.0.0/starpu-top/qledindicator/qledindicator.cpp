/***************************************************************************
 *   Copyright (C) 2010 by Tn                                              *
 *   thenobody@poczta.fm                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Library General Public License as       *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this program; if not, write to the                 *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QPainter>

#include "qledindicator.h"

const qreal QLedIndicator::scaledSize = 1000;

QLedIndicator::QLedIndicator(QWidget *parent) :
	QWidget(parent)
{
	this->checked = false;
	onColor1 = QColor(0, 255, 0);
	onColor2 = QColor(0, 192, 0);
	offColor1 = QColor(255, 0, 0);
	offColor2 = QColor(192, 0, 0);
}

void QLedIndicator::paintEvent(QPaintEvent *event)
{
	qreal realSize = qMin(width() / 1.3, height() / 1.3);

	QRadialGradient gradient;
	QPainter painter(this);
	QPen pen(Qt::black);
	pen.setWidth(1);

	painter.setRenderHint(QPainter::Antialiasing);
	painter.translate(width() / 2, height() / 2);
	painter.scale(realSize / scaledSize, realSize / scaledSize);

	gradient = QRadialGradient(QPointF(-500, -500), 1500, QPointF(-500, -500));
	gradient.setColorAt(0, QColor(224, 224, 224));
	gradient.setColorAt(1, QColor(28, 28, 28));
	painter.setPen(pen);
	painter.setBrush(QBrush(gradient));
	painter.drawEllipse(QPointF(0, 0), 500, 500);

	gradient = QRadialGradient(QPointF(500, 500), 1500, QPointF(500, 500));
	gradient.setColorAt(0, QColor(224, 224, 224));
	gradient.setColorAt(1, QColor(28, 28, 28));
	painter.setPen(pen);
	painter.setBrush(QBrush(gradient));
	painter.drawEllipse(QPointF(0, 0), 450, 450);

	painter.setPen(pen);
	if (isChecked())
	{
		gradient = QRadialGradient(QPointF(-500, -500), 1500,
				QPointF(-500, -500));
		gradient.setColorAt(0, onColor1);
		gradient.setColorAt(1, onColor2);
	}
	else
	{
		gradient = QRadialGradient(QPointF(500, 500), 1500, QPointF(500, 500));
		gradient.setColorAt(0, offColor1);
		gradient.setColorAt(1, offColor2);
	}
	painter.setBrush(gradient);
	painter.drawEllipse(QPointF(0, 0), 400, 400);
}

void QLedIndicator::setChecked(bool checked)
{
	this->checked = checked;
	update();
}

bool QLedIndicator::isChecked() const
{
	return this->checked;
}
