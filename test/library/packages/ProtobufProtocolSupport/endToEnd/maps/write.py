import maps_pb2

messageObj = maps_pb2.mapTest()

messageObj.mapfield1[1].bo = True

messageObj.mapfield2[1] = "chapel"
messageObj.mapfield2[2] = "protobuf"

file = open("out", "wb")
file.write(messageObj.SerializeToString())
file.close()
