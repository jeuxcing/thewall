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


void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if(code == PJON_CONNECTION_LOST) {
    Serial.print("Connection with device ID ");
    Serial.print(bus.packets[data].content[0], DEC);
    Serial.println(" is lost.");
  }
  if(code == PJON_PACKETS_BUFFER_FULL) {
    Serial.print("Packet buffer is full, has now a length of ");
    Serial.println(data, DEC);
    Serial.println("Possible wrong bus configuration!");
    Serial.println("higher PJON_MAX_PACKETS if necessary.");
  }
  if(code == PJON_CONTENT_TOO_LONG) {
    Serial.print("Content is too long, length: ");
    Serial.println(data);
  }
};

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &info) {
  uint8_t line_idx = payload[0];
  uint8_t led_idx = payload[1];
  uint8_t red = payload[2];
  uint8_t green = payload[3];
  uint8_t blue = payload[4];

  lines[line_idx].setPixelColor(led_idx, red, green, blue);
};


void setup() {
  Serial.begin(115200);
  
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
  bus.begin();

  bus.set_error(error_handler);
  bus.set_receiver(receiver_function);
}

unsigned long previous_show = 0;

void loop() {
  bus.receive(REFRESH_DELAY);
  bus.update();

  unsigned long current_time = millis();
  if (current_time - previous_show > REFRESH_DELAY) {
    for (int line_idx=0 ; line_idx<NB_LINES ; line_idx++) {
      lines[line_idx].show();
    }
    previous_show = millis();
  }
}
