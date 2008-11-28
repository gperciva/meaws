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

#include <iostream>
using namespace std;

#include "rhythm-try.h"
#define WINDOWSIZE 512
#define MIN_NOTE_FRAMES 9

#define SCALE 0.45
#define MAX_NOTE_ERROR 100.0

RhythmTry::RhythmTry()
{
	mainDisplay_ = new RhythmDisplayLines(this);
	mainDisplay_->setHOffset(33);
	layout()->addWidget(mainDisplay_);

	popupDisplay_ = new RhythmDisplayAmps();
	reset();
}

RhythmTry::~RhythmTry()
{
	amps_.clear();
//	delete mainDisplay_;
	delete popupDisplay_;
}

void RhythmTry::reset()
{
	AbstractTry::reset();
	amps_.clear();

	popupDisplay_->hide();
	popupDisplay_->reset();
	mainDisplay_->reset();
	tempo_ = 60;

	display(TRY_OK);
}

// gui framework
void RhythmTry::display(TryState state)
{
	switch (state)
	{
	case ( TRY_OK ):
					mainDisplay_->setBackgroundColor(Qt::white);
		break;
	case ( TRY_WARN ):
					mainDisplay_->setBackgroundColor(Qt::yellow);
		break;
	case ( TRY_BAD ):
					mainDisplay_->setBackgroundColor(Qt::red);
		break;
	}
}


// exercise logic
void RhythmTry::setAnswer(const QVector<double> answers)
{
	exerAnswer_ = answers;
	for (int i=0; i< exerAnswer_.size(); i++)
		exerAnswer_[i] = exerAnswer_[i]*44100.0/WINDOWSIZE;
}

bool RhythmTry::setTempo(int newTempo)
{
	for (int i=0; i< exerAnswer_.size(); i++)
		exerAnswer_[i] = exerAnswer_[i]*tempo_/newTempo;

	tempo_ = newTempo;
	return true;
};

bool RhythmTry::setUserInfo(User *user)
{
	if (cutoff_ != user->getInfo().onsetCutoff)
	{
		cutoff_ = user->getInfo().onsetCutoff;
		analyze();
	}
	if (alignFirstBeat_ != user->getInfo().alignFirstBeat)
	{
		alignFirstBeat_ = user->getInfo().alignFirstBeat;
		analyze();
	}
	return true;
};


// main funcs
bool RhythmTry::processAudio(AudioData *audioData)
{
	// yes, this removes the final portion of audio.
	// it's ok if we lose a few milliseconds.
	amps_.resize( audioData->totalFrames/WINDOWSIZE );

	// calculate frame audio
	double maxrms = 0;
	for (int i=0; i<amps_.size(); i++)
	{
		double rms = 0.0;
		for (int j=0; j<512; j++)
		{
			double val = audioData->buffer[512*i+j];
			rms += val*val;
		}
		rms = sqrt(rms);
		amps_[i] = rms;
		if (rms > maxrms)
			maxrms = rms;
	}

	// normalize
	if (maxrms > 0)
		for (int i=0; i<amps_.size(); i++)
			amps_[i] = amps_[i] / maxrms;


	// display everything for the double-click version
	hasAudio_ = true;
	popupDisplay_->setData(amps_);
	popupDisplay_->update();

	analyze();
	return true;
}

bool RhythmTry::analyze()
{
	if (!hasAudio_)
		return false;

	rvec onsets = findOnsets();

	// display
	popupDisplay_->setExtraData(onsets);
	int FIXME = MIN_NOTE_FRAMES;
	((RhythmDisplayAmps*) popupDisplay_)->setCutoff(cutoff_, FIXME);

	mainDisplay_->setData(exerAnswer_);
	mainDisplay_->setExtraData(onsets);

	//foreach (real o, onsets)
	//cout<<"rhythm-try: "<<o<<endl;

	int offset = calcOffsetAndScore(onsets);
	//cout<<offset<<endl;

	// display the detected onsets
	int start = -1*offset;
	if (start < 0)
		start = 0;
	int end = exerAnswer_[exerAnswer_.size()-1]-offset;

	// display the summary
	mainDisplay_->setHorizontal(start,end);
	mainDisplay_->update();

	return true;
}


