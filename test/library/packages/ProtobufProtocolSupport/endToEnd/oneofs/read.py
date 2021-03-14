import oneofs_pb2

messageObj = oneofs_pb2.Foo()

file = open("out", "rb")
messageObj.ParseFromString(file.read())
file.close()

if messageObj.co != oneofs_pb2.color.red:
    print("false")
else:
    print("true")

if messageObj.name != b'chapel':
    print("false")
else:
    print("true")

if messageObj.mfield.a != 0:
    print("false")
else:
    print("true")

if messageObj.ifield != 45:
    print("false")
else:
    print("true")
