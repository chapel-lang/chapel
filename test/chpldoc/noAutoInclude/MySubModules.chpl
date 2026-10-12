/*

  .. include:: /modules/MySubModules/SubA.rst
     :start-after: START_INCLUDE_HERE

  .. include:: /modules/MySubModules/SubB.rst

*/
module MySubModules {
  include module SubA;
  include module SubB;
  import this.SubA;
  import this.SubB;
  class MySubModuleClass: SubA.MyClass {
  }
  class MySubModuleOtherClass: SubB.MyOtherClass {
  }
}
