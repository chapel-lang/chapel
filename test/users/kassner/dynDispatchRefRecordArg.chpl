use LinkedLists;

class Foo
{
  proc doStuff(ref myList : LinkedList(int)) {}
}

class Bar : Foo
{
  override proc doStuff(ref myList : LinkedList(int))
  {
    writeln("doStuff: myList.length = ", myList.length);

    var myInt = 2;

    myList.append(myInt);

    writeln("doStuff: added '", myInt, "' to myList");
    writeln("doStuff: myList.length = ", myList.length);
  }
}

class MyClass
{
  var myList = new LinkedList(int);

  proc deinit()
  {
    myList.destroy();
  }

  proc run()
  {
    var bar : borrowed Foo = new borrowed Bar();

    writeln("run:\t myList.length = ", myList.length);

    writeln("run:\t calling doStuff");

    bar.doStuff(myList);

    writeln("run:\t myList.length = ", myList.length);

    for item in myList
    {
      writeln("run:\t found '", item, "' in myList.");
    }


  }
}

proc main()
{
  var c = new unmanaged MyClass();

  c.run();

  delete c;
}
