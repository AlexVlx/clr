/*
Copyright (c) 2015 - present Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <hip/hip_runtime.h>

#include "hip_internal.hpp"
#include <hip/hcc_detail/hip_surface_types.h>

struct hipSurface {
    hipArray* array;
    hipResourceDesc resDesc;
};

hipError_t hipCreateSurfaceObject(hipSurfaceObject_t* pSurfObject,
                                  const hipResourceDesc* pResDesc) {
  HIP_INIT_API(pSurfObject, pResDesc);

  assert(0 && "Unimplemented");

  HIP_RETURN(hipErrorUnknown);
}


hipError_t hipDestroySurfaceObject(hipSurfaceObject_t surfaceObject) {
  HIP_INIT_API(surfaceObject);

  assert(0 && "Unimplemented");

  HIP_RETURN(hipErrorUnknown);
}