// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "gopencl.h"

gint
main (gint argc, gchar **argv)
{
    GList *devices = NULL;
    gint n_devices = 0;
    GopenclContext *ctx = NULL;
    GError *err = NULL;
    gboolean ret = FALSE;
    gint i = 0;

    ret = gopencl_device_get_all_devices(GOPENCL_DEVICE_TYPE_GPU, &devices, &err);
    n_devices = g_list_length(devices);

    g_print("Found %d devices\n", n_devices);

    ctx = gopencl_context_new(devices, &err);
    if (ctx == NULL) {
        g_print("Error during context creation: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }

    g_print("Context successfully created\n");

    GopenclCommandqueue *queue[n_devices];

    for (i = 0; i < n_devices; i++) {
        GopenclDevice *dev = GOPENCL_DEVICE(g_list_nth_data(devices, i));
        queue[i] = gopencl_commandqueue_new(ctx, dev, GOPENCL_COMMANDQUEUE_PROFILING_ENABLE, &err);
    }

    return 0;
}

