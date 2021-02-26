use Spawn;
use FileSystem;

var baseDir = "endToEnd/";
var debug = false;

proc endToEndTest(package: string) {
  var line1:string;

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
  shell2 = spawnshell(packageDir+"/./write -nl 1");
  shell2.wait();
  shell2 = spawnshell("chpl -o "+packageDir+"/read "+packageDir+"/read.chpl");
  shell2.wait();
  shell2 = spawnshell(packageDir+"/./read -nl 1", stdout=PIPE);
  while shell2.stdout.readline(line1) {
    if(line1.strip() != "true") then writeln("Chapel to Chapel failed");
  }
  shell2.wait(); 

  if debug then writeln("Writing from Chapel and reading from Python");
  shell2 = spawnshell("chpl -o "+packageDir+"/write "+packageDir+"/write.chpl");
  shell2.wait();
  shell2 = spawnshell(packageDir+"/./write -nl 1");
  shell2.wait();
  shell2 = spawnshell("python3 "+packageDir+"/read.py", stdout=PIPE);
  while shell2.stdout.readline(line1) {
    if(line1.strip() != "true") then writeln("Chapel to Python failed");
  }
  shell2.wait(); 
  
  if debug then writeln("Writing from Python and reading from Chapel");
  shell2 = spawnshell("python3 "+packageDir+"/write.py");
  shell2.wait();
  shell2 = spawnshell("chpl -o "+packageDir+"/read "+packageDir+"/read.chpl");
  shell2.wait();
  shell2 = spawnshell(packageDir+"/./read -nl 1", stdout=PIPE);
  while shell2.stdout.readline(line1) {
    if(line1.strip() != "true") then writeln("Python to Chapel failed");
  }
  shell2.wait();
  
  // clean files
  shell2 = spawnshell("rm -r out "+packageDir+"/write "+packageDir+"/read");
  shell2.wait();  
  shell2 = spawnshell("rm -r "+packageDir+"/"+package+".chpl "+packageDir+"/"+package+"_pb2.py");
  shell2.wait();
}
