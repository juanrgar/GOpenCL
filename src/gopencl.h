// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "gopencl-platform.h"
#include "gopencl-device.h"
#include "gopencl-context.h"
#include "gopencl-commandqueue.h"

#define GOPENCL_ERROR (g_quark_from_string ("GOPENCL_ERROR"))

#define GOPENCL_SUCCESS              0
#define GOPENCL_INVALID_VALUE       -1
#define GOPENCL_INVALID_PLATFORM    -2
#define GOPENCL_INVALID_DEVICE_TYPE -3
#define GOPENCL_DEVICE_NOT_FOUND    -4
#define GOPENCL_OUT_OF_RESOURCES    -5
#define GOPENCL_OUT_OF_HOST_MEMORY  -6
#define GOPENCL_INVALID_PROPERTY    -5

