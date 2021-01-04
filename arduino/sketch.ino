#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

#define STASSID "zagrobelny"
#define STAPSK  "bridget1"
#define PIN D7
#define NUM_LEDS 6

const int led = D4;
const char *clientID = "esp8266_0";
const char *mqtt_server = "192.168.86.42";
const unsigned int interval = 1e3;
unsigned long last = 0;

int i = 0;
int offset = 255 / NUM_LEDS;
int l = NUM_LEDS * offset;

WiFiClient espClient;
PubSubClient client(espClient);
CRGB leds[NUM_LEDS];

bool colorOverride = false;
CRGB color;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  char buf[9] = {0};
  if (length > 1) {// && (char)payload[i] == '#') {
    buf[0] = '0';
    buf[1] = 'x';
    for (int i=1; i<length && i < 8; i++) {
      char c = (char)payload[i];
      buf[i+1] = c;
    }
    long l = strtol(buf, NULL, 16);
    if (l != 0) {
      colorOverride = true;
      color = CRGB((uint32_t)l);
    }
  }
}

void reconnect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  while (!client.connected()) {
    Serial.print("Attempting MQTT connection to ");
    Serial.print(mqtt_server);
    Serial.println(":1883...");
    if (client.connect(clientID)) {
      Serial.println("connected");
      client.subscribe("leds");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  WiFi.begin(STASSID, STAPSK);

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUM_LEDS);
}
void loop() { 
  digitalWrite(led, HIGH);

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (micros() - last > interval) {
    last = micros();
    if (!colorOverride) {
      fill_rainbow(leds, NUM_LEDS, i, offset);
    } else {
      fill_solid(leds, NUM_LEDS, color); 
    }
    FastLED.show();
    i = (i + 1) % l;
  }

  //while (true) {
  //  for (int i = 0; i < NUM_LEDS*offset; i++) {
  //    fill_rainbow(leds, NUM_LEDS, i, offset);
  //    FastLED.show();
  //    delay(10);
  //  }
  //}
}
