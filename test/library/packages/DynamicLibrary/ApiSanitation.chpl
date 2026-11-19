use DynamicLibrary;
use Reflection;
use ChplConfig;

proc test1() {
  writeln(Reflection.getRoutineName());

  const ext = dynamicLibrary.fileExt;

  if CHPL_TARGET_PLATFORM == 'darwin' {
    assert(ext == 'dylib');
  } else {
    assert(ext == 'so');
  }
}

enum Component {
  Name, Directory, Prefix, Suffix
}

proc testLoadCannotContainSep(expected: Component,
                              name: string,
                              directory: string='',
                              prefix: string=dynamicLibrary.prefix,
                              suffix: string=dynamicLibrary.suffix) {
  try {
    const lib = load(name, directory, prefix, suffix);
  } catch e: IllegalArgumentError {
    var searchFor = '';
    select expected {
      when Component.Directory do halt('Should not happen!');
      when Component.Name do searchFor = 'name';
      when Component.Prefix do searchFor = 'prefix';
      when Component.Suffix do searchFor = 'suffix';
    }

    const msg = e.message();
    assert(msg.contains(searchFor));

  } catch e: DynLoadError {
    // Failed to load (OK).
  } catch e {
    try! throw e;
  }
}

proc test2() {
  writeln(Reflection.getRoutineName());

  testLoadCannotContainSep(expected=Component.Name, 'foo/');
  testLoadCannotContainSep(expected=Component.Prefix, 'foo', prefix='/bad');
  testLoadCannotContainSep(expected=Component.Suffix, 'foo', suffix='./so');
  testLoadCannotContainSep(expected=Component.Directory, 'foo', directory='./path/to');
  testLoadCannotContainSep(expected=Component.Prefix, 'foo', directory='./', prefix='foo/bar');
  testLoadCannotContainSep(expected=Component.Suffix, 'foo', directory='./', suffix='foo/bar');
}

proc testLoadFromPathMustContainSep(path: string) {
  try {
    const lib = loadFromPath(path);
  } catch e: IllegalArgumentError {
    assert(e.message().contains('path'));
    assert(e.message().contains(path));
  } catch e: DynLoadError {
    // Failed to load (OK).
  } catch e {
    try! throw e;
  }
}

proc test3() {
  writeln(Reflection.getRoutineName());

  testLoadFromPathMustContainSep('foo');
  testLoadFromPathMustContainSep('libFoo.so');
  testLoadFromPathMustContainSep('libFoo.so');
  testLoadFromPathMustContainSep('TestCLibraryToLoad.so');
  testLoadFromPathMustContainSep('./TestCLibraryToLoad.so');
}

proc test4() {
  writeln(Reflection.getRoutineName());

  assert(dynamicLibrary.prefix == 'lib');
  assert(dynamicLibrary.suffix == '.' + dynamicLibrary.fileExt);
}

proc main() {
  const tests = [ test1, test2, test3, test4 ];
  for test in tests {
    test();
    if test != tests.last then writeln();
  }
}
