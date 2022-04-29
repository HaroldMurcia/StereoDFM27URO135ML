#include "utility.h"

gboolean block_until_playing(GstElement *pipeline)
{
    while (TRUE)
    {
        GstState state;
        GstState pending;

        // wait 0.1 seconds for something to happen
        GstStateChangeReturn ret = gst_element_get_state(pipeline, &state, &pending, 100000000);

        if (ret == GST_STATE_CHANGE_SUCCESS)
        {
            return TRUE;
        }
        else if (ret == GST_STATE_CHANGE_FAILURE)
        {
            printf("Failed to change state %s %s %s\n",
                   gst_element_state_change_return_get_name(ret),
                   gst_element_state_get_name(state),
                   gst_element_state_get_name(pending));

            return FALSE;
        }
    }
}

int main (int argc, char *argv[]){

    gst_debug_set_default_threshold(GST_LEVEL_WARNING);
    gst_init(&argc, &argv);
    
    ParamServer param_server;

    string pip_des;
    pip_des.append("tcambin name=camera serial=");
    pip_des.append(param_server.serial_cam0);
    pip_des.append(" use-dutils=true ");
    pip_des.append("! capsfilter name=filter ");
    pip_des.append("! queue max-size-buffers=2 leaky=downstream ");
    pip_des.append("! videoconvert ");
    pip_des.append("! xvimagesink double-buffer=true sync=false draw-borders=false");

    GError* err = NULL;
    const char* pipeline_description = pip_des.c_str();
    GstElement* pipeline = gst_parse_launch(pipeline_description, &err);
    GstElement* cam0 = gst_bin_get_by_name(GST_BIN(pipeline), "camera");
    GstElement* capsfilter = gst_bin_get_by_name(GST_BIN(pipeline), "filter");

    if (pipeline == NULL){
        printf("Could not create pipeline. Cause: %s\n", err->message);
        return 1;}

    if (cam0==NULL){
        printf("Could not retrieve cam0 from pipeline.");
        return 1;}

    if (capsfilter == NULL){
        printf("Could not retrieve capsfilter from pipeline.");
        return 1;}

    param_server.set_property(cam0,"Brightness","int");
    param_server.set_property(cam0,"GPIn","int");
    param_server.set_property(cam0,"GPOut","int");
    param_server.set_property(cam0,"Gain (dB/100)","int");
    param_server.set_property(cam0,"Offset Auto Center","boolean");
    param_server.set_property(cam0,"Offset X","int");
    param_server.set_property(cam0,"Offset Y","int");
    param_server.set_property(cam0,"Override Scanning Mode","int");
    param_server.set_property(cam0,"Strobe Enable","boolean");
    param_server.set_property(cam0,"Strobe Exposure","boolean");
    param_server.set_property(cam0,"Strobe Polarity","boolean");
    param_server.set_property(cam0,"Trigger Delay (us)","int");
    param_server.set_property(cam0,"Trigger Global Reset Release","boolean");
    param_server.set_property(cam0,"Trigger Mode","boolean");
    param_server.set_property(cam0,"camera-whitebalance","boolean");
    param_server.set_property(cam0,"whitebalance-auto","boolean");
    param_server.set_property(cam0,"whitebalance-blue","int");
    param_server.set_property(cam0,"whitebalance-green","int");
    param_server.set_property(cam0,"whitebalance-module-enabled","boolean");
    param_server.set_property(cam0,"whitebalance-red","int");

    param_server.set_format(capsfilter);
    gst_object_unref(capsfilter);

    cout << "\n" << "LiveStreamCam0 :"<< endl;
    param_server.print_params();

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    if (!block_until_playing(pipeline))
    {
        printf("Unable to start pipeline. \n");
    }

    param_server.set_property(cam0,"Brightness Reference","int");

    param_server.set_property(cam0,"Exposure Auto","boolean");
    param_server.set_property(cam0,"Exposure Min","int");
    param_server.set_property(cam0,"Exposure Max","int");
    param_server.set_property(cam0,"Exposure Time (us)","int");

    param_server.set_property(cam0,"Gain Auto","boolean");
    param_server.set_property(cam0,"Gain Min","double");
    param_server.set_property(cam0,"Gain Max","double");
    param_server.set_property(cam0,"Gain","int");

    param_server.set_property(cam0,"Exposure ROI Left","int");
    param_server.set_property(cam0,"Exposure ROI Width","int");
    param_server.set_property(cam0,"Exposure ROI Top","int");
    param_server.set_property(cam0,"Exposure ROI Height","int");
    gst_object_unref(cam0); 

    printf("Press enter to stop the stream.\n");
    getchar();

    gst_element_set_state(pipeline, GST_STATE_NULL);

    gst_object_unref(pipeline);
    return 0;
}
