def main() {         
  nested();
       
  def nested() {
    var x : int = 3;        
    writeln(x);          
    
    def deeplyNested() {
      var y : int = 4;
      writeln(y);
    }
    deeplyNested();     
  }
}
