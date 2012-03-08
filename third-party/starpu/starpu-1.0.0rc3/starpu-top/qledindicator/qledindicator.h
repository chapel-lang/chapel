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

#ifndef QLEDINDICATOR_H
#define QLEDINDICATOR_H

#include <QWidget>
#include <QResizeEvent>
#include <QColor>
#include <QDebug>

class QLedIndicator: public QWidget
{
	Q_PROPERTY(QColor onColor1 WRITE setOnColor1 READ getOnColor1 );Q_PROPERTY(QColor onColor2 WRITE setOnColor2 READ getOnColor2 );Q_PROPERTY(QColor offColor1 WRITE setOffColor1 READ getOffColor1 );Q_PROPERTY(QColor offColor2 WRITE setOffColor2 READ getOffColor2 );Q_OBJECT
public:
	QLedIndicator(QWidget *parent);

	void setOnColor1(QColor c)
	{
		onColor1 = c;
	}
	void setOffColor1(QColor c)
	{
		offColor1 = c;
	}
	void setOnColor2(QColor c)
	{
		onColor2 = c;
	}
	void setOffColor2(QColor c)
	{
		offColor2 = c;
	}

	QColor getOnColor1(void)
	{
		return onColor1;
	}
	QColor getOffColor1(void)
	{
		return offColor1;
	}
	QColor getOnColor2(void)
	{
		return onColor2;
	}
	QColor getOffColor2(void)
	{
		return offColor2;
	}

	void setChecked(bool checked);
	bool isChecked() const;

protected:
	virtual void paintEvent(QPaintEvent *event);

private:
	bool checked;
	static const qreal scaledSize;
	QColor onColor1, offColor1;
	QColor onColor2, offColor2;
	QPixmap ledBuffer;
};

#endif // QLEDINDICATOR_H
