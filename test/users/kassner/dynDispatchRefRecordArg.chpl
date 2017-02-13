use List;

class Foo
{
  proc doStuff(ref myList : list(int)) {}
}

class Bar : Foo
{
  proc doStuff(ref myList : list(int))
  {
    writeln("doStuff: myList.length = " + myList.length);

    var myInt = 2;

    myList.append(myInt);

    writeln("doStuff: added '" + myInt + "' to myList");
    writeln("doStuff: myList.length = " + myList.length);
  }
}

class MyClass
{
  var myList = new list(int);

  proc deinit()
  {
    myList.destroy();
  }

  proc run()
  {
    var bar : Foo = new Bar();

    writeln("run:\t myList.length = " + myList.length);

    writeln("run:\t calling doStuff");

    bar.doStuff(myList);

    writeln("run:\t myList.length = " + myList.length);

    for item in myList
    {
      writeln("run:\t found '" + item + "' in myList.");
    }

    delete bar;
  }
}

proc main()
{
  var c = new MyClass();

  c.run();

  delete c;
}
