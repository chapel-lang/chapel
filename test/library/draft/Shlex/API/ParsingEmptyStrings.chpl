use Shlex;
use List;
{
  var src = '"")abc';
  for pun in [false, true] do {
    //Punctuation_chars set to false implies whitespace_split set to true
    //For testing purposes, we set whitespace_split to false in the next part
    var ans = split(src,posix = true, punctuation_chars = pun);
    writeln(ans);
  }

  //Setting punctuation_chars to false, and whitespace_split to false
  {
    var lex = new shlex(src,posix = true, punctuation_chars = false);
    lex.whitespace_split = false;
    var ans: list(string);
    var pres = " ";
    while(lex.tokindex != -1 || !lex.pushback.isEmpty()){
      pres = lex.get_token();
      if(lex.posix || pres != ""){
        ans.pushBack(pres);
      }
    }
    writeln(ans);
  }

  for pos in [false, true] do {
    var ans = split('""a', posix = pos);
    writeln(ans);
  }
  for pos in [false, true] do {
    for src in ['""', '""a', 'hi there, "" I am "Chapel Programmer"'] do {
      var ans = split(src, posix=pos);
      writeln(ans);
    }
  }
}
