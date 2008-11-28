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

#include "choose-exercise.h"

QString ChooseExercise::chooseExercise(QString dir)
{
	QStringList items;
	items << tr("Rhythm") << tr("Intonation");
	bool ok;
	QString item = QInputDialog::getItem(0, tr("Choose testing method"),
	               tr("Testing Method:"), items,
	               0, false, &ok);
	if (ok && !item.isEmpty())
	{
		QString exerciseDir;
		if (item=="Rhythm")
			exerciseDir = dir.append("rhythm/");
		if (item=="Intonation")
			exerciseDir = dir.append("intonation/");
		QString openFilename = QFileDialog::getOpenFileName(
		            0,tr("Open file"), exerciseDir,
		            tr("Exercises (*.png)"));
		return openFilename;
	}
	return "";
}

QString ChooseExercise::chooseGame(QString dir)
{
	QStringList items;
	items << tr("Rhythm") << tr("Intonation");
	bool ok;
	QString item = QInputDialog::getItem(0, tr("Choose testing method"),
	               tr("Testing Method:"), items,
	               0, false, &ok);
	if (ok && !item.isEmpty())
	{
		QString gameFile;
		if (item=="Rhythm")
			gameFile = dir.append("rhythms.txt");
		if (item=="Intonation")
			gameFile = dir.append("intonation.txt");
		return gameFile;
	}
	return "";
}



QString ChooseExercise::chooseTry()
{
	// FIXME: directory
	QString openFilename = QFileDialog::getOpenFileName(0,tr("Open Attempt"),
	        "/home/gperciva/data/");
	return openFilename;
}


