// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#ifndef __GOPENCL_COMMANDQUEUE_H__
#define __GOPENCL_COMMANDQUEUE_H__

#include <glib-object.h>

#define GOPENCL_TYPE_COMMANDQUEUE               (gopencl_commandqueue_get_type())
#define GOPENCL_COMMANDQUEUE(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOPENCL_TYPE_COMMANDQUEUE, GopenclCommandqueue))
#define GOPENCL_IS_COMMANDQUEUE(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOPENCL_TYPE_COMMANDQUEUE))
#define GOPENCL_COMMANDQUEUE_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), GOPENCL_TYPE_COMMANDQUEUE, GopenclCommandqueueClass))
#define GOPENCL_IS_COMMANDQUEUE_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), GOPENCL_TYPE_COMMANDQUEUE))
#define GOPENCL_COMMANDQUEUE_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), GOPENCL_TYPE_COMMANDQUEUE, GopenclCommandqueueClass))

typedef struct _GopenclCommandqueue         GopenclCommandqueue;
typedef struct _GopenclCommandqueuePrivate  GopenclCommandqueuePrivate;
typedef struct _GopenclCommandqueueClass    GopenclCommandqueueClass;

struct _GopenclCommandqueue
{
    GObject parent_instance;

    GopenclCommandqueuePrivate *priv;
};

struct _GopenclCommandqueueClass
{
    GObjectClass parent_class;
};

GType gopencl_commandqueue_get_type (void);

GopenclCommandqueue *
gopencl_commandqueue_new (GopenclContext *context,
                          GopenclDevice  *device,
                          GError         **error);

#endif

