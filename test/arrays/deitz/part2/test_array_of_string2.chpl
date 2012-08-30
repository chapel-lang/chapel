var d : domain(2) = {1..3, 1..5};

var a : [d] string = "nada";

writeln(a);

[i in 1..3] {
  a(i,i) = (i + i):string;
}

writeln(a);
