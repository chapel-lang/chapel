/*
  chpldoc Primer
 
  This primer covers the use of chpldoc to document source code. For further
  information, please see $CHPL_HOME/doc/technotes/README.chpldoc
 
*/


// Creating and accessing the documentation:

// To generate the documentation for this and any source file, you may either
// compile using the command 'chpldoc <sourceName.chpl>' or using the command
// 'chpl --docs <sourceName.chpl>'.

// The documentation files will be found in a special folder created by the 
// chpldoc tool.  If you are compiling a Chapel file that is deeper than your
// current directory, this hierarchy will be reflected within the documentation
// folder.
// (i.e. foo/bar.chpl will generate documentation within docs/foo/)



// Documentation within the code:

/*
  Multiline comments found before a function are associated with that function
  as long as there are no code blocks between them.
*/

proc commented(val: int): string {
  var totalNum = val + 4;
  var str = "hello agent " + totalNum;
  return str;
}

/*
  The function can be a stub and still output its comment
*/
proc stub(val): bool {
  /* Comments within the function body are ignored */
}

// Single line comments are also ignored.  However, this does not prevent 
// the display of the function itself.
proc uncommented() {
  /*
    Longer comments within the function body are also ignored
  */
}

/*
  To prevent the display of a particular function, class, record, specified
  module, or global variable, simply preface it with:
  pragma "no doc"
*/
pragma "no doc"
proc undocumented() {
  // This function won't be in the final output.

  // Once Chapel has support for marking symbol as private, that can be used
  // to squash documentation instead.
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
