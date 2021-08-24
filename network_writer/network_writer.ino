#include <PJONSoftwareBitBang.h>

#define NB_LINES 2
#define NB_SEGMENTS (NB_LINES - 1)
#define NB_LED_SEGMENT (8 /* To be changed for real usage */)
#define NB_LEDS_LINE (NB_SEGMENTS * NB_LED_SEGMENT)


#define D1 5
PJONSoftwareBitBang bus;


void setup() {
  // Init network
  // Set the pin 12 as the communication pin
  bus.strategy.set_pin(D1);
  bus.set_id(13);
  
  // Only one bus
  bus.set_shared_network(false);
  // One way only
  bus.set_communication_mode(PJON_SIMPLEX);
  // No need for provenance
  bus.include_sender_info(false);
  // High fiability
  bus.set_crc_32(true);
}

uint8_t packets[5] = {0, 0, 30, 0, 0};


void loop() {
  for (int line_idx=0 ; line_idx<NB_LINES ; line_idx++) {
    packets[0] = (uint8_t)line_idx;
    for (int led_idx=0 ; led_idx<NB_LEDS_LINE ; led_idx++) {
      packets[1] = (uint8_t)led_idx;
      packets[2] = 30;
      bus.send_packet_blocking(12, packets, 5);
      delay(200);
      packets[2] = 0;
      bus.send_packet_blocking(12, packets, 5);
      delay(10);
    }
  }
}
