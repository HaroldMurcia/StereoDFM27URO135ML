#include "utility.h"

int main (int argc, char *argv[]){

    gst_debug_set_default_threshold(GST_LEVEL_WARNING);
    gst_init(&argc, &argv);
    
    ParamServer param_server;

    string pip_des;
    pip_des.append("tcambin name=camera serial=");
    pip_des.append(param_server.serial_cam1);
    pip_des.append(" use-dutils=true ");
    pip_des.append("! capsfilter name=filter ");
    pip_des.append("! queue max-size-buffers=2 leaky=downstream ");
    pip_des.append("! videoconvert ");
    pip_des.append("! xvimagesink double-buffer=true sync=false draw-borders=false");

    GError* err = NULL;
    const char* pipeline_description = pip_des.c_str();
    GstElement* pipeline = gst_parse_launch(pipeline_description, &err);
    GstElement* cam1 = gst_bin_get_by_name(GST_BIN(pipeline), "camera");
    GstElement* capsfilter = gst_bin_get_by_name(GST_BIN(pipeline), "filter");

    if (pipeline == NULL){
        printf("Could not create pipeline. Cause: %s\n", err->message);
        return 1;}

    if (cam1==NULL){
        printf("Could not retrieve cam1 from pipeline.");
        return 1;}

    if (capsfilter == NULL){
        printf("Could not retrieve capsfilter from pipeline.");
        return 1;}

    param_server.set_property(cam1,"Brightness","int");
    param_server.set_property(cam1,"Exposure Time (us)","int");
    param_server.set_property(cam1,"GPIn","int");
    param_server.set_property(cam1,"GPOut","int");
    param_server.set_property(cam1,"Gain","int");
    param_server.set_property(cam1,"Gain (dB/100)","int");
    param_server.set_property(cam1,"Offset Auto Center","boolean");
    param_server.set_property(cam1,"Offset X","int");
    param_server.set_property(cam1,"Offset Y","int");
    param_server.set_property(cam1,"Override Scanning Mode","int");
    param_server.set_property(cam1,"Strobe Enable","boolean");
    param_server.set_property(cam1,"Strobe Exposure","boolean");
    param_server.set_property(cam1,"Strobe Polarity","boolean");
    param_server.set_property(cam1,"Trigger Delay (us)","int");
    param_server.set_property(cam1,"Trigger Global Reset Release","boolean");
    param_server.set_property(cam1,"Trigger Mode","boolean");
    param_server.set_property(cam1,"camera-whitebalance","boolean");
    param_server.set_property(cam1,"whitebalance-auto","boolean");
    param_server.set_property(cam1,"whitebalance-blue","int");
    param_server.set_property(cam1,"whitebalance-green","int");
    param_server.set_property(cam1,"whitebalance-module-enabled","boolean");
    param_server.set_property(cam1,"whitebalance-red","int");
    gst_object_unref(cam1);

    param_server.set_format(capsfilter);
    gst_object_unref(capsfilter);

    cout << "\n" << "LiveStreamcam1 :"<< endl;
    param_server.print_params();

    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    printf("Press enter to stop the stream.\n");
    getchar();

    gst_element_set_state(pipeline, GST_STATE_NULL);

    gst_object_unref(pipeline);
    return 0;
}