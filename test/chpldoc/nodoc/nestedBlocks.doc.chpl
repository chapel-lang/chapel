/* This module should have no contents, as all of its code lives under nested
   blocks instead of at the top level */
module looksEmpty {
  {
    /* Even though this variable has a comment, it should not be visible
       to chpldoc because it lives in a nested block and is thus inaccessible
    */
    var hideMe = 14;

    // This constant does not have a chpldoc comment, but it still shouldn't be
    // visible
    const meTooPlease: bool;

    /* This function should similarly be hidden */
    proc ohAndMe() { }

    // As should this record
    record youDidNotSeeAnything { }
  }
}
