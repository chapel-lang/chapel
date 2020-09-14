import messagefield_pb2

messageObj = messagefield_pb2.messageA()

file = open("out", "rb")
messageObj.ParseFromString(file.read())
file.close()

if messageObj.a.b != 150 or messageObj.a.c != "String with spaces":
    print("false")
else:
    print("true")
    
if messageObj.f[0].d != 26 or messageObj.f[0].e != True:
    print("false")
else:
    print("true")
    
if messageObj.f[1].d != 36 or messageObj.f[1].e != False:
    print("false")
else:
    print("true")

if messageObj.g.a != 76:
    print("false")
else:
    print("true")

if messageObj.h[0].a != 26 or messageObj.h[1].a != 46:
    print("false")
else:
    print("true")
