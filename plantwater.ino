#include <morse.h>

#define PIN_STATUS 0
#define PIN_PUMP 3
#define PIN_SOIL A1
#define PIN_WATER A2

// Low-voltage / dry threshold for soil moisture. Start watering < this value.
#define SOIL_THRESHOLD_LOW_DRY 700
// High-voltage / wet threshold for soil moisture. Stop watering >= this value.
#define SOIL_THRESHOLD_HIGH_WET 900

// Low-voltage / dry threshold for water reservoir sensor.
#define WATER_THRESHOLD_LOW_EMPTY 550

// Voltages with 220kOhm (to ground) divider:
// air          0.00v   0
// spongy soil  2.68v   548
//              3.11v   636
// water        4.97v   1017
//              4.99v   1021

#define SENSE_DELAY_MS 1000
// Wait until it has been dry DELAY * DRY_COUNT ms before starting the pump.
// 1000 * 15 = 1 minute.
#define DRY_COUNT_THRESHOLD 15
// Continuously run the pump at most DELAY * PUMP_COUNT ms before resting.
// 1000 * (60 * 3) = 3 minutes.
#define PUMP_COUNT_MAX 180

/**
 * The number of times the soil has been observed dry. The pump won't be started
 * until DRY_COUNT_THRESHOLD dry observations have been made.
 */
int dryCount = 0;

LEDMorseSender status(PIN_STATUS, 15 /* words per minute */);

void setup() {
  pinMode(PIN_PUMP, OUTPUT);
  pinMode(PIN_SOIL, INPUT);
  pinMode(PIN_WATER, INPUT);
  dryCount = 0;

  status.setup();
  status.setMessage(String("setup "));
  status.sendBlocking();
}

/**
 * Checks water reservoir and soil moisture; aborts if the water reservoir is
 * empty, and runs the water pump if the soil has been dry.
 */
void loop() {
  status.setMessage(String("l "));
  status.sendBlocking();

  int water = analogRead(PIN_WATER);
  if (water <= WATER_THRESHOLD_LOW_EMPTY) {
    status.setMessage(String("empty "));
    status.sendBlocking();
  } else {
    int soil = getSoilUpdateDryCount();
    if (soil <= SOIL_THRESHOLD_LOW_DRY) {
      status.setMessage(String("d ")); // dry
      status.sendBlocking();
      if (dryCount >= DRY_COUNT_THRESHOLD) {
        runWaterPump();
      }
    }
  }
  delay(SENSE_DELAY_MS);
}

/**
 * Reads and returns the soil moisture sensor value.
 * Increments or resets dryCount according to the value v. a threshold.
 */
int getSoilUpdateDryCount() {
  int soil = analogRead(PIN_SOIL);
  if (soil < SOIL_THRESHOLD_LOW_DRY) {
    dryCount++;
  } else {
    dryCount = 0;
  }
  return soil;
}

/**
 * Turns the water pump on, then waits until the soil becomes wet, or times out.
 */
void runWaterPump() {
  status.setMessage(String("pump "));
  status.sendBlocking();
  status.setMessage(String("e ")); // engaged

  digitalWrite(PIN_PUMP, HIGH);
  int pumpCount = 0;
  while (
      getSoilUpdateDryCount() <= SOIL_THRESHOLD_HIGH_WET
      && ++pumpCount < PUMP_COUNT_MAX) {
    status.sendBlocking();
    delay(SENSE_DELAY_MS);
  }
  digitalWrite(PIN_PUMP, LOW);
  // If pumping timed out, wait as long again as we pumped.
  if (pumpCount >= PUMP_COUNT_MAX) {
    status.setMessage(String("r ")); // rest
    pumpCount = 0;
    while (++pumpCount < PUMP_COUNT_MAX) {
      status.sendBlocking();
      getSoilUpdateDryCount();
      delay(SENSE_DELAY_MS);
    }
  }
}
