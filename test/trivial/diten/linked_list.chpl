class ll {
  type itemType;
  var  list: unmanaged ll_entry(itemType)?;

  class a_class {
    var a: int;

    proc bar() { writeln(a); }
  }

  proc deinit() {
    var l = list;

    while l != nil {
      var h = l;

      l = l!.next;

      delete h;
    }
  }

  proc add_front(e) {
    list = new unmanaged ll_entry(itemType, e, list);
  }

  proc remove_front() {
    var e: itemType;

    if list != nil {
      var l = list;

      e    = list!.element;
      list = list!.next;

      delete l;
    }

    return e;
  }

  proc add_back(e) {
    if list == nil then
      list = new unmanaged ll_entry(itemType, e);
    else {
      var mylist = list;

      while mylist.next != nil do
        mylist = mylist.next;

      mylist.next = new unmanaged ll_entry(itemType, e);
    }
  }

  proc remove_back() {
    var e: itemType;

    if list != nil {
      if list.next != nil {
        var mylist = list;

        while ((mylist.next != nil) && (mylist.next.next != nil)) do
          mylist = mylist.next;

        var l = mylist.next;

        e = mylist.next.element;

        mylist.next = nil;

        delete l;
      }
      else {
        var l = list;

        e    = list.element;
        list = nil;

        delete l;
      }
    }
    return e;
  }

  proc contains(e)  {
    var mylist = list;

    while mylist != nil do
      if e == mylist.element then
        return true;

    return false;
  }

  proc remove_all_matching(e) {
    if list == nil then
      return;

    var mylist = list!;

    while mylist.next != nil do
      if mylist.next!.element == e then {
        var h = mylist.next;

        mylist.next = mylist.next!.next;

        delete h;
      }
      else
        mylist = mylist.next!;

    // if the first element item in the list is equal to e, remove it
    if list!.element == e then {
      var h = list;

      list = list!.next;

      delete h;
    }
  }

  proc reverse() {
    var mylist: unmanaged ll_entry(itemType)?;
    var e: itemType;

    if list == nil then
      return;

    while list != nil {
      var h = list;

      e      = list!.element;
      mylist = new unmanaged ll_entry(itemType, e, mylist);
      list   = list!.next;

      delete h;
    }

    list = mylist;
  }

  proc concatenate_front(list2: unmanaged ll(itemType)){
    if list2 == nil then
      return;

    var mylist = list2.list;

    while mylist != nil {
      add_front(mylist!.element);

      mylist = mylist!.next;
    }
  }

  proc print() {
    var mylist = list;

    while mylist != nil {
      writeln(mylist!.element);

      mylist = mylist!.next;
    }
  }

}

class ll_entry {
  // It would be better to make this nested within the ll class
  // But class nesting is not yet implemented
  type elementType;
  var element: elementType;
  var next: unmanaged ll_entry(elementType)?;
}

proc main(){
  var list1: unmanaged ll(int) = new unmanaged ll(int);
  var list2: unmanaged ll(int) = new unmanaged ll(int);

  for i in 1..10 {
    list1.add_front(i);
    list2.add_front(-i);
  }

  list1.add_front(2);
  list1.add_front(2);
  list1.add_front(2);
  list1.add_front(2);
  list1.add_front(2);

  list1.remove_all_matching(2);

  list1.print();

  list1.reverse();

  list1.print();
  list1.concatenate_front(list2);

  list2.remove_front();

  list1.print();

  delete list1;
  delete list2;
}
