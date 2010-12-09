record R {
  union U {
    var i:int; var r:real;
    def R.uu() {} // for now, this is an error
  }
}
