#include "gmime-extra.h"

GMimeStream *
g_mime_stream_stdout_new()
{
    GMimeStream *stream_stdout = NULL;
    GMimeStream *stream_buffered = NULL;

    stream_stdout = g_mime_stream_pipe_new (STDOUT_FILENO);
    if (!stream_stdout)
	return NULL;

    g_mime_stream_pipe_set_owner (GMIME_STREAM_PIPE (stream_stdout), FALSE);

    stream_buffered = g_mime_stream_buffer_new (stream_stdout, GMIME_STREAM_BUFFER_BLOCK_WRITE);

    g_object_unref (stream_stdout);

    return stream_buffered;
}

/**
 * copy a glib string into a talloc context, and free it.
 */
static char*
g_string_talloc_strdup (void *ctx, char *g_string)
{
    char *new_str = talloc_strdup (ctx, g_string);
    g_free (g_string);
    return new_str;
}

#if (GMIME_MAJOR_VERSION < 3)

char *
g_mime_message_get_date_string (void *ctx, GMimeMessage *message)
{
    char *date = g_mime_message_get_date_as_string (message);
    return g_string_talloc_strdup (ctx, date);
}

#else /* GMime >= 3.0 */

char *
g_mime_message_get_date_string (void *ctx, GMimeMessage *message)
{
    GDateTime* parsed_date = g_mime_message_get_date (message);
    if (parsed_date) {
	char *date = g_mime_utils_header_format_date (parsed_date);
	return g_string_talloc_strdup (ctx, date);
    } else {
	return talloc_strdup(ctx, "Thu, 01 Jan 1970 00:00:00 +0000");
    }
}
#endif