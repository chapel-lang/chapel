use IO, Map;

record recordthatcontainsadirent {
  var de: unmanaged dirent;
}

class dirent {
  var files = new map(string, recordthatcontainsadirent);
}

var root: unmanaged dirent = new unmanaged dirent();

var curdir: unmanaged dirent = root;

config var filename = "foo";

curdir.files[filename] = new recordthatcontainsadirent(new unmanaged dirent());
