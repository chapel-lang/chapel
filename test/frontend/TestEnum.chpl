
use Print;

// TODO: enums with values

enum colors { red, green, blue }

proc main() {
  {
    var c : colors = colors.blue;
    var x = chpl__enumToOrder(c);
    println(x);

    param p = chpl__enumToOrder(colors.blue);
    if p == 2 {
      println(true);
    } else {
      compilerError("unreachable");
    }
  }

  {
    var o = 1;
    var e = chpl__orderToEnum(o, colors);
    println(e == colors.green);

    param p = chpl__orderToEnum(2, colors);
    if p == colors.blue {
      println(true);
    } else {
      compilerError("unreachable");
    }
  }
}
