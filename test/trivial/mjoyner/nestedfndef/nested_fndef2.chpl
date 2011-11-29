proc main() {         
  nested();
       
  proc nested() {
    var x : int = 3;        
    writeln(x);          
    
    proc deeplyNested() {
      var y : int = 4;
      writeln(y);
    }
    deeplyNested();     
  }
}
