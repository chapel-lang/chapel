use stout;

proc main() {
   var e = new Error(int, "boo");
   var x:Try(int) = e;

   writeln(x.error());
}

