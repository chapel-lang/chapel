import enums_pb2

messageObj = enums_pb2.enumTest()

file = open("out", "rb")
messageObj.ParseFromString(file.read())
file.close()

if messageObj.a != enums_pb2.color.blue:
    print("false")
else:
    print("true")

if messageObj.b != 564:
    print("false")
else:
    print("true")
    
lst = [];
lst.append(enums_pb2.color.blue)
lst.append(enums_pb2.color.green)
if messageObj.c != lst:
    print("false")
else:
    print("true")
    
if messageObj.d != messageObj.fruit.orange:
    print("false")
else:
    print("true")
    
lst = [];
lst.append(messageObj.fruit.orange)
lst.append(messageObj.fruit.apple)
if messageObj.e != lst:
    print("false")
else:
    print("true")
