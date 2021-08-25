#include <PJONSoftwareBitBang.h>

#define NB_LINES 3
#define NB_SEGMENTS (NB_LINES - 1)
#define NB_LED_SEGMENT (24 /* To be changed for real usage */)
#define NB_LEDS_LINE (NB_SEGMENTS * NB_LED_SEGMENT)


#define D1 5
#define MY_ID 13
PJONSoftwareBitBang bus;



uint8_t packets[100];
bool acknowledge = false;

void ack_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &info) {
  if (length == 2 and payload[0] == 'O' and payload[1] == 'K')
    acknowledge = true;
}


void setup() {
  // Init network
  // Set the pin 12 as the communication pin
  bus.strategy.set_pin(D1);
  bus.set_id(MY_ID);
  
  // Only one bus
  bus.set_shared_network(false);
  // One way only
  bus.set_communication_mode(PJON_SIMPLEX);
  // No need for provenance
  bus.include_sender_info(true);
  // High fiability
  bus.set_crc_32(true);
  
  Serial.begin(115200);

  bus.set_receiver(ack_function);
}

bool mysend(uint16_t destination, uint8_t * payload, uint16_t length) {
  acknowledge = false;
  payload[length++] = MY_ID;

  for (int i=0 ; i<10 and !acknowledge ; i++) {
    unsigned long prev_time = millis();
    bus.send_packet(destination, payload, length);
    bus.receive();
    while (!acknowledge or (millis() - prev_time < 10)) {
      bus.receive();
    }
  }

  return acknowledge;
}


void loop() {
  for (int line_idx=0 ; line_idx<NB_LINES ; line_idx++) {
    packets[0] = (uint8_t)line_idx;
    for (int led_idx=0 ; led_idx<NB_LEDS_LINE ; led_idx++) {
      packets[1] = (uint8_t)led_idx;
      packets[2] = 30; packets[3] = 0; packets[4] = 0;
      mysend(12, packets, 5);
      delay(10);
      packets[2] = 0; packets[3] = 0; packets[4] = 0;
      mysend(12, packets, 5);
      delay(10);
    }
  }
}
