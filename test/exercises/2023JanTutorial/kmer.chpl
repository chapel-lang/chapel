// kmer.chpl
/*
   usage on puma/ocelote:
     chpl kmer.chpl
     ./kmer

   usage on laptop with podman (or docker):
     podman run --rm -v "$PWD":/myapp -w /myapp chapel/chapel chpl kmer.chpl
     podman run --rm -v "$PWD":/myapp -w /myapp chapel/chapel ./kmer

   For docker usage, see https://chapel-lang.org/install-docker.html

   Original version of kmer counting algorithm provided by
   Aryamaan Jain (github strikeraryu) on Chapel Discourse April 7, 2021.

   kmer_large_input.txt is from
   https://www.ncbi.nlm.nih.gov/nuccore/NC_001422.1?report=fasta
 */

use Map;
use IO;

// to have it read a different input file, run as follows:
//      ./kmer --infile="anotherFileName"
config const infile = "kmer_large_input.txt";

// set k to something different on the commandline with
//      ./kmer --k=7
config const k = 4;

// read in the input sequence from the file infile
var sequence : string;
var f = open(infile, iomode.r);
var fReader =  f.reader();
fReader.read(sequence);

// declare a dictionary/map to store the count per kmer
var nkmerCounts : map(string, int);

// count up the number of times each kmer occurs
for ind in 0..<(sequence.size-k) {
  nkmerCounts[sequence[ind..<ind+k]] += 1;
}

writeln("Number of unique k-mers in ", infile, " is ", nkmerCounts.size);
writeln();
writeln("nkmerCounts = ");
writeln(nkmerCounts);
