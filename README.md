LittleD
=======

A relational database for embedded devices and sensors nodes.

Introduction
------------

LittleD provides a SQL frontend to manage data under the relational model.
This project targets devices of about the same capabilities as the Arduino
Mega2560. In a nutshell, the system supports the creation of tables,
data insertion, and SELECT-FROM-WHERE syntax. Error messages are
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

Eclipse Setup
-------------

LittleD can be managed using an Eclipse Makefile project.
Either the packaged C/C++ Eclipse IDE or the installable CDT package
must be used.

First make sure that the repository is cloned.
In Eclipse click **File** -> **Import...**. Within the **Select** window, choose
under **C/C++** the **Existing Code as Makefile Project** option and click
**Next**. Give your Eclipse Project a name and browse to the root of the
project repository to use the code. Choose the correct compiler for your
system and then click **Finish**.

As noted above, there are a small number of make targets that can be used
with the project's Makefile. By right clicking the project in the
**Package Explorer** and selecting **Make Targets** -> **Create...**. Leaving
all options at the their default, enter the name of the target to add
(for instance, _tests_) and then click **OK**. You can then run the Make target
by right clicking on the project in the **Project Explorer** and choosing
**Make Targets** -> **Build...**. Select the target from the list that appears
and click **Build**. Finally, you can run individual files with main methods
by opening them and click the green **Run** button at the top.
