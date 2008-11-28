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

#include "abstract-display.h"

AbstractDisplay::AbstractDisplay(QWidget *parent)
{
	setParent(parent);
	setAutoFillBackground(true);
	hOffset_ = 0;
	reset();
}

AbstractDisplay::~AbstractDisplay()
{
	data_.clear();
	extraData_.clear();
}

void AbstractDisplay::reset()
{
	startOffset_ = 0;
	endOffset_ = 0;
	data_.clear();
	extraData_.clear();
	update();
}

void AbstractDisplay::setBackgroundColor(QPalette color)
{
	setPalette(color);
}

void AbstractDisplay::setHorizontal(int start, int end)
{
	startOffset_ = start;
	endOffset_ = end;
}

void AbstractDisplay::setData(rvec getData)
{
	data_ = getData;
	setHorizontal(0, data_.size()-1);
}

void AbstractDisplay::setExtraData(rvec extraData)
{
	extraData_ = extraData;
}

void AbstractDisplay::setHOffset(int offset)
{
	hOffset_ = offset;
}

