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

#ifndef USER_H
#define USER_H

// fundamental
#include "ui_userconfig.h"
#include <QStringList>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QTextStream>


#include "defs.h"

enum MetroTypes { AUDIO, VISUAL, INTRO, NONE };

typedef struct
{
	QString name;
	QString filename;
	int abilityLevel;
	int weekPractice;
	int weekPlay;
	int yearsPlaying;
	MetroTypes recordMetro;
	MetroTypes playbackMetro;

	int recordTime;
	double onsetCutoff;
	bool alignFirstBeat;

	double noteWidth;
	bool just;
}
UserInfo;

class User: public QObject
{
	Q_OBJECT

public:
	User();
	~User();
	void reset();
	bool newUser();
	bool close();
	bool open();
	bool save();
	bool saveAs();
	bool setUserInfo();

	UserInfo getInfo() const
	{
		return info_;
	};

//public slots:
//	void saveExercise(std::string exerciseName, double score);

private:
	bool maybeSave();
	bool queryName();

	bool saveFile(const QString &saveFilename);
	bool openFile(const QString &openFilename);

	// User info
	UserInfo info_;
	bool isModified;

//	QStringList exercises_;
//	QList<double> scores_;
};
#endif

