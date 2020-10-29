#!/bin/sh
program=$1
make -f makefile

if([ -z "$program" ])
then ./compiler $program
else ./compiler
fi
