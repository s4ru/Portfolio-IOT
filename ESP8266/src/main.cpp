#include <Arduino.h>
#include <ClosedCube_HDC1080.h>

ClosedCube_HDC1080 sensorHT;

// Constants
const int numReadings = 3;  // Adjust this value based on how many readings you want to take in each state

// State machine states
enum State {
  STATE_B,
  STATE_C
};

State currentState = STATE_B;  // Initial state
int readingsCount = 0;  // Counter for the number of readings in the current state
float sumTemperature = 0;
float sumHumidity = 0;

// Function declarations
void stateB();
void stateC();

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println("Iniciando la configuracion del chip");
  Serial.println("Iniciando configuracion del sensor");
  sensorHT.begin(0x40);
}

void loop() {
  switch (currentState) {
    case STATE_B:
      stateB();
      break;
    case STATE_C:
      stateC();
      break;
  }
}

// State B: Read Humidity
void stateB() {
  Serial.println("State B: Leyendo humedad");
  delay(2000);

  float humidity = sensorHT.readHumidity();
  sumHumidity += humidity;

  if (++readingsCount >= numReadings) {
    float avgHumidity = sumHumidity / numReadings;

    Serial.println("");
    Serial.print("Promedio de humedad en State B: ");
    Serial.println(avgHumidity, 2);  // Display average humidity with 2 decimal places

    currentState = STATE_C;  // Transition to next state
    readingsCount = 0;  // Reset readings count and sum for the next state
    sumHumidity = 0;
  }
}

// State C: Read Temperature
void stateC() {
  Serial.println("State C: Leyendo temperatura");
  delay(2000);

  float temperature = sensorHT.readTemperature();
  sumTemperature += temperature;

  if (++readingsCount >= numReadings) {
    float avgTemperature = sumTemperature / numReadings;

    Serial.println("");
    Serial.print("Promedio de temperatura en State C: ");
    Serial.println(avgTemperature, 2);  // Display average temperature with 2 decimal places

    currentState = STATE_B;  // Transition back to State B
    readingsCount = 0;  // Reset readings count and sum for the next state
    sumTemperature = 0;
  }
}

