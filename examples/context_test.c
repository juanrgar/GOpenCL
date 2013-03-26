// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "gopencl.h"

void
cb_func (gpointer user_data)
{
    g_print("error callback function called\n");
}

gint
main (gint argc, gchar **argv)
{
    GopenclContext *context = NULL;
    GList *devices = NULL;
    GClosure *error_callback = NULL;
    GError *err = NULL;
    gchar *str_buf = NULL;
    gboolean ret = FALSE;

    g_type_init();

    error_callback = g_cclosure_new(G_CALLBACK(cb_func), NULL, NULL);

    context = gopencl_context_new(devices, error_callback, &err);

    return 0;
}

