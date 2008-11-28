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

#include "rhythm-display-amps.h"

RhythmDisplayAmps::RhythmDisplayAmps()
{
	reset();
}

RhythmDisplayAmps::~RhythmDisplayAmps()
{}

void RhythmDisplayAmps::setCutoff(real cutoff, int bounds)
{
	cutoff_ = cutoff;
	bounds_ = bounds;
}



void RhythmDisplayAmps::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	// constants
	double hScale = width();
	if (data_.size() > 0)
		hScale = hScale / data_.size();
	double vScale = height();

	int x,y;

	// plot name
	painter.drawText( 4, 14, "Amplitudes");

	// cutoff + bounds
	if (cutoff_ != 0)
	{
		painter.setPen(QPen(Qt::blue, 0, Qt::DotLine));
		x = bounds_*hScale;
		y = height() - cutoff_*vScale;
		painter.drawLine(x, y, x, height());
		painter.drawLine(x, y, width()-x, y);
		painter.drawLine(width()-x, y, width()-x, height());
	}

	painter.setPen(QPen(Qt::black));
	// iterates over the data_
	for (int i=0; i<data_.size(); i++)
	{
		x = i * hScale;
		y = data_[i] * vScale;
		painter.drawLine(x, height()-y, x, height()-1);
	}

	// draws the detected lines
	for (int i=0; i<extraData_.size(); i++)
	{
		x = extraData_[i] * hScale;
		painter.fillRect(x-1, 0, 3, height()/3,
		                 Qt::red);
	}
}

