// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "gopencl-platform.h"
#include "gopencl-device.h"
#include "gopencl-context.h"
#include "gopencl-commandqueue.h"

#define GOPENCL_ERROR (g_quark_from_string ("GOPENCL_ERROR"))

enum gopencl_error {
    GOPENCL_SUCCESS = 0,

    GOPENCL_INVALID_VALUE,
    GOPENCL_INVALID_PLATFORM,
    GOPENCL_INVALID_DEVICE_TYPE,
    GOPENCL_DEVICE_NOT_FOUND,
    GOPENCL_OUT_OF_RESOURCES,
    GOPENCL_OUT_OF_HOST_MEMORY,
    GOPENCL_INVALID_DEVICE,
    GOPENCL_INVALID_CONTEXT
};

