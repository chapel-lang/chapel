import typesTest_pb2

messageObj = typesTest_pb2.Types()

messageObj.ui64 = 9223372036854
messageObj.ui32 = 429496729

messageObj.i64 = -600000
messageObj.i32 = 214748364

messageObj.bo = True

messageObj.si64 = -675348989989
messageObj.si32 = -214748364

messageObj.byt = b'\x97\xB3\xE6\xCC\x01';

messageObj.st = "Protobuf implementation for chapel"

messageObj.fi32 = 1000000
messageObj.fi64 = 100000000000

messageObj.fl = 444.23444
messageObj.db = 444444444444.23444

messageObj.sf32 = -4567;
messageObj.sf64 = 6473899292;

file = open("out", "wb")
file.write(messageObj.SerializeToString())
file.close()
