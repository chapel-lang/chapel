import enums_pb2

messageObj = enums_pb2.enumTest();

messageObj.a = enums_pb2.color.blue

messageObj.b = 564

messageObj.c.append(enums_pb2.color.blue)
messageObj.c.append(enums_pb2.color.green)

messageObj.d = messageObj.fruit.orange

messageObj.e.append(messageObj.fruit.orange)
messageObj.e.append(messageObj.fruit.apple)

file = open("out", "wb")
file.write(messageObj.SerializeToString())
file.close()
