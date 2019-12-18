/* Print the length of the longest common substring of two strings taken
   from input file "filename", then print a common substring of that length.  
   In computing the lcs, letters can be removed from either string, but not
   re-arranged.  For example, given strings "ABCBDAB" and "BDCABA", the lcs
   is 4 letters long ("BCBA").  This is written with a dynamic programming
   approach.
*/

config var filename = "strings";

const north = (-1, 0);
const northwest = (-1, -1);
const west = (0, -1);

proc main() {
  var s1, s2: string;
  var len1, len2: int;

  get_strings(s1, s2);
  len1 = (s1.length);
  len2 = (s2.length);

  var b: [-1..len1-1, -1..len2-1] (int, int);
  var c: [-1..len1-1, -1..len2-1] int;

  lcs_length(s1, s2, b, c);
  writeln("LCS length: ", c(len1-1, len2-1));
  print_lcs(b, s1, (len1-1, len2-1));
  writeln();
}

proc get_strings(out s1: string, out s2: string) {
  use IO;

  var infile = open(filename, iomode.r).reader();

  infile.read(s1);
  infile.read(s2);
  infile.close();
}

proc lcs_length(s1: string, s2: string, b, c) {
  /* Build up two tables, b, and c.  C(i,j) will contain
     the longest common substring length using characters up
     to s1(i), s2(j).  B contains directions to follow back to
     within the one of the strings to build an actual LCS.
  */
  var m = (s1.length);
  var n = (s2.length);
  for i in 0..#m {
    for j in 0..#n {
      if s1[i] == s2[j] {
        c(i,j) = c((i,j) + northwest) + 1;
        b(i,j) = northwest;
      } else if (c((i,j) + north) >= c((i,j) + west)) {
        c(i,j) = c((i,j) + north);
        b(i,j) = north;
      } else {
        c(i,j) = c((i,j) + west);
        b(i,j) = west;
      }
    }
  }
}

proc print_lcs(b, X, field: (int, int)) {
  /* Print a longest common substring using table b by following
     the directional pointers in b back through string X.
  */
  var (i, j) = field;

  if (i == -1 || j == -1) then
    return;

  if (b(field) == northwest) {
    print_lcs(b, X, field + b(field));
    write(X[i]);
  } else {
    print_lcs(b, X, field + b(field));
  }
}
