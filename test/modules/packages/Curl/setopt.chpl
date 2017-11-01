use Curl;

proc main() {
  const uniprotid = "P07204_TRBM_HUMAN";
  const uniproturl = "http://www.uniprot.org/uniprot/" + uniprotid + ".fasta";
  var response = "";

  try! {
    var c = open(url=uniproturl, mode=iomode.r);
    c.setopt(curlopt_followlocation, 1);
    var reader = c.reader();
    var line: string;

    while (reader.readline(line)) do
      response += line;
  }

  writeln(uniprotid);
  writeln(response);
}
