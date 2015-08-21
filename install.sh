#!/bin/sh

die() {
	echo "$@"
	exit 1;
}

stripslash() {
	DIRNAME=$(dirname "$1")
	BASENAME=$(basename "$1")
	[ "$DIRNAME" = "/" ] && DIRNAME=
	echo "$DIRNAME/$BASENAME"
}

docmd() {
	echo $@;
	CMD="$1"
	shift;
	"$CMD" "$@"
	return $?;
}

VERSION="1.0.0"

EXECUTABLES="set_difference set_intersection set_union"
LIB="libsetutils.so.1.0.0"
LIB_ALIAS_MAJOR="libsetutils.so.1"
LIB_ALIAS="libsetutils.so"

if [ ! -f "$LIB" ];
then
	make || die "Failed to compile. Run make."
fi

for EXECUTABLE in `echo $EXECUTABLES`;
do
	if [ ! -f "$EXECUTABLE" ];
	then
		make "$EXECUTABLE" || die "Failed to compile $EXECUTABLE"
	fi
	docmd strip "$EXECUTABLE"
done;

if [ -z "$DESTDIR" ];
then
	if [ ! -z "$PREFIX" ];
	then
		DESTDIR="$PREFIX"
	elif [ -w "/usr/bin" ];
	then
		DESTDIR="/usr"
	else
		echo "Cannot write to /usr/bin, trying home. Specify DESTDIR or PREFIX to install somewhere else." >&2
		DESTDIR="$HOME"
	fi
fi
DESTDIR="$(stripslash $DESTDIR)"
if [ ! -d "$DESTDIR"/bin ];
then
    mkdir -p "$DESTDIR/bin" || die "Failed to create $DESTDIR/bin"
fi
if [ ! -d "$DESTDIR/lib" ];
then
	mkdir -p "$DESTDIR/lib" || die "Failed to create $DESTDIR/lib"
fi

docmd install -t "$DESTDIR/bin" $EXECUTABLES || die "Failed to install executables to $DESTDIR/bin"

docmd install -t "$DESTDIR/lib" $LIB || die "Failed to install libraries to $DESTDIR/lib"

pushd $DESTDIR/bin >/dev/null 2>&1
docmd ln -sf $LIB $LIB_ALIAS_MAJOR
docmd ln -sf $LIB $LIB_ALIAS
popd >/dev/null 2>&1
echo 'Done!'
