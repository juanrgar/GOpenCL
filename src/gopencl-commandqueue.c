// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

/**
 * SECTION:gopencl-commandqueue
 * @short_description: Class that represents an OpenCL command queue.
 * @stability: Unstable
 *
 * A #GopenclCommandqueue is associated with a device and it serves as a pool of
 * work batches.
 **/


#include "xopencl.h"
#include "gopencl.h"
#include "gopencl-common.h"


G_DEFINE_TYPE (GopenclCommandqueue, gopencl_commandqueue, G_TYPE_OBJECT);

#define GOPENCL_COMMANDQUEUE_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), GOPENCL_TYPE_COMMANDQUEUE, GopenclCommandqueuePrivate))
struct _GopenclCommandqueuePrivate
{
    cl_command_queue cl_commandqueue;
};


enum
{
    PROP_0,

    PROP_COMMANDQUEUE_ID
};

static void
gopencl_commandqueue_set_property (GObject *object,
                                   guint property_id,
                                   const GValue *value,
                                   GParamSpec *pspec)
{
    GopenclCommandqueue *self = GOPENCL_COMMANDQUEUE(object);
    GopenclCommandqueuePrivate *priv = GOPENCL_COMMANDQUEUE_GET_PRIVATE(self);

    switch (property_id)
    {
    case PROP_COMMANDQUEUE_ID:
        priv->cl_commandqueue = g_value_get_pointer(value);
        g_message("command_queue id set %p\n", priv->cl_commandqueue);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

//static void
//gopencl_commandqueue_query_prop (GopenclCommandqueue *self,
//                                 cl_commandqueue_info param_name,
//                                 GValue *value)
//{
//    GopenclCommandqueuePrivate *priv = GOPENCL_COMMANDQUEUE_GET_PRIVATE(self);
//    size_t str_buf_len;
//    gchar *str_buf;
//
//    clGetCommandqueueInfo(priv->cl_commandqueue,
//                       param_name,
//                       0,
//                       NULL,
//                       &str_buf_len);
//    str_buf = (gchar *) g_malloc(sizeof(gchar) * str_buf_len);
//    clGetCommandqueueInfo(priv->cl_commandqueue,
//                       param_name,
//                       str_buf_len,
//                       str_buf,
//                       NULL);
//    g_value_set_static_string(value, str_buf);
//}

static void
gopencl_commandqueue_get_property (GObject *object,
                                   guint property_id,
                                   GValue *value,
                                   GParamSpec *pspec)
{
    GopenclCommandqueue *self = GOPENCL_COMMANDQUEUE(object);
    GopenclCommandqueuePrivate *priv = GOPENCL_COMMANDQUEUE_GET_PRIVATE(self);

    switch (property_id)
    {
    case PROP_COMMANDQUEUE_ID:
        g_value_set_pointer(value, priv->cl_commandqueue);
        break;
    default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
        break;
    }
}

static void
gopencl_commandqueue_dispose (GObject *object)
{
//     GopenclCommandqueue *self = GOPENCL_COMMANDQUEUE(object);
//     GopenclCommandqueuePrivate *priv = GOPENCL_COMMANDQUEUE_GET_PRIVATE(self);

    G_OBJECT_CLASS(gopencl_commandqueue_parent_class)->dispose(object);
}

static void
gopencl_commandqueue_finalize (GObject *object)
{
    GopenclCommandqueue *self = GOPENCL_COMMANDQUEUE(object);
    GopenclCommandqueuePrivate *priv = GOPENCL_COMMANDQUEUE_GET_PRIVATE(self);

    priv->cl_commandqueue = NULL;
    g_message("cl_commandqueue nulled\n");

    G_OBJECT_CLASS(gopencl_commandqueue_parent_class)->finalize(object);
}

static void
gopencl_commandqueue_class_init (GopenclCommandqueueClass *klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    gobject_class->set_property = gopencl_commandqueue_set_property;
    gobject_class->get_property = gopencl_commandqueue_get_property;

    pspec = g_param_spec_pointer("id",
                                 "Command queue ID",
                                 "Set queue's ID",
                                 G_PARAM_READWRITE);
    g_object_class_install_property(gobject_class,
                                    PROP_COMMANDQUEUE_ID,
                                    pspec);

    gobject_class->dispose = gopencl_commandqueue_dispose;
    gobject_class->finalize = gopencl_commandqueue_finalize;

    g_type_class_add_private(klass, sizeof(GopenclCommandqueuePrivate));
}

static void
gopencl_commandqueue_init (GopenclCommandqueue *self)
{
    GopenclCommandqueuePrivate *priv;

    self->priv = priv = GOPENCL_COMMANDQUEUE_GET_PRIVATE(self);

    priv->cl_commandqueue = NULL;
}

GopenclCommandqueue *
gopencl_commandqueue_new (GopenclContext                  *context,
                          GopenclDevice                   *device,
                          gopencl_commandqueue_properties properties,
                          GError                          **error)
{
    cl_command_queue cl_commandqueue = NULL;
    cl_device_id cl_device = 0;
    cl_context cl_context = 0;
    cl_int err = CL_SUCCESS;

    g_object_get(device, "id", &cl_device, NULL);
    g_object_get(context, "id", &cl_context, NULL);

    cl_commandqueue = clCreateCommandQueue(cl_context,
                                           cl_device,
                                           properties,
                                           &err);
    if (err != CL_SUCCESS) {
        gopencl_format_error(err, error);
        return NULL;
    }

    g_message("Created command queue %p for device %p\n", cl_commandqueue, cl_device);

    return g_object_new(GOPENCL_TYPE_COMMANDQUEUE,
                        "id", cl_commandqueue,
                        NULL);
}

/**
 * gopencl_commandqueue_ref:
 * @self: an instance of GopenclCommandqueue.
 *
 * Increments the object reference count.
 *
 * Returns: the instance itself.
 *
 * Since: 0.1
 */
gpointer
gopencl_commandqueue_ref (GopenclCommandqueue *self)
{
    cl_command_queue cl_queue = NULL;
    cl_int err = CL_SUCCESS;

    g_object_get(self, "id", &cl_queue, NULL);

    err = clRetainCommandQueue(cl_queue);
    if (err != CL_SUCCESS) {
        gopencl_format_error(err, NULL);
        return NULL;
    }

    return g_object_ref(self);
}

/**
 * gopencl_commandqueue_unref:
 * @self: an instance of GopenclCommandqueue.
 *
 * Decrements the object reference count.
 *
 * Since: 0.1
 */
void
gopencl_commandqueue_unref (GopenclCommandqueue *self)
{
    cl_command_queue cl_queue = NULL;
    cl_int err = CL_SUCCESS;

    g_object_get(self, "id", &cl_queue, NULL);

    err = clReleaseCommandQueue(cl_queue);
    if (err != CL_SUCCESS) {
        gopencl_format_error(err, NULL);
        return NULL;
    }

    return g_object_unref(self);
}

