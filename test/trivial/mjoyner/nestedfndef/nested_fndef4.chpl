function main() {

  function nested() {
     var w : int = 3;
     deeplyNested();
          
    function deeplyNested() {
      var x : int = 4;
      
      function reallyDeeplyNested() {
        var y : int = 5;
        superNested(); 
        
        function superNested() {
          var z : float = 6.0;
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
