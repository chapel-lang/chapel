use CTypes;
use HaltWrappers;
use List;
use NmpArray;

// Store for our numpy arrays.
private var arrayStore: list(unmanaged Array?);

// Find empty slot or append a new 'nil' descriptor. Returns slot index.
private proc findOpenArrayStoreSlotIdx() {
  for i in 0..<arrayStore.size do
    if arrayStore[i] == nil then
      return i;

  var ret = arrayStore.size;
  arrayStore.append(nil);
  return ret;
}

// Non-specialized impl that creates an array of a given type.
private proc createArrayImpl(rank: int, type T, in dims: [0..#rank] int) {
  var arr = Array.create(rank, T, dims);
  var ret = findOpenArrayStoreSlotIdx();
  ref slot = arrayStore[ret];
  slot = arr;
  return ret;
}

/***
  Create an array of 'int(64)'. The returned ID can be passed into exported
  functions that work on array IDs.
*/
export
proc nmpArrayCreateInt64(rank: int, in dims: [0..#rank] int): int {
  return createArrayImpl(rank, int(64), dims);
}

export
proc nmpArrayCreateReal64(rank: int, in dims: [0..#rank] int): int {
  return createArrayImpl(rank, real(64), dims);
}

private proc arrayFromIdOrHalt(id: int) ref {
  if id < 0 || id > arrayStore.size || arrayStore[id] == nil then
    halt('invalid array id: ' + id:string);
  ref ret = arrayStore[id];
  return ret;
}

/***
*/
export proc nmpArrayDestroy(id: int) {
  if id < 0 || id > arrayStore.size then return 0;
  ref arr = arrayStore[id];
  if arr == nil then return 0;
  delete arr;
  arr = nil;
  return 1;
}

/***
*/
export proc nmpArrayBufferPtrAsInt(id: int): int {
  if id < 0 || id > arrayStore.size then return 0;
  var arr = arrayStore[id];
  if arr == nil then return 0;
  var ptr = (arr!).ptrToBuffer();
  return ptr:int;
}

export proc nmpArrayPrint(id: int): void {
  var arr = arrayFromIdOrHalt(id);
  (arr!).display();
}

/***
*/
private proc nmpArrayBufferPtr(id: int): c_void_ptr {
  unimplementedFeatureHalt('interop', 'returning c_void_ptr');
  return nil;
}

