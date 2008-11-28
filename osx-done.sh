#!/bin/sh
DIR=/opt/local/lib/
FILES="aubio sndfile fftw z"
echo "New names"
for f in $FILES
do
	for l in $DIR/lib$f*.dylib.bak
	do
		n=`dirname $l`/`basename -s ".bak" $l`
		echo "mv $l $n"
		mv $l $n
	done
done

