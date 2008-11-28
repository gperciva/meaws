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

#include "abstract-exercise.h"
#include <iostream>
using namespace std;

// basic framework
AbstractExercise::AbstractExercise()
{
	tries_ = new QList<AbstractTry *>;
	currentTryNumber = -1;
	currentTry = NULL;

	centralLayout = new QVBoxLayout;
	centralLayout->setContentsMargins(1,1,1,1);

	instructionArea = new QWidget;
	instructionLayout = new QVBoxLayout;
	instructionLayout->setContentsMargins(1,1,1,1);
	instructionArea->setLayout(instructionLayout);
	centralLayout->addWidget(instructionArea);

	resultArea = new QWidget;
	resultLayout = new QVBoxLayout;
	resultLayout->setContentsMargins(1,1,1,1);
	resultLayout->setSpacing(0);
	resultArea->setLayout(resultLayout);
	centralLayout->addWidget(resultArea);

	// FIXME: temp
	resultArea->setMaximumWidth(628+2+2);
}

AbstractExercise::~AbstractExercise()
{
	while (!tries_->isEmpty())
		delete tries_->takeFirst();
	delete tries_;
	if (centralLayout != NULL)
	{
		if (instructionArea != NULL)
		{
			centralLayout->removeWidget(instructionArea);
			delete instructionArea;
			instructionArea = NULL;
		}
		if (resultArea != NULL)
		{
			centralLayout->removeWidget(resultArea);
			delete resultArea;
			resultArea = NULL;
		}
		delete centralLayout;
	}
}

void AbstractExercise::setup(QWidget* centralFrame)
{
	if (centralFrame->layout() != NULL)
		delete centralFrame->layout();
	centralFrame->setLayout(centralLayout);
	addTry();
}

bool AbstractExercise::close()
{
	return true;
}


// main funcs
bool AbstractExercise::open(QString newExerciseFilename)
{
	exerciseFilename = newExerciseFilename.toStdString();
	//exerciseFilename.remove(MEAWS_DIR).toStdString();

	instructionImageLabel = new QLabel;
	QPixmap *instructPixmap = new QPixmap(newExerciseFilename);

	instructionImageLabel->setPixmap(*instructPixmap);
	instructionImageLabel->setScaledContents(false);
	instructionImageLabel->setMaximumHeight(instructPixmap->height());
	instructionLayout->addWidget(instructionImageLabel);

	// extra height for content margins
	instructionArea->setMaximumHeight(instructPixmap->height()+2);
	return true;
}

bool AbstractExercise::tryProcessAudio(AudioData *audioData)
{
	if (currentTry != NULL)
		return currentTry->processAudio(audioData);
	return false;
}

QVector<MidiAnswer> AbstractExercise::openMidiAnswers(QString filename)
{
	QVector<MidiAnswer> answers(0);

	QFile answerFile(filename);
	// we don't actually error-check this yet.
	if (!answerFile.open(QFile::ReadOnly))
		return answers;

	// load exercise answers
	double pitch, time;
	int i=0;
	answers.resize(50);

	QTextStream answerText(&answerFile);
	while (!answerText.atEnd())
	{
		answerText>>pitch>>time;
		answers[i].pitch = pitch;
		answers[i].onset = time;
		i++;
		// double the size if we need more memory
		if (i >= answers.size())
			answers.resize(2*answers.size());
	}
	answerFile.close();
	answers.resize(i-1);

	return answers;
}


// maintain tries
void AbstractExercise::addTryAbstract(AbstractTry* newTry)
{
	resultLayout->addWidget( newTry );
	newTry->setTryNumber( tries_->count() );
	connect(newTry, SIGNAL(selectTry(int)),
	        this, SLOT(selectTry(int)));
	tries_->append(newTry);
	selectTry( tries_->count()-1 );
}

void AbstractExercise::delTryAbstract()
{
	if (currentTry == NULL)
	{
		return;
	}
	if (tries_->count() == 1)
	{
		resetTry();
	}
	else
	{
		// already set, but this removes it from tries_
		currentTry = tries_->takeAt(currentTryNumber);
		resultLayout->removeWidget( currentTry );
		delete currentTry;
		currentTry = NULL;
		resultLayout->activate();

		// renumber the remaining exercises
		for (int i=currentTryNumber; i<tries_->count(); i++)
		{
			(*tries_)[i]->setTryNumber(i);
		}
		if (currentTryNumber >= tries_->count())
			currentTryNumber = tries_->count()-1;
		selectTry(currentTryNumber);
	}
}

void AbstractExercise::resetTry()
{
	if (currentTry != NULL)
		currentTry->reset();
	selectTry(currentTryNumber);
}

void AbstractExercise::moveTry(int direction)
{
	int newTryNumber = currentTryNumber + direction;
	if ( (newTryNumber>=0) && (newTryNumber<tries_->count()))
	{
		currentTryNumber = newTryNumber;
		selectTry(currentTryNumber);
	}
}

void AbstractExercise::selectTry(int selected)
{
	if (currentTry != NULL)
		currentTry->setEnabled(false);

	currentTryNumber = selected;
	if ((currentTryNumber < 0) ||
	        (currentTryNumber > tries_->count()))
	{
		currentTry = NULL;
		return;
	}
	currentTry = (*tries_)[currentTryNumber];
	currentTry->setEnabled(true);
	emit updateMain(UPDATE);
}



// getting info
std::string AbstractExercise::getExerciseFilename()
{
	return exerciseFilename;
}

double AbstractExercise::getTryScore()
{
	if (currentTry != NULL)
		return currentTry->getScore();
	return -1;
}

QString AbstractExercise::getTryFilename()
{
	if (currentTry != NULL)
		return currentTry->getFilename();
	return "";
}

bool AbstractExercise::hasTryAudio()
{
	if (currentTry != NULL)
		return currentTry->hasAudio();
	return false;
}

bool AbstractExercise::setTryFile(QString filename)
{
	if (currentTry != NULL)
		return currentTry->setFile(filename);
	return false;
}

bool AbstractExercise::setUserInfo(User *user)
{
	if (currentTry != NULL)
		return currentTry->setUserInfo(user);
	return false;
}

bool AbstractExercise::setTempo(int getTempo)
{
	for (int i=0; i<tries_->count(); i++)
		if ( !currentTry->setTempo(getTempo) )
			return false;
	return true;
};

