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

Metro::Metro(QString meawsDataDir)
{
//	setParent(parent);
//	setMinimumSize(40,40);
	setCheckable(true);

	connect(this,SIGNAL(clicked(bool)),this,SLOT(toggleBigMetro(bool)));

	normalBeatColor=Qt::cyan;
	activeBeatColor=Qt::red;
	drawBeatColor = normalBeatColor;

	beatsLeft = -1;

	beatSound = new QSound(meawsDataDir + "sd.wav");
	audio = false;

	bigMetro_ = new BigMetro();
	connect(bigMetro_,SIGNAL(setBigState(bool)),this,SLOT(toggleBigMetro(bool)));

	timer = new QTimer();
	connect(timer, SIGNAL(timeout()), this, SLOT(beat()));
	setTempo(60);
}

Metro::~Metro()
{
	delete bigMetro_;
}

void Metro::toggleBigMetro(bool checked)
{
	if (checked)
	{
		setChecked(true);
		bigMetro_->show();
	}
	else
	{
		setChecked(false);
		bigMetro_->hide();
	}
}

void Metro::start()
{
	beat();
	timer->start();
}

void Metro::stop()
{
	timer->stop();
}

void Metro::setTempo(int tempo)
{
	// in QT 4.2, the timer interval (in ms) must be an integer
	int timeBetweenBeats = 60000/tempo;
	timer->setInterval(timeBetweenBeats);
}


void Metro::beat()
{
	if (beatsLeft != 0)
	{
		drawBeatColor = activeBeatColor;
		// big is visible
		if (isChecked())
			bigMetro_->beat();
		if (audio)
			beatSound->play();
		update();
		QTimer::singleShot(100, this, SLOT(beatFinished()));
		beatsLeft--;
	}
	else
		stop();
}

void Metro::beatFinished()
{
	drawBeatColor = normalBeatColor;
	update();
}


void Metro::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	QRectF area(1, 1, width()-2, height()-2);
	// big is visible
	if (isChecked())
		painter.setBrush(normalBeatColor);
	else
		painter.setBrush(drawBeatColor);
	painter.drawEllipse(area);
}


