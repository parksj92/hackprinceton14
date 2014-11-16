#include <pebble.h>

enum {
STATUS_KEY = 0,	
MESSAGE_KEY = 1,
CATEGORY_KEY = 2
};  
  
static Window *s_settings_window;
static TextLayer *s_text_layer;


static void settings_window_load(Window *window) {
  // Create time TextLayer
  s_text_layer = text_layer_create(GRect(0, 0, 144, 50));
  text_layer_set_background_color(s_text_layer, GColorClear);
  text_layer_set_text_color(s_text_layer, GColorBlack);
  text_layer_set_text(s_text_layer, "Click Select to Start Timer");
  
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentLeft);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer));
}
  
static void settings_window_unload(Window *window) {
  // Destroy TextLayer
    text_layer_destroy(s_text_layer);
}

void win_settings_init(void) {
    // Create main Window element and assign to pointer
  s_settings_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_settings_window, (WindowHandlers) {
    .load = settings_window_load,
    .unload = settings_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_settings_window, true);
  
}