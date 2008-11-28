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

#ifndef MEAWSGAMEH
#define MEAWSGAMEH

#include "defs.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QMessageBox>

#include "choose-exercise.h"

#include "rhythm-exercise.h"
#include "intonation-exercise.h"
//#include "exerciseControl.h"
//#include "exerciseShift.h"

typedef struct
{
	QString name;
	QStringList filenames;
	int minbpm;
	int maxbpm;
}
Level;

class Game: public QObject
{
	Q_OBJECT

public:
	Game(QString getMeawsDataDir);
	~Game();
	void reset();

	bool openGame(bool multipleExercises);
	AbstractExercise* getNextExercise(double lastScore);

	AbstractExercise* openExerciseFile(QString filename);

	int getTempo();
	QString getLevelName();

	bool multipleExercises()
	{
		return passPercent>0;
	};
private:
	bool openGameFile(QString filename);

	int passPercent;
	int minPass;
	int currentPassed;
	QList<Level> levels_;
	int currentLevel;
	int currentExercise;

	QString nextExercise;
	QString meawsDataDir;
};
#endif

