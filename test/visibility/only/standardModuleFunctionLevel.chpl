proc a() {
  use FileSystem only isFile;

  writeln(isFile("standardModuleFunctionLevel.chpl"));
}

a();
