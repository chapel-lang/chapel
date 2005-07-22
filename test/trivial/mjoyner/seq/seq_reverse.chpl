/* test to show sequence reverse is working */
function main() {
  var seq1 : seq of integer = (/ 5 /); 
  var sequ2 : seq of integer = (/ 4, 18 /);
  var seq3 : seq of integer =  (/ 1, 2, 3, 4, 5, 6, 7 /);  


  /* reverse the sequences */
  var rev_seq1 = reverse(seq1);
  var rev_seq2 = reverse(sequ2);
  var rev_seq3 = reverse(seq3);

  writeln("sequence 1 is ", seq1);
  writeln("the reverse of sequence 1 is ", rev_seq1); 
  writeln("sequence 2 is ", sequ2);
  writeln("the reverse of sequence 2 is ", rev_seq2);
  writeln("sequence 3 is ", seq3);
  writeln("the reverse of sequence 3 is ", rev_seq3);
}