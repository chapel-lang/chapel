
module MyModule {
  import MySubModules;
  import MyOtherModule;
  /*
    Link to other docs

    * :class:`SubA.MyClass`

    * :proc:`SubA.myProc`

    * :class:`SubB.MyOtherClass`

    * :iter:`SubB.myIter`

    * :record:`myRec`

    * :proc:`myFunc`

    * :proc:`other`

    * :proc:`last`
  */
  proc myFunc() {
    writeln("Hello from MyModule");
  }



  /*
    .. include:: /modules/MyOtherModule.rst
      :start-after: START_INCLUDE_HERE
  */
  proc other() {}

  proc last() {}
}
