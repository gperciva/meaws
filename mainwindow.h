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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// gui
#include "ui_mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>

// Meaws stuff
#include "user.h"
#include "game.h"
#include "metro.h"
#include "abstract-exercise.h"
#include "audio-backend.h"

#include "defs.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
	MainWindow();
	~MainWindow();
	// grade a previously recorded file, for testing
	void grade(const char *exercise, const char *audioFile);

protected:
	void closeEvent(QCloseEvent *event);

public slots:
	void updateMain(MainWindowStates state);
	void setupBackend();
	void analyze();
	void setAttempt(bool);

private slots:
	void on_action_New_User_triggered();
	void on_action_Open_User_triggered();
	void on_action_Save_User_triggered();
	void on_action_Save_User_As_triggered();
	void on_action_User_Info_triggered();
	void on_action_Close_User_triggered();
	void on_action_Quit_Meaws_triggered();

	void on_action_Open_Game_triggered();
	void on_action_Open_Single_Exercise_triggered();
	void on_action_Close_Exercise_triggered();
	void on_action_Next_Exercise_triggered();
	void on_action_Set_Tempo_triggered();

	void on_action_Toggle_Try_triggered();
	void on_action_Previous_Try_triggered();
	void on_action_Next_Try_triggered();
	void on_action_Add_Try_triggered();
	void on_action_Reset_Try_triggered();
	void on_action_Delete_Try_triggered();
	void on_action_Open_Try_triggered();


	void on_action_About_Meaws_triggered();
	void on_action_About_Marsyas_triggered();

private:
	// gui stuff
	void createUi();
	void displayMessages();
	QActionGroup *userExtraActions;
	QActionGroup *exerciseExtraActions;
	QActionGroup *tryExtraActions;
	QLabel *statusMessage;
	QLabel *tempoDisplay;

	void readSettings();
	void writeSettings();

	// meaws stuff
	User *user_;
	Metro *metro_;
	AudioBackend *audio_;
	Game *game_;

	AbstractExercise *exercise_;


	// internal logic
	bool openGame(bool multipleExercises);
	bool nextExercise();
	bool setupExercise();
	void toggleRunning();
	void setTempo(int tempo);

	bool attemptRunningBool;
};

#endif
