CC=g++
SOURCES=./main.cpp ./SHA1.cpp 
EXE=./SHA1.exe
TESTFILE=./test.txt
all:
	$(CC) $(SOURCES) -o $(EXE)
test:
	$(CC) $(SOURCES) -o $(EXE) && $(EXE) $(TESTFILE)
testfile:
	py ./generate_test.py 10485760 #generate file 10 MB
wintest:
	$(CC) $(SOURCES) -o $(EXE)
	$(EXE) $(TESTFILE)
	@echo. 
	certutil -hashfile $(TESTFILE) SHA1
unixtest:
	$(CC) $(SOURCES) -o $(EXE)
	$(EXE) $(TESTFILE)
	@echo ""
	openssl dgst -sha1 -hex $(TESTFILE)
help: 
	@echo make 			- just build SHA1.exe
	@echo make test		- simple test
	@echo make testfile	- generate 10 MB test file
	@echo make wintest	- simple windows test with certutil
	@echo make unixtest	- simple unix/linux test with openssl