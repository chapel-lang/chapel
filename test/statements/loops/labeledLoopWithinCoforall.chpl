coforall i in 1..1 {
  label outer while true {
    var i = 1;
    do {
      writeln(i);
      if i == 3 then
        break outer;
      i += 1;
    } while (i < 100);
  }
}

coforall i in 1..1 {
  label outer do {
    var i = 1;
    do {
      writeln(i);
      if i == 3 then
        break outer;
      i += 1;
    } while (i < 100);
  } while true;
}

coforall i in 1..1 {
  label outer for i in (1..) {
    var i = 1;
    do {
      writeln(i);
      if i == 3 then
        break outer;
      i += 1;
    } while (i < 100);
  }
}
