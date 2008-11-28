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

#ifndef MEAWS_TRY_RHYTHM_H
#define MEAWS_TRY_RHYTHM_H

#include "defs.h"
#include "abstract-try.h"
#include "rhythm-display-lines.h"
#include "rhythm-display-amps.h"

//using namespace MarsyasQt;


class RhythmTry : public AbstractTry
{
public:
	RhythmTry();
	~RhythmTry();
	void reset();

	// main func
	bool processAudio(AudioData *audioData);

	// gui framework
	void display(TryState state);

	// exercise logic
	void setAnswer(const QVector<double> answers);
	bool setTempo(int newTempo);
	bool setUserInfo(User *user);


private:
	bool analyze();
	QVector<double> amps_;
	QVector<double> exerAnswer_;

	/*
		// gui
		RhythmDisplayLines *mainDisplay_;
		RhythmDisplayAmps *popupDisplay_;
	*/

	// user prefs
	double cutoff_;
	bool alignFirstBeat_;
	int tempo_;

	// internals calculations
	rvec findOnsets();
	int calcOffsetAndScore(const rvec &audioOnsets);
	double calcError(const rvec audioOnsets, int offset);
	double findNearest(const rvec &list, real value);

};
#endif

