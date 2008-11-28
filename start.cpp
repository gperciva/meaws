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

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{

	// causes linking troubles in MSVC
	//Q_INIT_RESOURCE(icons);

	QApplication app(argc, argv);
	MainWindow mainWin;
	if (argc < 3)
	{
		mainWin.show();
	}
	else
	{
		mainWin.grade(argv[1], argv[2]);
		return 0;
	}
	return app.exec();
}

