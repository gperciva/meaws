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

#include "intonation-exercise.h"
#include <QFile>

IntonationExercise::IntonationExercise()
{}

IntonationExercise::~IntonationExercise()
{}

bool IntonationExercise::open(QString exerciseFilename)
{
	AbstractExercise::open(exerciseFilename);

        QString answerFilename = exerciseFilename;
        answerFilename.replace(answerFilename.size()-4, 4, ".exp");
        QVector<MidiAnswer> midiAnswer =
            AbstractExercise::openMidiAnswers(answerFilename);

        exerAnswer.resize( midiAnswer.size() );
        for (int i=0; i < exerAnswer.size(); i++)
                exerAnswer[i] = midiAnswer[i].pitch;

	return true;
}

void IntonationExercise::addTry()
{
	IntonationTry *newTry = new IntonationTry();
	newTry->setAnswer(exerAnswer);
	AbstractExercise::addTryAbstract(newTry);
}

void IntonationExercise::delTry()
{
	AbstractExercise::delTryAbstract();
}


