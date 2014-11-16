#include <pebble.h>
#include "win-settings.h"  
#include "timerwindow.h"
  
static Window *s_main_window;
static TextLayer *s_time_layer;

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 0, 288, 100));
  
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  
  text_layer_set_text(s_time_layer, "Timer App!\nThis app keeps track of \n how you spend your time.\nPress the Select Key \nto start a timer.");
  
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentLeft);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  // Destroy TextLayer
    text_layer_destroy(s_time_layer);
}

void up_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  win_settings_init();   
}

void select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
  timer_settings_init();
}

void main_config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_click_subscribe(BUTTON_ID_UP, up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_SELECT, select_single_click_handler);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  //Create click provider (buttons)
  window_set_click_config_provider(s_main_window, (ClickConfigProvider) main_config_provider);
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}


static void deinit() {
  // Destroy Window
   window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
