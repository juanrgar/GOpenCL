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

enum
{
    ERROR_OCURRED,
    LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

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
        g_message("error callback set %p\n", priv->error_callback);
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
    GopenclContext *self = GOPENCL_CONTEXT(object);
    GopenclContextPrivate *priv = GOPENCL_CONTEXT_GET_PRIVATE(self);

    if (priv->error_callback) {
        g_closure_unref(priv->error_callback);
        priv->error_callback = NULL;
        g_message("error_callback unrefd and nulled\n");
    }

    G_OBJECT_CLASS(gopencl_context_parent_class)->dispose(object);
}

static void
gopencl_context_finalize (GObject *object)
{
    GopenclContext *self = GOPENCL_CONTEXT(object);
    GopenclContextPrivate *priv = GOPENCL_CONTEXT_GET_PRIVATE(self);
    cl_int err = CL_SUCCESS;

    if (priv->cl_context) {
        err = clReleaseContext(priv->cl_context);
        if (err != CL_SUCCESS) {
            // TODO
        }
        priv->cl_context = NULL;
        g_message("cl_context released and nulled\n");
    }

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

//    signals[ERROR_OCURRED] = g_signal_newv();

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
    priv->error_callback = NULL;
}

static void
pfn_notify (const char *errinfo,
            const void *private_info,
            size_t cb,
            void *user_data)
{
    GClosure *closure = (GClosure *) user_data;
    GValue cb_params[3];

    g_closure_invoke(closure, NULL, 3, cb_params, NULL);
}

GopenclContext *
gopencl_context_new (GList *devices,
                     gpointer *user_data,
                     GError **error)
{
    cl_context context = NULL;
    cl_uint num_devices = 0;
    cl_int err = CL_SUCCESS;
    cl_device_id device_id = NULL;
    GClosure *cb_closure = NULL;
    gint i = 0;

    if ((devices == NULL) ||
        ((error_callback == NULL) && (user_data != NULL))) {
        g_set_error(error,
                    GOPENCL_ERROR,
                    GOPENCL_INVALID_VALUE,
                    "Invalid value");
        return NULL;
    }

    num_devices = g_list_length(devices);

    if (num_devices == 0) {
        g_set_error(error,
                    GOPENCL_ERROR,
                    GOPENCL_INVALID_VALUE,
                    "Invalid value");
        return NULL;
    }

    cl_device_id cl_devices[num_devices];
    g_message("allocated %d devices\n", num_devices);

    for (i = 0; i < num_devices; i++) {
        GopenclDevice *dev = GOPENCL_DEVICE(g_list_nth_data(devices, i));
        g_object_get(dev, "id", &device_id, NULL);
        g_message("device %p to be added to context\n", device_id);
        cl_devices[i] = device_id;
    }

    context = clCreateContext(0, num_devices, cl_devices, pfn_notify, cb_closure, &err);
    if (err != CL_SUCCESS) {
        g_set_error(error,
                    GOPENCL_ERROR,
                    GOPENCL_INVALID_VALUE,
                    "Invalid value");
        return NULL;
    }

    return g_object_new(GOPENCL_TYPE_CONTEXT,
                        "id", context,
                        "error-callback", cb_closure,
                        NULL);
}

gpointer
gopencl_context_ref (GopenclContext *self)
{
    GopenclContextPrivate *priv = GOPENCL_CONTEXT_GET_PRIVATE(self);
    cl_int err = CL_SUCCESS;

    err = clRetainContext(priv->cl_context);
    if (err != CL_SUCCESS) {
        // TODO
    }
    g_object_ref(self);
}

void
gopencl_context_unref (GopenclContext *self)
{
    GopenclContextPrivate *priv = GOPENCL_CONTEXT_GET_PRIVATE(self);
    cl_int err = CL_SUCCESS;

    err = clReleaseContext(priv->cl_context);
    if (err != CL_SUCCESS) {
        // TODO
    }
    g_object_unref(self);
}

