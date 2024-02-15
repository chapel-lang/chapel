// kmer.chpl
/*
   usage on puma/ocelote:
     chpl kmer.chpl
     ./kmer

   usage on laptop with podman (or docker):
     podman run --rm -v "$PWD":/myapp -w /myapp chapel/chapel chpl kmer.chpl
     podman run --rm -v "$PWD":/myapp -w /myapp chapel/chapel ./kmer

     # can change the infilename on command line because it is a configuration const
     podman run --rm -v "$PWD":/myapp -w /myapp chapel/chapel ./kmer --infilename="kmer.chpl"

   For docker usage, see https://chapel-lang.org/install-docker.html

   Original version of kmer counting algorithm provided by
   Aryamaan Jain (github strikeraryu) on Chapel Discourse April 7, 2021.

   kmer_large_input.txt is from
   https://www.ncbi.nlm.nih.gov/nuccore/NC_001422.1?report=fasta
 */

use Map, IO;

// to have it read a different input file, run as follows:
//      ./kmer --infilename="anotherFileName"
config const infilename = "kmer_large_input.txt";

// set k to something different on the commandline with
//      ./kmer --k=7
config const k = 4;

// read in the input sequence from the file infile and strip out newlines
var sequence, line : string;
var infile =  openReader(infilename);
while infile.readLine(line, stripNewline=true) {
  sequence += line;
}

// declare a dictionary/map to store the count per kmer
var nkmerCounts: map(string, int);

// count up the number of times each kmer occurs
for ind in 0..<(sequence.size-k) {
  nkmerCounts[sequence[ind..#k]] += 1;
}

writeln("Number of unique k-mers in ", infilename, " is ", nkmerCounts.size);
writeln("\nCounts:");
for kmer in nkmerCounts.keys() {
  writeln(kmer, " : ", nkmerCounts[kmer]);
}
