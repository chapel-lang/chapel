function main() {

  function nested() {
     var w : int = 3;
     deeplyNested();
          
    function deeplyNested() {
      var x : int = 4;
      writeln(w);
        
      function reallyDeeplyNested() {
        var y : int = 5;
        writeln(x);
        superNested(); 
        
        function superNested() {
          var z : float = 6.0;
          writeln(y);
          writeln(z);
        }
      }
      reallyDeeplyNested();        
    }     
  }
  nested(); 
}
