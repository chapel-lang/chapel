import repeatedfield_pb2

messageObj = repeatedfield_pb2.repeatedField()

file = open("out", "rb")
messageObj.ParseFromString(file.read())
file.close()

lst = [];
lst.append(1)
lst.append(2445464)
if messageObj.ui64 != lst:
    print("false")
else:
    print("true")

lst = [];
lst.append(1)
lst.append(24454)
if messageObj.ui32 != lst:
    print("false")
else:
    print("true")
    
lst = [];
lst.append(-100)
lst.append(244540000000)
if messageObj.i64 != lst:
    print("false")
else:
    print("true")
    
lst = [];
lst.append(-500);
lst.append(2445489);
if messageObj.i32 != lst:
    print("false")
else:
    print("true")
    
lst = [];
lst.append(True);
lst.append(False);
if messageObj.bo != lst:
    print("false")
else:
    print("true")
    
lst = [];
lst.append(-500)
lst.append(-24454890000)
if messageObj.si64 != lst:
    print("false")
else:
    print("true")
    
lst = [];
lst.append(-50)
lst.append(-2445489)
if messageObj.si32 != lst:
    print("false")
else:
    print("true")

lst = []
lst.append(67)
lst.append(8907)
if messageObj.fi32 != lst:
    print("false")
else:
    print("true")

lst = []
lst.append(500)
lst.append(2445489000)
if messageObj.fi64 != lst:
    print("false")
else:
    print("true")

lst = []
lst.append(4.12)
lst.append(4500.3)
newLst = [round(x, 2) for x in messageObj.fl]
if newLst != lst:
    print("false")
else:
    print("true")

lst = []
lst.append(67.2345)
lst.append(8907980.5657)
if messageObj.db != lst:
    print("false")
else:
    print("true")

lst = []
lst.append(-500)
lst.append(-244548)
if messageObj.sfi32 != lst:
    print("false")
else:
    print("true")

lst = []
lst.append(-45)
lst.append(-4500000000)
if messageObj.sfi64 != lst:
    print("false")
else:
    print("true")
    
lst = []
lst.append(b"\x01\x87\x76")
lst.append(b"\x00\x01\x02\x03")
if messageObj.byt != lst:
    print("false")
else:
    print("true")
    
lst = []
lst.append("aniket")
lst.append("String with spaces")
if messageObj.st != lst:
    print("false")
else:
    print("true")
