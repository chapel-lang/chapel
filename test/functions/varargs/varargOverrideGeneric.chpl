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
var oLog = new owned DefaultLogger();
Log = oLog.borrow();
Log!.Println("I want to print from my DefaultLogger");
var oLog2 = new owned DefaultLogger2();
var Log2 = oLog2.borrow();
Log2!.Println("I want to print from my DefaultLogger2");
