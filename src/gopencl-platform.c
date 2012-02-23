/*
 * Copyright/Licensing information.
 */

#ifdef __APPLE__
    #include <OpenCL/opencl.h>
#else
    #include <CL/opencl.h>
#endif

#include "gopencl.h"

#include "gopencl-platform.h"


G_DEFINE_TYPE (GopenclPlatform, gopencl_platform, G_TYPE_OBJECT);

#define GOPENCL_PLATFORM_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GOPENCL_TYPE_PLATFORM, GopenclPlatformPrivate))
struct _GopenclPlatformPrivate
{
    cl_platform_id *cl_platform;
};


enum
{
    PROP_0,

    PROP_PLATFORM_ID
};

static void
gopencl_platform_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec)
{
    GopenclPlatform *self = GOPENCL_PLATFORM (object);
    GopenclPlatformPrivate *priv = GOPENCL_PLATFORM_GET_PRIVATE (self);

    switch (property_id)
    {
    case PROP_PLATFORM_ID:
        g_free (priv->cl_platform);
        priv->cl_platform = g_value_get_pointer (value);
        g_print ("platform id set %p\n", priv->cl_platform);
        break;

    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gopencl_platform_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec)
{
//     GopenclPlatform *self = GOPENCL_PLATFORM (object);
//     GopenclPlatformPrivate *priv = GOPENCL_PLATFORM_GET_PRIVATE (self);

    switch (property_id)
    {
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
        break;
    }
}

static void
gopencl_platform_dispose (GObject *object)
{

    G_OBJECT_CLASS (gopencl_platform_parent_class)->dispose (object);
}

static void
gopencl_platform_finalize (GObject *object)
{
    GopenclPlatform *self = GOPENCL_PLATFORM (object);
    GopenclPlatformPrivate *priv = GOPENCL_PLATFORM_GET_PRIVATE (self);

    if (priv->cl_platform != NULL) {
        g_free (priv->cl_platform);
        priv->cl_platform = NULL;
        g_print ("cl_platform freed\n");
    }

    G_OBJECT_CLASS (gopencl_platform_parent_class)->finalize (object);
}

static void
gopencl_platform_class_init (GopenclPlatformClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GParamSpec *pspec;

    gobject_class->set_property = gopencl_platform_set_property;
    gobject_class->get_property = gopencl_platform_get_property;

    pspec = g_param_spec_pointer ("id",
                                  "Platform ID",
                                  "Set platform's ID",
                                  G_PARAM_CONSTRUCT_ONLY | G_PARAM_WRITABLE);
    g_object_class_install_property (gobject_class,
                                     PROP_PLATFORM_ID,
                                     pspec);

    gobject_class->dispose = gopencl_platform_dispose;
    gobject_class->finalize = gopencl_platform_finalize;

    g_type_class_add_private (klass, sizeof (GopenclPlatformPrivate));
}

static void
gopencl_platform_init (GopenclPlatform *self)
{
    GopenclPlatformPrivate *priv;

    self->priv = priv = GOPENCL_PLATFORM_GET_PRIVATE (self);

    priv->cl_platform = NULL;
}

static GopenclPlatform *
gopencl_platform_new (cl_platform_id *cl_platform)
{
    return g_object_new (GOPENCL_TYPE_PLATFORM,
                         "id", cl_platform,
                         NULL);
}

gint
gopencl_platform_get_platform_ids (gint num_entries,
                                   GList **platforms,
                                   GError **err)
{
    guint i;
    cl_platform_id cl_platforms[num_entries];
    cl_uint cl_num_platforms;

    if (platforms == NULL) {
        return GOPENCL_ERROR;
    }

    clGetPlatformIDs ((cl_int) num_entries, cl_platforms, &cl_num_platforms);

    g_print ("num platforms returned %d\n", cl_num_platforms);

    for (i = 0; i < cl_num_platforms; i++) {
        cl_platform_id *cl_plat = g_memdup ((const cl_platform_id *) &cl_platforms[i],
                                            sizeof (cl_platform_id));
        GopenclPlatform *g_plat = gopencl_platform_new (cl_plat);
        *platforms = g_list_prepend (*platforms, g_plat);
    }

    return GOPENCL_SUCCESS;
}
