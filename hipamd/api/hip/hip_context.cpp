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
#include "platform/runtime.hpp"
#include "utils/versions.hpp"

std::vector<amd::Context*> g_devices;

namespace hip {

thread_local amd::Context* g_context = nullptr;
thread_local std::stack<amd::Context*> g_ctxtStack;
thread_local hipError_t g_lastError = hipSuccess;
std::once_flag g_ihipInitialized;

std::map<amd::Context*, amd::HostQueue*> g_nullStreams;

void init() {
  if (!amd::Runtime::initialized()) {
    amd::Runtime::init();
  }

  const std::vector<amd::Device*>& devices = amd::Device::getDevices(CL_DEVICE_TYPE_GPU, false);

  for (unsigned int i=0; i<devices.size(); i++) {
    const std::vector<amd::Device*> device(1, devices[i]);
    amd::Context* context = new amd::Context(device, amd::Context::Info());
    if (!context) return;

    if (context && CL_SUCCESS != context->create(nullptr)) {
      context->release();
    } else {
      g_devices.push_back(context);
    }
  }
}

amd::Context* getCurrentContext() {
  return g_context;
}

void setCurrentContext(unsigned int index) {
  assert(index<g_devices.size());
  g_context = g_devices[index];
}

amd::HostQueue* getNullStream() {
  auto stream = g_nullStreams.find(getCurrentContext());
  if (stream == g_nullStreams.end()) {
    amd::Device* device = getCurrentContext()->devices()[0];
    cl_command_queue_properties properties = CL_QUEUE_PROFILING_ENABLE;
    amd::HostQueue* queue = new amd::HostQueue(*hip::getCurrentContext(), *device, properties,
                                               amd::CommandQueue::RealTimeDisabled,
                                               amd::CommandQueue::Priority::Normal);
    g_nullStreams[getCurrentContext()] = queue;
    return queue;
  }
  syncStreams();
  return stream->second;
}

};

using namespace hip;

hipError_t hipInit(unsigned int flags) {
  HIP_INIT_API(flags);

  HIP_RETURN(hipSuccess);
}

hipError_t hipCtxCreate(hipCtx_t *ctx, unsigned int flags,  hipDevice_t device) {
  HIP_INIT_API(ctx, flags, device);

  if (static_cast<size_t>(device) >= g_devices.size()) {
    HIP_RETURN(hipErrorInvalidValue);
  }

  *ctx = reinterpret_cast<hipCtx_t>(g_devices[device]);

  // Increment ref count for device primary context
  g_devices[device]->retain();
  g_ctxtStack.push(g_devices[device]);

  HIP_RETURN(hipSuccess);
}

hipError_t hipCtxSetCurrent(hipCtx_t ctx) {
  HIP_INIT_API(ctx);

  if (ctx == nullptr) {
    if(!g_ctxtStack.empty()) {
      g_ctxtStack.pop();
    }
  } else {
    hip::g_context = reinterpret_cast<amd::Context*>(as_amd(ctx));
    if(!g_ctxtStack.empty()) {
      g_ctxtStack.pop();
    }
    g_ctxtStack.push(hip::getCurrentContext());
  }

  HIP_RETURN(hipSuccess);
}

hipError_t hipCtxGetCurrent(hipCtx_t* ctx) {
  HIP_INIT_API(ctx);

  *ctx = reinterpret_cast<hipCtx_t>(hip::getCurrentContext());

  HIP_RETURN(hipSuccess);
}

hipError_t hipCtxGetSharedMemConfig(hipSharedMemConfig* pConfig) {
  HIP_INIT_API(pConfig);

  *pConfig = hipSharedMemBankSizeFourByte;

  HIP_RETURN(hipSuccess);
}

hipError_t hipRuntimeGetVersion(int *runtimeVersion) {
  HIP_INIT_API(runtimeVersion);

  if (!runtimeVersion) {
    HIP_RETURN(hipErrorInvalidValue);
  }

  *runtimeVersion = AMD_PLATFORM_BUILD_NUMBER;

  HIP_RETURN(hipSuccess);
}

