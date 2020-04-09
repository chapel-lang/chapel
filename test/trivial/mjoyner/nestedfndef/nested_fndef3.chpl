proc main() {

  proc nested() {
     var w : int = 3;
     writeln(w);
     deeplyNested();
          
    proc deeplyNested() {
      var x : int = 4;
      writeln(x);
      
      proc reallyDeeplyNested() {
        var y : int = 5;
        writeln(y);
        superNested(); 
        
        proc superNested() {
          var z : real = 6.0;
          writeln(z);
        }
      }
      reallyDeeplyNested();        
    }     
  }
  nested(); 
}

