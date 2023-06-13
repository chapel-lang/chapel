/* wordCount.chpl
   
  Purpose:
    For all of the files in a given directory, does a wordcount per file.
    An example of using Chapel for processing files in parallel.  This
    same code can do multi-core and/or distributed memory + multicore parallelism.

  Usage:
    Clone the main branch of the chapel compiler.
      > git clone https://github.com/chapel-lang/chapel.git

    Set up the environment to build the compiler and run Chapel programs
      > cd chapel
      > source util/setchplenv.bash

    Build the Chapel compiler, this will take awhile...
      > make

    Compile this example
      > cd test/studies/mstrout/
      > chpl wordCount.chpl

    Run this example
      > ./wordCount

    Doing wordCount on a different directory of files
      > ./wordCount --InputDir="path-to-diff-directory"

    Doing wordCount using a different number of threads per node
      > CHPL_RT_NUM_THREADS_PER_LOCALE=1 ./wordCount
      > CHPL_RT_NUM_THREADS_PER_LOCALE=2 ./wordCount
      > CHPL_RT_NUM_THREADS_PER_LOCALE=4 ./wordCount

    Running on multiple locales:
      See the notes at the end of this file.

  Credits:
    Mostly based on https://github.hpe.com/michelle-strout/CodeExamples/tree/main/Chapel/DataAnalysisExample/final-word-count.chpl,
    which was heavily edited by Ben McD.
*/
use IO;
use List;
use Map;
use FileSystem;
use Time;
use BlockDist;

config const inputDir = "InputFiles";
config const debug = true;
config const minCount = 2;      // minimum count for a word to show up in output

//-----------------------------------------------------------------------
// Read in the input csv files and at the same time do a wordcount per csv file.

var t: stopwatch;
t.start();

var filenamesList: list(string);

// findFiles() returns an iterator so putting all file names in a list
// to enable creating an array of file names.
for f in findFiles(inputDir) {
    filenamesList.pushBack(f);
}
// Create an array blocked into pieces per locale.
var filenames = Block.createArray(0..#filenamesList.size, string);
filenames = filenamesList;

// Execute using distributed parallelism across locales if executing
// using multilocales and within each locale in parallel.
forall (i, file) in zip(filenames.domain, filenames) {
  // Open up the input file.
  var f = open(file, ioMode.r);
  if debug then writeln("\n\n====== filename = ", file);

  // Create a reader of the input file
  var reader = f.reader(locking=false);

  // declare a Map to maintain the count
  var wordCount = new map(string,int);

  // Read in all of the lines of the file.
  var line : string;
  while reader.readLine(line) {
    // count each time a word appears in the abstract column of this line
    for word in line.split(" ") {
      var wordNoNewline = word.strip();
      if wordNoNewline.size > 0 then wordCount[wordNoNewline] += 1;
    }
  }

  // filter out any words that do not show up more than once
  var infrequentWords = new list(string);
  for (word,count) in zip(wordCount.keys(), wordCount.values()) {
    if count<=minCount then infrequentWords.pushBack(word);
  }
  for word in infrequentWords {
    wordCount.remove(word);
  }

  if debug then writeln("\nwordCount = ", wordCount);
}

// Uncomment if you want to time this program.
//writeln(t.elapsed());

/* 
  Notes on running in multi-locale mode
    See https://chapel-lang.org/docs/usingchapel/multilocale.html
    for more general notes.  These notes work for a Mac.

    The main ideas are that the chapel compiler and runtime need to
    be compiled to use a certain communication layer.  Additionally
    some other environment variables need to be set for those communication
    layers.  Below are the instructions for ofi and GASNET.
    It is recommended that for each set of instructions you create a new 
    terminal to start with unset environment variables.

  ofi
    // ofi uses the MPI launcher
    > brew install open-mpi
    > export CHPL_COMM=ofi
    > export CHPL_LAUNCHER=mpirun4ofi

    // to set up PATH and other environment variables
    > cd $CHPL_HOME
    > source util/setchplenv.bash

    // build compiler and runtime and runtime will link to ofi and mpi launcher
    > make 

    // need to indicate where open-mpi was installed by homebrew.
    // Found with `which mpirun` and then need the directory that has
    // `bin`, `lib`, and `include` subdirectories.
    export MPI_DIR=/usr/local  

    // Change into the directory with this example.
    > cd test/studies/mstrout
    > chpl wordCount.chpl

    // For some reason we need to have libfabric use the loopback interface
    // on the Mac.
    > export FI_TCP_IFACE=lo0

    > ./wordCount -nl 1
    > ./wordCount -nl 2

  GASNET
    // environment variables for GASNET
    export CHPL_COMM=gasnet
    export CHPL_TARGET_CPU=none
    export GASNET_SPAWNFN=L
    export GASNET_QUIET=yes
    export GASNET_ROUTE_OUTPUT=0
    export QT_AFFINITY=no
    export CHPL_QTHREAD_ENABLE_OVERSUBSCRIPTION=1
    export CHPL_RT_MASTERIP=127.0.0.1
    export CHPL_RT_WORKERIP=127.0.0.0
    unset CHPL_LAUNCHER

    // to set up PATH and other environment variables
    > cd $CHPL_HOME
    > source util/setchplenv.bash

    // build compiler and runtime with gasnet
    > make -j 4

    // Change into the directory with this example.
    > cd test/studies/mstrout
    > chpl wordCount.chpl

    > ./wordCount -nl 1
    > ./wordCount -nl 2

 */
