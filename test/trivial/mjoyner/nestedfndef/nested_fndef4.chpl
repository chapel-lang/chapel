def main() {

  def nested() {
     var w : int = 3;
     deeplyNested();
          
    def deeplyNested() {
      var x : int = 4;
      
      def reallyDeeplyNested() {
        var y : int = 5;
        superNested(); 
        
        def superNested() {
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
