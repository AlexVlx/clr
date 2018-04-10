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

hipError_t hipDeviceCanAccessPeer(int* canAccessPeer, hipCtx_t thisCtx, hipCtx_t peerCtx) {
  HIP_INIT_API(canAccessPeer, thisCtx, peerCtx);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipMemcpyPeer(void* dst, hipCtx_t dstCtx, const void* src, hipCtx_t srcCtx,
                         size_t sizeBytes) {
  HIP_INIT_API(dst, dstCtx, src, srcCtx, sizeBytes);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipMemcpyPeerAsync(void* dst, hipCtx_t dstDevice, const void* src, hipCtx_t srcDevice,
                              size_t sizeBytes, hipStream_t stream) {
  HIP_INIT_API(dst, dstDevice, src, srcDevice, sizeBytes, stream);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipDeviceCanAccessPeer(int* canAccessPeer, int deviceId, int peerDeviceId) {
  HIP_INIT_API(canAccessPeer, deviceId, peerDeviceId);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipDeviceDisablePeerAccess(int peerDeviceId) {
  HIP_INIT_API(peerDeviceId);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipDeviceEnablePeerAccess(int peerDeviceId, unsigned int flags) {
  HIP_INIT_API(peerDeviceId, flags);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipMemcpyPeer(void* dst, int dstDevice, const void* src, int srcDevice,
                         size_t sizeBytes) {
  HIP_INIT_API(dst, dstDevice, src, srcDevice, sizeBytes);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipMemcpyPeerAsync(void* dst, int dstDevice, const void* src, int srcDevice,
                              size_t sizeBytes, hipStream_t stream) {
  HIP_INIT_API(dst, dstDevice, src, srcDevice, sizeBytes, stream);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipCtxEnablePeerAccess(hipCtx_t peerCtx, unsigned int flags) {
  HIP_INIT_API(peerCtx, flags);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipCtxDisablePeerAccess(hipCtx_t peerCtx) {
  HIP_INIT_API(peerCtx);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}