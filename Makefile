# compiles and runs the main file. on repeated runs, compiles only the updated files
# TARGET: DEPENDENCY
#   <TAB> COMMAND
#
# automatic variables
# $@ = used for target variable
# $< = the 1st prerequisite
# $^ = all of the above prerequisites
#

CC = c++
CCFLAGS = -std=c++17 -Wall -g

all: main.cc
	${CC} -o main main.cc
	chmod 777 main


.PHONY: all clean
	# specifically specifying that these are not file name
clean:
	rm -rvf *.o *.out *.exe main

