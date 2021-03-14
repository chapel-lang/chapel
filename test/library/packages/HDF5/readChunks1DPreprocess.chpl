use HDF5, HDF5Preprocessors, Hdf5PathHelp;

config const infileName = "readChunks1DInput.h5";
config const dsetName = "Ai";

const pathPrefix = readPrefixEnv();
if pathPrefix != "" {
  use FileSystem;
  copyFile(infileName, pathPrefix + infileName);
}

const script = """
#!/usr/bin/env bash
# Requires that stdin is a list of integers, one per line
# Writes the result back to stdout, one per line

while read line ; do
  # add 1 to the input and print the result
  line=$(($line + 1))
  echo $line
done < /dev/stdin
""";

var myScript = new owned ScriptPreprocessor(script);

for A in hdf5ReadChunks(pathPrefix+infileName, dsetName,
                        chunkShape={1..8}, eltType=int, preprocessor=myScript) {
  writeln(A);
}
