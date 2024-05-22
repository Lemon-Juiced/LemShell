# This file is used to compile the LemShell project.
# If additional files are added to the project, they should be added to the list of files to compile.
# Unix-based systems need the additional flag -pthread to compile the project.
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    PTHREAD = -pthread
endif
ifeq ($(UNAME_S),Darwin)
    PTHREAD = -pthread
endif

all:
	g++ lemshell.cpp -o lemshell bi_com.cpp styler.cpp $(PTHREAD)