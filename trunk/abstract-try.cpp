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

#include "abstract-try.h"

AbstractTry::AbstractTry()
{
	QVBoxLayout *layout = new QVBoxLayout;
	layout->setContentsMargins(0,0,0,0);
	setLayout(layout);

	tryNumber_ = 0;
	file_ = NULL;

	reset();
}

AbstractTry::~AbstractTry()
{
	if (file_ != NULL)
	{
		delete file_;
		file_ = NULL;
	}
}

void AbstractTry::reset()
{
	if (file_ != NULL)
	{
		delete file_;
		file_ = NULL;
	}
	file_ = new QTemporaryFile("meaws-XXXXXXXX.wav");
	file_->open();
	filename_ = file_->fileName();
	hasAudio_ = false;
	score_ = -1;
}

void AbstractTry::setEnabled(bool enable)
{
	if (enable)
	{
		setLineWidth(2);
		setContentsMargins(0,0,0,0);
		setFrameStyle(QFrame::Box|QFrame::Plain);
	}
	else
	{
		setFrameStyle(QFrame::NoFrame);
		setContentsMargins(2,2,2,2);
	}
}

void AbstractTry::mousePressEvent(QMouseEvent *event)
{
	emit selectTry(tryNumber_);
	event->accept();
};

void AbstractTry::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (hasAudio_)
	{
		if (popupDisplay_->isVisible())
			popupDisplay_->hide();
		else
			popupDisplay_->show();
	}
	event->accept();
}


bool AbstractTry::setFile(QString filename)
{
	if (file_ != NULL)
	{
		delete file_;
		file_ = NULL;
	}
	filename_ = filename;
	return true;
};


real AbstractTry::mean(const rvec &list)
{
	real sum = 0;
	foreach(real r, list)
	{
		sum += r;
	}
	return sum/list.size();
}

real AbstractTry::median(const rvec &list)
{
	if (list.size() == 0)
		return 0;
	rvec sort = rvec(list);
	qSort(sort.begin(), sort.end());
	return sort[sort.size()/2];
}

real AbstractTry::meanSquaredError(const rvec &list, const real answer)
{
	real sum = 0;
	foreach (real r, list)
	{
		sum += (r-answer)*(r-answer);
	}
	return sum/list.size();
}

