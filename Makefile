INC = -I "C:\Program Files (x86)\Python34\include"
LIB = -L "C:\Program Files (x86)\Python34\libs"

all:
	g++ -std=c++11 -shared $(INC) $(LIB) -O3 ProcMod.cpp ProcMod.def -lpython34 -lpsapi -o ProcMod.pyd

Run:
	cmd /k python Test.py
