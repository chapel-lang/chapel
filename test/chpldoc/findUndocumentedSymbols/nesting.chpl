module M1 {
  module SubModule {
    proc noComment() {}
  }
}

/**/
module M2 {
  module SubModule {
    proc noComment() {}
  }
}

module M3 {
  /**/
  module SubModule {
    proc noComment() {}
  }
}

module M4 {
  module SubModule {
    /**/
    proc noComment() {}
  }
}

@chpldoc.nodoc
module M5 {
  module SubModule {
    proc noComment() {}
  }
}

module M6 {
  @chpldoc.nodoc
  module SubModule {
    proc noComment() {}
  }
}

module M7 {
  module SubModule {
    @chpldoc.nodoc
    proc noComment() {}
  }
}

@deprecated
module M8 {
  module SubModule {
    proc noComment() {}
  }
}

module M9 {
  @unstable
  module SubModule {
    proc noComment() {}
  }
}

proc main() {}
