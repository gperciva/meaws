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

#ifndef METRO_H
#define METRO_H

#include <QTimer>
#include <QPainter>
#include <QSound>
#include <QToolButton>
#include "bigmetro.h"

class Metro : public QToolButton
{
	Q_OBJECT

public:
	Metro(QString meawsDataDir);
	~Metro();
	void start();
	void stop();
	void setTempo(int tempo);


	void setAudio(bool enable)
	{
		audio = enable;
	};
	void setBeatsLeft(int getBeatsLeft)
	{
		beatsLeft = getBeatsLeft;
	};

protected:
	void paintEvent(QPaintEvent *);

private slots:
	void toggleBigMetro(bool checked);
	void beat();
	void beatFinished();

private:
	QColor normalBeatColor;
	QColor activeBeatColor;
	QColor drawBeatColor;

	// tempo
	QTimer *timer;
	int beatsLeft;

	// audio stuff
	bool audio;
	QSound *beatSound;

	// big metronome
	BigMetro *bigMetro_;
};

#endif
