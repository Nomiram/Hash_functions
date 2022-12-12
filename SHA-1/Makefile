CC=g++ -O2
SOURCES=./main.cpp ./SHA1.cpp 
EXE=./SHA1.exe
TESTFILE=./test.txt
all:
	$(CC) $(SOURCES) -o $(EXE)
test: $(TESTFILE)
	$(CC) $(SOURCES) -o $(EXE) && $(EXE) $(TESTFILE)
testfile:
	py ./generate_test.py 10485760 #generate file 10 MB
wintest: $(TESTFILE)
	$(CC) $(SOURCES) -o $(EXE)
	$(EXE) $(TESTFILE)
	@echo. 
	certutil -hashfile $(TESTFILE) SHA1
unixtest: $(TESTFILE)
	$(CC) $(SOURCES) -o $(EXE)
	$(EXE) $(TESTFILE)
	@echo ""
	openssl dgst -sha1 -hex $(TESTFILE)
$(TESTFILE):
	py ./generate_test.py 10485760 #generate file 10 MB
clean:
	rm *.exe
	rm $(TESTFILE)
help: 
	@echo make 			- just build SHA1.exe
	@echo make test		- simple test
	@echo make testfile	- generate 10 MB test file
	@echo make wintest	- simple windows test with certutil
	@echo make unixtest	- simple unix/linux test with openssl