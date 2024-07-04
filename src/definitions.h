#ifndef definitions_h
#define definitions_h
#include <Arduino.h>

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0
#define WHITE           0xFFFF



// E-Paper Display status Shared Variables
bool is_displayed;
bool display_image_flag;
bool changed_image_flag;

// E-Paper Color Arrays
unsigned char *color_arrays[7];


//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

// RTC Shared Variables
RTC_DATA_ATTR bool first_boot = true;
RTC_DATA_ATTR int curr_image;
RTC_DATA_ATTR unsigned long last_sleep_time;
RTC_DATA_ATTR int total_images;
RTC_DATA_ATTR String image_names[50];

#endif