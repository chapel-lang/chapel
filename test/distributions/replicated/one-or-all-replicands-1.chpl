// This test records the current behavior. We may want to change some of it
// when executing on multiple locales.
// See issue #24595

use ReplicatedDist;

var localDom = {1..3};
var replDom = localDom dmapped new replicatedDist();

proc useArray(in arr) {
  showArray(arr, "in useArray");
}

proc showArray(arr, msg) {
  writeln(msg);
  for loc in Locales do on loc do writeln(" ", arr);
}

proc makeReplicatedArray() {
  var result: [replDom] real;
  for loc in Locales do on loc do
    for i in localDom do
      result[i] = here.id + i*0.1;
  return result;
}

proc main {
  var replArr1 = makeReplicatedArray();
  showArray(replArr1, "replArr1");

  useArray(replArr1);              // passes only the first replicand

  var replArr2: replArr1.type;
  replArr2; // ensure the next line is an assignment
  replArr2 = replArr1;             // copies only the first replicand
  showArray(replArr2, "replArr2");

  var replArr3 = replArr1;         // uses only the first replicand
  showArray(replArr3, "replArr3");

  var replArr4 = replArr1;         // uses ALL replicands b/c copy elision
  showArray(replArr4, "replArr4");
}
