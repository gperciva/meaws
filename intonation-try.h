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

#ifndef MEAWS_TRY_INTONATION_H
#define MEAWS_TRY_INTONATION_H

#include "defs.h"
#include "abstract-try.h"

#include "intonation-display-bars.h"
#include "intonation-display-pitches.h"

struct AreaStats
{
	double median;
	double rmsError;
};

struct NoteError
{
	double magnitude;
	int direction;
};

class IntonationTry : public AbstractTry
{
public:
	IntonationTry();
	~IntonationTry();
	void reset();

	// main func
	bool processAudio(AudioData *audioData);

	// gui framework
	void display(TryState state);

	// exercise logic
	void setAnswer(const rvec answers);
	bool setUserInfo(User *user);


public slots:
	void doubleclicked();

private:
	bool analyze();

	rvec pitches_;
	rvec exerAnswer_;

	double noteWidth_;
	bool just_;

	rvec pitchSegment();
	AreaStats calcStats(int start, int length, rvec& list);

	rvec calcErrors(const rvec& bounds);
	NoteError calcNoteError(const rvec &notePitches, real 
          expectedPitch);

	double getExpectedPitch(double midiPitch);
};
#endif
