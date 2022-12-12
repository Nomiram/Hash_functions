CC=g++
SOURCES=main.cpp gost3411_94.cpp gost28147_89.cpp
EXE=./gost.exe
TESTFILE1=./test.txt
TESTFILE2=./test2.txt
all:
	$(CC) $(SOURCES) -o $(EXE)
test: $(TESTFILE1)
	@echo test from GOST
	@echo must be 0852f5623b89dd57aeb4781fe54df14eeafbc1350613763a0d770aa657ba1a47
	$(CC) $(SOURCES) -o $(EXE)
	$(EXE) $(TESTFILE2)
	@echo test 10MB
	$(CC) $(SOURCES) -o $(EXE)
	$(EXE) $(TESTFILE1)
$(TESTFILE1):
	py ./generate_test.py 10485760 #generate file 10 MB
clean:
	rm *.exe
	rm $(TESTFILE1)
help: 
	@echo make 		- just build $(EXE)
	@echo make test	- simple test
	@echo make clean	- rm all files