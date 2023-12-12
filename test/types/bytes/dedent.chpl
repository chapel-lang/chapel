// mainly, just check that bytes.dedent compiles

var x:bytes = b"""
                 hello
                 there
              """;
var y = x.dedent(ignoreFirst=true);
writeln(y.decode());
