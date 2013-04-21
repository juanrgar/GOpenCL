// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "xopencl.h"
#include "gopencl.h"
#include "gopencl-common.h"


G_DEFINE_TYPE (GopenclDevice, gopencl_device, G_TYPE_OBJECT);

#define GOPENCL_DEVICE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), GOPENCL_TYPE_DEVICE, GopenclDevicePrivate))
struct _GopenclDevicePrivate
{
    cl_device_id cl_device;
};


enum
{
    PROP_0,

    PROP_DEVICE_ID,
    PROP_DEVICE_EXTENSIONS,
    PROP_DEVICE_NAME,
    PROP_DEVICE_VENDOR
};

static void
gopencl_device_set_property (GObject *object,
                             guint property_id,
                             const GValue *value,
                             GParamSpec *pspec)
{
    GopenclDevice *self = GOPENCL_DEVICE(object);
    GopenclDevicePrivate *priv = GOPENCL_DEVICE_GET_PRIVATE(self);

    switch (property_id)
    {
    case PROP_DEVICE_ID:
        priv->cl_device = g_value_get_pointer(value);
        g_message("device id set %p\n", priv->cl_device);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

static void
gopencl_device_query_prop_str (GopenclDevice *self,
                               cl_device_info param_name,
                               GValue *value)
{
    GopenclDevicePrivate *priv = GOPENCL_DEVICE_GET_PRIVATE(self);
    size_t str_buf_len;
    gchar *str_buf;

    clGetDeviceInfo(priv->cl_device,
                    param_name,
                    0,
                    NULL,
                    &str_buf_len);
    str_buf = (gchar *) g_malloc(sizeof(gchar) * str_buf_len);
    clGetDeviceInfo(priv->cl_device,
                    param_name,
                    str_buf_len,
                    str_buf,
                    NULL);
    g_value_set_static_string(value, str_buf);
}

static void
gopencl_device_get_property (GObject *object,
                             guint property_id,
                             GValue *value,
                             GParamSpec *pspec)
{
    GopenclDevice *self = GOPENCL_DEVICE(object);
    GopenclDevicePrivate *priv = GOPENCL_DEVICE_GET_PRIVATE(self);

    switch (property_id)
    {
    case PROP_DEVICE_ID:
        g_value_set_pointer(value, priv->cl_device);
        break;
    case PROP_DEVICE_EXTENSIONS:
        gopencl_device_query_prop_str(self, CL_DEVICE_EXTENSIONS, value);
        break;
    case PROP_DEVICE_NAME:
        gopencl_device_query_prop_str(self, CL_DEVICE_NAME, value);
        break;
    case PROP_DEVICE_VENDOR:
        gopencl_device_query_prop_str(self, CL_DEVICE_VENDOR, value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

static void
gopencl_device_dispose (GObject *object)
{

    G_OBJECT_CLASS(gopencl_device_parent_class)->dispose(object);
}

static void
gopencl_device_finalize (GObject *object)
{
    GopenclDevice *self = GOPENCL_DEVICE(object);
    GopenclDevicePrivate *priv = GOPENCL_DEVICE_GET_PRIVATE(self);

    priv->cl_device = NULL;
    g_message("cl_device nulled\n");

    G_OBJECT_CLASS(gopencl_device_parent_class)->finalize(object);
}

static void
gopencl_device_class_init (GopenclDeviceClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    gobject_class->set_property = gopencl_device_set_property;
    gobject_class->get_property = gopencl_device_get_property;

    pspec = g_param_spec_pointer("id",
                                 "Device ID",
                                 "Set device's ID",
                                 G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    g_object_class_install_property(gobject_class,
                                    PROP_DEVICE_ID,
                                    pspec);

    pspec = g_param_spec_string("name",
                                "Device name",
                                "Device name string",
                                "no-name",
                                G_PARAM_READABLE);
    g_object_class_install_property(gobject_class,
                                    PROP_DEVICE_NAME,
                                    pspec);

//     pspec = g_param_spec_string("profile",
//                                 "Platform profile",
//                                 "OpenCL profile string",
//                                 "no-profile",
//                                 G_PARAM_READABLE);
//     g_object_class_install_property(gobject_class,
//                                     PROP_PLATFORM_PROFILE,
//                                     pspec);

    gobject_class->dispose = gopencl_device_dispose;
    gobject_class->finalize = gopencl_device_finalize;

    g_type_class_add_private(klass, sizeof(GopenclDevicePrivate));
}

static void
gopencl_device_init (GopenclDevice *self)
{
    GopenclDevicePrivate *priv;

    self->priv = priv = GOPENCL_DEVICE_GET_PRIVATE(self);

    priv->cl_device = NULL;
}

static GopenclDevice *
gopencl_device_new (cl_device_id cl_device)
{
    return g_object_new(GOPENCL_TYPE_DEVICE,
                        "id", cl_device,
                        NULL);
}

gint
gopencl_device_get_devices (GopenclPlatform     *platform,
                            gopencl_device_type device_type,
                            GList               **devices,
                            GError              **err)
{
    guint i;
    cl_uint cl_num_devices;
    cl_int cl_err = 0;

    if ((platform == NULL) || (!GOPENCL_IS_PLATFORM(platform))) {
        g_set_error(err,
                    GOPENCL_ERROR,
                    GOPENCL_INVALID_PLATFORM,
                    "Invalid platform");
        return GOPENCL_INVALID_PLATFORM;
    }

    if (devices == NULL) {
        g_set_error(err,
                    GOPENCL_ERROR,
                    GOPENCL_INVALID_VALUE,
                    "Invalid value for devices");
        return GOPENCL_INVALID_VALUE;
    }

    cl_platform_id cl_platform;
    g_object_get(platform, "id", &cl_platform, NULL);
    cl_err = clGetDeviceIDs(cl_platform, device_type, 0, NULL, &cl_num_devices);

    g_message("num devices returned %d\n", cl_num_devices);

    if (cl_num_devices == 0) {
        g_set_error(err,
                    GOPENCL_ERROR,
                    GOPENCL_DEVICE_NOT_FOUND,
                    "Device not found");
        return GOPENCL_DEVICE_NOT_FOUND;
    }

    cl_device_id cl_devices[cl_num_devices];
    cl_err = clGetDeviceIDs(cl_platform, device_type, cl_num_devices, cl_devices, NULL);
    if (cl_err != CL_SUCCESS) {
        return gopencl_format_error(cl_err, err);
    }

    for (i = 0; i < cl_num_devices; i++) {
        GopenclDevice *g_device = gopencl_device_new(cl_devices[i]);
        *devices = g_list_prepend(*devices, g_device);
    }

    return (gint) cl_num_devices;
}

gint
gopencl_device_get_all_devices (gopencl_device_type device_type,
                                GList               **devices,
                                GError              **error)
{
    GList *platforms = NULL;
    GError *err = NULL;
    gint platforms_found = 0;
    gint i, j;
    GList *devices_platform = NULL;
    gint devices_platform_i = 0;
    gint devices_found = 0;
    GopenclDevice *device = NULL;

    platforms_found = gopencl_platform_get_platforms(&platforms, &err);
    if (err) {
        *error = err;
        return platforms_found;
    }

    for (i = 0; i < platforms_found; i++) {
        GopenclPlatform *plat = GOPENCL_PLATFORM(g_list_nth_data(platforms, i));
        devices_platform_i = gopencl_device_get_devices(plat, 
                                                        device_type,
                                                        &devices_platform,
                                                        &err);
        if (err) {
            *error = err;
            return devices_platform_i;
        }

        devices_found += devices_platform_i;

        for (j = 0; j < devices_platform_i; j++) {
            device = GOPENCL_DEVICE(g_list_nth_data(devices_platform, j));
            *devices = g_list_prepend(*devices, device);
        }
    }

    return devices_found;
}

