import sys
ARR = "abcdefghijklmnopqrstuvwxyz"
if sys.argv[1]:
    print(sys.argv[1])
    with open("test.txt","w",encoding="utf8") as f:
        for i in range(int(sys.argv[1])):
            f.write(ARR[i%len(ARR)])
