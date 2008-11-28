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

#ifndef ABSTRACT_DISPLAY_H
#define ABSTRACT_DISPLAY_H

#include <QWidget>
#include <QPainter>
#include "defs.h"

class AbstractDisplay: public QWidget
{
	Q_OBJECT
public:
	AbstractDisplay(QWidget *parent = 0);
	~AbstractDisplay();
	void reset();

	void setBackgroundColor(QPalette color);
	void setHOffset(int offset);

	/// scales data.  DO AFTER setData() !
	void setHorizontal(int start, int end);

	void setData(rvec getData);
	void setExtraData(rvec extraData);

protected:
	void paintEvent(QPaintEvent *event) = 0;

	int hOffset_;
	int startOffset_, endOffset_;
	rvec data_, extraData_;
};

#endif

