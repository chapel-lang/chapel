module HDF5Preprocessors {
  use HDF5;

  class AddNPreprocessor: HDF5Preprocessor {
    const n: int;

    override proc preprocess(A: []) {
      forall a in A {
        a += n;
      }
    }
  }

  class ScriptPreprocessor: HDF5Preprocessor {
    const script: string;

    override proc preprocess(A: []) {
      use FileSystem, Path, Spawn;

      try! {
        // opentmp() doesn't seem to give me a file I can get the name of :(
        //var f = opentmp();
        //const scriptName = f.realPath();

        const scriptName = "./hdf5TempScript.bash";
        var f = open(scriptName, iomode.cw);

        // write the script to a file
        {
          var writer = f.writer();
          writer.writeln(script);
          writer.flush();
          f.fsync();
        }
        f.close();

        // give the file executable permission
        chmod(scriptName, 0o755);

        // spawn the script, connect stdin and stdout
        var sub = spawn([scriptName], stdin=PIPE, stdout=PIPE);

        cobegin {
          // dump the array to the script's stdin
          {
            for a in A do
              sub.stdin.writeln(a);

            sub.stdin.close();
          }

          // read the array from the script's stdout
          for a in A {
            sub.stdout.readln(a);
          }
        }

        remove(scriptName);
      }
    }
  }
}
