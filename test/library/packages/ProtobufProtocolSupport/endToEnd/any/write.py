import any_pb2

messageObj = any_pb2.anyTest()
obj = any_pb2.test()

messageObj.a = 123

obj.a = "chapel"
obj.b = True
messageObj.anyfield.Pack(obj)

file = open("out", "wb")
file.write(messageObj.SerializeToString())
file.close()
