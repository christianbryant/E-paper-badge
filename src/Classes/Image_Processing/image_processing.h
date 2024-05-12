#ifndef image_processing_h
#define image_processing_h
#include <Arduino.h>
class Image_processing {
    public:
        Image_processing();
        void read_image_array(fs::FS &fs, const char * path);
        void process_file_bin();
        void setup_color_arrays_bin();
        void sd_setup();
        void allocate_color_arrays();
        void free_color_arrays()
        String get_image_name(int index);
        int get_total_images();
        int get_curr_image();
        unsigned char *get_color_array(int index);
    private:
        unsigned char *color_arrays[7];
        RTC_DATA_ATTR String image_names[50];
};
#endif