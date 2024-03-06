record GRD {
  type t = int;
}

record K {
  var myfield: GRD(?);
}

{
  writeln(isGeneric(K(?)));
  var kk = new K(new GRD(int));
}
