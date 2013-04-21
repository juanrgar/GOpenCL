// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "xopencl.h"
#include "gopencl.h"
#include "gopencl-common.h"


G_DEFINE_TYPE (GopenclPlatform, gopencl_platform, G_TYPE_OBJECT);

#define GOPENCL_PLATFORM_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GOPENCL_TYPE_PLATFORM, GopenclPlatformPrivate))
struct _GopenclPlatformPrivate
{
    cl_platform_id cl_platform;
};


enum
{
    PROP_0,

    PROP_PLATFORM_ID,
    PROP_PLATFORM_PROFILE,
    PROP_PLATFORM_VERSION,
    PROP_PLATFORM_NAME,
    PROP_PLATFORM_VENDOR,
    PROP_PLATFORM_EXTENSIONS
};

static void
gopencl_platform_set_property (GObject *object,
                               guint property_id,
                               const GValue *value,
                               GParamSpec *pspec)
{
    GopenclPlatform *self = GOPENCL_PLATFORM(object);
    GopenclPlatformPrivate *priv = GOPENCL_PLATFORM_GET_PRIVATE(self);

    switch (property_id)
    {
    case PROP_PLATFORM_ID:
        priv->cl_platform = g_value_get_pointer(value);
        g_message("platform id set %p\n", priv->cl_platform);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

static void
gopencl_platform_query_prop (GopenclPlatform *self,
                             cl_platform_info param_name,
                             GValue *value)
{
    GopenclPlatformPrivate *priv = GOPENCL_PLATFORM_GET_PRIVATE(self);
    size_t str_buf_len;
    gchar *str_buf;

    clGetPlatformInfo(priv->cl_platform,
                       param_name,
                       0,
                       NULL,
                       &str_buf_len);
    str_buf = (gchar *) g_malloc(sizeof(gchar) * str_buf_len);
    clGetPlatformInfo(priv->cl_platform,
                       param_name,
                       str_buf_len,
                       str_buf,
                       NULL);
    g_value_set_static_string(value, str_buf);
}

static void
gopencl_platform_get_property (GObject *object,
                               guint property_id,
                               GValue *value,
                               GParamSpec *pspec)
{
    GopenclPlatform *self = GOPENCL_PLATFORM(object);
    GopenclPlatformPrivate *priv = GOPENCL_PLATFORM_GET_PRIVATE(self);

    switch (property_id)
    {
    case PROP_PLATFORM_ID:
        g_value_set_pointer(value, priv->cl_platform);
        break;
    case PROP_PLATFORM_PROFILE:
        gopencl_platform_query_prop(self, CL_PLATFORM_PROFILE, value);
        break;
    case PROP_PLATFORM_VERSION:
        gopencl_platform_query_prop(self, CL_PLATFORM_VERSION, value);
        break;
    case PROP_PLATFORM_NAME:
        gopencl_platform_query_prop(self, CL_PLATFORM_NAME, value);
        break;
    case PROP_PLATFORM_VENDOR:
        gopencl_platform_query_prop(self, CL_PLATFORM_VENDOR, value);
        break;
    case PROP_PLATFORM_EXTENSIONS:
        gopencl_platform_query_prop(self, CL_PLATFORM_EXTENSIONS, value);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

static void
gopencl_platform_dispose (GObject *object)
{
//     GopenclPlatform *self = GOPENCL_PLATFORM(object);
//     GopenclPlatformPrivate *priv = GOPENCL_PLATFORM_GET_PRIVATE(self);

    G_OBJECT_CLASS(gopencl_platform_parent_class)->dispose(object);
}

static void
gopencl_platform_finalize (GObject *object)
{
    GopenclPlatform *self = GOPENCL_PLATFORM(object);
    GopenclPlatformPrivate *priv = GOPENCL_PLATFORM_GET_PRIVATE(self);

    priv->cl_platform = NULL;
    g_message("cl_platform nulled\n");

    G_OBJECT_CLASS(gopencl_platform_parent_class)->finalize(object);
}

static void
gopencl_platform_class_init (GopenclPlatformClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    gobject_class->set_property = gopencl_platform_set_property;
    gobject_class->get_property = gopencl_platform_get_property;

    pspec = g_param_spec_pointer("id",
                                 "Platform ID",
                                 "Set platform's ID",
                                 G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    g_object_class_install_property(gobject_class,
                                    PROP_PLATFORM_ID,
                                    pspec);

    pspec = g_param_spec_string("profile",
                                "Platform profile",
                                "OpenCL profile string",
                                "no-profile",
                                G_PARAM_READABLE);
    g_object_class_install_property(gobject_class,
                                    PROP_PLATFORM_PROFILE,
                                    pspec);

    pspec = g_param_spec_string("version",
                                "Platform version",
                                "Platform version string",
                                "no-version",
                                G_PARAM_READABLE);
    g_object_class_install_property(gobject_class,
                                    PROP_PLATFORM_VERSION,
                                    pspec);

    pspec = g_param_spec_string("name",
                                "Platform name",
                                "Platform name string",
                                "no-name",
                                G_PARAM_READABLE);
    g_object_class_install_property(gobject_class,
                                    PROP_PLATFORM_NAME,
                                    pspec);

    pspec = g_param_spec_string("vendor",
                                "Platform vendor",
                                "Platform vendor string",
                                "no-vendor",
                                G_PARAM_READABLE);
    g_object_class_install_property(gobject_class,
                                    PROP_PLATFORM_VENDOR,
                                    pspec);

    pspec = g_param_spec_string("extensions",
                                "Platform extensions",
                                "Space-separated list of extension names",
                                "no-extensions",
                                G_PARAM_READABLE);
    g_object_class_install_property(gobject_class,
                                    PROP_PLATFORM_EXTENSIONS,
                                    pspec);

    gobject_class->dispose = gopencl_platform_dispose;
    gobject_class->finalize = gopencl_platform_finalize;

    g_type_class_add_private(klass, sizeof(GopenclPlatformPrivate));
}

static void
gopencl_platform_init (GopenclPlatform *self)
{
    GopenclPlatformPrivate *priv;

    self->priv = priv = GOPENCL_PLATFORM_GET_PRIVATE(self);

    priv->cl_platform = NULL;
}

static GopenclPlatform *
gopencl_platform_new (cl_platform_id cl_platform)
{
    return g_object_new(GOPENCL_TYPE_PLATFORM,
                        "id", cl_platform,
                        NULL);
}

gint
gopencl_platform_get_platforms (GList **platforms,
                                GError **err)
{
    guint i = 0;
    cl_uint cl_num_platforms = 0;
    cl_int cl_err = 0;

    if (platforms == NULL) {
        g_set_error(err,
                     GOPENCL_ERROR,
                     GOPENCL_INVALID_VALUE,
                     "Invalid value");
        return GOPENCL_INVALID_VALUE;
    }

    cl_err = clGetPlatformIDs(0, NULL, &cl_num_platforms);

    g_message("num platforms returned %d\n", cl_num_platforms);

    if (cl_num_platforms == 0) {
        g_set_error(err,
                     GOPENCL_ERROR,
                     GOPENCL_INVALID_VALUE,
                     "Invalid value");
        return GOPENCL_INVALID_VALUE;
    }

    cl_platform_id cl_platforms[cl_num_platforms];
    cl_err = clGetPlatformIDs(cl_num_platforms, cl_platforms, NULL);
    if (cl_err != CL_SUCCESS) {
        return gopencl_format_error(cl_err, err);
    }

    for (i = 0; i < cl_num_platforms; i++) {
        g_message("appended platform\n");
        GopenclPlatform *g_plat = gopencl_platform_new(cl_platforms[i]);
        *platforms = g_list_prepend(*platforms, g_plat);
    }

    return (gint) cl_num_platforms;
}

gboolean
gopencl_platform_is_full_profile (GopenclPlatform *self)
{
    gchar *str_buf = NULL;
    gboolean ret = FALSE;

    g_object_get(self, "profile", &str_buf, NULL);
    ret = (g_strcmp0(str_buf, "FULL_PROFILE") == 0);
    g_free(str_buf);

    return ret;
}


gboolean
gopencl_platform_is_embedded_profile (GopenclPlatform *self)
{
    gchar *str_buf = NULL;
    gboolean ret = FALSE;

    g_object_get(self, "profile", &str_buf, NULL);
    ret = (g_strcmp0(str_buf, "EMBEDDED_PROFILE") == 0);
    g_free(str_buf);

    return ret;
}

