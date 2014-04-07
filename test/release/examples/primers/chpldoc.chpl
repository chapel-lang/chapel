/*
  chpldoc Primer
 
  This primer covers the use of chpldoc to document source code. For further
  information, please see $CHPL_HOME/doc/technotes/README.chpldoc
 
*/


// Creating and accessing the documentation:

// To generate the documentation for this and any source file, you may either
// compile using the command 'chpldoc <sourceName.chpl>' for documentation only
// or using the command 'chpl --docs <sourceName.chpl>' to create both 
// documentation and an executable.

// The documentation files will be found in a special folder created by the 
// chpldoc tool.  If you are compiling a Chapel file that is deeper than your
// current directory, this heirarchy will be reflected within the documentation
// folder.
// (i.e. foo/bar.chpl will generate documentation within docs/foo/)



// Documentation within the code:

/*
  Multiline comments found before a method are associated with that method
  as long as there are no code blocks between them.
*/

proc commented(val: int): string {
  var totalNum = val + 4;
  var str = "hello agent " + totalNum;
  return str;
}

/*
  The method can be a stub and still output its comment
*/
proc stub(val): bool {
  /* Comments within the method body are ignored */
}

// Single line comments are also ignored.  However, this does not prevent 
// the display of the method itself.
proc uncommented() {
  /*
    Longer comments within the method body are also ignored
  */
}

/*
  If the source code defines a module, it can also have a comment associated
  with it
*/
module Defined {

  /*
    And classes can display their comments, too
  */
  class Foo {
    /*
      Including the comments associated with their fields
    */
    var a: int;

    /*
      And class specific methods
    */
    proc getA(): int {
      return a;
    }
  }

  /*
    If a class inherits from another class, those fields, methods, and their
    comments are output in both the super and the subclass
  */
  class Bar : Foo {

    proc setA(newVal: int) {
      a = newVal;
    }   
  }
}
