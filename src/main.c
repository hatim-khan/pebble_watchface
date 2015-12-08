#include <pebble.h>


//ptrs
static Window *s_main_window;
static TextLayer *s_time_layer;
static GFont s_time_font;

static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;





static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, s_buffer);
}


static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}



static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
   
  
  
  
  //create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_PERFECT_DOS_68));
  
  //apply to text layer
  text_layer_set_font(s_time_layer, s_time_font);
  
  
  
  // Get information about the Window
  GRect bounds = layer_get_bounds(window_layer);
  
   
  //BITMAP AREA
  //create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_BITMAP_DASHES);
  
  //create bitmap layer to display GBitmap
  s_background_layer = bitmap_layer_create(bounds);
  
  //set the bitmap onto the layer and add it to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  

  // Create the TextLayer with specific bounds
  s_time_layer = text_layer_create(
      GRect(0, PBL_IF_ROUND_ELSE(58, 52), bounds.size.w, 50));

  // Improve the layout to be more like a watchface
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00:00");
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
 
  
 
  

}

static void main_window_unload(Window *window) {
    text_layer_destroy(s_time_layer); //delete
    fonts_unload_custom_font(s_time_font);
  
    //take care of bitmap
    gbitmap_destroy(s_background_bitmap);
    bitmap_layer_destroy(s_background_layer);
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
   
  //set time
  update_time();
  //register
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
 
  window_set_background_color(s_main_window, GColorBlack);

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

