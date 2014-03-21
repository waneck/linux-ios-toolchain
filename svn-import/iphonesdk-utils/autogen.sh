#!/bin/sh
aclocal
autoconf
libtoolize -c -i
autoheader
automake -a -c
