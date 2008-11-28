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

#ifndef MEAWS_EXERCISE_ABSTRACT_H
#define MEAWS_EXERCISE_ABSTRACT_H

#include <QString>
#include <QLabel>
#include <QVector>
#include "defs.h"
#include "abstract-try.h"


class AbstractExercise : public QObject
{
	Q_OBJECT

public:
	// basic framework
	AbstractExercise();
	~AbstractExercise();
	void setup(QWidget* centralFrame);
	bool close();

	// main funcs
	virtual bool open(QString newExerciseFilename);
	virtual bool tryProcessAudio(AudioData *audioData);
	QVector<MidiAnswer> openMidiAnswers(QString filename);

	// maintain tries
	virtual void addTry() = 0;
	virtual void delTry() = 0;
	virtual void resetTry();
	void moveTry(int direction);

	// getting info
	std::string getExerciseFilename();
	double getTryScore();
	bool hasTryAudio();
	QString getTryFilename();

	// setting info
	bool setTryFile(QString filename);
	bool setTempo(int getTempo);
	bool setUserInfo(User *user);

signals:
	void updateMain(MainWindowStates);

protected slots:
	virtual void selectTry(int selected);

protected:
	// maintain tries
	void addTryAbstract(AbstractTry* newTry);
	void delTryAbstract();

	// gui framework
	QVBoxLayout *centralLayout;
	QWidget *instructionArea;
	QWidget *resultArea;
	QLayout *instructionLayout;
	QLabel *instructionImageLabel;
	QLayout *resultLayout;

	// maintain tries
	QList<AbstractTry *> *tries_;
	int currentTryNumber;
	AbstractTry* currentTry; // this only points to
	// tries[currentTryNumber_].
	// Do not delete this!

	// other
	std::string exerciseFilename;
	int tempo;
};
#endif

