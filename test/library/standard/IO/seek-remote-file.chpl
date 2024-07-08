use IO;

{
  var outfile = open("out.txt", ioMode.cw);
  coforall i in 0..9 {
    on Locales[i % Locales.size] {
      var myWriter = outfile.writer(locking=false);
      myWriter.seek(i*2..#2);
      myWriter.write(i, " ");
    }
  }
}

{
  var infile = open("out.txt", ioMode.r);
  on Locales[numLocales-1] {
    var myReader = infile.reader(locking=false);
    myReader.seek(2..#2);
    var i = myReader.read(int);
    myReader.seek(7..#2);
    var j = myReader.read(int);
    writeln((i,j));
  }
}
