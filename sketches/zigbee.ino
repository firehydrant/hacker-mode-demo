#ifndef ZIGBEE_MODE_ZCZR
#error "Zigbee coordinator mode is not selected in Tools->Zigbee mode"
#endif

#include <Zigbee.h>

#define SWITCH_ENDPOINT_NUMBER 5
uint8_t input = 2;

ZigbeeColorDimmerSwitch zbSwitch = ZigbeeColorDimmerSwitch(SWITCH_ENDPOINT_NUMBER);

int lastSteadyState = LOW;
int lastFlickerableState = LOW;
int currentState;
unsigned long lastDebounceTime = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(input, INPUT_PULLUP);

  zbSwitch.setManufacturerAndModel("Espressif", "ZigbeeSwitch");
  zbSwitch.allowMultipleBinding(true);
  Zigbee.addEndpoint(&zbSwitch);
  // leave network pairing on for 3 minutes on startup to discover new devices in pairing mode
  Zigbee.setRebootOpenNetwork(180);

  if (!Zigbee.begin(ZIGBEE_COORDINATOR))
  {
    Serial.println("Zigbee failed to start!");
    Serial.println("Rebooting...");
    ESP.restart();
  }

  Serial.println("Waiting for Light to bound to the switch");
  while (!zbSwitch.bound())
  {
    Serial.printf(".");
    delay(500);
  }
  Serial.println();
  delay(1000);

  zbSwitch.setLightColor(255, 255, 255);
  delay(1000);
}

void loop()
{
  currentState = digitalRead(input);

  if (currentState == LOW)
  {
    Serial.println("no alerts");
    zbSwitch.setLightColor(255, 255, 255);
    delay(2000);
  }
  else
  {
    Serial.println("alerts!!");
    zbSwitch.setLightColor(255, 0, 0);
    delay(1000);
    zbSwitch.setLightColor(0, 0, 255);
    delay(1000);
  }
}