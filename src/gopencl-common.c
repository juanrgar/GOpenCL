// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.


#include "gopencl.h"
#include "xopencl.h"

gint
gopencl_format_error (cl_int cl_err,
                      GError **err)
{
    switch (cl_err) {
        case CL_INVALID_PLATFORM:
            g_set_error(err,
                        GOPENCL_ERROR,
                        GOPENCL_INVALID_PLATFORM,
                        "Invalid platform");
            return GOPENCL_INVALID_PLATFORM;
        case CL_INVALID_DEVICE_TYPE:
            g_set_error(err,
                        GOPENCL_ERROR,
                        GOPENCL_INVALID_DEVICE_TYPE,
                        "Invalid device type");
            return GOPENCL_INVALID_DEVICE_TYPE;
        case CL_INVALID_VALUE:
            g_set_error(err,
                        GOPENCL_ERROR,
                        GOPENCL_INVALID_VALUE,
                        "Invalid value");
        case CL_DEVICE_NOT_FOUND:
            g_set_error(err,
                        GOPENCL_ERROR,
                        GOPENCL_DEVICE_NOT_FOUND,
                        "Device not found");
            return GOPENCL_DEVICE_NOT_FOUND;
        case CL_OUT_OF_RESOURCES:
            g_set_error(err,
                        GOPENCL_ERROR,
                        GOPENCL_OUT_OF_RESOURCES,
                        "Out of resources");
            return GOPENCL_OUT_OF_RESOURCES;
        case CL_OUT_OF_HOST_MEMORY:
            g_set_error(err,
                        GOPENCL_ERROR,
                        GOPENCL_OUT_OF_HOST_MEMORY,
                        "Out of host memory");
            return GOPENCL_OUT_OF_HOST_MEMORY;
        default:
            return 0;
    }

    return 0;
}
