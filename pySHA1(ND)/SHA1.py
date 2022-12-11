import sys
ARR = "abcdefghijklmnopqrstuvwxyz"
if sys.argv[1]:
    print(sys.argv[1])
    with open("test.txt","rb") as f:
        arr = f.read(16)
        while arr:

