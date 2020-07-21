use Spawn;

var baseDir = "endToEnd/";
var debug = false;

var shell1 = spawnshell("ls " + baseDir , stdout=PIPE);

var line1:string;
while shell1.stdout.readline(line1) {
  endToEndTest(line1);
}

shell1.wait();

proc endToEndTest(s: string) {
  // remove new line `\n`
  var package = s[0..s.size-2];
  if debug then writeln("Running for package " + package);

  var packageDir = baseDir + package;
  var protoFile = packageDir + "/protoFile/" + package + ".proto";
  var shell2 = spawnshell("protoc --chpl_out="+packageDir+" "+ protoFile);
  shell2.wait();
  shell2 = spawnshell("protoc -I="+packageDir+"/protoFile "+ "--python_out="+packageDir+" "+ protoFile);
  shell2.wait();

  if debug then writeln("Writing and reading from Chapel");
  shell2 = spawnshell("chpl -o "+packageDir+"/write "+packageDir+"/write.chpl");
  shell2.wait();
  shell2 = spawnshell(packageDir+"/./write");
  shell2.wait();
  shell2 = spawnshell("chpl -o "+packageDir+"/read "+packageDir+"/read.chpl");
  shell2.wait();
  shell2 = spawnshell(packageDir+"/./read", stdout=PIPE);
  while shell2.stdout.readline(line1) {
    write(line1);
  }
  shell2.wait(); 

  if debug then writeln("Writing from Chapel and reading from Python");
  shell2 = spawnshell("chpl -o "+packageDir+"/write "+packageDir+"/write.chpl");
  shell2.wait();
  shell2 = spawnshell(packageDir+"/./write");
  shell2.wait();
  shell2 = spawnshell("python3 "+packageDir+"/read.py", stdout=PIPE);
  while shell2.stdout.readline(line1) {
    write(line1);
  }
  shell2.wait(); 
  
  if debug then writeln("Writing from Python and reading from Chapel");
  shell2 = spawnshell("python3 "+packageDir+"/write.py");
  shell2.wait();
  shell2 = spawnshell("chpl -o "+packageDir+"/read "+packageDir+"/read.chpl");
  shell2.wait();
  shell2 = spawnshell(packageDir+"/./read", stdout=PIPE);
  while shell2.stdout.readline(line1) {
    write(line1);
  }
  shell2.wait();
  
  // clean files
  shell2 = spawnshell("rm -r out "+packageDir+"/write "+packageDir+"/read");
  shell2.wait();  
  shell2 = spawnshell("rm -r "+packageDir+"/"+package+".chpl "+packageDir+"/"+package+"_pb2.py");
  shell2.wait();
}
