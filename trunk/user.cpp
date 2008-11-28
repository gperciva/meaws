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

#include "user.h"

// Framework
User::User()
{
	reset();
}

User::~User()
{}

bool User::close()
{
	if (maybeSave())
	{
		reset();
		return true;
	}
	return false;
}

bool User::maybeSave()
{
	if (isModified)
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(0, tr("Meaws"),
		        tr("This user has unsaved data.\n"
		           "Do you want to save your changes?"),
		        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

void User::reset()
{
	isModified = false;
	info_.name = "";
	info_.filename = "";
	info_.abilityLevel = 0;
	info_.weekPractice = 0;
	info_.weekPlay = 0;
	info_.yearsPlaying = 0;
	info_.recordMetro = AUDIO;
	info_.playbackMetro = VISUAL;

	info_.recordTime = 30;
	info_.onsetCutoff = 0.05;
	info_.alignFirstBeat = false;
	info_.noteWidth = 0.4;
	info_.just = false;
}

bool User::setUserInfo()
{
	// basic config
	QDialog *userConfig = new QDialog;
	Ui::UserConfig ui;
	ui.setupUi(userConfig);
	ui.abilityLevelInput->addItems((QStringList()<<
	        "Novice" << "Beginner" << "Moderate" << "Good"
	        << "Expert" << "Fantastic"));
	ui.weekPracticeInput->addItems((QStringList()<<
	        "Never" << "0-1" << "2-4" << "5-9" <<
	        "10-14" << "15-19" << "20+"));
	ui.weekPlayInput->addItems((QStringList()<<
	        "Never" << "0-1" << "2-4" << "5-9" <<
	        "10-14" << "15-19" << "20+"));
	ui.yearsPlayingInput->addItems((QStringList()<<
	        "0" << "1" << "2" << "3-4" << "5-6" << "7-10"
	        << "11-14" << "15-19" << "20+"));
	ui.recordingMetroInput->addItems((QStringList()<<
	        "Audio and visual"<<"Visual"<<"Intro"<<"None"));
	ui.playbackMetroInput->addItems((QStringList()<<
	        "Audio and visual"<<"Visual"<<"Intro"<<"None"));

	// load current values
	ui.usernameInput->setText(info_.name);
	ui.abilityLevelInput->setCurrentIndex(info_.abilityLevel);
	ui.weekPracticeInput->setCurrentIndex(info_.weekPractice);
	ui.weekPlayInput->setCurrentIndex(info_.weekPlay);
	ui.yearsPlayingInput->setCurrentIndex(info_.yearsPlaying);
	ui.recordingMetroInput->setCurrentIndex(info_.recordMetro);
	ui.playbackMetroInput->setCurrentIndex(info_.playbackMetro);

	ui.recordTimeInput->setValue(info_.recordTime);
	ui.onsetCutoffInput->setValue(info_.onsetCutoff);
	ui.alignFirstBeatInput->setChecked(info_.alignFirstBeat);
	ui.noteWidthInput->setValue(info_.noteWidth);
	ui.justInput->setChecked(info_.just);

	// save new values
	if (userConfig->exec() == QDialog::Accepted)
	{
		info_.name = ui.usernameInput->text();
		info_.abilityLevel = ui.abilityLevelInput->currentIndex();
		info_.weekPractice = ui.weekPracticeInput->currentIndex();
		info_.weekPlay = ui.weekPlayInput->currentIndex();
		info_.yearsPlaying = ui.yearsPlayingInput->currentIndex();

		info_.recordMetro = (MetroTypes) ui.recordingMetroInput->currentIndex();
		info_.playbackMetro = (MetroTypes) ui.playbackMetroInput->currentIndex();

		info_.recordTime = ui.recordTimeInput->value();
		info_.onsetCutoff = ui.onsetCutoffInput->value();
		info_.alignFirstBeat = ui.alignFirstBeatInput->isChecked();
		info_.noteWidth = ui.noteWidthInput->value();
		info_.just = ui.justInput->isChecked();
		return true;
	}
	return false;
}



// Basic functions
bool User::newUser()
{
	if (maybeSave())
	{
		reset();
		if (queryName())
		{
			return true;
		}
	}
	return false;
}

bool User::queryName()
{
	bool ok;
	QString text = QInputDialog::getText(0, tr("Name"),
	               tr("User name:"), QLineEdit::Normal,
	               QDir::home().dirName(), &ok);
	if (ok && !text.isEmpty())
	{
		info_.name = text;
		// isModified = true;
		return true;
	}
	return false;
}

bool User::open()
{
	if (maybeSave())
	{
		QString openFilename = QFileDialog::getOpenFileName(
		            0,tr("Open File"), QDir::homePath() );
		if (!openFilename.isEmpty())
		{
			if (openFile(openFilename))
			{
				return true;
			}
		}
	}
	return false;
}

bool User::openFile(const QString &openFilename)
{
	QFile file(openFilename);
	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(0, tr("Application"),
		        tr("Cannot read file %1:\n%2.")
		        .arg(openFilename)
		        .arg(file.errorString()));
		return false;
	}

	QTextStream in(&file);

	info_.filename = openFilename;
	in>>info_.name;
	in>>info_.abilityLevel;
	in>>info_.weekPractice;
	in>>info_.weekPlay;
	in>>info_.yearsPlaying;
	// TODO: workaround for typecasting
	int typecastread;
	in>>typecastread;
	info_.recordMetro = (MetroTypes) typecastread;
	in>>typecastread;
	info_.playbackMetro = (MetroTypes) typecastread;

	/*
		QString exerciseName;
		double score;
		while (!in.atEnd())
		{
			in>>exerciseName>>score;
			exercises_.append(exerciseName);
			scores_.append(score);
		}
	*/
	isModified = false;
	return true;
}

bool User::save()
{
	if (info_.filename.isEmpty())
	{
		return saveAs();
	}
	return saveFile(info_.filename);
}

bool User::saveAs()
{
	QString saveAsFilename = QFileDialog::getSaveFileName(
	            0,tr("Save File"), QDir::homePath() );
	if (saveAsFilename.isEmpty())
		return false;
	return saveFile(saveAsFilename);
}

bool User::saveFile(const QString &saveFilename)
{
	QFile file(saveFilename);
	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox::warning(0, tr("Application"),
		        tr("Cannot write file %1:\n%2.")
		        .arg(saveFilename)
		        .arg(file.errorString()));
		return false;
	}
	info_.filename = saveFilename;

	QTextStream out(&file);
	//QApplication::setOverrideCursor(Qt::WaitCursor);
	out << info_.name << "\n";
	out << info_.abilityLevel << "\n";
	out << info_.weekPractice << "\n";
	out << info_.weekPlay << "\n";
	out << info_.yearsPlaying << "\n";
	out << info_.recordMetro << "\n";
	out << info_.playbackMetro << "\n";

	/*
		for (int i=0; i<exercises_.size(); i++)
		{
			out<<qPrintable(exercises_.at(i))<<" ";
			out<<scores_.at(i)<<endl;
		}
	*/
	isModified = false;
	return true;
}

/*
void
User::saveExercise(std::string exerciseName, double score)
{
	QString temp(exerciseName.c_str());
	exercises_.append(temp);
	scores_.append(score);
	isModified = true;
}
*/


