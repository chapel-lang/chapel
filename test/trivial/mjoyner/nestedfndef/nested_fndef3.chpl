function main() {

  function nested() {
     var w : int = 3;
     writeln(w);
     deeplyNested();
          
    function deeplyNested() {
      var x : int = 4;
      writeln(x);
      
      function reallyDeeplyNested() {
        var y : int = 5;
        writeln(y);
        superNested(); 
        
        function superNested() {
          var z : float = 6.0;
          writeln(z);
        }
      }
      reallyDeeplyNested();        
    }     
  }
  nested(); 
}

