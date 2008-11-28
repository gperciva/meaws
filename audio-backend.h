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

#ifndef AUDIO_BACKEND_H
#define AUDIO_BACKEND_H

#include "defs.h"
#include <QObject>
#include <QTimer>
#include <QSound>
#include <QMessageBox>

#include "libs/RtAudio/RtAudio.h"

// RtAudio stuff
//typedef signed short  MY_TYPE;
//#define FORMAT RTAUDIO_SINT16
//#define SCALE  32767.0

#define FORMAT RTAUDIO_FLOAT32


class AudioBackend: public QObject
{
	Q_OBJECT

public:
	// basic framework
	AudioBackend();
	~AudioBackend();
	void reset();

	bool loadFile(QString filename);
	void newFile(QString filename);

	void setRecordingTime(int time)
	{
		time_ = time;
	}
	AudioData* getAudioData()
	{
		return &audioData_;
	};
	static int input( void *outputBuffer, void *inputBuffer,
	                  unsigned int nBufferFrames, double streamTime,
	                  RtAudioStreamStatus status, void *audioData );


public slots:
	// communication
	void start();
	void stop();

signals:
	void setAttempt(bool);

private:
	void normalize();
	RtAudio adc_;  // only used for recording

	// depending on usage, could be input OR output!
	QString filename_;

	QTimer autoStop_;
	double time_;
	unsigned int device_;
	AudioData audioData_;
	bool hasAudio_;
};
#endif

