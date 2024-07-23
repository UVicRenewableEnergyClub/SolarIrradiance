#include <SD.h>
#include <Arduino.h>
#include <Wire.h>
#include "RTClib.h"

// the setup function runs once when you press reset or power the board

File sd_file;
String file_name;
RTC_DS3231 rtc;

void setup() {
  pinMode(A0, INPUT);  //initalize pin as input
  pinMode(10, OUTPUT); // init pin as sd out
  Serial.begin(9600);
  if (!SD.begin(10)) {
  Serial.println("Initialization failed!");
    return;
  }
   if (!rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  // Check if the RTC lost power and if so, set the time
  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  Serial.println("Initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  file_name = "Sensor1.csv";
  sd_file = SD.open(file_name, FILE_WRITE);
  // if the file opened okay, write to it:
  if (sd_file) {
    Serial.println("Writing to file...");
    sd_file.close();
  } else {
    Serial.println("Error opening file");
  }
}
double repvalue = 0; //representatitve value the NANO reads off the analog pin. 0=0, 5V=1023
double voltage = 0; //variable for the measured voltage after it has been converted from its analog representative value
double current = 0.17; // a current of 170mA will be used as a constant estimate of the VI curve of solar panels. from specs of solar panel (not perfect but a educated estimate) 
double power = 0; //variable to store the power in Watts 
double efficiency = 0.20; //varible to take into account the amount of radiation converted to useable energy. Commercial solar panels have efficency of 15-20%. 
double scaledpower = 0; //a varible to store the power when its scaled to 100% based on the efficiency of panel
double panelarea = 0.009025; // Surface area of panel
double irradiance = 0; //final reported irradiance value
String time_collected = "";
boolean first_value = true;

// the loop function runs over and over again forever
void loop() {
  repvalue = analogRead(A0); //get repvalue from Pin A0
  voltage = (((repvalue)*5)/1023)*2; // convert to voltage. 2 10KOHM resistors in series, good till 10V input so should be suitable for our solar panel
  power = voltage* current; // convert to power using measured voltage and current approximation 
  scaledpower = power / efficiency ; //convert the power to total avilable power based on the panel efficency
  irradiance = scaledpower / panelarea; // convert to irradiance based on panel area
  String dateTimeString = getDateTimeString(); // Get the formatted date and time string
  WRITE_TO_SD(irradiance, dateTimeString);
  delay(1000);
}

// dummy
String getDateTimeString() 
{
  DateTime now = rtc.now();
  String dateTimeString = String(now.year()) + "/" +
                          String(now.month()) + "/" +
                          String(now.day()) + " " +
                          String(now.hour()) + ":" +
                          String(now.minute()) + ":" +
                          String(now.second());
  return dateTimeString;
}

void WRITE_TO_SD(double irradiance, String time_collected){

    sd_file = SD.open(file_name, FILE_WRITE);
  
    if(sd_file){
      sd_file.println((String) irradiance + "," + time_collected + ","); // readable
      // sd_file.print((String) irradiance + "," + time_collected + ","); // practical
      sd_file.close();
    } else {
      Serial.println("SD card error.");
    }

}
