.. default-domain:: chpl

.. module:: Norm
   :synopsis: Matrix and Vector Norms

Norm
====
**Usage**

.. code-block:: chapel

   use Norm;


Matrix and Vector Norms

The Norm module currently defines:

  * 1-norm, 2-norm, infinity norm and Frobenius norm for 1D arrays
  * 1-norm, infinity norm and Frobenius norm for 2D arrays

The :enum:`normType` argument for the first few overloads of :proc:`!norm` is
an enumeration indicating the norm to compute.

The norm function may also be called without specifying the :enum:`normType`, in
which case the default norm for that array rank will be used.


.. enum:: enum normType { norm1, norm2, normInf, normFrob }

   
   Indicates the different types of norms supported by this module:
   
     * 1-norm
     * 2-norm
     * Infinity norm
     * Frobenius norm
   


.. function:: proc norm(x: [], p: normType)

   
   Compute the norm indicated by `p` on the 1D array `x`.
   
   :rtype: x.eltType
   

.. function:: proc norm(x: [?D], p: normType)

   
   Compute the norm indicated by `p` on the 2D array `x`.
   
   `p` cannot be `normType.norm2`.
   
   :rtype: x.eltType
   

.. function:: proc norm(x: [])

   
   Compute the default norm on `x`.
   
   For a 1D array this is the 2-norm, for a 2D array, this is the Frobenius
   norm.
   
   :rtype: x.eltType
   

