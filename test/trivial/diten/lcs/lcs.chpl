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

def main() {
  var s1, s2: string;
  var len1, len2: int;

  get_strings(s1, s2);
  len1 = length(s1);
  len2 = length(s2);

  var b: [0..len1, 0..len2] (int, int);
  var c: [0..len1, 0..len2] int;

  lcs_length(s1, s2, b, c);
  writeln("LCS length: ", c(len1, len2));
  print_lcs(b, s1, (len1, len2));
  writeln();
}

def get_strings(out s1: string, out s2: string) {
  var infile = new file(filename, "r", ".");

  infile.open();
  infile.read(s1);
  infile.read(s2);
  infile.close();
}

def lcs_length(s1: string, s2: string, b, c) {
  /* Build up two tables, b, and c.  C(i,j) will contain
     the longest common substring length using characters up
     to s1(i), s2(j).  B contains directions to follow back to
     within the one of the strings to build an actual LCS.
  */
  var m = length(s1);
  var n = length(s2);
  for i in 1..m {
    for j in 1..n {
      if s1.substring(i) == s2.substring(j) {
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

def print_lcs(b, X, field: (int, int)) {
  /* Print a longest common substring using table b by following
     the directional pointers in b back through string X.
  */
  var (i, j) = field;

  if (i == 0 || j == 0) then
    return;

  if (b(field) == northwest) {
    print_lcs(b, X, field + b(field));
    write(X.substring(i));
  } else {
    print_lcs(b, X, field + b(field));
  }
}
