record R {
  var x;

  proc return_ref() ref
  {
    writeln("setter");
    return x;
  }
  proc return_ref()
  {
    writeln("getter");
    return x;
  }
  
  proc call_return_ref() ref
  {
    return return_ref();
  }

}

{
  var r = new R(1);

  writeln("should be reading 1");
  var x = r.return_ref(); // getter
  writeln(x);

  writeln("setting it to 2");
  r.return_ref() = 2; // setter
  writeln(r.x);

  writeln("should be reading 2 (setter)");
  var y = r.call_return_ref(); // setter
  writeln(r.x);

  writeln("setting it to 3");
  r.call_return_ref() = 3; // setter
  writeln(r.x);
}

{
  var r = new R("one");

  writeln("should be reading one");
  var x = r.return_ref(); // getter
  writeln(x);

  writeln("setting it to two");
  r.return_ref() = "two"; // setter
  writeln(r.x);

  writeln("should be reading two (setter)");
  var y = r.call_return_ref(); // setter
  writeln(r.x);

  writeln("setting it to three");
  r.call_return_ref() = "three"; // setter
  writeln(r.x);
}
