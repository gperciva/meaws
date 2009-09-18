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

#include "mainwindow.h"

MainWindow::MainWindow()
{
	QString dataDir = QCoreApplication::applicationDirPath();
#ifdef Q_WS_MAC
	dataDir = dataDir.section('/', 0, -4);
	//dataDir = dataDir + "/Resources/data/";
	dataDir = dataDir + "/data/";
#else
	dataDir = dataDir + "/data/";
#endif

	user_ = new User();
	metro_ = new Metro(dataDir);
	game_ = new Game(dataDir);
	audio_ = new AudioBackend();
	connect(audio_, SIGNAL(setAttempt(bool)),
	        this, SLOT(setAttempt(bool)));

	exercise_ = NULL;

	createUi();

	readSettings();
	updateMain(NOTHING);
}

MainWindow::~MainWindow()
{
	if (exercise_ != NULL)
	{
		delete exercise_;
		exercise_ = NULL;
	}
	delete game_;
	delete metro_;
	delete audio_;
	delete user_;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (exercise_ != NULL)
	{
		if (exercise_->close())
		{
			delete exercise_;
			exercise_ = NULL;
		}
		else
			event->ignore();
	}
	if (!user_->close())
		event->ignore();
	event->accept();
	writeSettings();
}

void MainWindow::readSettings()
{
	QSettings settings("Meaws", "Meaws");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(700, 400)).toSize();
	resize(size);
	move(pos);
}

