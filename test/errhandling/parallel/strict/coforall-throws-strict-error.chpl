proc test() {
  coforall i in 1..10 {
    throw new Error(i:string);
  }
}


test();
