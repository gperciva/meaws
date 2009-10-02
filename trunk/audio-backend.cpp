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

#include "audio-backend.h"
#include "libs/monowav/monowav.h"

#include <stdio.h>
using namespace std;

/*   ***************************
 *   ***                     ***
 *   *** Basic program stuff ***
 *   ***                     ***
 *   ***************************
 */

AudioBackend::AudioBackend()
{
	audioData_.totalFrames = 0;
	time_ = 30.0;
	if ( adc_.getDeviceCount() < 1 )
	{
		QMessageBox::critical(NULL, "Recording error",
		        "No audio device_s found!");
		// FIXME: handle more gracefully
		exit( 1 );
	}
//	adc_.showWarnings( true );
	device_ = 0;
}

AudioBackend::~AudioBackend()
{
	reset();
}

void AudioBackend::reset()
{
	if ( audioData_.totalFrames != 0 )
	{
		free( audioData_.buffer );
		audioData_.totalFrames = 0;
		audioData_.bufferBytes = 0;
		audioData_.frameCounter = 0;
	}
	hasAudio_ = false;
}

void AudioBackend::newFile(QString filename)
{
	reset();
	filename_ = filename;
}

bool AudioBackend::loadFile(QString filename)
{
	reset();

	monowav_sound *sound;
	sound = monowav_read(filename.toAscii());
	if (sound->status != MONOWAV_OK) {
		QMessageBox::critical(NULL, "Cannot open file",
		        "Cannot open file " + filename);
		return false;
	}
	// convert to our special audio storage
	audioData_.totalFrames = sound->length;
	unsigned long totalBytes = audioData_.totalFrames * sizeof( AUDIO_TYPE );
	audioData_.buffer = (AUDIO_TYPE*) malloc( totalBytes );
	for (int i=0; i<sound->length; i++) {
		audioData_.buffer[i] = ((AUDIO_TYPE) sound->buffer[i] / (AUDIO_TYPE) SHRT_MAX);
		cout<<audioData_.buffer[i]<<endl;
	}
	free(sound->buffer);
	free(sound);

/*
	SndfileHandle infile(filename.toAscii());
	if (not infile)
	{
		QMessageBox::critical(NULL, "Cannot open file",
		        "Cannot open file " + filename);
		return false;
	}

	audioData_.totalFrames = infile.frames();

	sf_count_t totalBytes = audioData_.totalFrames * sizeof( AUDIO_TYPE );
	audioData_.buffer = 0;
	audioData_.buffer = (AUDIO_TYPE*) malloc( totalBytes );
	if ( audioData_.buffer == 0 )
	{
		QMessageBox::critical(NULL, "Memory error",
		        "Cannot allocate memory for audio buffer.");
		return false;
	}

	if (audioData_.totalFrames != infile.read(audioData_.buffer,
	        audioData_.totalFrames ))
	{
		QMessageBox::critical(NULL, "Cannot load sound file",
		        "Error reading sound file.");
		return false;
	}
*/
	normalize();
	hasAudio_ = true;
	filename_ = filename;
	return true;
}


void AudioBackend::start()
{
	if (hasAudio_)
	{
		if (filename_.endsWith(".wav"))
			QSound::play(filename_);
		else
			QMessageBox::critical(NULL, "Playback error",
				"Cannot play non-wav files");
	}
	else
	{
		unsigned int bufferFrames;
		unsigned int fs = 44100;
		bufferFrames = 512;
		RtAudio::StreamParameters iParams;
		iParams.deviceId = device_;
		iParams.nChannels = 1;
		iParams.firstChannel = 0;

		audioData_.buffer = 0;
		try
		{
			adc_.openStream( NULL, &iParams, FORMAT, fs, &bufferFrames,
			                &input, (void *)&audioData_ );
		}
		catch ( RtError& e )
		{
			QMessageBox::critical(NULL, "Audio error",
			        QString::fromStdString(e.getMessage()));
		}

		audioData_.bufferBytes = bufferFrames * sizeof( AUDIO_TYPE );
		audioData_.totalFrames = (unsigned long) (fs * time_);
		audioData_.frameCounter = 0;
		unsigned long totalBytes;
		totalBytes = audioData_.totalFrames * sizeof( AUDIO_TYPE );

		// Allocate the entire audioData_ buffer before starting stream.
		audioData_.buffer = (AUDIO_TYPE*) malloc( totalBytes );
		if ( audioData_.buffer == 0 )
		{
			QMessageBox::critical(NULL, "Memory error",
			        "Cannot allocate memory for audio buffer");
			if ( adc_.isStreamOpen() ) adc_.closeStream();
		}
		try
		{
			adc_.startStream();
		}
		catch ( RtError& e )
		{
			QMessageBox::critical(NULL, "Audio error",
			        QString::fromStdString(e.getMessage()));
			if ( adc_.isStreamOpen() )
				adc_.closeStream();
		}

		autoStop_.start(time_*1000);
	}
}

