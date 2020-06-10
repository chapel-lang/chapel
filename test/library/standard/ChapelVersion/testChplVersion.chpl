use Version;

proc writeit(param x) {
  writeln(x," : ", x.type:string);
}

writeln(chplVersion);
writeit(chplVersion.major);
writeit(chplVersion.minor);
writeit(chplVersion.update);
writeit(chplVersion.commit);
