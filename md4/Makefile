all:
	g++ ./main.cpp ./md4.cpp -o md4.exe
test:
	g++ ./main.cpp ./md4.cpp -o ./md4.exe && ./md4.exe ./test.txt
testfile:
	py ./generate_test.py 10485760 #generate file 10 MB
wintest:
	g++ ./main.cpp ./md4.cpp -o md4.exe
	./md4.exe test.txt
	@echo. 
	certutil -hashfile ./test.txt MD4
unixtest:
	g++ ./main.cpp ./md4.cpp -o md4.exe
	./md4.exe test.txt
	@echo "" 
	openssl dgst -md4 -hex test.txt
help: 
	@echo make 			- just build md4.exe
	@echo make test		- simple test
	@echo make testfile	- generate 10 MB test file
	@echo make wintest	- simple windows test with certutil
	@echo make unixtest	- simple unix/linux test with openssl
