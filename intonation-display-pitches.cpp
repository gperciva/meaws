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

#include "intonation-display-pitches.h"
#include <iostream>
using namespace std;

IntonationDisplayPitches::IntonationDisplayPitches()
{
	reset();
}

IntonationDisplayPitches::~IntonationDisplayPitches()
{}

void IntonationDisplayPitches::reset()
{
	minVal_ = 54;
	highVal_ = 89;
	AbstractDisplay::reset();
}

void IntonationDisplayPitches::setVertical(double minVal, double highVal)
{
	minVal_ = minVal;
	highVal_ = highVal;
}

void IntonationDisplayPitches::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	// constants
	double hScale = width();
	if (data_.size() > 0)
		hScale = hScale / data_.size();
	double vScale = height() / (highVal_ - minVal_);

	int x,y;

	// plot name
	painter.drawText( 4, 14, "pitches");

	// draws the pitches
	painter.setPen(Qt::red);
	for (int i=0; i<data_.size(); i++)
	{
		x = i * hScale;
		y = height() - (data_[i]-minVal_) * vScale;
		// don't bother bound checking, since Qt does this
		//painter.drawPoint( x, y);
		painter.drawRect(x,y,1,1);
	}

	painter.setPen(Qt::blue);
	int oldx;
	x = 0;
	for (int i=0; i<extraData_.size()/2; i++)
	{
		oldx = x;
		x = extraData_[2*i] * hScale;
		painter.drawLine(x, 0, x, height());
		y = extraData_[2*i+1];
		if (y>0)
		{
			y = height() - (y - minVal_) * vScale;
			painter.drawLine(oldx, y, x, y);

		}
	}
}

