class Update {
  var value: uint(64);
  var forward: unmanaged Update?;
}

class Bucket {
  var updateList: unmanaged Update?;
  var numUpdates: int = 0;
}

class UpdateManager {
  var updateList: unmanaged Update?;

  proc getUpdate() {
    if updateList == nil {
      return new unmanaged Update();
    } else {
      var update = updateList!;
      updateList = update.forward;
      return update;
    }
  }

  proc returnUpdate(update) {
    update.forward = updateList;
    updateList = update;
  }
}

use maxHeap;
class Buckets {
/*
  // wanted these to be nested, but it caused some problems
  class Update {
    var value: uint(64);
    var forward: Update;
  }

  class Bucket {
    var updateList: Update;
    var numUpdates: int = 0;
  }
*/

  const numLocs: int = numLocales;
  var pendingUpdates = 0;
  var BucketArray: [0..#numLocs] unmanaged Bucket; // = [0..#numLocs] new borrowed Bucket(nil, 0);
  var heap = new owned MaxHeap(numLocs);
  var updateManager = new owned UpdateManager();

  // These postinit() and deinit() are a workaround for #11314.
  // Once that is resolved, this code can be simplified (as in #11321) by:
  //  - removing postinit() and deinit(),
  //  - uncommenting the default expression for the field BucketArray above,
  //  - removing the explicit type of the BucketArray field.
  proc postinit() {
    forall bucket in BucketArray do
      bucket = new unmanaged Bucket(nil, 0);
  }
  proc deinit() {
    delete BucketArray;
  }

  proc insertUpdate(ran: uint(64), loc: int) {
    local {
      var bucket = BucketArray(loc);
      var update = updateManager.getUpdate();
      update.value = ran;
      update.forward = bucket.updateList;

      pendingUpdates += 1;
      bucket.updateList = update;
      bucket.numUpdates += 1;
      if bucket.numUpdates == 1 then
        heap.insert(loc, 1);
      else
        heap.increaseKey(loc);
    }
  }

  proc getUpdates(buf:[]) {
    var loc, nu: int;
    local {
      (loc, nu) = heap.extractMax();
      var update = BucketArray(loc).updateList;
      var i = 0;

      while update != nil {
        var tmp = update!.forward;
        buf(i) = update!.value;
        updateManager.returnUpdate(update!);
        update = tmp;
        i += 1;
      }
      if nu != i then
        writeln(here.id, ": i = ", i, " nu = ", nu);
      BucketArray(loc).updateList = nil;
      BucketArray(loc).numUpdates = 0;
      pendingUpdates -= nu;
    }
    return (loc, nu);
  }

  proc printCounts() {
    for i in 0..#numLocs {
      var b = BucketArray(i);
      writeln(here.id, ": bucket ", i, " has ", b.numUpdates, " updates");
    }
  }
}

/*
var buckets = new Buckets(numLocs=4);
var A: [0..100] uint(64);

proc main() {
  buckets.insertUpdate(32, 0);
  buckets.printCounts();
  buckets.insertUpdate(7, 2);
  buckets.insertUpdate(7, 2);
  buckets.insertUpdate(7, 0);
  buckets.printCounts();
  buckets.heap.printHeap();
  var (loc, nu) = buckets.getUpdates(A);
  writeln((loc, nu));
  for i in 0..#nu do
    writeln(A(i));
  buckets.insertUpdate(3,2);
  buckets.insertUpdate(4,2);
  buckets.insertUpdate(5,2);
  (loc, nu) = buckets.getUpdates(A);
  writeln((loc, nu));
  for i in 0..#nu do
    writeln(A(i));
}
*/
