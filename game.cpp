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

#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Game::Game(QString getMeawsDataDir)
{
	reset();
	meawsDataDir = getMeawsDataDir;
}

Game::~Game()
{}

void Game::reset()
{
	passPercent = 0;
	minPass = 0;
	currentLevel = -1;
	currentExercise = -1;
	currentPassed = 0;
	nextExercise = "";
	levels_.clear();
}

bool Game::openGame(bool multipleExercises)
{
	QString dir(meawsDataDir);
	QString filename;
	if (multipleExercises)
	{
		dir.append("game/");
		filename = ChooseExercise::chooseGame(dir);
		if (filename.isEmpty())
			return false;
		return openGameFile(filename);
	}
	else
	{
		filename = ChooseExercise::chooseExercise(dir);
		if (filename.isEmpty())
			return false;
		reset();
		nextExercise = filename.remove(0,dir.length());
		return true;
	}
}

bool Game::openGameFile(QString filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QMessageBox::critical(0, "Error",
		        "Cannot find or open game file.");
		return false;
	}
	reset();
	currentLevel = 0;

	QTextStream in(&file);
	QString line;
	line = in.readLine();
	passPercent = line.toInt();
	line = in.readLine();
	minPass = line.toInt();

	Level level;
	level.name = "";
	while (!in.atEnd())
	{
		line = in.readLine();
		if (line.isEmpty())
		{
			if (level.name != "")
				levels_.append(level);
			if (in.atEnd())
				continue;
			level.name = in.readLine();
			level.minbpm = in.readLine().toInt();
			level.maxbpm = in.readLine().toInt();
			level.filenames.clear();
		}
		else
		{
			level.filenames.append(line);
		}
	}
	return true;
}

AbstractExercise* Game::openExerciseFile(QString filename)
{
	if (filename == "")
	{
		QMessageBox::critical(0, "Error",
		        "Cannot open an empty filename.");
		if (!openGame(false))
			return NULL;
	}
	else
		nextExercise = filename;
	AbstractExercise *exercise = NULL;

	// prepare new Exercise object
	QStringList part = nextExercise.split("/");
	if (part[0]=="rhythm")
		exercise = new RhythmExercise();
	if (part[0]=="intonation")
		exercise = new IntonationExercise();

	nextExercise = "";
	if (exercise != NULL)
		if ( exercise->open(meawsDataDir + filename) )
			return exercise;
	return NULL;
}

AbstractExercise* Game::getNextExercise(double lastScore)
{
	// if it isn't a game
	if (currentLevel == -1)
	{
		return openExerciseFile(nextExercise);
	}

	// decide which level
	if (lastScore > passPercent)
	{
		currentPassed++;
	}
	if ((currentPassed >= minPass) ||
	        (currentPassed >= levels_[currentLevel].filenames.size()))
	{
		if (currentLevel > (levels_.size()-2))
		{
			QMessageBox::information(0, "You win",
			        "You have completed every level!");
			currentLevel = levels_.size()-1;
		}
		else
		{
			currentLevel++;
			currentPassed = 0;
			currentExercise = -1;
			QMessageBox::information(0, "Advance level",
			        "You have advanced to level " +
			        QString::number(currentLevel) +
			        ": " + levels_[currentLevel].name);
		}
	}

	Level level = levels_[currentLevel];
	int number;
	if (levels_[currentLevel].filenames.size() < 2)
	{
		number = 0;
	}
	else
	{
		// pick next exercise
		int chooseFrom = level.filenames.size();
		srand ( time(0) );
		do
		{
			number = rand() % chooseFrom;
		}
		while (number == currentExercise);
	}

	nextExercise = level.filenames[number];
	currentExercise = number;

	return openExerciseFile(nextExercise);
}

int Game::getTempo()
{
	if (currentLevel == -1)
	{
		return 60;
	}
	else
	{
		Level level = levels_[currentLevel];
		int range = level.maxbpm - level.minbpm;
		srand ( time(0) );
		if (range > 0)
			return rand() % range + level.minbpm;
		else
			return level.minbpm;
	}
}

QString Game::getLevelName()
{
	if (currentLevel > -1)
	{
		return levels_[currentLevel].name;
	}
	return "";
}
