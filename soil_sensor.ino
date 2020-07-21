                                                         #include <ESP8266WiFi.h>
#include <ThingSpeak.h>

// WiFi settings
const char* ssid = "ssid";
const char* password = "password";
const int PowerPin = D8;

char thingSpeakAPIKey[17] = "TS_API_KEY";
char thingSpeakChannelID[6] = "TS_CHANNEL";

int moistureSensor = A0;

//flag for saving data
const char* thingSpeakserver = "api.thingspeak.com";
const char* host = "moisturedeepsleep";

// Time to sleep (in seconds):
const uint64_t sleepTimeS = 60 * 60e6; //60 seconds * 60 minutes * 2 hours


void setup()
{
  // Serial
  Serial.begin(115200);
  pinMode(PowerPin, OUTPUT);
  digitalWrite(PowerPin, HIGH);
  WiFi.hostname(String(host));
  // Connect to WiFi
  WiFi.begin(ssid, password);
  int connectRetries = 0;

  while (WiFi.status() != WL_CONNECTED && connectRetries < 10) {
    delay(500);
    Serial.print(".");
    connectRetries++;
  }
  if (connectRetries < 10) {
    Serial.println("");
    Serial.println("WiFi connected");

    // Print the IP address
    Serial.println(WiFi.localIP());

    int sensorValue = analogRead(moistureSensor);
    Serial.print("Value: ");
    Serial.println(sensorValue);
    postReading(sensorValue, thingSpeakAPIKey, thingSpeakChannelID);

  }
  // Sleep
  Serial.println("ESP8266 in sleep mode");
  ESP.deepSleep(sleepTimeS);
}

void loop()
{

}

void postReading(int reading,  char* APIKey, char* channelID) {
  WiFiClient client;
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  ThingSpeak.setField(1, reading);
  int x = ThingSpeak.writeFields(atoi(channelID), APIKey);
  Serial.println(x);
}
