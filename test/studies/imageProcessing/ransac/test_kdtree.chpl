
/*****
      test_kdtree.chpl -
      Self-checking test bench for the kdtree module.

      Call:
        test_kdtree

      c 2015-2018 Primordial Machine Vision Systems
*****/


use kdtree;


/**** assemble_tree ****/

/***
    test_assemble:  Self-checking test bench for the add_node and
                    assemble_tree procedures.
    returns:   true if all tests pass, false if any fail
***/
proc test_assemble() : bool {
  var passed = true;                    /* overall pass-fail flag */

  writeln("Testing assemble_tree");

  if (!test_assemble_1()) then passed = false;
  if (!test_assemble_2()) then passed = false;

  if (passed) then writeln("  all tests passed");
  else writeln("  some test failed");

  return passed;
}

/***
    test_assemble_1:  Test case for assemble_tree.  This is the data set
                      used in Brown's paper.
    returns:   true if all tests pass, false if any fail
***/
proc test_assemble_1() : bool {
  var tree = new unmanaged kdtree(15, int, 3);    /* tree we're building */
  var expflag : uint(8);                /* expected value of flags */
  var passed = true;                    /* overall pass-fail flag */

  writeln("  starting test 1");

  if (15 != tree.Ltree.dim(0).high) {
    writeln("    expected 15 elements in tree, got ", tree.Ltree.dim(0).high);
    passed = false;
  }

  /* The payload will be the node in the tree array when we're done.
                                     (7,2,6)
                   (5,4,2)                            (9,5,3)
          (2,1,3)           (1,6,8)           (8,4,2)           (9,6,7)
      (6,3,1) (2,3,3)   (3,4,5) (4,7,9)   (9,4,1) (8,1,5)   (8,7,6) (9,7,8)
  */
  tree.add_node((2, 3, 3),  9);
  tree.add_node((5, 4, 2),  2);
  tree.add_node((9, 6, 7),  7);
  tree.add_node((4, 7, 9), 11);
  tree.add_node((8, 1, 5), 13);
  tree.add_node((7, 2, 6),  1);
  tree.add_node((9, 4, 1), 12);
  tree.add_node((8, 4, 2),  6);
  tree.add_node((9, 7, 8), 15);
  tree.add_node((6, 3, 1),  8);
  tree.add_node((3, 4, 5), 10);
  tree.add_node((1, 6, 8),  5);
  tree.add_node((9, 5, 3),  3);
  tree.add_node((2, 1, 3),  4);
  tree.add_node((8, 7, 6), 14);

  if (15 != tree.nstored) {
    writeln("  expected 15 elements staged in tree, got ", tree.nstored);
    passed = false;
  }

  tree.assemble_tree();

  for i in 1..15 {
    if (tree.tree(i).val != i) {
      writef("  expected payload at index %2i (%i,%i,%i) to be %2i, got %2i\n",
             i, tree.tree(i).pt(0),tree.tree(i).pt(1),tree.tree(i).pt(2),
             i, tree.tree(i).val);
      passed = false;
    }
    if (1 == i) {
      expflag = 1 | tree.HAS_L | tree.HAS_R;
    } else if (i < 4) {
      expflag = 2 | tree.HAS_L | tree.HAS_R;
    } else if (i < 8) {
      expflag = 3 | tree.HAS_L | tree.HAS_R;
    } else {
      expflag = 1 | tree.IS_LEAF;
    }

    if (tree.tree(i).flags != expflag) {
      writef("  expected flags at index %2i (%i,%i,%i) to be 0x%02xu, got 0x%02xu\n",
             i, tree.tree(i).pt(0),tree.tree(i).pt(1),tree.tree(i).pt(2),
             expflag, tree.tree(i).flags);
      passed = false;
    }
  }

  delete tree;

  return passed;
}

