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

#include "intonation-display-bars.h"
#include <iostream>
using namespace std;

IntonationDisplayBars::IntonationDisplayBars(QWidget *parent)
{
	setParent(parent);
}

IntonationDisplayBars::~IntonationDisplayBars()
{}

void IntonationDisplayBars::reset()
{}


// format of errors:
// [ absolute intonation mistake, error direction]
void
IntonationDisplayBars::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
        // constants
        double hScale = width() - hOffset_;
        if (data_.size() > 0)
                hScale = hScale / (data_.size()/2);
        double vScale = height();

	int midY = height()/2;

	// dotted center line
	painter.setPen(QPen(Qt::SolidPattern, 1, Qt::DashLine));
	painter.drawLine(0, midY, width(), midY);


	// fill in the bars
	for (int i=0; i<data_.size()/2; i++)
	{
		double errorMagnitude = data_[2*i+0];
		int errorDirection = data_[2*i+1];

		int start = i*hScale+hOffset_;
		int end = (i+1)*hScale+hOffset_;
		int y = (errorMagnitude*vScale);

		int colorR = 0;
		int colorG = 0;
		int colorB = 0;

		errorMagnitude *= 255;

		if (errorDirection == 1)
		{
			colorR = 255-errorMagnitude;
			colorB = 255-errorMagnitude;
		}
		if (errorDirection == 0)
			colorG = 255-errorMagnitude;
		if (errorDirection == -1)
			colorB = 255-errorMagnitude;
		painter.setPen(Qt::NoPen);
		painter.setBrush(QColor(colorR,colorG,colorB));

		//cout<<errorMagnitude<<"\t"<<start<<"\t"<<end<<endl;
		if (errorDirection == 0)
		{
			painter.drawRect(start,midY-y,end-start,y*2);
		}
		else
		{
			y = -y*errorDirection;
			painter.drawRect(start,midY,end-start,y);
		}


		// draws the vertical lines
		painter.setPen(QColor(Qt::black));
		painter.drawLine(start, 0, start, height()-1);
	}
}

