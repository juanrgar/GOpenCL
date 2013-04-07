// Copyright (c) 2013, Juan R. García Blanco <juanrgar@gmail.com>
// All rights reserved.
// See LICENCE for more information.

#include "gopencl.h"

void
cb_func (const gchararray errinfo,
         gconstpointer private_info,
         gsize cb,
         gpointer user_data)
{
    g_print("user_data %p\n", user_data);
    g_print("error callback function called\n");
}

gint
main (gint argc, gchar **argv)
{
    GopenclContext *context = NULL;
    GList *devices = NULL;
    GError *err = NULL;
    gchar *str_buf = NULL;
    gboolean ret = FALSE;

    g_type_init();

    ret = gopencl_device_get_all_devices(GOPENCL_DEVICE_TYPE_CPU, &devices, &err);
    g_print("1. ret %d\n", ret);

    context = gopencl_context_new(devices, cb_func, NULL, &err);
    if (context == NULL) {
        g_print("context null\n");
    }

    return 0;
}

