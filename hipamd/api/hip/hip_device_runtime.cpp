/*
Copyright (c) 2018 - present Advanced Micro Devices, Inc. All rights reserved.

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

hipError_t hipChooseDevice(int* device, const hipDeviceProp_t* properties) {

  HIP_INIT_API(device, properties);

  if (device == nullptr || properties == nullptr) {
    return hipErrorInvalidValue;
  }

  *device = 0;
  cl_uint maxMatchedCount = 0;
  int count = 0;
  ihipDeviceGetCount(&count);

  for (cl_int i = 0; i< count; ++i) {
    hipDeviceProp_t currentProp = {0};
    cl_uint validPropCount = 0;
    cl_uint matchedCount = 0;
    hipError_t err = hipGetDeviceProperties(&currentProp, i);
    if (properties->major != 0) {
      validPropCount++;
      if(currentProp.major >= properties->major) {
        matchedCount++;
      }
    }
    if (properties->minor != 0) {
      validPropCount++;
      if(currentProp.minor >= properties->minor) {
        matchedCount++;
      }
    }
    if(properties->totalGlobalMem != 0) {
        validPropCount++;
        if(currentProp.totalGlobalMem >= properties->totalGlobalMem) {
            matchedCount++;
        }
    }
    if(properties->sharedMemPerBlock != 0) {
        validPropCount++;
        if(currentProp.sharedMemPerBlock >= properties->sharedMemPerBlock) {
            matchedCount++;
        }
    }
    if(properties->maxThreadsPerBlock != 0) {
        validPropCount++;
        if(currentProp.maxThreadsPerBlock >= properties->maxThreadsPerBlock ) {
            matchedCount++;
        }
    }
    if(properties->totalConstMem != 0) {
        validPropCount++;
        if(currentProp.totalConstMem >= properties->totalConstMem ) {
            matchedCount++;
        }
    }
    if(properties->multiProcessorCount != 0) {
        validPropCount++;
        if(currentProp.multiProcessorCount >=
          properties->multiProcessorCount ) {
            matchedCount++;
        }
    }
    if(properties->maxThreadsPerMultiProcessor != 0) {
        validPropCount++;
        if(currentProp.maxThreadsPerMultiProcessor >=
          properties->maxThreadsPerMultiProcessor ) {
            matchedCount++;
        }
    }
    if(properties->memoryClockRate != 0) {
        validPropCount++;
        if(currentProp.memoryClockRate >= properties->memoryClockRate ) {
            matchedCount++;
        }
    }
    if(properties->memoryBusWidth != 0) {
        validPropCount++;
        if(currentProp.memoryBusWidth >= properties->memoryBusWidth ) {
            matchedCount++;
        }
    }
    if(properties->l2CacheSize != 0) {
        validPropCount++;
        if(currentProp.l2CacheSize >= properties->l2CacheSize ) {
            matchedCount++;
        }
    }
    if(properties->regsPerBlock != 0) {
        validPropCount++;
        if(currentProp.regsPerBlock >= properties->regsPerBlock ) {
            matchedCount++;
        }
    }
    if(properties->maxSharedMemoryPerMultiProcessor != 0) {
        validPropCount++;
        if(currentProp.maxSharedMemoryPerMultiProcessor >=
          properties->maxSharedMemoryPerMultiProcessor ) {
            matchedCount++;
        }
    }
    if(properties->warpSize != 0) {
        validPropCount++;
        if(currentProp.warpSize >= properties->warpSize ) {
            matchedCount++;
        }
    }
    if(validPropCount == matchedCount) {
      *device = matchedCount > maxMatchedCount ? i : *device;
      maxMatchedCount = std::max(matchedCount, maxMatchedCount);
    }
  }

  return hipSuccess;
}

hipError_t hipDeviceGetAttribute(int* pi, hipDeviceAttribute_t attr, int device) {

  HIP_INIT_API(pi, attr, device);

  if (pi == nullptr) {
    return hipErrorInvalidValue;
  }

  int count = 0;
  ihipDeviceGetCount(&count);
  if (device < 0 || device >= count) {
    return hipErrorInvalidDevice;
  }

  //FIXME: should we cache the props, or just select from deviceHandle->info_?
  hipDeviceProp_t prop = {0};
  hipError_t err = hipGetDeviceProperties(&prop, device);
  if (err != hipSuccess) return err;

  switch (attr) {
  case hipDeviceAttributeMaxThreadsPerBlock:
    *pi = prop.maxThreadsPerBlock;
    break;
  case hipDeviceAttributeMaxBlockDimX:
    *pi = prop.maxThreadsDim[0];
    break;
  case hipDeviceAttributeMaxBlockDimY:
    *pi = prop.maxThreadsDim[1];
    break;
  case hipDeviceAttributeMaxBlockDimZ:
    *pi = prop.maxThreadsDim[2];
    break;
  case hipDeviceAttributeMaxGridDimX:
    *pi = prop.maxGridSize[0];
    break;
  case hipDeviceAttributeMaxGridDimY:
    *pi = prop.maxGridSize[1];
    break;
  case hipDeviceAttributeMaxGridDimZ:
    *pi = prop.maxGridSize[2];
    break;
  case hipDeviceAttributeMaxSharedMemoryPerBlock:
    *pi = prop.sharedMemPerBlock;
    break;
  case hipDeviceAttributeTotalConstantMemory:
    *pi = prop.totalConstMem;
    break;
  case hipDeviceAttributeWarpSize:
    *pi = prop.warpSize;
    break;
  case hipDeviceAttributeMaxRegistersPerBlock:
    *pi = prop.regsPerBlock;
    break;
  case hipDeviceAttributeClockRate:
    *pi = prop.clockRate;
    break;
  case hipDeviceAttributeMemoryClockRate:
    *pi = prop.memoryClockRate;
    break;
  case hipDeviceAttributeMemoryBusWidth:
    *pi = prop.memoryBusWidth;
    break;
  case hipDeviceAttributeMultiprocessorCount:
    *pi = prop.multiProcessorCount;
    break;
  case hipDeviceAttributeComputeMode:
    *pi = prop.computeMode;
    break;
  case hipDeviceAttributeL2CacheSize:
    *pi = prop.l2CacheSize;
    break;
  case hipDeviceAttributeMaxThreadsPerMultiProcessor:
    *pi = prop.maxThreadsPerMultiProcessor;
    break;
  case hipDeviceAttributeComputeCapabilityMajor:
    *pi = prop.major;
    break;
  case hipDeviceAttributeComputeCapabilityMinor:
    *pi = prop.minor;
    break;
  case hipDeviceAttributePciBusId:
    *pi = prop.pciBusID;
    break;
  case hipDeviceAttributeConcurrentKernels:
    *pi = prop.concurrentKernels;
    break;
  case hipDeviceAttributePciDeviceId:
    *pi = prop.pciDeviceID;
    break;
  case hipDeviceAttributeMaxSharedMemoryPerMultiprocessor:
    *pi = prop.maxSharedMemoryPerMultiProcessor;
    break;
  case hipDeviceAttributeIsMultiGpuBoard:
    *pi = prop.isMultiGpuBoard;
    break;
  default:
    return hipErrorInvalidValue;
  }

  return hipSuccess;
}

hipError_t hipDeviceGetByPCIBusId(int* device, const char*pciBusIdstr) {

  HIP_INIT_API(device, pciBusIdstr);

  if (device == nullptr || pciBusIdstr == nullptr) {
    return hipErrorInvalidValue;
  }

  int pciBusID = -1;
  int pciDeviceID = -1;
  int pciDomainID = -1;

  if (sscanf (pciBusIdstr, "%04x:%02x:%02x", &pciDomainID, &pciBusID, &pciDeviceID) == 0x3) {
    int count = 0;
    ihipDeviceGetCount(&count);
    for (cl_int i = 0; i < count; i++) {
      int pi = 0;
      hipDevice_t dev;
      hipDeviceGet(&dev, i);
      hipDeviceGetAttribute(&pi, hipDeviceAttributePciBusId, dev);

      if (pciBusID == pi) {
        *device = i;
        break;
      }
    }
  }

  return hipSuccess;
}

hipError_t hipDeviceGetCacheConfig ( hipFuncCache_t * cacheConfig ) {
  HIP_INIT_API(cacheConfig);

  if(cacheConfig == nullptr) {
    return hipErrorInvalidValue;
  }

  *cacheConfig = hipFuncCache_t();

  return hipSuccess;
}

hipError_t hipDeviceGetLimit ( size_t* pValue, hipLimit_t limit ) {

  HIP_INIT_API(pValue, limit);

  if(pValue == nullptr) {
    return hipErrorInvalidValue;
  }
  if(limit == hipLimitMallocHeapSize) {
    hipDeviceProp_t prop;
    hipGetDeviceProperties(&prop, 0);

    *pValue = prop.totalGlobalMem;
    return hipSuccess;
  } else {
    return hipErrorUnsupportedLimit;
  }
}

/**
hipError_t hipDeviceGetP2PAttribute ( int* value, hipDeviceP2PAttr attr, int  srcDevice, int  dstDevice ) {
  assert(0);
  return hipSuccess;
}
**/