/***
    test_assemble_2:  Test case for assemble_tree.  This is the data set
                      used in Brown's paper.
    returns:   true if all tests pass, false if any fail
***/
proc test_assemble_2() : bool {
  var tree = new unmanaged kdtree(14, int, 3);    /* tree we're building */
  var expflag : uint(8);                /* expected value of flags */
  var passed = true;                    /* overall pass-fail flag */

  writeln("  starting test 2");

  if (15 != tree.Ltree.dim(0).high) {
    writeln("    expected 15 elements in tree, got ", tree.Ltree.dim(0).high);
    passed = false;
  }

  /* The payload will be the node in the tree array when we're done.  Same
     case as test_assemble_1 except we leave out the last node, which changes
     the first median and re-shuffles the tree fairly dramatically.
                                     (6,3,1)
                   (5,4,2)                            (8,4,2)
          (2,3,3)           (1,6,8)           (8,1,5)           (8,7,6)
      (2,1,3)    x      (3,4,5) (4,7,9)   (9,4,1) (7,2,6)   (9,5,3) (9,6,7)

 */
  tree.add_node((2, 3, 3),  4);
  tree.add_node((5, 4, 2),  2);
  tree.add_node((9, 6, 7), 15);
  tree.add_node((4, 7, 9), 11);
  tree.add_node((8, 1, 5),  6);
  tree.add_node((7, 2, 6), 13);
  tree.add_node((9, 4, 1), 12);
  tree.add_node((8, 4, 2),  3);
  tree.add_node((6, 3, 1),  1);
  tree.add_node((3, 4, 5), 10);
  tree.add_node((1, 6, 8),  5);
  tree.add_node((9, 5, 3), 14);
  tree.add_node((2, 1, 3),  8);
  tree.add_node((8, 7, 6),  7);

  if (14 != tree.nstored) {
    writeln("  expected 14 elements staged in tree, got ", tree.nstored);
    passed = false;
  }

  tree.assemble_tree();

  for i in 1..14 {
    if (9 == i) {
      if (0 != tree.tree(i).val) {
        writef("  expected payload at index %2i (%i,%i,%i) to be %2i, got %2i\n",
               i, tree.tree(i).pt(0),tree.tree(i).pt(1),tree.tree(i).pt(2),
               0, tree.tree(i).val);
        passed = false;
      }
    } else if (tree.tree(i).val != i) {
      writef("  expected payload at index %2i (%i,%i,%i) to be %2i, got %2i\n",
             i, tree.tree(i).pt(0),tree.tree(i).pt(1),tree.tree(i).pt(2),
             i, tree.tree(i).val);
      passed = false;
    }
    if (1 == i) {
      expflag = 1 | tree.HAS_L | tree.HAS_R;
    } else if (i < 4) {
      expflag = 2 | tree.HAS_L | tree.HAS_R;
    } else if (4 == i) {
      expflag = 3 | tree.HAS_L;
    } else if (i < 8) {
      expflag = 3 | tree.HAS_L | tree.HAS_R;
    } else if (8 == i) {
      expflag = 1 | tree.IS_LEAF;
    } else if (9 == i) {
      expflag = tree.IS_NIL;
    } else {
      expflag = 1 | tree.IS_LEAF;
    }

    if (tree.tree(i).flags != expflag) {
      writef("  expected flags at index %2i (%i,%i,%i) to be 0x%02xu, got 0x%02xu\n",
             i, tree.tree(i).pt(0),tree.tree(i).pt(1),tree.tree(i).pt(2),
             expflag, tree.tree(i).flags);
      passed = false;
    }
  }

  delete tree;

  return passed;
}


/**** find_nearest_point ****/

/***
    test_nearest_point:  Self-checking test bench for find_nearest_point method
                         and helpers.
    returns:   true if all tests pass, false if any fail
***/
proc test_nearest_point() : bool {
  var passed = true;                    /* overall pass-fail flag */

  writeln("Testing find_nearest_point");

  if (!test_traverse_to_point()) then passed = false;
  if (!test_find_nearest()) then passed = false;

  if (passed) then writeln("  all tests passed");
  else writeln("  some test failed");

  return passed;
}

