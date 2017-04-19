use stout;

proc main() {

   var x : Try(int);
   x = Try(int).error(int, "boo");
   var e = new Error(int, "lame");
   writeln((true, x.isError()));
   x = e;
   writeln((true, x.isError()));
   writeln((false, x.isSome()));
   x = Try(int).some(5);
   writeln((false, x.isError()));
   writeln((true, x.isSome()));
   writeln((true, x == x));
   x = Try(int).error(int, "cat");
   writeln((true, x.isError()));
   writeln((false, x.isSome()));
   var y = Try(int).some(10);
   writeln((false, x == y));
   x = y;
   writeln((true, x == y));
   writeln(x.get());
   writeln(x());
}

