enum MyEnum {
 A, B, C
}

iter myiter() {
  yield MyEnum.A;
  yield MyEnum.B;
  yield MyEnum.C;
}


proc main() {
  var err = 0;
  var x:MyEnum;


  for i in myiter() {
    var str:string = i:string;
    var slen:ssize_t = str.length:ssize_t;
    err = 1;
    if i == MyEnum.B then err = 0;
    if err == 0 {
      x = i;
      break;
    } else if err != 1 then break;
  }

  __primitive("chpl_error", x:string);
}
