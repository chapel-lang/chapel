proc main() {

  proc nested() {
     var w : int = 3;
     deeplyNested();
          
    proc deeplyNested() {
      var x : int = 4;
      writeln(w);
        
      proc reallyDeeplyNested() {
        var y : int = 5;
        writeln(x);
        superNested(); 
        
        proc superNested() {
          var z : real = 6.0;
          writeln(y);
          writeln(z);
        }
      }
      reallyDeeplyNested();        
    }     
  }
  nested(); 
}