hipError_t hipCtxDestroy(hipCtx_t ctx) {
  HIP_INIT_API(ctx);

  amd::Context* amdContext = reinterpret_cast<amd::Context*>(as_amd(ctx));
  if (amdContext == nullptr) {
    HIP_RETURN(hipErrorInvalidValue);
  }

  // Release last tracked command
  hip::getNullStream()->setLastQueuedCommand(nullptr);

  // Need to remove the ctx of calling thread if its the top one
  if (!g_ctxtStack.empty() && g_ctxtStack.top() == amdContext) {
    g_ctxtStack.pop();
  }

  // Remove context from global context list
  for (unsigned int i = 0; i < g_devices.size(); i++) {
    if (g_devices[i] == amdContext) {
      // Decrement ref count for device primary context
      amdContext->release();
    }
  }

  HIP_RETURN(hipSuccess);
}

hipError_t hipCtxPopCurrent(hipCtx_t* ctx) {
  HIP_INIT_API(ctx);

  amd::Context* amdContext = reinterpret_cast<amd::Context*>(as_amd(ctx));
  if (amdContext == nullptr) {
    HIP_RETURN(hipErrorInvalidContext);
  }

  if (!g_ctxtStack.empty()) {
    amdContext = g_ctxtStack.top();
    g_ctxtStack.pop();
  } else {
    HIP_RETURN(hipErrorInvalidContext);
  }

  HIP_RETURN(hipSuccess);
}

hipError_t hipCtxPushCurrent(hipCtx_t ctx) {
  HIP_INIT_API(ctx);

  amd::Context* amdContext = reinterpret_cast<amd::Context*>(as_amd(ctx));
  if (amdContext == nullptr) {
    HIP_RETURN(hipErrorInvalidContext);
  }

  hip::g_context = amdContext;
  g_ctxtStack.push(hip::getCurrentContext());

  HIP_RETURN(hipSuccess);
}

hipError_t hipDriverGetVersion(int* driverVersion) {
  HIP_INIT_API(driverVersion);

  auto* deviceHandle = g_devices[0]->devices()[0];
  const auto& info = deviceHandle->info();

  if (driverVersion) {
    *driverVersion = AMD_PLATFORM_BUILD_NUMBER * 100 +
                     AMD_PLATFORM_REVISION_NUMBER;
  } else {
    HIP_RETURN(hipErrorInvalidValue);
  }

  HIP_RETURN(hipSuccess);
}

hipError_t hipCtxGetDevice(hipDevice_t* device) {
  HIP_INIT_API(device);

  if (device != nullptr) {
    for (unsigned int i = 0; i < g_devices.size(); i++) {
      if (g_devices[i] == hip::getCurrentContext()) {
        *device = static_cast<hipDevice_t>(i);
        HIP_RETURN(hipSuccess);
      }
    }
  } else {
    HIP_RETURN(hipErrorInvalidValue);
  }

  return hipErrorUnknown;
}

hipError_t hipCtxGetApiVersion(hipCtx_t ctx, int* apiVersion) {
  HIP_INIT_API(apiVersion);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipCtxGetCacheConfig(hipFuncCache_t* cacheConfig) {
  HIP_INIT_API(cacheConfig);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipCtxSetCacheConfig(hipFuncCache_t cacheConfig) {
  HIP_INIT_API(cacheConfig);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipCtxSetSharedMemConfig(hipSharedMemConfig config) {
  HIP_INIT_API(config);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipCtxSynchronize(void) {
  HIP_INIT_API(1);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipCtxGetFlags(unsigned int* flags) {
  HIP_INIT_API(flags);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipDevicePrimaryCtxGetState(hipDevice_t dev, unsigned int* flags, int* active) {
  HIP_INIT_API(dev, flags, active);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipDevicePrimaryCtxRelease(hipDevice_t dev) {
  HIP_INIT_API(dev);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipDevicePrimaryCtxRetain(hipCtx_t* pctx, hipDevice_t dev) {
  HIP_INIT_API(pctx, dev);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipDevicePrimaryCtxReset(hipDevice_t dev) {
  HIP_INIT_API(dev);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipDevicePrimaryCtxSetFlags(hipDevice_t dev, unsigned int flags) {
  HIP_INIT_API(dev, flags);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}