void AudioBackend::stop()
{
	autoStop_.stop();
	emit setAttempt(false);
	if (!hasAudio_)
	{
		if ( adc_.isStreamOpen() )
			adc_.closeStream();
		audioData_.totalFrames = audioData_.frameCounter;

		// convert to our special audio storage
		monowav_sound *sound = (monowav_sound*) malloc(sizeof(sound));
		sound->length = audioData_.totalFrames;
		sound->buffer = (short*) malloc(2 * sound->length);
		for (int i=0; i<sound->length; i++) {
			sound->buffer[i] = (short) ((AUDIO_TYPE) audioData_.buffer[i] * SHRT_MAX);
		}
		// write wav file
		int result = monowav_write(filename_.toAscii(), sound);
		free(sound->buffer);
		free(sound);
/*
		SndfileHandle outfile(filename_.toAscii(),
		        SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_FLOAT,
		        1, 44100);
		if (not outfile)
		{
			QMessageBox::critical(NULL, "Cannot open file",
			        "Cannot write to file " + filename_);
			// FIXME: handle more gracefully
			exit(1);
		}

		outfile.write(audioData_.buffer, audioData_.totalFrames);
*/

		normalize();
		hasAudio_ = true;
	}

}

void AudioBackend::normalize()
{
	if (audioData_.totalFrames == 0)
		return;
	double mean = 0.0;
	AUDIO_TYPE min = 1;
	AUDIO_TYPE max = -1;
	for (unsigned long i=0; i<audioData_.totalFrames; i++)
	{
		AUDIO_TYPE *val = &audioData_.buffer[i];
		// clip
		if (*val > 1.0)
			*val = 1.0;
		if (*val < -1.0)
			*val = -1.0;
		// get info for normalize
		if (*val > max)
			max = *val;
		if (*val < min)
			min = *val;
		mean += *val;
	}

	// remove DC offset
	mean = mean / audioData_.totalFrames;
	max += mean;
	min += mean;

	// find value closest to clipping
	min = -1.0*min;
	if (max < min)
		max = min;
	double amplify;
	if ((max < 1.0) && (max != 0))
		amplify = 1.0 / max;
	else
		amplify = 1.0;

	for (unsigned long i=0; i<audioData_.totalFrames; i++)
	{
		AUDIO_TYPE *val = &audioData_.buffer[i];
		*val -= mean;
		// clip
		if (*val > 1.0)
			*val = 1.0;
		if (*val < -1.0)
			*val = -1.0;
		// normalize
		*val *= amplify;
	}
}

// Interleaved buffers
int AudioBackend::input( void *outputBuffer, void *inputBuffer,
        unsigned int nBufferFrames, double streamTime, RtAudioStreamStatus
        status, void *audioData_ )
{
	(void) outputBuffer;
	(void) streamTime;
	(void) status;

	AudioData *iData = (AudioData*) audioData_;

	// Simply copy the audioData_ to our allocated buffer.
	unsigned int frames = nBufferFrames;
	if ( iData->frameCounter + nBufferFrames > iData->totalFrames )
	{
		frames = iData->totalFrames - iData->frameCounter;
		iData->bufferBytes = frames * sizeof( AUDIO_TYPE );
	}

	unsigned long offset = iData->frameCounter;
	memcpy( iData->buffer+offset, inputBuffer, iData->bufferBytes );
	iData->frameCounter += frames;

	if ( iData->frameCounter >= iData->totalFrames )
		return 2;
	return 0;
}


