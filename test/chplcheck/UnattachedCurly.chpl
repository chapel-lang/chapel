module UnattachedCurly{

  module M1  {

    interface I
    {

    }
    interface I2{ }

    record myRec {
      enum foo // my comment
      {
        a, b, c
      }
    }
    record myRecBad
    {
      enum bar
      {
        a, b, c
      }
      enum other{
        d, e, f
      }
    }
    record myRecWithI : I, I2{

    }
    record myRecWithIBad : I, I2
    {
      proc foo()
      {

      }
    }
    proc myRecWithI.mySecondary() where false
    {

    }
    proc myRecWithI.mySecondary()
    where true{

    }


    class MyClass
    {

    }
    class MyInherits /*yay inheritance*/ : MyClass{

    }

    class AnotherChild : MyClass {
      proc myFunc()
      {

      }
    }

  }
  module M2
  {
    proc foo(a)  {
      if a < 5
      {

      } else
      {

      }
    }
  }
  module M3 // start the module
  {
    proc myFunc()
    {

    }
    proc someOtherFunc() // I weirdly commented this
    {

    }
    proc iHaveIntents() type{
      return int;
    }
    proc iHaveThrows() : int throws{
      return 1;
    }
    proc someFuncWithArgs(a: int, b: int) where a.type == b.type {

    }
    proc someFuncWithArgsP2(a: int, b: int) where a.type == b.type{
      proc myNestedFunc() const : int throws
      {
        if a < b
        {

        } else
        {

        }
        return 1;
      }
    }
    proc someFuncWithArgsP3(a: int, b: int)
    where a.type == b.type {
      proc myNestedFunc where true {
        if a < b
        {

        } else if b > a
        {

        }
        else // what a silly conditional
        {

        }
      }
      proc dontBreakIt() { return 5; }
      proc myNestedFunc where false{
        try!
        {

        }
        catch e: Error
        {

        }
        catch (e: Error)
        {

        }
        catch
        {

        }
        try!{

        }
        catch e: Error{

        }
        catch (e: Error){ // this is fine

        }
        catch{

        }
        var y = try! dontBreakIt();
      }
    }
    proc someFuncWithArgsP4(a: int, b: int)
      where a.type == b.type
      {
      try{

      } catch // NO
      {
        halt("oops");
      }
    }
  }
}
