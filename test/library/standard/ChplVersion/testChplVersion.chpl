use ChplVersion;

proc writeit(param x) {
  writeln(x," : ", x.type:string);
}

write("chpl version ");
writeit(CHPL_VERSION);
writeit(CHPL_VERSION_MAJOR);
writeit(CHPL_VERSION_MINOR);
writeit(CHPL_VERSION_UPDATE);
writeit(CHPL_VERSION_OFFICIAL);
writeit(CHPL_VERSION_SHA);
