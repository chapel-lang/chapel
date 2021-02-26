import oneofs_pb2

messageObj = oneofs_pb2.Foo()

messageObj.co = oneofs_pb2.color.green
messageObj.name = b"chapel";

messageObj.mfield.a = 67
messageObj.ifield = 45

file = open("out", "wb")
file.write(messageObj.SerializeToString())
file.close()
