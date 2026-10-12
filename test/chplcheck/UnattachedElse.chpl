module UnattachedElse {

  const condA = true,
        condB = false;


  if condA {

  } else if condB {

  } else {

  }

  if condA {

  }
  else if condB {

  }
  else {

  }

  if condA {

  }
  else if condB {

  }

  if condA {

  }
  else {

  }

  if condA {

  }  else if condB {

  }  else {

  }

  if condA {

  }else if condB {

  }else {

  }

  if condA {

  }
  // this comment should prevent the fixit
  else if condB {

  }
  else {

  }


  if condA {

  } else if condB {

  } /*this comment will also prevent the fixit*/ else {

  }

  if condA then {

  } else if condB then {

  } else {

  }

  if condA then
    ;
  else if condB then
    ;
  else
    ;

  var x = if condA then 1
    else 2;

}