hipError_t hipDeviceGetPCIBusId ( char* pciBusId, int  len, int  device ) {

  HIP_INIT_API((void*)pciBusId, len, device);

  int count;
  ihipDeviceGetCount(&count);
  if (device < 0 || device > count) {
    return hipErrorInvalidDevice;
  }

  if (pciBusId == nullptr || len < 0) {
    return hipErrorInvalidValue;
  }

  hipDeviceProp_t prop;
  hipGetDeviceProperties(&prop, device);

  snprintf (pciBusId, len, "%04x:%02x:%02x.0",
                    prop.pciDomainID,
                    prop.pciBusID,
                    prop.pciDeviceID);

  return hipSuccess;
}

hipError_t hipDeviceGetSharedMemConfig ( hipSharedMemConfig * pConfig ) {
  HIP_INIT_API(pConfig);

  *pConfig = hipSharedMemBankSizeFourByte;

  return hipSuccess;
}

hipError_t hipDeviceGetStreamPriorityRange ( int* leastPriority, int* greatestPriority ) {
  assert(0);
  return hipSuccess;
}

hipError_t hipDeviceReset ( void ) {
  HIP_INIT_API();

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipDeviceSetCacheConfig ( hipFuncCache_t cacheConfig ) {
  HIP_INIT_API(cacheConfig);

  // No way to set cache config yet.

  return hipSuccess;
}

hipError_t hipDeviceSetLimit ( hipLimit_t limit, size_t value ) {
  return hipErrorUnknown;
}

hipError_t hipDeviceSetSharedMemConfig ( hipSharedMemConfig config ) {
  HIP_INIT_API(config);

  // No way to set cache config yet.

  return hipSuccess;
}

hipError_t hipDeviceSynchronize ( void ) {
  return hipSuccess;
}

hipError_t hipGetDevice ( int* deviceId ) {
  HIP_INIT_API(deviceId);

  if (deviceId != nullptr) {
    for (unsigned int i = 0; i < g_devices.size(); i++) {
      if (g_devices[i] == g_context) {
        *deviceId = i;
        return hipSuccess;
      }
    }
  } else {
    return hipErrorInvalidValue;
  }

  return hipErrorUnknown;
}

hipError_t hipGetDeviceCount ( int* count ) {
  HIP_INIT_API(count);

  return ihipDeviceGetCount(count);
}

hipError_t hipGetDeviceFlags ( unsigned int* flags ) {
  return hipErrorUnknown;
}

hipError_t hipIpcGetEventHandle ( hipIpcEventHandle_t* handle, hipEvent_t event ) {
  HIP_INIT_API(handle, event);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipIpcOpenEventHandle ( hipEvent_t* event, hipIpcEventHandle_t handle ) {
  HIP_INIT_API(event, handle);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

hipError_t hipSetDevice ( int  device ) {
  HIP_INIT_API(device);

  if (static_cast<unsigned int>(device) < g_devices.size()) {
    g_context = g_devices[device];

    return hipSuccess;
  }
  return hipErrorInvalidValue;
}

hipError_t hipSetDeviceFlags ( unsigned int  flags ) {
  HIP_INIT_API(flags);

  assert(0 && "Unimplemented");

  return hipSuccess;
}

hipError_t hipSetValidDevices ( int* device_arr, int  len ) {
  HIP_INIT_API(device_arr, len);

  assert(0 && "Unimplemented");

  return hipErrorUnknown;
}

