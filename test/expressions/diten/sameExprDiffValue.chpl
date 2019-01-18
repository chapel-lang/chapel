record T {
  var hour, minute, second, microsecond: int;
}

proc isEqualToSelf(t: T) {
  const sec1 = t.hour*3600 + t.minute*60 + t.second + t.microsecond / 1000000.0;
  const sec2 = t.hour*3600 + t.minute*60 + t.second + t.microsecond / 1000000.0;
  return sec1 == sec2;
}


var a = new T(1,2,3,4);

writeln(isEqualToSelf(a));
