#include <WiFi.h>
#include <PubSubClient.h>

#define WIFISSID "Buffalo-G-C400"
#define PASSWORD "5nnkffkncx35u"
#define TOKEN "BBFF-L0MWw4LOeifWz3Me8wX1a8dl4oRpS9"
#define MQTT_CLIENT_NAME "mathmathscien"

#define VARIABLE_LABEL "sensor"
#define DEVICE_LABEL "esp32"

#define SENSOR 35

char mqttBroker[] = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
char str_sensor[10];
int numPublished = 0;

WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  //p[length] = NULL;
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connecting...");

    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 2 seconds");
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(74880);
  WiFi.begin(WIFISSID, PASSWORD);
  pinMode(SENSOR, INPUT);

  Serial.println();
  Serial.print("Waiting for WiFi...");

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", "");
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL);

  float sensor = analogRead(SENSOR);
  dtostrf(sensor, 4, 2, str_sensor);

  int currentTs = 319053000 + 250 * numPublished++;
  sprintf(payload, "%s {\"value\": %s, ", payload, str_sensor);
  sprintf(payload, "%s \"timestamp\": 1649", payload);
  sprintf(payload, "%s%d}}", payload, currentTs);
 
  Serial.println("Publishing data to Ubidots Cloud");
  Serial.println(payload);
  
  client.publish(topic, payload);
  client.loop();
  delay(200);
}
