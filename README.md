LittleD
=======

A relational database for embedded devices and sensors nodes.

Introduction
------------

LittleD provides a SQL frontend to manage data under the relational model.
This project targets devices of about the same capabilities as the Arduino
Mega2560. In a nutshell, the system supports the creation of tables,
the data insertion, and SELECT-FROM-WHERE syntax. Error messages are
supported but not mandatory. A simple configuration header file dictates
which features are compiled in so the database's codespace footprint
can be controlled according to application needs. All code except
that needed to interface with Arduinos is written in C.

This project is part of ongoing research being conducted at the
University of British Columbia's Okanagan Campus under the direction
of Dr. Ramon Lawrence. The work has been supported by Dr. Lawrence's
Distributed Data Lab, the Irving K. Barber School of Arts and Sciences,
the Natural Sciences and Engineering Research Council of Canada,
along with a list of people too numerous to name.

Building Instructions
---------------------

To simply build the database locally, in your shell of choice navigate
to the repository root and execute the command

	make

To build the unit tests, issue the command

	make tests

and navigate to the _bin/tests_ directory within the repository's root
directory. You may run all tests by
	
	./runalltests

Single tests are included in the same directory. The documentation
can be generated using

	make docs

and reside in the _doc/doxygen_ directory. Note that doxygen must be installed
for the documentation to be generated. Finally,

	make clean

deletes all files created at compile time.
