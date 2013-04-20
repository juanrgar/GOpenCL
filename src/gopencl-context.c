// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

/**
 * SECTION:gopencl-context
 * @short_description: Class that represents an OpenCL context.
 * @stability: Unstable
 *
 * A #GopenclContext serves as a definition of computing environment.
 **/

#include "xopencl.h"
#include "gopencl.h"


G_DEFINE_TYPE (GopenclContext, gopencl_context, G_TYPE_OBJECT);

#define GOPENCL_CONTEXT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE((obj), GOPENCL_TYPE_CONTEXT, GopenclContextPrivate))
struct _GopenclContextPrivate
{
    cl_context cl_context;
    GList *devices;
};

enum
{
    PROP_0,
    PROP_CONTEXT_ID,
    PROP_DEVICES
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
    case PROP_DEVICES:
        priv->devices = g_value_get_pointer(value);
        g_message("devices list stored %p\n", priv->devices);
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
    case PROP_DEVICES:
        g_value_set_pointer(value, priv->devices);
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

    if (priv->devices) {
        priv->devices = NULL;
    }

    G_OBJECT_CLASS(gopencl_context_parent_class)->finalize(object);
}

static void
gopencl_context_class_init (GopenclContextClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;
    GType signal_types[3];

    gobject_class->set_property = gopencl_context_set_property;
    gobject_class->get_property = gopencl_context_get_property;

    pspec = g_param_spec_pointer("id",
                                 "Context ID",
                                 "Set context ID",
                                 G_PARAM_READWRITE);

    /**
     * GopenclContext:id:
     *
     * Associated cl_context_id in the OpenCL environment.
     *
     * Since: 0.1
     */
    g_object_class_install_property(gobject_class,
                                    PROP_CONTEXT_ID,
                                    pspec);

    pspec = g_param_spec_pointer("devices",
                                 "Context associated devices",
                                 "Set context devices",
                                 G_PARAM_READWRITE);
    /**
     * GopenclContext:devices:
     *
     * List of devices that build up the context.
     *
     * Since: 0.1
     */
    g_object_class_install_property(gobject_class,
                                    PROP_DEVICES,
                                    pspec);

    signal_types[0] = G_TYPE_STRING;
    signal_types[1] = G_TYPE_POINTER;
    signal_types[2] = G_TYPE_UINT;

    /**
     * GopenclContext::error-ocurred:
     * @object: the object...
     * @err_info:
     * @priv_info:
     * @cb:
     *
     * The ::error-ocurred signal is emitted each time an error within the OpenCL context occurs.
     *
     * Since: 0.1
     */
    signals[ERROR_OCURRED] = g_signal_newv("error-ocurred",
                                           G_TYPE_FROM_CLASS(gobject_class),
                                           G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                                           NULL,
                                           NULL,
                                           NULL,
                                           g_cclosure_user_marshal_VOID__STRING_POINTER_UINT,
                                           G_TYPE_NONE,
                                           3,
                                           signal_types);

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

static void
pfn_notify (const char *errinfo,
            const void *private_info,
            size_t cb,
            void *user_data)
{
    GValue instance_and_params[4] = { 0 };
    GopenclContext *self = GOPENCL_CONTEXT(user_data);
    GopenclContextPrivate *priv = GOPENCL_CONTEXT_GET_PRIVATE(self);

    g_value_init(&instance_and_params[0], GOPENCL_TYPE_CONTEXT);
    g_value_set_object(&instance_and_params[0], self);

    g_value_init(&instance_and_params[1], G_TYPE_STRING);
    g_value_set_string(&instance_and_params[1], errinfo);

    g_value_init(&instance_and_params[2], G_TYPE_POINTER);
    g_value_set_pointer(&instance_and_params[2], (void *) private_info);

    g_value_init(&instance_and_params[3], G_TYPE_UINT);
    g_value_set_uint(&instance_and_params[3], cb);

    g_signal_emitv(instance_and_params, signals[ERROR_OCURRED], 0, NULL);
}

GopenclContext *
gopencl_context_new (GList *devices,
                     GError **error)
{
    GopenclContext *self = NULL;
    cl_context context = NULL;
    cl_uint num_devices = 0;
    cl_int err = CL_SUCCESS;
    cl_device_id device_id = NULL;
    gint i = 0;

    if ((devices == NULL)) { 
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

    self = g_object_new(GOPENCL_TYPE_CONTEXT, NULL);

    context = clCreateContext(0, num_devices, cl_devices, pfn_notify, self, &err);
    if (err != CL_SUCCESS) {
        gopencl_context_unref(self);
        g_set_error(error,
                    GOPENCL_ERROR,
                    GOPENCL_INVALID_VALUE,
                    "Invalid value");
        return NULL;
    }

    g_object_set(self, "id", context, NULL);
    g_object_set(self, "devices", devices, NULL);

    return self;
}

/**
 * gopencl_context_ref:
 * @self: an instance of GopenclContext.
 *
 * Increments the reference count of a GopenclContext.
 *
 * Returns: the instance itself.
 *
 * Since: 0.1
 */
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

