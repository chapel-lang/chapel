proc main() {

  proc nested() {
     var w : int = 3;
     deeplyNested();
          
    proc deeplyNested() {
      var x : int = 4;
      
      proc reallyDeeplyNested() {
        var y : int = 5;
        superNested(); 
        
        proc superNested() {
          var z : real = 6.0;
          writeln(w);
          writeln(x);
          writeln(y);
          writeln(z);
        }
      }
      reallyDeeplyNested();        
    }     
  }
  nested(); 
}
