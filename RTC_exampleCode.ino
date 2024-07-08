#include <Wire.h>
#include "RTClib.h"
#include <SD.h>
#include <SPI.h>

// Create an RTC object
RTC_DS3231 rtc;

// SD card chip select pin
const int chipSelect = 10;

// Function to get the current date and time string
String getDateTimeString() {
  DateTime now = rtc.now();
  String dateTimeString = String(now.year()) + "/" +
                          String(now.month()) + "/" +
                          String(now.day()) + " " +
                          String(now.hour()) + ":" +
                          String(now.minute()) + ":" +
                          String(now.second());
  return dateTimeString;
}

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  
  // Initialize the RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Check if the RTC lost power and if so, set the time
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // Following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  Serial.println("SD card initialized.");
}

void loop() {
  // Get the formatted date and time string
  String dateTimeString = getDateTimeString();

  // Print the current date and time to the Serial Monitor
  Serial.println(dateTimeString);

  // Log the date and time to a text file on the SD card
  File logFile = SD.open("datalog.txt", FILE_WRITE);
  if (logFile) {
    logFile.println(dateTimeString);
    logFile.close();
    Serial.println("Data logged.");
  } else {
    Serial.println("Error opening datalog.txt");
  }

  delay(1000);
}
