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

#ifndef MEAWS_TRY_ABSTRACT_H
#define MEAWS_TRY_ABSTRACT_H

#include <QFrame>
#include <QVBoxLayout>
#include <QTemporaryFile>
#include <QMouseEvent>

#include <QVector>
#include <qalgorithms.h>
#include <math.h>
#include <aubio/aubio.h>

#include "defs.h"
#include "user.h"
#include "abstract-display.h"

enum TryState { TRY_OK, TRY_WARN, TRY_BAD };
struct MidiAnswer
{
	double pitch;
	double onset;
};

class AbstractTry : public QFrame
{
	Q_OBJECT

public:
	AbstractTry();
	~AbstractTry();
	virtual void reset();

	// main func
	virtual bool processAudio(AudioData *audioData) = 0;

	// gui framework
	void setEnabled(bool enable);
	virtual void display(TryState state) = 0;

	void mousePressEvent(QMouseEvent *event);
	void mouseDoubleClickEvent(QMouseEvent *event);

	void setTryNumber(int num)
	{
		tryNumber_ = num;
	};

	// exercise logic
	bool hasAudio()
	{
		return hasAudio_;
	};
	double getScore()
	{
		return score_;
	};
	virtual bool setTempo(int getTempo)
	{
		tempo_ = getTempo;
		return true;
	};
	virtual bool setUserInfo(User *user)
	{
		(void) user;
		return true;
	};

	// non-interactive grading
	bool setFile(QString file);


	// other
	QString getFilename()
	{
		return filename_;
	};

signals:
	void selectTry(int);

protected:
	virtual bool analyze() = 0;
	real mean(const rvec &list);
	real median(const rvec &list);
	real meanSquaredError(const rvec &list, const real answer);

	// exercise logic
	int tryNumber_;
	bool hasAudio_;
	double score_;

	// bookkeeping
	QTemporaryFile* file_;
	QString filename_;

	// gui
	AbstractDisplay *mainDisplay_;
	AbstractDisplay *popupDisplay_;

	int tempo_;
};
#endif

