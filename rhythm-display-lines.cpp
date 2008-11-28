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

#include "rhythm-display-lines.h"
#include <iostream>
using namespace std;

RhythmDisplayLines::RhythmDisplayLines(QWidget *parent)
{
	setParent(parent);
	reset();
}

RhythmDisplayLines::~RhythmDisplayLines()
{}

void
RhythmDisplayLines::paintEvent(QPaintEvent *)
{
	QPainter painter(this);

	// constants
	double hScale = width() - hOffset_;
	if (endOffset_ > 0)
		hScale = hScale / (endOffset_ - startOffset_);
	double vScale = height();

	int x;

	// draws the expected lines
	// the last line is the overall size!
	for (int i=0; i<data_.size()-1; i++)
	{
		x = data_[i] * hScale + hOffset_;
		painter.fillRect(x-1, 0, 3, vScale*2/3,
		                 Qt::black);
	}

	// draws the detected lines
	for (int i=0; i<extraData_.size(); i++)
	{
		x = (extraData_[i] - startOffset_) * hScale + hOffset_;
		painter.fillRect(x-1, vScale/3, 3, vScale*1,
		                 Qt::red);
	}
}

