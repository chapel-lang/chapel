class myClass {
  function myWrite(a: string ...?rank) {
    for i in 1..rank do
      write(a(i));
  }
}

var a: myClass;
a.myWrite("This ", "should ", "print.");
