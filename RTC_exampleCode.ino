//DS3231 VCC -> Arduino 5V
//DS3231 GND -> Arduino GND
//DS3231 SDA -> Arduino A4
//DS3231 SCL -> Arduino A5

#include <Wire.h>
#include "RTClib.h"

// Create an RTC object
RTC_DS3231 rtc;

void setup() {
  // Start the serial communication
  Serial.begin(9600);

  // Check if the RTC is connected properly
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Check if the RTC lost power and if so, set the time
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Uncomment the next line to set the RTC to a specific date & time
    // rtc.adjust(DateTime(2023, 7, 8, 12, 0, 0));
  }
}

void loop() {
  // Get the current date and time
  DateTime now = rtc.now();

  // Print the current date and time to the Serial Monitor
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);
}
