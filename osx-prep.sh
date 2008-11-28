#!/bin/sh
DIR=/opt/local/lib/
FILES="aubio sndfile fftw z"
echo "New names"
for f in $FILES
do
	for l in $DIR/lib$f*.dylib
	do
		n=$l.bak
		mv $l $l.bak
		echo "mv $l $n"
	done
done

