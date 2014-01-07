/**
@author		Graeme Douglas
@brief
@details
@copyright	Copyright 2013 Graeme Douglas
@license	Licensed under the Apache License, Version 2.0 (the "License");
		you may not use this file except in compliance with the License.
		You may obtain a copy of the License at
			http://www.apache.org/licenses/LICENSE-2.0

@par
		Unless required by applicable law or agreed to in writing,
		software distributed under the License is distributed on an
		"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
		either express or implied. See the License for the specific
		language governing permissions and limitations under the
		License.
*/

#include "SD.h"
#include "littled/arduinodebug.h"
#include "littled/dbparser/dbparser.h"
#include "littled/dbstorage/SD_c_iface.h"
#include "littled/dboutput/query_output.h"
#include "littled/utils/gen_bench_relations.h"

//File myFile;

int my_putc( char c, FILE *t) {
  Serial.write( c );
}

int printfilechars(char *relationname)
{
    Serial.println("PRINT FILE CONTENTS\n");
    Serial.flush();
    db_fileref_t toPrint;
    unsigned char buffer = 0;

    if (toPrint = db_openreadfile(relationname))
    {
        int readsize = 1*sizeof(char);
        while (db_fileread(toPrint, &buffer, readsize) == readsize)
        {
            Serial.println(buffer, HEX);
            //printf("0x%x : %c\n", (int)(buffer), (char)buffer);
            buffer = 0;
        }
        db_fileclose(toPrint);
        return 0;
    }
    return -1;
}

void setup()
{
	Serial.begin(9600);
	Serial.flush();
	while (!Serial) {
		;
	}
	// Use pin 10 for Uno, 53 for mega
	//pinMode(10, OUTPUT);
	pinMode(53, OUTPUT);
	if (!SD.begin(4))
	{
		Serial.println("sd init failed!");
		Serial.flush();
		return;
	}
	//fdevopen(&my_putc, 0);		// Uncomment this for printf!
	Serial.println("initialization done.");
	Serial.flush();
	
	Serial.println("BEGIN\n");
	Serial.flush();
	
	/*
	db_fileref_t tempf = db_openwritefile("t");
	unsigned char buf = 1;
	while (buf < 10)
	{
		db_filewrite(tempf, &buf, 1);
		buf++;
	}
	db_fileclose(tempf);
	printfilechars("t");
	db_fileremove("t");
	
	ad_printstr("asdfghj");
	ad_printint(10);
	ad_printint_hex(15);
	ad_printint_bin(3);
	ad_printchar('g');
	*/
	
	Serial.println("Init relation");
	Serial.flush();
	// name, numattr, numtuples, bound, seed, type.
	gen_bench_relation("r", 2, 5, 30, 2003, 1);
	//printfilechars("r");
	
	Serial.println("Init query data");
	Serial.flush();
	int size = 1000;
	unsigned char memseg[size];
	db_query_mm_t mm;
	init_query_mm(&mm, memseg, size);
	char command[] = "SELECT * FROM r;";
	Serial.println("Parse query");
	Serial.flush();
	db_op_base_t *rootp = parse(command, &mm);
	Serial.println("Init tuple");
	Serial.flush();
	db_tuple_t tuple;
	init_tuple(&tuple, rootp->header->tuple_size, rootp->header->num_attr, &mm);
	int count = 0;
	Serial.println("Execute query");
	Serial.flush();
	#if 1
	while(1==next(rootp, &tuple, &mm))
	{
		count++;
		Serial.flush();
	}
	#endif
	#if 0
	char *output = formatQuery(rootp, &mm);
	Serial.println(output);
	Serial.flush();
	free(output);
	#endif
	Serial.println(count);
	
/*
	//printQuery(rootp, &mm);
*/
	
	Serial.println("\nEND");
	Serial.flush();




/*

  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");
  }

  // open a new file and immediately close it:
  Serial.println("Creating example.txt...");
  myFile = SD.open("example.txt", FILE_WRITE);
  myFile.close();

  // Check to see if the file exists: 
  if (SD.exists("example.txt")) {
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");  
  }

  // delete the file:
  Serial.println("Removing example.txt...");
  SD.remove("example.txt");

  if (SD.exists("example.txt")){ 
    Serial.println("example.txt exists.");
  }
  else {
    Serial.println("example.txt doesn't exist.");  
  }
*/
}

void loop()
{
	/*
	while (1) {
		;
	}
	*/
}
