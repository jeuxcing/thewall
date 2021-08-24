#include <PJONSoftwareBitBang.h>
#include <Adafruit_NeoPixel.h>

#define REFRESH_DELAY 50
#define NB_LINES 2
#define NB_SEGMENTS (NB_LINES - 1)
#define NB_LED_SEGMENT (8 /* To be changed for real usage */)
#define NB_LEDS_LINE (NB_SEGMENTS * NB_LED_SEGMENT)

//             D2 D3 D4 D5  D6
int pins[5] = {4, 0, 2, 14, 12};
Adafruit_NeoPixel lines[NB_LINES];

#define D1 5
PJONSoftwareBitBang bus;


void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &info) {
  uint8_t line_idx = payload[0];
  uint8_t led_idx = payload[1];
  uint8_t red = payload[2];
  uint8_t green = payload[3];
  uint8_t blue = payload[4];

  lines[line_idx].setPixelColor(led_idx, red, green, blue);
};


void setup() {
  // Init led strips
  for (int lines_idx=0 ; lines_idx<NB_LINES ; lines_idx++) {
    //lines[lines_idx] = Adafruit_NeoPixel(NB_LEDS_LINE, pins[lines_idx]);
    lines[lines_idx].updateLength(NB_LEDS_LINE);
    lines[lines_idx].setPin(pins[lines_idx]);
    lines[lines_idx].begin();
    lines[lines_idx].clear();
    lines[lines_idx].show();
  }

  // Init network
  // Set the pin 12 as the communication pin
  bus.strategy.set_pin(D1);
  bus.set_id(12);
  
  // Only one bus
  bus.set_shared_network(false);
  // One way only
  bus.set_communication_mode(PJON_SIMPLEX);
  // No need for provenance
  bus.include_sender_info(false);
  // High fiability
  bus.set_crc_32(true);
  // Define network callback
  bus.set_receiver(receiver_function);
}

unsigned long previous_show = 0;

void loop() {
  bus.receive();

  unsigned long current_time = millis();
  if (current_time - previous_show > REFRESH_DELAY) {
    for (int line_idx=0 ; line_idx<NB_LINES ; line_idx++) {
      lines[line_idx].show();
    }
    previous_show = millis();
  }
}
