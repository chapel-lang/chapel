class Logger {
  proc  Println(v...?eltType) {
    writeln("BaseLogger: ", (...v));
  }
}

class DefaultLogger: Logger {
  override proc  Println(v...?eltType) {
    writeln("DefaultLogger: ", (...v));
  }
}

class DefaultLogger2: Logger {
  override proc  Println(v...?eltType) {
    writeln("DefaultLogger2: ", (...v));
  }
}

var Log: borrowed Logger?;
Log = (new owned DefaultLogger()).borrow();
Log!.Println("I want to print from my DefaultLogger");
Log = (new owned DefaultLogger2()).borrow();
Log!.Println("I want to print from my DefaultLogger2");
