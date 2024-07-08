use Shlex;
//General tests
{
  var src = "hi there, my name is 'alice bob' and I am a 'programmer'";
  writeln(split(src));
  writeln(split(src, punctuation_chars = true));
  src = "echo \"please preserve      white space\"";
  writeln(split(src));
  var ans = split("abc 'def \"ghi xyz\" jkl' mno");
  writeln(ans);
  writeln(split(ans[1]));

  proc split_helper(s:string) {
    use List;
    var ret: list(string);
    var lex = new shlex(s);
    var tok = lex.get_token();
    //since posix is false, we just need to check for empty string as EOF
    while(tok != '') {
      ret.pushBack(tok);
      tok = lex.get_token();
    }
    return ret;
  }
  
  var lst = ['x', 'foo bar', '  foo  bar  ', 'foo   bar    bla     fasel',
            ' x y  z              xxxx', '\\x bar', '\\ x bar', '\\ bar', 
            'foo \\x bar', 'foo \\ bar', 'foo "bar" bla', '"foo" "bar" "bla"',
            '"foo" bar "bla"', '"foo" bar bla', "foo 'bar' bla",
            "'foo' 'bar' 'bla'", 'blurb foo"bar"bar"fasel" baz', ':-) ;-)',
            'foo#bar\nbaz', 'foo\\ bar', "'foo\\ bar'"];
  for ele in lst do writeln(split_helper(ele));
}

//Test semi-colon splitting
{
  for delimiter in (';', ';;', ';&', ';;&') do {
    var src = ['echo hi ' + delimiter + ' echo bye',
                    'echo hi' + delimiter + ' echo bye',
                    'echo hi' + delimiter + 'echo bye'];
    for ss in src do {
      var ans = split(ss, punctuation_chars = true);
      writeln(ans);
    }
  }
}

//Test AmpersandAndPipe
{
  for delimiter in ('&&', '&', '|&', ';&', ';;&', '||', '|', '&|', ';|', ';;|')
  do {
    var src = ['echo hi ' + delimiter + ' echo bye',
                    'echo hi' + delimiter + ' echo bye',
                    'echo hi' + delimiter + 'echo bye'];
    for ss in src do {
      var ans = split(ss, punctuation_chars = true);
      writeln(ans);
    }
  }
}

//Test SplitRedirect
{
  for delimiter in ('<', '|') do {
    var src = ['echo hi ' + delimiter + ' echo bye',
                    'echo hi' + delimiter + ' echo bye',
                    'echo hi' + delimiter + 'echo bye'];
    for ss in src do {
      var ans = split(ss, punctuation_chars = true);
      writeln(ans);
    }
  }
}

//Test SplitParen
{
  var src = ['( echo hi )','(echo hi)'];
  for ss in src do {
    var ans = split(ss, punctuation_chars = true);
    writeln(ans);
  }
}

//Test Syntax Split Custom
{
  var ss = "~/a && b-c --color=auto || d *.py?";
  var ans = split(ss, punctuation_chars = true, 
                  custom_punctuation_chars = '|');
  writeln(ans);
}

//Test Punctuation in Wordchars
{
  var src = 'a_b__c';
  var ans = split(src, custom_punctuation_chars = '_');
  writeln(ans);
}

//TestPunctuationWithPosix
{
  var src = 'f >"abc"';
  var ans = split(src, posix = true, punctuation_chars = true);
  writeln(ans);
  src = 'f >\\"abc\\"';
  ans = split(src, posix = true, punctuation_chars = true);
  writeln(ans);
}
