const x = 10;

var y = 0;

proc f() ref {
    if setter then return y;
              else return x;
}

f() = 11;

writeln(x, " " , y);
