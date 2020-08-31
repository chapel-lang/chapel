import repeatedfield_pb2

messageObj = repeatedfield_pb2.repeatedField()

messageObj.ui64.append(1)
messageObj.ui64.append(2445464)

messageObj.ui32.append(1)
messageObj.ui32.append(24454)

messageObj.i64.append(-100)
messageObj.i64.append(244540000000)

messageObj.i32.append(-500)
messageObj.i32.append(2445489)

messageObj.bo.append(True)
messageObj.bo.append(False)

messageObj.si64.append(-500)
messageObj.si64.append(-24454890000)

messageObj.si32.append(-50)
messageObj.si32.append(-2445489)

messageObj.fi32.append(67)
messageObj.fi32.append(8907)

messageObj.fi64.append(500)
messageObj.fi64.append(2445489000)

messageObj.fl.append(4.12)
messageObj.fl.append(4500.3)

messageObj.db.append(67.2345)
messageObj.db.append(8907980.5657)

messageObj.sfi32.append(-500)
messageObj.sfi32.append(-244548)

messageObj.sfi64.append(-45)
messageObj.sfi64.append(-4500000000)

messageObj.byt.append(b'\x01\x87\x76')
messageObj.byt.append(b'\x00\x01\x02\x03')

messageObj.st.append("aniket")
messageObj.st.append("String with spaces")

file = open("out", "wb")
file.write(messageObj.SerializeToString())
file.close()
