#pragma once

#ifndef ULTILITY_H
#define ULTILITY_H

#include <ros/ros.h>
#include <ros/console.h>
#include <sensor_msgs/Image.h>

#include <tiscamera_interface/tiscamera_interface.hpp>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/video/videooverlay.h>
#include <stdio.h>
#include <unistd.h>
#include "tcamprop.h"

#include <yaml-cpp/yaml.h>

#include <string>

using namespace std;

class ParamServer{

public:
    
    YAML::Node node = YAML::LoadFile("src/Stereo_DFM_27URO135_ML/config/params.yaml");
    YAML::Node stereo_dfm_27uro135_ml = node["stereo_dfm_27uro135_ml"];

    YAML::Node properties = stereo_dfm_27uro135_ml["properties"];
    YAML::Node formats = stereo_dfm_27uro135_ml["formats"];

    string serial_cam0;
    string serial_cam1;

    string ros_encoding_type;  
    int32_t width;
    int32_t height;

    ParamServer(){
        YAML::Node serials = stereo_dfm_27uro135_ml["serials"];
        serial_cam0 = serials["cam0"].as<std::string>();
        serial_cam1 = serials["cam1"].as<std::string>();
        width = formats["width"].as<int32_t>();
        height = formats["height"].as<int32_t>(); 
        YAML::Node ros_encoding_type_tmp = node["rosEncodingType"];
        string format = formats["format"].as<std::string>();
        ros_encoding_type = ros_encoding_type_tmp[format].as<std::string>();
    }

    void set_property(GstElement *camera,const char *name,const char *type){
        GValue property = G_VALUE_INIT;
    
        if (strcmp(type, "int") == 0){
            g_value_init(&property,G_TYPE_INT);
            g_value_set_int(&property,properties[name].as<int>());
            tcam_prop_set_tcam_property(TCAM_PROP(camera), name, &property);
        }
        else if(strcmp(type, "boolean") == 0){   
            g_value_init(&property,G_TYPE_BOOLEAN);
            g_value_set_boolean(&property,properties[name].as<bool>());
            tcam_prop_set_tcam_property(TCAM_PROP(camera), name, &property);
        }
        else {
            printf("type no valido: %s",type);
        }
        g_value_unset(&property);
    }

    void set_format(GstElement *capsfilter){
        string name_string = formats["name"].as<std::string>();
        string format_string = formats["format"].as<std::string>();

        const char* name_char = name_string.c_str();
        const char* format_char = format_string.c_str();

        GstStructure *structure = gst_structure_from_string(name_char, NULL);
        gst_structure_set(structure,
                        "format",G_TYPE_STRING,format_char,
                        "width",G_TYPE_INT,formats["width"].as<int>(),
                        "height",G_TYPE_INT,formats["height"].as<int>(),
                        "framerate",GST_TYPE_FRACTION,formats["fps"].as<int>(), 1,
                        NULL);

        GstCaps *caps = gst_caps_new_empty();

        gst_caps_append_structure(caps,structure);
        g_object_set(G_OBJECT(capsfilter), "caps", caps, NULL);
        gst_caps_unref(caps);
    }

    void print_params(){
        cout << "\n" <<stereo_dfm_27uro135_ml << "\n" << endl;
    }
};
#endif