#include <pebble.h>
#include "finalpage.h"
  
static Window *s_settings_window;
static SimpleMenuLayer *s_menu_layer;
static SimpleMenuItem menu_items[9];
static SimpleMenuSection s_menu_section[1];
static double elapsed_time;

static void menu_select_callback(int index, void *ctx) {
  menu_items[index].subtitle = menu_items[index].title;
  const char* cat = menu_items[index].title;
  layer_mark_dirty(simple_menu_layer_get_layer(s_menu_layer));
  fin_settings_init(elapsed_time, cat);
}

static void settings_window_load(Window *window) {
  
  menu_items[0] = (SimpleMenuItem){
    .title = "Home",
    .callback = menu_select_callback,
  };
  menu_items[1] = (SimpleMenuItem){
    .title = "Work",
    .callback = menu_select_callback,
  };
  menu_items[2] = (SimpleMenuItem){
    .title = "Social",
    .callback = menu_select_callback,
  };
  menu_items[3] = (SimpleMenuItem){
    .title = "Exercise",
    .callback = menu_select_callback,
  };
  menu_items[4] = (SimpleMenuItem){
    .title = "Errand",
    .callback = menu_select_callback,
  };
  menu_items[5] = (SimpleMenuItem){
    .title = "Transit",
    .callback = menu_select_callback,
  };
  menu_items[6] = (SimpleMenuItem){
    .title = "Sleep",
    .callback = menu_select_callback,
  };
  menu_items[7] = (SimpleMenuItem){
    .title = "Eat",
    .callback = menu_select_callback,
  };
  menu_items[8] = (SimpleMenuItem){
    .title = "Other",
    .callback = menu_select_callback,
  };
  
   s_menu_section[0] = (SimpleMenuSection){
    .num_items = 9,
    .items = menu_items,
};
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  s_menu_layer = simple_menu_layer_create(bounds, window, s_menu_section, 1, NULL);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), simple_menu_layer_get_layer(s_menu_layer));
}
  
static void settings_window_unload(Window *window) {
  // Destroy TextLayer
    simple_menu_layer_destroy(s_menu_layer);
}

void cat_settings_init(double time) {
  elapsed_time = time;
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