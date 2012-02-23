/*
 * Copyright/Licensing information.
 */

#ifndef __GOPENCL_PLATFORM_H__
#define __GOPENCL_PLATFORM_H__

#include <glib-object.h>

#define GOPENCL_TYPE_PLATFORM               (gopencl_platform_get_type())
#define GOPENCL_PLATFORM(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOPENCL_TYPE_PLATFORM, GopenclPlatform))
#define GOPENCL_IS_PLATFORM(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOPENCL_TYPE_PLATFORM))
#define GOPENCL_PLATFORM_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), GOPENCL_TYPE_PLATFORM, GopenclPlatformClass))
#define GOPENCL_IS_PLATFORM_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), GOPENCL_TYPE_PLATFORM))
#define GOPENCL_PLATFORM_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), GOPENCL_TYPE_PLATFORM, GopenclPlatformClass))

typedef struct _GopenclPlatform         GopenclPlatform;
typedef struct _GopenclPlatformPrivate  GopenclPlatformPrivate;
typedef struct _GopenclPlatformClass    GopenclPlatformClass;

struct _GopenclPlatform
{
    GObject parent_instance;

    GopenclPlatformPrivate *priv;
};

struct _GopenclPlatformClass
{
    GObjectClass parent_class;
};

GType gopencl_platform_get_type (void);

gint
gopencl_platform_get_platform_ids (gint num_entries,
                                   GList **platforms,
                                   GError **err);

#endif
