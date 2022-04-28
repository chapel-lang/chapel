config type dataType = string;
config param useItem = false;

test("chapel":dataType);
test("Türkçe":dataType);
test("":dataType);

proc test(param s: dataType) {
  for param i in 0..<s.size {
    param char = if useItem then s.item(i) else s[i];
    if dataType==string then
      compilerWarning("Received ", char,
                      ".\tIts type: ", char.type:string,
                      ". Its size: ", char.size:string,
                      ". Its numBytes: ", char.numBytes:string);
    else
      if useItem then
        compilerWarning("Received a ", char.type:string,
                        ".\tIts size: ", char.size:string);
      else
        compilerWarning("Received ", char:string,
                        ".\tIts type: ", char.type:string);
  }
}
