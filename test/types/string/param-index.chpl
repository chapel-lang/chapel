config type dataType = string;
config param useItem = false;

test("chapel":dataType);
test("Türkçe":dataType);
test("":dataType);

proc test(param s: dataType) {
  for param i in 0..<s.size {
    param char = if useItem then s.item(i) else s[i];
    param charInfo = if dataType==string then char else char.toByte():string;
    compilerWarning("Received ", charInfo,
                    ".\tIts size: ", char.size:string,
                    ". Its numBytes: ", char.numBytes:string);
  }
}
