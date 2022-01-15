use Shlex;

//General strings checking
{
  var src="hi there, my name is 'alice bob' and I am a 'programmer'";
  writeln(split(src));
  writeln(split(src, punctuation_chars=true));
  src="echo \"please preserve      white space\"";
  writeln(split(src));
  var ans = split("abc 'def \"ghi xyz\" jkl' mno");
  writeln(ans);
  writeln(split(ans[1]));
  src="abc'def";
  // Splitting strings with unbalanced quotes correctly give errors
  // writeln(split(src)); 
}

//Test semi-colon splitting
{
  for delimiter in (';', ';;', ';&', ';;&') do {
    var src=['echo hi '+delimiter+' echo bye',
                    'echo hi'+delimiter+' echo bye',
                    'echo hi'+delimiter+'echo bye'];
    var res=['echo', 'hi', delimiter, 'echo', 'bye'];
    for ss in src do {
      var ans=split(ss, punctuation_chars=true);
      writeln(ss, ans, delimiter, res);
      assert(ans.size==res.size);
      for ele in 0..#ans.size do assert(res[ele]==ans[ele]);
    }
  }
}

//Test AmpersandAndPipe
{
  for delimiter in ('&&', '&', '|&', ';&', ';;&', '||', '|', '&|', ';|', ';;|')
  do {
    var src=['echo hi '+delimiter+' echo bye',
                    'echo hi'+delimiter+' echo bye',
                    'echo hi'+delimiter+'echo bye'];
    var res=['echo', 'hi', delimiter, 'echo', 'bye'];
    for ss in src do {
      var ans=split(ss, punctuation_chars=true);
      writeln(ss, ans, delimiter, res);
      assert(ans.size==res.size);
      for ele in 0..#ans.size do assert(res[ele]==ans[ele]);
    }
  }
}

//Test SplitRedirect
{
  for delimiter in ('<', '|') do {
    var src=['echo hi '+delimiter+' echo bye',
                    'echo hi'+delimiter+' echo bye',
                    'echo hi'+delimiter+'echo bye'];
    var res=['echo', 'hi', delimiter, 'echo', 'bye'];
    for ss in src do {
      var ans=split(ss, punctuation_chars=true);
      writeln(ss, ans, delimiter, res);
      assert(ans.size==res.size);
      for ele in 0..#ans.size do assert(res[ele]==ans[ele]);
    }
  }
}

//Test SplitParen
{
  var src = ['( echo hi )','(echo hi)'];
  var res = ['(', 'echo', 'hi', ')'];
  for ss in src do {
    var ans=split(ss, punctuation_chars=true);
    writeln(ss, ans, res);
    assert(ans.size == res.size);
    for ele in 0..#ans.size do assert(ans[ele]==res[ele]);
  }
}

//Test Syntax Split Custom
{
  var res = ['~/a', '&', '&', 'b-c', '--color=auto', '||', 'd', '*.py?'];
  var ss = "~/a && b-c --color=auto || d *.py?";
  var ans = split(ss, punctuation_chars=true, custom_punctuation_chars='|');
  writeln(ans);
  assert(ans.size == res.size);
  for ele in 0..#ans.size do assert(ans[ele]==res[ele]);
}

//Test Punctuation in Wordchars
{
  var src='a_b__c';
  var res=['a', '_', 'b', '__', 'c'];
  var ans = split(src, custom_punctuation_chars='_');
  writeln(ans);
  assert(ans.size == res.size);
  for ele in 0..#ans.size do assert(ans[ele]==res[ele]);
}

//TestPunctuationWithPosix
{
  var src='f >"abc"';
  var res=['f', '>', 'abc'];
  var ans = split(src, posix=true, punctuation_chars=true);
  writeln(ans);
  assert(ans.size == res.size);
  for ele in 0..#ans.size do assert(ans[ele]==res[ele]);
  src='f >\\"abc\\"';
  res=['f', '>', '"abc"'];
  ans = split(src, posix=true, punctuation_chars=true);
  writeln(ans);
  assert(ans.size == res.size);
  for ele in 0..#ans.size do assert(ans[ele]==res[ele]);
}

//TestEmptyStringHandling : TODO 
//(Since assigning NULL to strings in Posix mode does not work)
// {
//   var src='"")abc';
//   var res=['', ')', 'abc'];
//   for pun in [false, true] do {
//     var ans = split(src,posix=true, punctuation_chars=pun);
//     writeln(ans);
//     assert(ans.size == res.size);
//     for ele in 0..#ans.size do assert(ans[ele]==res[ele]);
//   }
//   res=["''", ')', 'abc'];
//   var ans= split(src, posix=false, punctuation_chars=true);
//   writeln(ans);
//   assert(ans.size == res.size);
//   for ele in 0..#ans.size do assert(ans[ele]==res[ele]);
// }

// {
//   var src=['', '""'];
//   for ss in src do {
//     var ans=split(ss, posix=true);
//     writeln(ans, ans.size);
//   }
//   for ss in src do {
//     var ans=split(ss, posix=false);
//     writeln(ans, ans.size);
//   }
// }

//Test Quote
{
  var safe = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  safe += '0123456789'+'@%_-+=:,./';
  assert(quote(safe) == safe);
  assert(quote('') == '""');
  assert(quote('test file name')=="'test file name'");
  var filename='somefile; rm -rf home';
  var command = 'ls -l '+(quote(filename));
  writeln(command);
  var r_comm= 'ssh home '+quote(command);
  writeln(r_comm);
}

//Test Join
{
  writeln(join(['a', 'b', 'c']));
  writeln(join(['echo', '-n', 'Multiple words']));
  writeln(join(['echo', 'filename; rm -rf home']));
  //Correctly gives error on passing a ref which is not a array of strings
  // or homogeneous list of strings
  // writeln(join([1, 2, 3 , 4]));
}