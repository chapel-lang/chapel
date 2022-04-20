config type dataType = string;

test("chapel":dataType);
test("Türkçe":dataType);
test("":dataType);

proc test(param s: dataType) {
  for param i in 0..<s.size {
    param char = s[i];
    param charInfo = if dataType==string then char else char.toByte():string;
    compilerWarning("Received ", charInfo,
                    ".\tIts size: ", char.size:string,
                    ". Its numBytes: ", char.numBytes:string);
  }
}
