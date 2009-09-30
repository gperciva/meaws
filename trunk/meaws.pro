# main stuff
SOURCES = mainwindow.cpp start.cpp
HEADERS = mainwindow.h defs.h
FORMS = mainwindow.ui
RESOURCES = icons.qrc

HEADERS += user.h
SOURCES += user.cpp
FORMS += userconfig.ui

HEADERS += choose-exercise.h game.h
SOURCES += choose-exercise.cpp game.cpp
HEADERS += metro.h bigmetro.h
SOURCES += metro.cpp bigmetro.cpp

HEADERS += audio-backend.h
SOURCES += audio-backend.cpp


# exercises
HEADERS += abstract-exercise.h abstract-try.h abstract-display.h
SOURCES += abstract-exercise.cpp abstract-try.cpp abstract-display.cpp


HEADERS += intonation-exercise.h intonation-try.h
HEADERS += intonation-display-pitches.h intonation-display-bars.h
SOURCES += intonation-exercise.cpp intonation-try.cpp
SOURCES += intonation-display-pitches.cpp intonation-display-bars.cpp

HEADERS += rhythm-exercise.h rhythm-try.h
HEADERS += rhythm-display-amps.h rhythm-display-lines.h
SOURCES += rhythm-exercise.cpp rhythm-try.cpp
SOURCES += rhythm-display-amps.cpp rhythm-display-lines.cpp



# old exercises, not working in current code
#HEADERS += exerciseShift.h exerciseControl.h
#SOURCES += exerciseShift.cpp exerciseControl.cpp

HEADERS += libs/RtAudio/RtAudio.h
SOURCES += libs/RtAudio/RtAudio.cpp

HEADERS += libs/monowav/monowav.h
SOURCES += libs/monowav/monowav.c


## app icon
macx {
  RC_FILE = icons/meaws.icns
}

TARGET = out/meaws
VERSION = 0.5
#CONFIG = qt

macx:DEFINES += __MACOSX_CORE__
unix: !macx:DEFINES += __LINUX_ALSA__
win32:DEFINES += __WINDOWS_DS__


LIBS += -laubio -lfftw3
#unix:!macx:LIBS += -static


unix:!macx:LIBS += -lasound
macx:LIBS += -framework CoreAudio -framework CoreMidi -framework CoreFoundation


# FIXME: this is necessary (?) because QFileDialog cannot look
# inside a bloody bundle.  :/
#   VERY very mad.  :/
#CONFIG -= app_bundle
MEAWS_DATADIR = out/data/
#!macx: MEAWS_DATADIR = out/data/
#macx: MEAWS_DATADIR = out/meaws.app/Contents/Resources/data/


message("Installing data to $$MEAWS_DATADIR")
data.path = $$MEAWS_DATADIR
data.files = data/sd.wav
data.files += data/game/
data.files += data/rhythm/
data.files += data/intonation/
# might as well let people create their own exercises
ata.files += data/*.py
data.files += data/*.txt
data.files += data/header.ly
INSTALLS += data