void MainWindow::writeSettings()
{
	QSettings settings("Meaws", "Meaws");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

void MainWindow::createUi()
{
	setupUi(this);
	connect(action_About_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	userExtraActions = new QActionGroup(menu_User);
	userExtraActions->addAction(action_Save_User);
	userExtraActions->addAction(action_Save_User_As);
	userExtraActions->addAction(action_User_Info);
	userExtraActions->addAction(action_Close_User);

	exerciseExtraActions = new QActionGroup(menu_Exercise);
	exerciseExtraActions->addAction(action_Close_Exercise);
	exerciseExtraActions->addAction(action_Next_Exercise);
	exerciseExtraActions->addAction(action_Set_Tempo);

	tryExtraActions = new QActionGroup(menu_Try);
	tryExtraActions->addAction(action_Previous_Try);
	tryExtraActions->addAction(action_Next_Try);
	tryExtraActions->addAction(action_Add_Try);
	tryExtraActions->addAction(action_Reset_Try);
	tryExtraActions->addAction(action_Delete_Try);

	tempoDisplay = new QLabel();
	bar_Try->addWidget(metro_);
	bar_Try->addWidget(tempoDisplay);

	statusMessage = new QLabel;
	statusBar()->addPermanentWidget(statusMessage);
}



void MainWindow::displayMessages()
{
	QString title = "Meaws";
	QString addInfo;
	addInfo = user_->getInfo().name;
	if (!addInfo.isEmpty())
		title.append(QString(" - %1").arg(addInfo));
	addInfo = game_->getLevelName();
	if (!addInfo.isEmpty())
		title.append(QString(" - %1").arg(addInfo));
	setWindowTitle(title);

	if (exercise_ != NULL)
	{
		if (exercise_->hasTryAudio())
		{
			statusMessage->setText(
			    QString("Score: " +
			            QString::number(
			                exercise_->getTryScore(),'f',1)
			            + "%"));
		}
		else
		{
			statusMessage->setText("");
		}
	}
}

void MainWindow::updateMain(MainWindowStates state)
{
	switch (state)
	{
	case UPDATE:
		displayMessages();
		break;
	case NOTHING:
		userExtraActions	->setEnabled(false);

		menu_Exercise	->setEnabled(false);
		bar_Exercise	->setEnabled(false);
		exerciseExtraActions->setEnabled(false);

		menu_Try	->setEnabled(false);
		bar_Try		->setEnabled(false);

		statusBar()->showMessage(tr("Need User"));
		displayMessages();
		break;
	case USER:
		userExtraActions	->setEnabled(true);

		menu_Exercise	->setEnabled(true);
		exerciseExtraActions->setEnabled(false);
		bar_Exercise	->setEnabled(true);

		menu_Try	->setEnabled(false);
		bar_Try		->setEnabled(false);

		tempoDisplay->clear();
		statusBar()->showMessage(tr("Need Exercise"));
		displayMessages();
		break;
	case EXERCISE:
		menu_Exercise	->setEnabled(true);
		exerciseExtraActions->setEnabled(true);

		menu_Try	->setEnabled(true);
		bar_Try		->setEnabled(true);
		// shouldn't be necessary, but it is.  :(
		metro_		->setEnabled(true);
		tempoDisplay	->setEnabled(true);

		statusBar()->showMessage(tr("Ready"));
		displayMessages();
		break;
	case PAUSED:
		action_Toggle_Try->setIcon(QIcon(":/icons/player_play.png"));
		action_Toggle_Try->setStatusTip(tr("Start"));

		menu_User	->setEnabled(true);
		menu_Exercise	->setEnabled(true);
		bar_User	->setEnabled(true);
		bar_Exercise	->setEnabled(true);
		tryExtraActions	->setEnabled(true);

		statusBar()->showMessage(tr("Ready"));
		displayMessages();
		break;

	case RUNNING:
		action_Toggle_Try->setIcon(QIcon(":/icons/player_pause.png"));
		action_Toggle_Try->setStatusTip(tr("Stop"));

		menu_User	->setEnabled(false);
		bar_User	->setEnabled(false);
		menu_Exercise	->setEnabled(false);
		bar_Exercise	->setEnabled(false);
		tryExtraActions	->setEnabled(false);

		statusBar()->showMessage(tr("Running Exercise"));
		displayMessages();
		break;
	}
}




// User actions
void MainWindow::on_action_New_User_triggered()
{
	if (user_->newUser())
		updateMain(USER);
}

void MainWindow::on_action_Close_User_triggered()
{
	if (exercise_ != NULL)
		if (exercise_->close())
		{
			delete exercise_;
			exercise_ = NULL;
			updateMain(NOTHING);
			if (user_->close())
				updateMain(NOTHING);
		}
}

void MainWindow::on_action_Quit_Meaws_triggered()
{
	close();
}


void MainWindow::on_action_Open_User_triggered()
{
	if (user_->open())
		updateMain(USER);
}

void MainWindow::on_action_Save_User_triggered()
{
	user_->save();
}

void MainWindow::on_action_Save_User_As_triggered()
{
	user_->saveAs();
}

void MainWindow::on_action_User_Info_triggered()
{
	if (user_->setUserInfo())
	{
		// FIXME: doesn't work here for some reason?!?!
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		if (exercise_ != NULL)
			exercise_->setUserInfo(user_);
		updateMain(UPDATE);
		QApplication::restoreOverrideCursor();
	}
}


// Exercise actions
void MainWindow::on_action_Open_Game_triggered()
{
	if (openGame(true))
		updateMain(EXERCISE);
}

void MainWindow::on_action_Open_Single_Exercise_triggered()
{
	if (openGame(false))
		updateMain(EXERCISE);
}

void MainWindow::on_action_Close_Exercise_triggered()
{
	if (exercise_ != NULL)
	{
		delete exercise_;
		exercise_ = NULL;
	}
	game_->reset();
	updateMain(USER);
}

void MainWindow::on_action_Next_Exercise_triggered()
{
	if (nextExercise())
		updateMain(EXERCISE);
}

void MainWindow::on_action_Set_Tempo_triggered()
{
	bool ok;
	int tempo = QInputDialog::getInteger(this, tr("Set Tempo"),
	            tr("Beats Per Minute:"), 60,
	            30, 120, 1, &ok);
	if (ok)
		setTempo(tempo);
}


// Try actions
void MainWindow::on_action_Toggle_Try_triggered()
{
	toggleRunning();
}

void MainWindow::on_action_Previous_Try_triggered()
{
	if (exercise_ != NULL)
	{
		exercise_->moveTry(-1);
		setupBackend();
	}
}

void MainWindow::on_action_Next_Try_triggered()
{
	if (exercise_ != NULL)
	{
		exercise_->moveTry(1);
		setupBackend();
	}
}

void MainWindow::on_action_Add_Try_triggered()
{
	if (exercise_ != NULL)
		exercise_->addTry();
}

void MainWindow::on_action_Reset_Try_triggered()
{
	if (exercise_ != NULL)
	{
		exercise_->resetTry();
		setupBackend();
	}
}

void MainWindow::on_action_Delete_Try_triggered()
{
	if (exercise_ != NULL)
	{
		exercise_->delTry();
		setupBackend();
	}
}

void MainWindow::on_action_Open_Try_triggered()
{
	QString openFilename = QFileDialog::getOpenFileName(0,
	        tr("Open Pre-Recorded Try"), "/home/gperciva/data/");
	if (!openFilename.isEmpty())
	{
		exercise_->setTryFile(openFilename);
		audio_->loadFile(exercise_->getTryFilename());
		analyze();
	}
}


// Help actions
void MainWindow::on_action_About_Meaws_triggered()
{
	QMessageBox::about(this, tr("About Meaws"),
	                   tr("Meaws (Musician Evaulation and "
	                      "Audition for Winds and Strings) "
	                      "is a learning tool for "
	                      "musicians.  It listens to a "
	                      "musician and provides feedback on "
	                      "their performance."
	                     ));
}

void MainWindow::on_action_About_Marsyas_triggered()
{
	//QtMarAbout::aboutMarsyas(this);
}



// internal logic
bool MainWindow::openGame(bool multipleExercises)
{
	if (game_->openGame(multipleExercises))
	{
		if (exercise_ != NULL)
			delete exercise_;
		exercise_ = game_->getNextExercise(-1);
		if (exercise_ != NULL)
			return setupExercise();
		else
			QMessageBox::critical(0, "Error",
			        "Open Game failed");
	}
	return false;
}

bool MainWindow::nextExercise()
{
	if (game_->multipleExercises())
	{
		if (exercise_ != NULL)
			delete exercise_;
		exercise_ =
		    game_->getNextExercise(exercise_->getTryScore());
		if (exercise_ != NULL)
			setupExercise();
		return true;
	}
	else
	{
		return openGame(false);
	}
	return false;
}

bool MainWindow::setupExercise()
{
	connect(exercise_, SIGNAL(updateMain(MainWindowStates)),
	        this, SLOT(updateMain(MainWindowStates)));
	attemptRunningBool = false;
	exercise_->setup(centralwidget);
	exercise_->setUserInfo(user_);
	setupBackend();
	setTempo(game_->getTempo());
	return true;
}

void MainWindow::setupBackend()
{
	if ( exercise_->hasTryAudio() )
	{
		audio_->loadFile(exercise_->getTryFilename());
	}
	else
	{
		audio_->setRecordingTime(user_->getInfo().recordTime);
		audio_->newFile(exercise_->getTryFilename());
	}
}


void MainWindow::toggleRunning()
{
	setAttempt(!attemptRunningBool);
}

void MainWindow::setAttempt(bool running)
{
	if (running == attemptRunningBool)
		return;
	// change state
	attemptRunningBool = running;
	if (attemptRunningBool)
	{
		MetroTypes metroPrep;
		if (exercise_->hasTryAudio())
			metroPrep = user_->getInfo().playbackMetro;
		else
			metroPrep = user_->getInfo().recordMetro;

		if (metroPrep == AUDIO)
			metro_->setAudio(true);
		else
			metro_->setAudio(false);
		if (metroPrep == INTRO)
			metro_->setBeatsLeft(4);
		else
			metro_->setBeatsLeft(-1);

		audio_->start();
		metro_->start();
		updateMain(RUNNING);
	}
	else
	{
		audio_->stop();
		metro_->stop();
		updateMain(PAUSED);
		analyze();
	}
}

void MainWindow::analyze()
{
	if ( !exercise_->hasTryAudio() )
	{
		QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
		exercise_->setUserInfo(user_);
		if (exercise_->tryProcessAudio(audio_->getAudioData()))
		{
			updateMain(UPDATE);
		}
		QApplication::restoreOverrideCursor();
	}
}

void MainWindow::grade(const char *exercise, const char *audioFile)
{
	exercise_ = game_->openExerciseFile(QString(exercise));
	if (exercise_ == NULL)
		return;
	setupExercise();

	exercise_->setTryFile( audioFile );
	audio_->loadFile(exercise_->getTryFilename());
	analyze();

	QString print(audioFile);
	print.append("\t");
	print.append( QString().setNum(exercise_->getTryScore()) );
//	qDebug(print.toAscii());
}

void MainWindow::setTempo(int tempo)
{
	metro_->setTempo(tempo);
	exercise_->setTempo(tempo);
	tempoDisplay->setText(QString::number(tempo) + " BPM");
}

