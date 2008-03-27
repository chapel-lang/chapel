/*
 * Multilocale Example
 *
 * This example showcases the on-statement.  To run this example, use
 * the "-nl #" flag to specify the number of locales.  For example, to
 * run on 2 locales, run "a.out -nl 2".
 *
 */

//
// output the number of locales we are running on by accessing the
// numLocales constant
//
writeln("running this example on ", numLocales, " locales");

//
// loop over the locales and use the on-statement to execute code on
// every locale; the predefined constant 'here' corresponds to the
// locale the program is currently running and 'here.id' contains a
// unique integer between 0 and numLocales-1.
//
for i in 0..numLocales-1 {
  on Locales(i) {
    writeln("hello locale ", here.id);
  }
}

//
// build a linked list where each node in the linked list
// resides on a subsequent locale
//
class Node {      // node class for a linked list
  var data: real; // data in node
  var next: Node; // reference to next node in linked list
}

var head = new Node(0);
var tmp = head;
for i in 1..numLocales-1 {
  on Locales(i) {
    tmp.next = new Node(i);
    tmp = tmp.next;
  }
}

//
// walk over the linked list and output each node and the locale on
// which it exists using .locale.id to determine the locale
//
var tmp = head;
while tmp {
  writeln("node with data = ", tmp.data, " on locale ", tmp.locale.id);
  tmp = tmp.next;
}

