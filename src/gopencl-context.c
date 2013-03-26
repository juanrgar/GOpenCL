// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "xopencl.h"
#include "gopencl.h"


G_DEFINE_TYPE (GopenclContext, gopencl_context, G_TYPE_OBJECT);

#define GOPENCL_CONTEXT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), GOPENCL_TYPE_CONTEXT, GopenclContextPrivate))
struct _GopenclContextPrivate
{
    cl_context cl_context;
    GClosure   *error_callback;
};

enum
{
    PROP_0,

    PROP_CONTEXT_ID,
    PROP_ERROR_CALLBACK
};

static void
gopencl_context_set_property (GObject *object,
                              guint property_id,
                              const GValue *value,
                              GParamSpec *pspec)
{
    GopenclContext *self = GOPENCL_CONTEXT(object);
    GopenclContextPrivate *priv = GOPENCL_CONTEXT_GET_PRIVATE(self);

    switch (property_id)
    {
    case PROP_CONTEXT_ID:
        priv->cl_context = g_value_get_pointer(value);
        g_message("context id set %p\n", priv->cl_context);
        break;
    case PROP_ERROR_CALLBACK:
        priv->error_callback = g_value_get_pointer(value);
        g_message("error callback set %p\n", priv->cl_context);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

//static void
//gopencl_context_query_prop_str (GopenclContext *self,
//                               cl_context_info param_name,
//                               GValue *value)
//{
//    GopenclContextPrivate *priv = GOPENCL_CONTEXT_GET_PRIVATE(self);
//    size_t str_buf_len;
//    gchar *str_buf;
//
//    clGetContextInfo(priv->cl_context,
//                    param_name,
//                    0,
//                    NULL,
//                    &str_buf_len);
//    str_buf = (gchar *) g_malloc(sizeof(gchar) * str_buf_len);
//    clGetContextInfo(priv->cl_context,
//                    param_name,
//                    str_buf_len,
//                    str_buf,
//                    NULL);
//    g_value_set_static_string(value, str_buf);
//}

static void
gopencl_context_get_property (GObject *object,
                              guint property_id,
                              GValue *value,
                              GParamSpec *pspec)
{
    GopenclContext *self = GOPENCL_CONTEXT(object);
    GopenclContextPrivate *priv = GOPENCL_CONTEXT_GET_PRIVATE(self);

    switch (property_id)
    {
    case PROP_CONTEXT_ID:
        g_value_set_pointer(value, priv->cl_context);
        break;
    case PROP_ERROR_CALLBACK:
        g_value_set_pointer(value, priv->error_callback);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

static void
gopencl_context_dispose (GObject *object)
{
    G_OBJECT_CLASS(gopencl_context_parent_class)->dispose(object);
}

static void
gopencl_context_finalize (GObject *object)
{
    GopenclContext *self = GOPENCL_CONTEXT(object);
    GopenclContextPrivate *priv = GOPENCL_CONTEXT_GET_PRIVATE(self);

    priv->cl_context = NULL;
    g_message("cl_context nulled\n");
    g_closure_unref(priv->error_callback);
    priv->error_callback = NULL;
    g_message("error_callback unrefd\n");

    G_OBJECT_CLASS(gopencl_context_parent_class)->finalize(object);
}

static void
gopencl_context_class_init (GopenclContextClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    gobject_class->set_property = gopencl_context_set_property;
    gobject_class->get_property = gopencl_context_get_property;

    pspec = g_param_spec_pointer("id",
                                 "Context ID",
                                 "Set context ID",
                                 G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);
    g_object_class_install_property(gobject_class,
                                    PROP_CONTEXT_ID,
                                    pspec);

    pspec = g_param_spec_pointer("error-callback",
                                 "Error callback function",
                                 "Set error callback function",
                                 G_PARAM_READWRITE);
    g_object_class_install_property(gobject_class,
                                    PROP_ERROR_CALLBACK,
                                    pspec);

    gobject_class->dispose = gopencl_context_dispose;
    gobject_class->finalize = gopencl_context_finalize;

    g_type_class_add_private(klass, sizeof(GopenclContextPrivate));
}

static void
gopencl_context_init (GopenclContext *self)
{
    GopenclContextPrivate *priv;

    self->priv = priv = GOPENCL_CONTEXT_GET_PRIVATE(self);

    priv->cl_context = NULL;
}

GopenclContext *
gopencl_context_new (const GList *devices,
                     GClosure *error_callback,
                     GError **error)
{
    cl_context context = NULL;

    g_closure_invoke(error_callback, NULL, 0, NULL, NULL);

    return g_object_new(GOPENCL_TYPE_CONTEXT,
                        "id", context,
                        "error-callback", error_callback,
                        NULL);
}

