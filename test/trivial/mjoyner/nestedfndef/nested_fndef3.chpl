fun main() {

  fun nested() {
     var w : int = 3;
     writeln(w);
     deeplyNested();
          
    fun deeplyNested() {
      var x : int = 4;
      writeln(x);
      
      fun reallyDeeplyNested() {
        var y : int = 5;
        writeln(y);
        superNested(); 
        
        fun superNested() {
          var z : float = 6.0;
          writeln(z);
        }
      }
      reallyDeeplyNested();        
    }     
  }
  nested(); 
}

