// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(A0, INPUT);  //initalize pin as input
  Serial.begin(9600);
}
double repvalue = 0;
double voltage = 0;
double current = 0.18;
double power = 0;
double efficiency = 0.15;
double scaledpower = 0;
double panelarea = 0.009025;
double irradiance = 0;
// the loop function runs over and over again forever
void loop() {
  repvalue = analogRead(A0);
  voltage = (((repvalue)*5)/1023)*2; // convert to voltage. 2 10KOHM resistors in series, good till 10V input
  power = voltage* current;
  scaledpower = power / efficiency ;
  irradiance = scaledpower / panelarea;
  Serial.println(voltage);
  delay(1000);
}
