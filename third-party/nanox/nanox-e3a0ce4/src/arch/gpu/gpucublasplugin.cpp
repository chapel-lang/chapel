/*************************************************************************************/
/*      Copyright 2009 Barcelona Supercomputing Center                               */
/*                                                                                   */
/*      This file is part of the NANOS++ library.                                    */
/*                                                                                   */
/*      NANOS++ is free software: you can redistribute it and/or modify              */
/*      it under the terms of the GNU Lesser General Public License as published by  */
/*      the Free Software Foundation, either version 3 of the License, or            */
/*      (at your option) any later version.                                          */
/*                                                                                   */
/*      NANOS++ is distributed in the hope that it will be useful,                   */
/*      but WITHOUT ANY WARRANTY; without even the implied warranty of               */
/*      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                */
/*      GNU Lesser General Public License for more details.                          */
/*                                                                                   */
/*      You should have received a copy of the GNU Lesser General Public License     */
/*      along with NANOS++.  If not, see <http://www.gnu.org/licenses/>.             */
/*************************************************************************************/

#include "plugin.hpp"

#include <iostream>

#include <cuda_runtime.h>
#include <cublas.h>


namespace nanos {
namespace ext {

class GPUCublasPlugin : public Plugin
{
   public:
      GPUCublasPlugin() : Plugin( "CUBLAS Warmup Plugin", 1 ) {}

      void config( Config& config ) {}

      void init()
      {
         cublasInit();
         cudaFree( NULL );
      }
};

}
}

nanos::ext::GPUCublasPlugin NanosXPlugin;

