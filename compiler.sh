#!/bin/sh
program = $1
make -f makefile

if($1)
then ./compiler $program
else ./compiler
fi
