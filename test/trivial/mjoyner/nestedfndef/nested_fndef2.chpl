fun main() {         
  nested();
       
  fun nested() {
    var x : int = 3;        
    writeln(x);          
    
    fun deeplyNested() {
      var y : int = 4;
      writeln(y);
    }
    deeplyNested();     
  }
}
