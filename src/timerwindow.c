#include <pebble.h>
#include "categorywindow.h"
  
static Window *s_timer_window;
static TextLayer *s_text_layer, *s_time_layer;  

// These things will keep track of time
static double elapsed_time = 0;
static AppTimer* update_timer = NULL;
static double start_time = 0;
static double pause_time = 0;


double float_time_ms() {
  time_t seconds;
  uint16_t milliseconds;
  time_ms(&seconds, &milliseconds);
  return (double)seconds + ((double)milliseconds / 1000.0);
}

void stop_stopwatch() {
  pause_time = float_time_ms();
  if(update_timer != NULL) {
    app_timer_cancel(update_timer);
    update_timer = NULL;
  }
}

void update_stopwatch() {
  static char big_time[] = "00:00:00";
  // Now convert to hours/minutes/seconds.
  int tenths = (int)(elapsed_time * 10) % 10;
  int seconds = (int)elapsed_time % 60;
  int minutes = (int)elapsed_time / 60 % 60;
  int hours = (int)elapsed_time / 3600;
  // We can't fit three digit hours, so stop timing here.
  if(hours > 99) {
    stop_stopwatch();
    return;
  }
  if(hours < 1) {
    snprintf(big_time, 12, "%02d:%02d:%01d", minutes, seconds, tenths);
  } else {
    snprintf(big_time, 12, "%02d:%02d:%02d", hours, minutes, seconds);
  }
  // Now draw the strings.
  text_layer_set_text(s_time_layer, big_time);

}

void handle_timer(void* data) {
  double now = float_time_ms();
  elapsed_time = now - start_time;
  update_timer = app_timer_register(100, handle_timer, NULL);
  update_stopwatch();
}

static void start_timer(){
  start_time = float_time_ms();
  update_timer = app_timer_register(100, handle_timer, NULL);
}

static void timer_window_load(Window *window) {
  // Create time TextLayer
  s_text_layer = text_layer_create(GRect(0, 0, 144, 50));
  text_layer_set_background_color(s_text_layer, GColorClear);
  text_layer_set_text_color(s_text_layer, GColorBlack);
  text_layer_set_text(s_text_layer, "Timer Started");
  
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentLeft);

  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 51, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "Timer Started");
  
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentLeft);
  
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}
  
static void timer_window_unload(Window *window) {
  // Destroy TextLayer
    text_layer_destroy(s_text_layer);
    text_layer_destroy(s_time_layer);
}

void timer_select_single_click_handler(ClickRecognizerRef recognizer, void *context) {
    stop_stopwatch();
    cat_settings_init(elapsed_time);
}

void timer_config_provider(Window *window) {
 // single click / repeat-on-hold config:
  window_single_click_subscribe(BUTTON_ID_SELECT, timer_select_single_click_handler);
}

void timer_settings_init(void) {
    // Create main Window element and assign to pointer
  s_timer_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_timer_window, (WindowHandlers) {
    .load = timer_window_load,
    .unload = timer_window_unload
  });

  //Create click provider (buttons)
  window_set_click_config_provider(s_timer_window, (ClickConfigProvider) timer_config_provider);
  
  
  // Show the Window on the watch, with animated=true
  window_stack_push(s_timer_window, true);
  start_timer();
  
}