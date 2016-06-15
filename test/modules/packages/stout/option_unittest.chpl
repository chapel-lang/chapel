use stout;

proc main() {

   var x = Option(int).none(int);
   writeln(x.isNone());
   writeln(x.isSome());
   x = Option(int).some(5);
   writeln(x.isNone());
   writeln(x.isSome());
   writeln(x == x);
   x = Option(int).none(int);
   writeln(x.isNone());
   writeln(x.isSome());
   var y : Option(int);
   y = Option(int).some(10);
   writeln(x == y);
   x = y;
   writeln(x == y);
   writeln(x.get());
   writeln(x());
}

