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

#include "intonation-try.h"
//#include "Transcriber.h"
#include <aubio/aubio.h>

#include <iostream>
using namespace std;


#define SCALE 50.0
#define WINDOWSIZE 1024
#define HOPSIZE 512
#define FRAME_PITCH_RANGE 2.0
#define NO_PITCH -1

#define RMS_SILENCE 0.5
#define NOTE_BOUNDARY 0.5
#define RMS_TOO_CHAOTIC 0.5
IntonationTry::IntonationTry() {
	mainDisplay_ = new IntonationDisplayBars();
	layout()->addWidget(mainDisplay_);
	mainDisplay_->setHOffset(80);

	popupDisplay_ = new IntonationDisplayPitches();
	// initialize value, but this isn't used
	noteWidth_ = 1.0;
}

IntonationTry::~IntonationTry() {}

// gui framework
void IntonationTry::display(TryState state) {
	switch (state) {
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


void IntonationTry::reset() {
	AbstractTry::reset();
	pitches_.clear();
	popupDisplay_->hide();
	popupDisplay_->reset();
	mainDisplay_->reset();
	display(TRY_OK);
}


// exercise logic
void IntonationTry::setAnswer(const rvec answers) {
	exerAnswer_ = answers;
}

bool IntonationTry::setUserInfo(User *user) {
	if (noteWidth_ != user->getInfo().noteWidth) {
		noteWidth_ = user->getInfo().noteWidth;
		analyze();
	}
	if (just_ != user->getInfo().noteWidth) {
		just_ = user->getInfo().just;
		analyze();
	}

	return true;
}

// main funcs
bool IntonationTry::processAudio(AudioData *audioData) {
	// yes, this removes the final portion of audio.
	// it's ok if we lose a few milliseconds.
	int totalFrames = audioData->totalFrames / HOPSIZE -
	                  WINDOWSIZE/HOPSIZE;
	pitches_.resize( totalFrames );

	aubio_pitchdetection_t *o = new_aubio_pitchdetection(
	                                WINDOWSIZE, HOPSIZE, 1, 44100,
	                                aubio_pitch_yinfft,
	                                aubio_pitchm_midi
	                            );

	fvec_t *in = new_fvec(WINDOWSIZE, 1);
	float *pread;

	for (int i=0; i<pitches_.size(); i++) {
		double rms = 0.0;
		pread = fvec_get_channel(in,0);
		for (int j=0; j<WINDOWSIZE; j++) {
			AUDIO_TYPE *val = &audioData->buffer[HOPSIZE*i+j];
			rms += (*val) * (*val);
			pread[j] = audioData->buffer[HOPSIZE*i+j];
		}
		if (rms > RMS_SILENCE)
			pitches_[i] = aubio_pitchdetection(o,in);
		else
			pitches_[i] = 0;
	}

	del_aubio_pitchdetection(o);
	aubio_cleanup();

	popupDisplay_->setData(pitches_);
	popupDisplay_->update();
	//popupDisplay_->setVertical(2*NO_PITCH,12);

	analyze();
	hasAudio_ = true;
	return true;
}

bool IntonationTry::analyze() {
	if (pitches_.size() == 0)
		return false;

	rvec bounds = pitchSegment();

	// calc errors
	// display note boundaries and expected pitches done
	// inside here, too
/*{
	cout<<"bounds:"<<endl;
	foreach (real b, bounds)
		cout<<b<<endl;
*/
	rvec errors = calcErrors(bounds);
/*
	cout<<"errors:"<<endl;
	foreach (real e, errors)
		cout<<e<<endl;
	cout<<"***"<<endl;
*/
	mainDisplay_->setData(errors);
	mainDisplay_->update();

	real errorSum = 0.0;
	int numBad = 0;
	for (int i=0; i<errors.size()/2; i++) {
		errorSum += errors[2*i];
		if (errors[2*i] == 1)
			numBad++;
	}
	if (numBad > 1) {
		display(TRY_BAD);
		score_ = 0;
		return false;
	} else if (numBad > 0) {
		display(TRY_WARN);
	}

	score_ = 100 - errorSum*SCALE;
	if (score_ < 0)
		score_ = 0;

	return true;
}


// internal calculations

rvec IntonationTry::pitchSegment() {
	rvec bounds(50);
	bounds[0] = 0;
	int boundIndex = 1;
	int prevFrame = 0;
	double prevMedian = 0.0;
	int minFrames = noteWidth_*44100.0/HOPSIZE;
	if (minFrames < 1)
		minFrames = 1;
	for (int i=minFrames; i<pitches_.size()-minFrames; i++) {
		AreaStats stats = calcStats(i,minFrames,pitches_);
		//cout<<stats.rmsError<<endl;

		if (stats.rmsError < 0.1) {
			if ( (fabs(stats.median - prevMedian) >
			        NOTE_BOUNDARY)) {
				if (i > prevFrame + minFrames) {
					//			cout<<i<<"\t"<<stats.median<<endl;
					bounds[boundIndex] = i;
					prevFrame = i;
					prevMedian = stats.median;
					boundIndex++;
					if (boundIndex >= bounds.size())
						bounds.resize(2*bounds.size());
				} else {

//					cout<<"replace: "<<i<<endl;
					prevMedian = stats.median;
					prevFrame = i;
					bounds[boundIndex] = i;
				}

			}

		}
	}
	bounds[boundIndex] = pitches_.size();
	bounds.resize(boundIndex+1);
	return bounds;
}

AreaStats IntonationTry::calcStats(int start, int length, rvec& list) {
	AreaStats stats;
	stats.median = 0;
	stats.rmsError = 0;

	if ( length<=0 )
		return stats;
	rvec sublist = list.mid(start,length);

	// find median the lazy way
	qSort(sublist.begin(), sublist.end());
	stats.median = sublist[sublist.size()/2];

	// find rms error
	double sum = 0;
	for (int i=0; i<sublist.size(); i++) {
		real error = sublist[i] - stats.median;
		sum += error*error;
	}
	stats.rmsError = sqrt(sum) / sublist.size();
	return stats;
}

// organization of errors:
// [ magnitude, direction]
rvec IntonationTry::calcErrors(const rvec& bounds) {
	rvec errors(2*(exerAnswer_.size()-1));

	rvec expected(2*bounds.size());
	for (int i=0; i<bounds.size(); i++) {
		expected[2*i] = bounds[i];
		expected[2*i+1] = 0;
	}

	int audioNote = 0;
	for (int exerNote=0; exerNote < exerAnswer_.size()-1; exerNote++) {
		real expectedPitch = getExpectedPitch(exerAnswer_[exerNote]);
		rvec notePitches(0);
		int noteStart, noteEnd;

		// find next non-zero note.
		do {
			noteStart = bounds[audioNote];
			noteEnd = bounds[audioNote+1];
			notePitches = pitches_.mid(noteStart,
			                           noteEnd - noteStart);
//cout<<median(notePitches)<<"\t"<<exerNote<<"\t"<<noteStart<<endl;

			audioNote++;
			if (audioNote == bounds.size()) {
//			        &&(exerNote<exerAnswer_.size()-1)) {
//				cout<<audioNote<<"\t"<<exerNote<<endl;
				display(TRY_BAD);
				errors.resize(0);
//cout<<"bad end"<<endl;
				return errors;
			}
		} while (median(notePitches) == 0);
//&&(expectedPitch != 0));

		expected[2*audioNote+1] = expectedPitch;

		NoteError noteError = calcNoteError(notePitches, expectedPitch);

		// set note error
		errors[2*exerNote+0] = noteError.magnitude;

		// set note direction
		errors[2*exerNote+1] = noteError.direction;
	}
	popupDisplay_->setExtraData(expected);
	return errors;
}

NoteError IntonationTry::calcNoteError(const rvec &notePitches,
                                       real expectedPitch) {
	real magnitudeError = 0.0;
	real directionError = 0.0;

	real med = median(notePitches);
	int countFrames = 0;
	for (int i=0; i<notePitches.size(); i++) {
		if (notePitches[i] == 0)
			continue;
		if (fabs(notePitches[i] - med) > FRAME_PITCH_RANGE)
			continue;
		countFrames++;

		real deltaError = notePitches[i] -
		                  expectedPitch;

		directionError += deltaError;
		magnitudeError += deltaError*deltaError;
	}

//	// normalize for note length
	if (countFrames > 0) {
		magnitudeError /= countFrames;
		directionError /= countFrames;
	}

//	cout<<expectedPitch<<"\t"<<magnitudeError<<"\t"<<directionError;
//	cout<<"\t"<<sqrt(magnitudeError)<<endl;

	NoteError error;
	// was the error consistently in one direction?
	if (directionError < -0.1*sqrt(magnitudeError))
		error.direction = -1;
	else if (directionError > 0.1*sqrt(magnitudeError))
		error.direction = 1;
	else
		error.direction = 0;

	// clip amount of error
	if (magnitudeError > 1.0)
		magnitudeError = 1.0;
	error.magnitude = magnitudeError;

	return error;
}

double IntonationTry::getExpectedPitch(double midiPitch) {
	if (just_) {
		double pitch;
		double justRatios [12] = { 1, 16/15.0, 9/8.0,
		                           6/5.0, 5/4.0, 4/3.0, 7/5.0, 3/2.0,
		                           8/5.0, 5/3.0, 7/4.0, 15/8.0};
		int octave = (midiPitch - exerAnswer_[0]) / 12;
		int degree = (midiPitch - exerAnswer_[0]) - octave*12;
		double ratio = justRatios[degree];
		pitch = 440.0 * pow(2.0, ((exerAnswer_[0] - 69.0) / 12.0));
		pitch *= pow(2.0, octave);
		pitch *= ratio;
		pitch = 69.0 + 12.0 * (log(pitch/440.0)/log(2.0));
		return pitch;
	} else {
		return midiPitch;
	}
}

