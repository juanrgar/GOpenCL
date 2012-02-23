
#include "gopencl.h"

gint
main (gint argc, gchar **argv)
{
    GList *platforms = NULL;

    g_type_init();

    gopencl_platform_get_platform_ids (1, &platforms, NULL);

    g_list_free_full (platforms, g_object_unref);

    return 0;
}
