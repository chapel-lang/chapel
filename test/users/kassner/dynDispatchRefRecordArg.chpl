use LinkedLists;

class Foo
{
  proc doStuff(ref myList : LinkedList(int)) {}
}

class Bar : Foo
{
  override proc doStuff(ref myList : LinkedList(int))
  {
    writeln("doStuff: myList.size = ", myList.size);

    var myInt = 2;

    myList.append(myInt);

    writeln("doStuff: added '", myInt, "' to myList");
    writeln("doStuff: myList.size = ", myList.size);
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

    writeln("run:\t myList.size = ", myList.size);

    writeln("run:\t calling doStuff");

    bar.doStuff(myList);

    writeln("run:\t myList.size = ", myList.size);

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
