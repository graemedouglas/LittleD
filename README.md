LittleD
=======

Hi there! You may be interested in a sister project, [IonDB - A key-value store for embedded devices](https://github.com/iondbproject/iondb).

"What is this?"
---------------

LittleD is a relational database for microprocessor devices, using as little
as 1kB of RAM for most queries. LittleD supports
SELECT-FROM-WHERE syntax, including inner joins, projections, and selections
over arbitrary expressions. LittleD also has support for CREATE TABLE and
INSERT statements. Only integers and fixed-width strings are supported
at this time. In the future, query pre-compilation, network transmission,
network relational operators, aggregation, and improved indexing
are hoped to be implemented.

If you are interested in the design of LittleD, you might want to check out
my published research: [here](http://dl.acm.org/citation.cfm?id=2554891) or
[here](https://people.ok.ubc.ca/rlawrenc/research/Papers/LittleD.pdf).

Show me some code!
------------------

Sure!
    
```c
    #include "Littled/dbparser/dbparser.h"
    
    #define BYTES_LEN 400
    
    int main(void)
    {
        char          memseg[BYTES_LEN];
        db_query_mm_t mm;
        db_op_base_t* root;
        db_tuple_t    tuple;

        init_query_mm(&mm, memseg, BYTES_LEN);
        parse("CREATE TABLE sensors (id int, temp int);", &mm);

        init_query_mm(&mm, memseg, BYTES_LEN);
        parse("INSERT INTO sensors VALUES (1, 221);", &mm);
        init_query_mm(&mm, memseg, BYTES_LEN);
        parse("INSERT INTO sensors VALUES (2, 89884);", &mm);
        init_query_mm(&mm, memseg, BYTES_LEN);
        parse("INSERT INTO sensors VALUES (3, 112);", &mm);
        init_query_mm(&mm, memseg, BYTES_LEN);
        parse("INSERT INTO sensors VALUES (4, 455);", &mm);
        init_query_mm(&mm, memseg, BYTES_LEN);
        parse("INSERT INTO sensors VALUES (5, 3313);", &mm);
        init_query_mm(&mm, memseg, BYTES_LEN);
        parse("INSERT INTO sensors VALUES (6, 11);", &mm);
        init_query_mm(&mm, memseg, BYTES_LEN);
        parse("INSERT INTO sensors VALUES (7, 99996);", &mm);
        init_query_mm(&mm, memseg, BYTES_LEN);
        parse("INSERT INTO sensors VALUES (8, 6565);", &mm);
        init_query_mm(&mm, memseg, BYTES_LEN);
        parse("INSERT INTO sensors VALUES (9, 6565);", &mm);
    
        init_query_mm(&mm, memseg, BYTES_LEN);
        root = parse("SELECT * FROM sensors;", &mm);
        if (root == NULL)
        {
            printf("NULL root\n");
        }
        else
        {
            init_tuple(&tuple, root->header->tuple_size, root->header->num_attr, &mm);

            while (next(root, &tuple, &mm) == 1) 
            {
                int id = getintbyname(&tuple, "id", root->header);
                int sensor_val = getintbyname(&tuple, "temp", root->header);;
                printf("sensor val: %i (%i)\n", sensor_val, id);
            }
        }

        return 0;
    }
```

Detailed Summary
----------------

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

Can I contribute?
-----------------

Absolutely! Contributions are welcome! Please make pull requests off the master
branch.
