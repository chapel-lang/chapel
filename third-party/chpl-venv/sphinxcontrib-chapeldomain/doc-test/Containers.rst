.. default-domain:: chpl

Module: Containers
==================
.. module:: Containers
.. class:: Vector
   
  <where the class level comment should live>

   .. attribute:: var eltType
   .. attribute:: var capacity: int(64)
      
      where and attribute comment will live...

      it can include line breaks
    
   .. attribute:: var lastIdx: int(64)
      
      where and attribute comment will live...

      it can include line breaks
    
   .. attribute:: var dom: domain(1)
      
      where and attribute comment will live...

      it can include line breaks
    
   .. attribute:: var elements: [.(this, "dom")] .(this, "eltType")
      
      where and attribute comment will live...

      it can include line breaks
    
   .. method:: proc Vector(type eltType, cap: _unknown, offset: _unknown)
      
    Where the method comment will live

    :arg type eltType: this is also a comment
    :arg int cap: me too
  

   .. method:: proc push(val: .(this, "eltType"))
      
    :rtype: eltType
    :returns: This is comment on push() metho
  

   .. method:: proc low()

   .. method:: proc high()

   .. method:: proc pop()

   .. method:: proc top() ref

   .. method:: proc this(idx) ref

   .. itermethod:: iter these() ref

   .. method:: proc size()

   .. method:: proc empty()

   inherited from object

