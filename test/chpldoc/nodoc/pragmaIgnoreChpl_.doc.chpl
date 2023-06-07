module M {

  pragma "chpldoc ignore chpl prefix"
  proc chpl_proc() {
    // this chplproc is documented without a comment
    writeln("chpl_proc");
  }


  /* this chpl_proc is documented with a comment*/
  pragma "chpldoc ignore chpl prefix"
  proc chpl_proc(msg: string) {
    writeln("chpl_proc(msg: string)");
  }

  pragma "chpldoc ignore chpl prefix"
  /* this chpl_proc is documented with a comment*/
  proc chpl_proc(msgs : [] string) {
    writeln("chpl_proc(msgs: [] string)");
  }

  /* this chpl_proc is not documented */
  proc chpl_proc(x: int) {
    writeln("chpl_proc(x: int)");
  }

  /* this chpl_proc is not documented */
  pragma "chpldoc ignore chpl prefix"
  @chpldoc.nodoc
  proc chpl_proc(x: real) {
    writeln("chpl_proc(x: real)");
  }

  /* this type is not documented by default*/
  record chpl_someType {
    var x: int;
    var y: int;
  }

  /* this proc on chpl_someType is not documented by default */
  proc chpl_someType.someProc() {
    writeln("chpl_someType.someProc()");
  }


}
