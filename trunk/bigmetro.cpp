/*
** Copyright (C) 2008 Graham Percival <gperciva@uvic.ca>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "metro.h"

BigMetro::BigMetro()
{
	setWindowTitle("Visual metronome");
	resize(400,200);

	normalBeatColor=Qt::cyan;
	activeBeatColor=Qt::red;
	left = true;
}

BigMetro::~BigMetro()
{}

void BigMetro::closeEvent(QCloseEvent *event)
{
	emit setBigState(false);
	event->accept();
}

void BigMetro::beat()
{
	left = !left;
	update();
}

void BigMetro::paintEvent(QPaintEvent *)
{
	qDebug("big metro paint");
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QRectF area;

	// left circle
	area = QRectF(2, 2, width()/2-4, height()-4);
	if (left)
		painter.setBrush(activeBeatColor);
	else
		painter.setBrush(normalBeatColor);
	painter.drawEllipse(area);

	// right circle
	area = QRectF(2+width()/2, 2, width()/2-4, height()-4);
	if (left)
		painter.setBrush(normalBeatColor);
	else
		painter.setBrush(activeBeatColor);
	painter.drawEllipse(area);
}

