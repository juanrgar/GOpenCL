// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#ifndef __GOPENCL_CONTEXT_H__
#define __GOPENCL_CONTEXT_H__

#include <glib-object.h>

#include "xopencl.h"

#define GOPENCL_TYPE_CONTEXT               (gopencl_context_get_type())
#define GOPENCL_CONTEXT(obj)               (G_TYPE_CHECK_INSTANCE_CAST ((obj), GOPENCL_TYPE_CONTEXT, GopenclContext))
#define GOPENCL_IS_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GOPENCL_TYPE_CONTEXT))
#define GOPENCL_CONTEXT_CLASS(klass)       (G_TYPE_CHECK_CLASS_CAST ((klass), GOPENCL_TYPE_CONTEXT, GopenclContextClass))
#define GOPENCL_IS_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_TYPE ((klass), GOPENCL_TYPE_CONTEXT))
#define GOPENCL_CONTEXT_GET_CLASS(obj)     (G_TYPE_INSTANCE_GET_CLASS ((obj), GOPENCL_TYPE_CONTEXT, GopenclContextClass))

typedef struct _GopenclContext         GopenclContext;
typedef struct _GopenclContextPrivate  GopenclContextPrivate;
typedef struct _GopenclContextClass    GopenclContextClass;

struct _GopenclContext
{
    GObject parent_instance;

    GopenclContextPrivate *priv;
};

struct _GopenclContextClass
{
    GObjectClass parent_class;
};

GType gopencl_context_get_type (void);

typedef enum _gopencl_context_type gopencl_context_type;

GopenclContext *
gopencl_context_new (const GList *devices,
                     GClosure    *error_callback,
                     GError      **error);

#endif

