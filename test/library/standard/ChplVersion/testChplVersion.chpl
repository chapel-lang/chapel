use ChapelVersion;

proc writeit(param x) {
  writeln(x," : ", x.type:string);
}

write("chpl version ");
writeit(versionString);
writeln(versionTuple, " : ", versionTuple.type:string);
writeit(versionMajor);
writeit(versionMinor);
writeit(versionUpdate);
writeit(versionIsRelease);
writeit(versionSHA);
