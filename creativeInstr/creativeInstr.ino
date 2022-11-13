

/*

  * Creative Instrument
  * v2.0

  
  5V - VCC
  D8 - Buzzer
  D9 - Trig Pin
  D10 - Echo Pin
  D11 - Switch

  A0 - Potentiometer

*/

// Global variables 
const int trigPin = 9;
const int echoPin = 10;
int pPin = 8;
const int potentioPin = A0;

long currentTime = 0;
int distanceOffset = 0;
int delayTime = 10;
int pVoltRead = 0;
int trueVolt = 0;
float myList[2];


void setup() 
{
  pinMode(11, INPUT); //button
  pinMode(A0, INPUT); //potentiometer
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(9600); // Starts the serial communication
}


void loop() 
{
  pVoltRead = analogRead(potentioPin);          // Potentiometer input
  // Serial.print("PotentioMeter Reading: ");
  // Serial.println(pVoltRead);
  trueVolt = (pVoltRead * 5.0/1023);           // Conversion of potentiometer input (mapped from 1 to 5 volt) 
  Serial.println(trueVolt);
 
  // Time interval 
  if (millis() - currentTime > 200 || currentTime == 0) 
  {
    // If the switch is pressed, reset the offset
    if (digitalRead(11) == 1) 
    {
      distanceOffset = findDistance();
      Serial.print("new offset: ");
      Serial.println(distanceOffset);
      currentTime = millis();
    }
  }
  
 // Calculating distance considering the offset
  int distance = findDistance() - distanceOffset;
  // Serial.println("Distance: ");
  // Serial.println(distance);
  // int pitch = intoNotes(map(distance, 0, 20, 1, 7));            // Uncommented
  //int vib = map(analogRead(A0), 1023, 0, 0, 100);

  int pitch_ = intoNotes(map(distance, 0, 20, 1, 7));             // Notes stored in a temp variable
  noteModifier(trueVolt);                                         // Function called that stores values in a list (declared globally)

  // Frequency and Time duration calculation
  int pitch = pitch_ * myList[0];
  int toneDuration = myList[1];

  
  // If the distance is less than 20 cm, play the tune, else notone()
  if(distance <= 20)
  {
    tone(pPin, pitch,toneDuration);                              // Controls 1. PinNumber, 2. Frequency, 3. Time Duration
    // tone(pPin, pitch);
    // Serial.println("sound");
  }
  else
  {
    noTone(pPin);
  }

  delay(delayTime);
}


// Function to measure distance of an obstacle - in centimeter
int findDistance() 
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  return (duration * 0.034 / 2);
}


// Function that determines the frequency of each sound
// Takes distance calculated as a parameter - argument is mapped from (0 to 20) cm before it is fed into the function
float intoNotes(int x)
{
  switch(x)
  {
    case 1:
      return 261.63;
    case 2:
      return 293.66;
    case 3:
      return 329.63;
    case 4:
      return 349.23;
    case 5:
      return 392.00;
    case 6:
      return 440.00;
    case 7:
      return 493.88;
    default:
      return 0;
  }
}


// Function that modifies a list (myList) that has two elements:
// myList[0] has a coefficient that alters the pitch of a tone
// myList[1] has actual time duration
// Void function that returns nothing
void noteModifier(int volt)
{
  float val1 = 0;

  if (volt > 4 && volt <= 4.6)
    val1 = 5;
  else if (volt > 3 && volt <= 4)
    val1 = 4;
  else if (volt > 2 && volt <= 3)
    val1 = 3;
  else if (volt > 1 && volt <= 2)
    val1 = 2;
  else
    val1 = 1;

  myList[0] = val1;
  myList[1] = val1 * 1000;
  
}


