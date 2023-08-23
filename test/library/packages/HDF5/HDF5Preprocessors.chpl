module HDF5Preprocessors {
  use HDF5;

  class AddNPreprocessor: HDF5Preprocessor {
    const n: int;

    override proc preprocess(ref A: []) {
      forall a in A {
        a += n;
      }
    }
  }

  class ScriptPreprocessor: HDF5Preprocessor {
    const script: string;

    override proc preprocess(ref A: []) {
      use FileSystem, OS.POSIX, Path, Subprocess;

      try! {
        // openTempFile() doesn't seem to give me a file I can get the name of :(
        //var f = openTempFile();
        //const scriptName = f.realPath();

        const scriptName = "./hdf5TempScript.bash";
        var f = open(scriptName, ioMode.cw);

        // write the script to a file
        {
          var writer = f.writer();
          writer.writeln(script);
          writer.flush();
          f.fsync();
        }
        f.close();

        // give the file executable permission
        chmod(scriptName.c_str(), 0o755:mode_t);

        // spawn the script, connect stdin and stdout
        var sub = spawn([scriptName], stdin=pipeStyle.pipe, stdout=pipeStyle.pipe);

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
