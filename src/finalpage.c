#include <pebble.h>
#include <stdlib.h>
#include "timerwindow.h"
  
static Window *s_settings_window;
static TextLayer *s_text_layer,*s_text_layer1,*s_time_layer1, *s_time_layer, *s_text_layer2;
static double elapsed_time;
static const char* cat;  

enum {
STATUS_KEY = 0,	
TIME_KEY = 1,
CATEGORY_KEY = 2
};

static void settings_window_load(Window *window) {
  // Create time TextLayer
  s_text_layer = text_layer_create(GRect(0, 0, 144, 50));
  text_layer_set_background_color(s_text_layer, GColorClear);
  text_layer_set_text_color(s_text_layer, GColorBlack);
  text_layer_set_text(s_text_layer, "Category Selected: ");
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentLeft);
  
  // Create time TextLayer
  s_text_layer1 = text_layer_create(GRect(0, 10, 144, 50));
  text_layer_set_background_color(s_text_layer1, GColorClear);
  text_layer_set_text_color(s_text_layer1, GColorBlack);
  text_layer_set_text(s_text_layer1, cat);
  text_layer_set_font(s_text_layer1, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentLeft);

  // Create time TextLayer
  s_time_layer1 = text_layer_create(GRect(0, 50, 144, 50));
  text_layer_set_background_color(s_time_layer1, GColorClear);
  text_layer_set_text_color(s_time_layer1, GColorBlack);
  text_layer_set_text(s_time_layer1, "Total Time Elapsed:");
  text_layer_set_font(s_time_layer1, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_time_layer1, GTextAlignmentLeft);
  
  static char big_time[] = "00:00:00";
  // Now convert to hours/minutes/seconds.
  int tenths = (int)(elapsed_time * 10) % 10;
  int seconds = (int)elapsed_time % 60;
  int minutes = (int)elapsed_time / 60 % 60;
  int hours = (int)elapsed_time / 3600;
  if(hours < 1) {
    snprintf(big_time, 12, "%02d:%02d:%01d", minutes, seconds, tenths);
  } else {
    snprintf(big_time, 12, "%02d:%02d:%02d", hours, minutes, seconds);
  }

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 60, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, big_time);
  
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentLeft);
  
    // Create time TextLayer
  s_text_layer2 = text_layer_create(GRect(0, 70, 144, 50));
  text_layer_set_background_color(s_text_layer2, GColorClear);
  text_layer_set_text_color(s_text_layer2, GColorBlack);
  text_layer_set_text(s_text_layer2, "Press Select to start new timer!");
  
  text_layer_set_font(s_text_layer2, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_text_layer2, GTextAlignmentLeft);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer1));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer1));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer2));
  
  int elapsed_int = (int) elapsed_time;
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Elapsed Time Converted");
  
  //Send message
  DictionaryIterator* dictionaryIterator = NULL;
  app_message_outbox_begin (&dictionaryIterator);
  if (dictionaryIterator == NULL) {
    APP_LOG(APP_LOG_LEVEL_DEBUG, "Null Iterator");
    return;
  }
  dict_write_uint8 (dictionaryIterator, STATUS_KEY, 0x1);
  dict_write_int32 (dictionaryIterator,TIME_KEY, elapsed_int);
  dict_write_cstring (dictionaryIterator,CATEGORY_KEY, cat);
  dict_write_end (dictionaryIterator);
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Wrote to Dict");
  
  app_message_outbox_send();
  
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Sent");
}
  
void f_select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  //pop three windows
  window_stack_pop(true);
  window_stack_pop(true);
  Window* menu = window_stack_pop(true);
}

void final_config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_click_subscribe(BUTTON_ID_SELECT, f_select_single_click_handler);
}

static void settings_window_unload(Window *window) {
  // Destroy TextLayer
    text_layer_destroy(s_text_layer);
    text_layer_destroy(s_time_layer);
}

//This handles incoming message from the phone
static void in_received_handler(DictionaryIterator *iter, void *context) {

}

//Dropped messages
static void in_dropped_handler(AppMessageResult reason, void *context) {

}

//Failed to send
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {

}


static void app_message_init(void) {
  // Register message handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  // Init buffers
  app_message_open(64, 64);
}

void fin_settings_init(double elapsed, const char* choice) {
    // Create main Window element and assign to pointer
  cat = choice;
  elapsed_time = elapsed;
  s_settings_window = window_create();

  app_message_init();
  
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_settings_window, (WindowHandlers) {
    .load = settings_window_load,
    .unload = settings_window_unload
  });

  //Create click provider (buttons)
  window_set_click_config_provider(s_settings_window, (ClickConfigProvider) final_config_provider);
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_settings_window, true);
  
}