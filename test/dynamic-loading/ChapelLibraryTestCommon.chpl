//
// Contains code that is shared between the host / loaded Chapel programs.
//
module ChapelLibraryTestCommon {
  public use ChapelDynamicLoading;
  public use ChapelProgramRegistration;
  public use CTypes;
  public use Reflection;

  config const hi = 512;
  const sep = '-' * 72;

  // Toggle this to 'true' when confident it will work.
  var printUsingWriteln = false;

  // TODO: We should provide this.
  inline proc chapelLibraryExtension param {
    use ChplConfig;
    if CHPL_TARGET_PLATFORM == 'darwin' then return 'dylib';
    return 'so';
  }

  inline proc chapelLibraryPath param {
    return './lib/libChapelLibrary.' + chapelLibraryExtension;
  }

  inline proc debugf() {
    extern proc printf(fmt): void;
    printf('\n');
  }

  inline proc debugf(args...?n) {
    use IO.FormattedIO;

    extern proc printf(fmt, args...): void;
    extern proc printf(fmt): void;

    for param i in 0..<args.size {
      const str = try! '%s'.format(args[i]);
      printf('%s', str.c_str());
    }

    printf('\n');
  }

  inline proc println(args...?n) {
    if printUsingWriteln {
      writeln((...args));
    } else {
      debugf((...args));
    }
  }

  inline proc println() {
    if printUsingWriteln {
      writeln();
    } else {
      debugf();
    }
  }

  inline proc preBufferPtrCache(param upto: int) {
    if upto > 0 {
      println('-- pre-buffering with ', upto, ' procedure pointers');

      // TODO: This loop should get unrolled, creating 'upto' different
      //       anonymous procedures. But I cannot test it quite yet.
      for param i in 0..<upto {
        const p = proc(): void {};
      }
    }
  }

  inline proc perProgramSetupInModuleInit(param preBufferPtrCacheUpto: int) {
    const isRoot = chpl_programInfoHere.id == chpl_programInfoHere.rootId;
    const prgDesc = if isRoot then 'EXECUTABLE' else 'LIBRARY';

    printTestHeader('SETUP (' + prgDesc + ')');

    const id = chpl_programInfoHere.id : int;
    println('-- program ID = ', id);

    const path = chpl_programInfoHere.path;
    println('-- program path = ', path);

    preBufferPtrCache(upto=preBufferPtrCacheUpto);

    println();
  }

  inline proc printfGreetingsHere() {
    extern proc printf(args...): void;
    const id = here.id : c_int;
    printf('Calling \'printf\' from locale: %d\n', id);
  }

  inline proc writelnGreetingsHere() {
    const id = here.id : int(64);
    writeln('Calling \'writeln\' from locale: ', id);
  }

  inline proc matches(expect: ?t, actual: t, param print=true): bool {
    if print {
      println('Expect: ', expect);
      println('Actual: ', actual);
    }
    return expect == actual;
  }

  inline proc printTestHeader(testName: string) {
    println(sep);
    println(testName);
    println(sep);
  }
}