// Internal calculations
rvec RhythmTry::findOnsets()
{
	rvec onsets(50);

	// find peaks
	int minSpace = MIN_NOTE_FRAMES;
	int prevValIndex = 0;
	int valIndex = 0;
	double prevValValue = 1.0;

	for (int i=minSpace; i<amps_.size()-minSpace; i++)
	{
		if ( (amps_[i] > amps_[i-1]) &&
		        (amps_[i] > amps_[i+1]) &&
		        (amps_[i] > cutoff_) )
		{
			double localMax = amps_[i];
			if (i < prevValIndex+minSpace)
			{
				if (localMax > prevValValue)
				{
					// replace previous valley
					// with this one
					onsets[valIndex-1] = i;
					prevValIndex = i;
					prevValValue = localMax;
				}
			}
			else
			{
				// new valley found
				onsets[valIndex] = i;
				valIndex++;
		// double the size if we need more memory
		if (valIndex >= onsets.size())
			onsets.resize(2*onsets.size());
				prevValIndex = i;
				prevValValue = localMax;
			}
		}
	}
	onsets.resize(valIndex);
	return onsets;
}


double RhythmTry::findNearest(const rvec &list, real value)
{
	//cout<<endl;
	//cout<<"find: "<<value<<endl;
	real bestDelta = fabs(list[0] - value);
	for (int i=1; i<list.size(); i++)
	{
		//cout<<"try: "<<list[i]<<endl;
		real delta = fabs(list[i] - value);
		if ( delta < bestDelta )
			bestDelta = delta;
	}
	//cout<<bestDelta<<endl;
	return bestDelta;
}

double RhythmTry::calcError(const rvec audioOnsets, int offset)
{
	rvec exerciseOnsets = exerAnswer_;
	exerciseOnsets.resize( exerciseOnsets.size()-1);
	double error = 0.0;
	// find best match for exerciseOnsets
	for (int i=0; i<exerciseOnsets.size(); i++)
	{
		double noteError;
		noteError = findNearest(audioOnsets,
		            exerciseOnsets[i] - offset);
		noteError = noteError * noteError;
		noteError = noteError / exerciseOnsets.size();
		noteError = min(noteError, MAX_NOTE_ERROR);
		error += noteError;
		//cout<<noteError<<" ";
	}
	//cout<<endl;
	// find best match for audioOnsets
	for (int i=0; i<audioOnsets.size(); i++)
	{
		double noteError;
		noteError = findNearest(exerciseOnsets,
		            audioOnsets[i] + offset);
		noteError = noteError * noteError;
		noteError = noteError / audioOnsets.size();
		noteError = min(noteError, MAX_NOTE_ERROR);
		error += noteError;
		//	cout<<noteError<<" ";
	}
	// find average error
	error = error * SCALE;
	return error;
}

int
RhythmTry::calcOffsetAndScore(const rvec &audioOnsets)
{
	rvec exerciseOnsets = exerAnswer_;
	exerciseOnsets.resize( exerciseOnsets.size()-1);

	/*
		// FIXME: keep on deleting initial onsets as long as it
		// increases the score.

		// discard an onset which is too early or too late
		// exerAnswers has one extra "onset" (the end of last note)
		if (onsets.getSize() > exerAnswer.getSize()-1)
			if (onsets(0) < 5)
				onsets = onsets.getSubVector(1,onsets.getSize()-1);
		if (onsets.getSize() > exerAnswer.getSize()-1)
			if (onsets(onsets.getSize()-1) > amps_.getSize()-5)
				onsets = onsets.getSubVector(0,onsets.getSize()-1);
	*/

	int diffNotes = abs(audioOnsets.size() - (exerciseOnsets.size()));
	if (diffNotes > 3)
	{
		display(TRY_BAD);
		return true;
	}
	else if (diffNotes > 0)
		display(TRY_WARN);
	else
		display(TRY_OK);


	// shift detected onsets to produce highest score.
	// last exerAnswer isn't a real onset, so don't submit it.
	int offset;

	double error;
	if (alignFirstBeat_)
	{
		offset = -audioOnsets[0];
		error = calcError(audioOnsets, offset);
	}
	else
	{
		offset = round(AbstractTry::mean(exerciseOnsets) -
		               AbstractTry::mean(audioOnsets));
		error = calcError(audioOnsets, offset);

		if (diffNotes > 0)
		{
			double testFirst =
				calcError(audioOnsets,-audioOnsets[0]);
			if (testFirst < error)
			{
				error = testFirst;
				offset = -audioOnsets[0];
			}
		}
	}

	score_ = 100 - error;

	// cap max score at 75 if there's extra/missing notes
	if (diffNotes > 0)
		if (score_ > 75)
			score_ = 75;

	if (score_ < 0)
		score_ = 0;
	return offset;
}

