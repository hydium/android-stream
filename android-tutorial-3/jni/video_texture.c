#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <pthread.h>

#include <stdbool.h>
#include <unistd.h>


// void* image_ptr;
// GstBuffer *prev_buffer = NULL;
// GstSample *prev_sample = NULL;
GstBuffer *buffer = NULL;
GstSample *sample = NULL;
GstMapInfo map;

GstElement *pipeline, *appsink;

// pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void gstreamer_init (void)
{
	GError *error = NULL;
	gst_init (NULL, NULL);

	// pipeline = gst_parse_launch("v4l2src device=/dev/video0 ! image/jpeg, width=1920, height=1080, pixel-aspect-ratio=1/1, framerate=60/1 ! jpegdec ! videoconvert ! video/x-raw, width=1920, height=1080, format=\"RGB\" ! appsink name=appsink", &error);
	pipeline = gst_parse_launch("videotestsrc ! video/x-raw, width=1920, height=1080, format=\"RGB\" ! appsink name=appsink", &error);

	appsink = gst_bin_get_by_name (GST_BIN (pipeline), "appsink");
	g_object_set (appsink, "max-buffers", 1, NULL);

	gst_element_set_state(pipeline, GST_STATE_PLAYING);
}


void* get_frame(void)
{	
	sample = gst_app_sink_pull_sample (GST_APP_SINK (appsink));

	buffer = gst_sample_get_buffer (sample);
	gst_buffer_map ( buffer, &map, GST_MAP_READ);

	return (void *) map.data;
}

void got_frame(void)
{
	gst_buffer_unmap(buffer, &map);
	gst_sample_unref(sample);
}


void gstreamer_destroy(void)
{
	gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);	
}