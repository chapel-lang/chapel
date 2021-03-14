import maps_pb2

messageObj = maps_pb2.mapTest()

file = open("out", "rb")
messageObj.ParseFromString(file.read())
file.close()

if messageObj.mapfield1[1].bo != True:
    print("false")
else:
    print("true")

if messageObj.mapfield2[1] != "chapel" or messageObj.mapfield2[2] != "protobuf":
    print("false")
else:
    print("true")
