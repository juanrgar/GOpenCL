// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "gopencl.h"

gint
main (gint argc, gchar **argv)
{
    GList *devices = NULL;
    GopenclContext *ctx = NULL;
    GError *err = NULL;
    gboolean ret = FALSE;

    ret = gopencl_device_get_all_devices(GOPENCL_DEVICE_TYPE_GPU, &devices, &err);

    g_print("Found %d devices\n", g_list_length(devices));

    ctx = gopencl_context_new(devices, &err);
    if (ctx == NULL) {
        g_print("Error during context creation: %s\n", err->message);
        g_error_free(err);
        err = NULL;
    }

    g_print("Context successfully created\n");

    return 0;
}

