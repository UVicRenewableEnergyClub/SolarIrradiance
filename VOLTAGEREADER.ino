// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(A0, INPUT);  //initalize pin as input
  Serial.begin(9600);
}
double repvalue = 0; //representatitve value the NANO reads off the analog pin. 0=0, 5V=1023
double voltage = 0; //variable for the measured voltage after it has been converted from its analog representative value
double current = 0.18; // a current of 180mA will be used as a constant estimate of the VI curve of solar panels. from specs of solar panel (not perfect but a educated estimate) 
double power = 0; //variable to store the power in Watts 
double efficiency = 0.15; //varible to take into account the amount of radiation converted to useable energy. Commercial solar panels have efficency of 15-20%. 
double scaledpower = 0; //a varible to store the power when its scaled to 100% based on the efficiency of panel
double panelarea = 0.009025; // Surface area of panel
double irradiance = 0; //final reported irradiance value
// the loop function runs over and over again forever
void loop() {
  repvalue = analogRead(A0); //get repvalue from Pin A0
  voltage = (((repvalue)*5)/1023)*2; // convert to voltage. 2 10KOHM resistors in series, good till 10V input so should be suitable for our solar panel
  power = voltage* current; // convert to power using measured voltage and current approximation 
  scaledpower = power / efficiency ; //convert the power to total avilable power based on the panel efficency
  irradiance = scaledpower / panelarea; // convert to irradiance based on panel area
 // Serial.println(voltage);
  Serial.println(irradiance);
  delay(1000);
}
