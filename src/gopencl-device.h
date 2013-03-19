// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#ifndef __GOPENCL_DEVICE_H__
#define __GOPENCL_DEVICE_H__

#include <glib-object.h>

#define GOPENCL_TYPE_DEVICE               (gopencl_device_get_type())
#define GOPENCL_DEVICE(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOPENCL_TYPE_DEVICE, GopenclDevice))
#define GOPENCL_IS_DEVICE(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOPENCL_TYPE_DEVICE))
#define GOPENCL_DEVICE_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), GOPENCL_TYPE_DEVICE, GopenclDeviceClass))
#define GOPENCL_IS_DEVICE_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), GOPENCL_TYPE_DEVICE))
#define GOPENCL_DEVICE_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), GOPENCL_TYPE_DEVICE, GopenclDeviceClass))

typedef struct _GopenclDevice         GopenclDevice;
typedef struct _GopenclDevicePrivate  GopenclDevicePrivate;
typedef struct _GopenclDeviceClass    GopenclDeviceClass;

struct _GopenclDevice
{
    GObject parent_instance;

    GopenclDevicePrivate *priv;
};

struct _GopenclDeviceClass
{
    GObjectClass parent_class;
};

GType gopencl_device_get_type (void);

typedef enum _gopencl_device_type gopencl_device_type;

enum _gopencl_device_type {
    GOPENCL_DEVICE_TYPE_DEFAULT,
    GOPENCL_DEVICE_TYPE_GPU
};

gint
gopencl_platform_get_device_ids (GopenclPlatform *platform,
                                 gopencl_device_type device_type,
                                 gint num_entries,
                                 GList **devices,
                                 GError **err);

#endif
