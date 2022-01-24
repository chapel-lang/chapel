use Shlex;
// TestEmptyStringHandling : TODO 
// (Since assigning NULL to strings in Posix mode does not work)
{
  var src='"")abc';
  for pun in [false, true] do {
    var ans = split(src,posix=true, punctuation_chars=pun);
    writeln(ans);
  }

  for pos in [false, true] do {
    var ans=split('""', posix=pos);
    writeln(ans);
  }
}
