import any_pb2

messageObj = any_pb2.anyTest()
obj = any_pb2.test()

file = open("out", "rb")
messageObj.ParseFromString(file.read())
file.close()

messageObj.anyfield.Unpack(obj)

if messageObj.a != 123:
    print("false")
else:
    print("true")

if obj.a != 'chapel' or obj.b != True:
    print("false")
else:
    print("true")
