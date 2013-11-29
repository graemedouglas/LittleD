/**
@file		mainpage.h
@brief		Doxygen mainpage.
@details
@mainpage	Manual and Documentation for @systemname
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
@todo		Create style guide.

@section	Introduction

This document will provide information for developers attempting to use
the system in a production enviroment.

@section	Features

	- SQL Parser and Execution Engine:
		- @c SELECT clause projections excluding aggregation
		- @c FROM clause scans and joins excluding @c OUTER/NATURAL
		  joins
		- @c WHERE clause filtering
		- @c NULL handling
		- Support for common types
			- Integers (size depends on target platform)
			- Fixed-length strings
		- Common operators and functions supported
			- @c AND, @c OR, @c XOR, @c !, @c ^, @c &, @c |,
			  @c +, @c -, @c *, @c /, @c %, @c ISNULL, @c LENGTH
		- Aribitrary expressions in supported clauses
	- Designed for sensor nodes and embedded devices with constrained
	  resources
		- Query execution can occur with as little as 1 kB of RAM!
		- Compile time configurable options; pick the fetures you
		  require for your application
		- Efficient stack-based per-query memory manager
		- Device and system freedom; easily portable, just write your
		  own storage layer
		- ContikiOS ready (for devices support CFS file storage layer)
*/
