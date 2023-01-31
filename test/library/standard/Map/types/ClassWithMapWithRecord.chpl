use IO, Map;
const filename = "foo";

/////////////////////////////////////////////////////////////////////////////
// a simplified example

record MyRecord {
  var de: unmanaged object;
// this works: var de = new unmanaged object();
}

class MyClass {
  var files = new map(string, MyRecord);
}

const myC = new unmanaged MyClass();

// TODO: The following seems to create an extra new unmanaged object()
// that is not deleted by `myC.files[filename].de` below.
// Once this test passes memleaks testing, remove the --memLeaks .execopts.
myC.files[filename] = new MyRecord(new unmanaged object());

writeln(myC);  // {files = {foo: (de = {})}}

delete myC.files[filename].de;
delete myC;

/////////////////////////////////////////////////////////////////////////////
// the original recursive case by @cassella

record recordthatcontainsadirent {
  var de: unmanaged dirent;
}

class dirent {
  var files = new map(string, recordthatcontainsadirent);
}

var curdir = new unmanaged dirent();

curdir.files[filename] = new recordthatcontainsadirent(new unmanaged dirent());

writeln(curdir);  // {files = {foo: (de = {files = {}})}}

delete curdir.files[filename].de;
delete curdir;
