/*
 * JSONControlledPowerSupply0
 * 
 * Source: Bard

can you give code for the esp32, where it gets a command value (in volts) from json. 
Then uses a ADC to measure the output of a variable power supply..
Then use a PWM signal to adjust the power supply until it's output matches the json value...

Can you adjust the last example, 
so the command value is in the range of 0 to 60 volts, 
but also includes a second JSON command that limits the output range 
to set a SafetyVoltageLimit?


JSON
{
  "commandValue": 5,
  "safetyVoltageLimit": 60
}



 */
int baud = 115200;//  115200 19200

#include <Logins.h>
/* ====  store content below in: libraries/Logins/Logins.h
char ssid[] = "xxxxxxxxxx";// your network SSID (name)
char pass[] = "xxxxxxxxxx";// your network password
==== */


#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>

// Define the ADC pin and PWM pin
#define ADC_PIN 36
#define PWM_PIN 32

// Create a WiFiClient object
WiFiClient client;

// Create a JSONDocument object
//StaticJsonDocument doc;


    const size_t capacity = JSON_OBJECT_SIZE(22) + 1024;
DynamicJsonDocument doc(capacity);


// Create a PWM object
PWM pwm1;

// Define the safety voltage limit
float safetyVoltageLimit = 60;

void setup() {
  // Initialize serial communication
  Serial.begin(baud);

  // Connect to WiFi
//  WiFi.begin("YOUR_SSID", "YOUR_PASSWORD");
  WiFi.begin (ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network");

  // Initialize the ADC
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);

  // Initialize the PWM
  pwm1.setPin(PWM_PIN);
  pwm1.setFrequency(1000);
  pwm1.setDutyCycle(0);
}

void loop() {
  // Check if there is data available from the WiFi client
  if (client.available()) {
    // Read the data from the WiFi client
    String data = client.readStringUntil('\r');

    // Deserialize the JSON data
    DeserializationError error = deserializeJson(doc, data);

    // Check if the JSON deserialization was successful
    if (error) {
      Serial.println("Failed to deserialize JSON data");
      return;
    }

    // Get the command value from the JSON data
    float commandValue = doc["commandValue"];

    // Get the safety voltage limit from the JSON data
    float safetyVoltageLimit = doc["safetyVoltageLimit"];

    // Limit the command value to the safety voltage limit
    if (commandValue > safetyVoltageLimit) {
      commandValue = safetyVoltageLimit;
    }

    // Measure the output of the variable power supply
    float outputVoltage = analogRead(ADC_PIN) * 3.3 / 4096;

    // Calculate the PWM duty cycle required to match the command value
    float pwmDutyCycle = commandValue / 3.3 * 100;

    // Set the PWM duty cycle
    pwm1.setDutyCycle(pwmDutyCycle);

    // Print the command value and output voltage to the serial monitor
    Serial.println("Command value: " + String(commandValue));
    Serial.println("Output voltage: " + String(outputVoltage));
  }
}
