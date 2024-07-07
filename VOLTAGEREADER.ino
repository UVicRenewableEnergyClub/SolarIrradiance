#include <SD.h>
#include <Arduino.h>

// the setup function runs once when you press reset or power the board

File sd_file;
String file_name;

void setup() {
  pinMode(A0, INPUT);  //initalize pin as input
  pinMode(10, OUTPUT); // init pin as sd out
  Serial.begin(9600);
  if (!SD.begin(10)) {
  Serial.println("Initialization failed!");
    return;
  }
  Serial.println("Initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  file_name = GET_TIME() + ".csv";
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
double current = 0.18; // a current of 180mA will be used as a constant estimate of the VI curve of solar panels. from specs of solar panel (not perfect but a educated estimate) 
double power = 0; //variable to store the power in Watts 
double efficiency = 0.15; //varible to take into account the amount of radiation converted to useable energy. Commercial solar panels have efficency of 15-20%. 
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
  time_collected = GET_TIME(); // dummy
  WRITE_TO_SD(irradiance, time_collected);
  delay(1000);
}

// dummy
String GET_TIME(){

  unsigned long time_elapsed = millis();
  return (String) time_elapsed;
  
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
