#include <string.h>
#include <math.h>
#include <vector>
#include <cstdlib>

#include "hardware/timer.h"
#include "pico_display_2.hpp"

using namespace pimoroni;

uint16_t buffer[PicoDisplay2::WIDTH * PicoDisplay2::HEIGHT];
PicoDisplay2 pico_display(buffer);


struct repeating_timer update_timer;
struct repeating_timer cycle_timer;

bool update;
uint fps = 0;
Point text_location(80, 0);

void update_display() {
  pico_display.set_pen(120, 40, 60);
  pico_display.clear();

  pico_display.set_pen(255, 255, 255);
  if(update) {
    pico_display.text("FPS: " + std::to_string(fps), Point(0, 0), 320);
  } else {
    pico_display.text("FPS: 0", Point(0, 0), 320);
  }

  pico_display.text("Hello World", text_location, 320);

  pico_display.update();
}

bool cycle_timer_callback(struct repeating_timer *t) {
  update = !update;
  return true;
}

int main() {
  pico_display.init();
  pico_display.set_backlight(255);

  add_repeating_timer_ms(-2000, cycle_timer_callback, nullptr, &cycle_timer);

  bool updated = false;

  while(true) {
    if(update) {
      uint32_t t_start = millis();
      text_location.y = 110 + (110 * sin(t_start / 200));
      update_display();
      uint32_t t_taken = millis() - t_start;
      fps = 1000 / t_taken;
      updated = false;
      sleep_ms(20);
    } else {
      if(!updated) {
      update_display();
        updated = true;
      }
      sleep_ms(50);
    }
  }

  return 0;
}