/***
    test_traverse_to_point:  Self-checking test bench for traverse_to_point
                             method.
    returns:   true if all tests pass, false if any fail
***/
proc test_traverse_to_point() : bool {
  var tree = new unmanaged kdtree(14, int, 3);    /* tree we're building */
  var node : int;                       /* node number returned */
  var match : bool;                     /* true if point in tree */
  var passed = true;                    /* overall pass-fail flag */

  writeln("  testing traverse_to_point");

  /* This is the test case from test_assemble_2.  The payload values are the
     node numbers. */
  tree.add_node((2, 3, 3),  4);
  tree.add_node((5, 4, 2),  2);
  tree.add_node((9, 6, 7), 15);
  tree.add_node((4, 7, 9), 11);
  tree.add_node((8, 1, 5),  6);
  tree.add_node((7, 2, 6), 13);
  tree.add_node((9, 4, 1), 12);
  tree.add_node((8, 4, 2),  3);
  tree.add_node((6, 3, 1),  1);
  tree.add_node((3, 4, 5), 10);
  tree.add_node((1, 6, 8),  5);
  tree.add_node((9, 5, 3), 14);
  tree.add_node((2, 1, 3),  8);
  tree.add_node((8, 7, 6),  7);

  tree.assemble_tree();

  match = tree.traverse_to_point((2,3,3), node);
  if (!match) {
    writeln("    test 1 - expected node in tree, did not find");
    passed = false;
  }
  if (node != 4) {
    writeln("    test 1 - expected node  4, got ", node);
    passed = false;
  }

  match = tree.traverse_to_point((7,2,6), node);
  if (!match) {
    writeln("    test 2 - expected node in tree, did not find");
    passed = false;
  }
  if (node != 13) {
    writeln("    test 2 - expected node 13, got ", node);
    passed = false;
  }

  match = tree.traverse_to_point((9,5,1), node);
  if (match) {
    writeln("    test 3 - did not expect node in tree, found");
    passed = false;
  }
  if (node != 14) {
    writeln("    test 3 - expected node 14, got ", node);
    passed = false;
  }

  match = tree.traverse_to_point((5,5,3), node);
  if (match) {
    writeln("    test 4 - did not expect node in tree, found");
    passed = false;
  }
  if (node != 10) {
    writeln("    test 4 - expected node 10, got ", node);
    passed = false;
  }

  match = tree.traverse_to_point((6,3,1), node);
  if (!match) {
    writeln("    test 5 - expected node in tree, did not find");
    passed = false;
  }
  if (node != 1) {
    writeln("    test 5 - expected node  1, got ", node);
    passed = false;
  }

  match = tree.traverse_to_point((2,1,4), node);
  if (match) {
    writeln("    test 6 - did not expect node in tree, found");
    passed = false;
  }
  if (node != 4) {
    writeln("    test 6 - expected node  4, got ", node);
    passed = false;
  }

  match = tree.traverse_to_point((2,2,3), node);
  if (match) {
    writeln("    test 6 - did not expect node in tree, found");
    passed = false;
  }
  if (node != 8) {
    writeln("    test 7 - expected node  8, got ", node);
    passed = false;
  }

  delete tree;

  return passed;
}

/***
    test_find_nearest:  Self-checking test for the find_nearest_point method.
    returns:   true if all tests pass, false if any fail
***/
proc test_find_nearest() : bool {
  var tree = new unmanaged kdtree(14, int, 3);    /* tree we're building */
  var val : int;                        /* node value returned at closest */
  var passed = true;                    /* overall pass-fail flag */

  writeln("  testing find_nearest_point");

  /* This is the test case from test_assemble_2.  The payload values are the
     node numbers. */
  tree.add_node((2, 3, 3),  4);
  tree.add_node((5, 4, 2),  2);
  tree.add_node((9, 6, 7), 15);
  tree.add_node((4, 7, 9), 11);
  tree.add_node((8, 1, 5),  6);
  tree.add_node((7, 2, 6), 13);
  tree.add_node((9, 4, 1), 12);
  tree.add_node((8, 4, 2),  3);
  tree.add_node((6, 3, 1),  1);
  tree.add_node((3, 4, 5), 10);
  tree.add_node((1, 6, 8),  5);
  tree.add_node((9, 5, 3), 14);
  tree.add_node((2, 1, 3),  8);
  tree.add_node((8, 7, 6),  7);

  tree.assemble_tree();

  val = tree.find_nearest_point((2,3,3));
  if (4 != val) {
    writeln("    test 1 expected value  4, got " , val);
    passed = false;
  }

  val = tree.find_nearest_point((3,4,4));
  if (10 != val) {
    writeln("    test 2 expected value 10, got " , val);
    passed = false;
  }

  val = tree.find_nearest_point((8,3,6));
  if (13 != val) {
    writeln("    test 3 expected value 13, got " , val);
    passed = false;
  }

  val = tree.find_nearest_point((8,3,3));
  if (3 != val) {
    writeln("    test 4 expected value  3, got ", val);
    passed = false;
  }

  /* See test 7 above - parent is (2,1,3).  (2,3,3) is equidistant and
     therefore does not override parent. */
  val = tree.find_nearest_point((2,2,3));
  if (8 != val) {
    writeln("    test 5 expected value  8, got ", val);
    passed = false;
  }

  delete tree;

  return passed;
}



/**** Top Level ****/

proc main() {
  var passed = true;                    /* overall pass/fail flag */

  if (!test_assemble()) then passed = false;
  if (!test_nearest_point()) then passed = false;

  writeln("\nOverall test results");
  if (passed) then writeln("  all tests passed");
  else writeln("  some test FAILED");
}
