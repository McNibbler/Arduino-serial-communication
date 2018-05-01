/* Serial Communication Through USB Test
 * Thomas Kaunzinger
 * May 1, 2018
 * 
 * Garbage code that I can use to see if I can read from the arduino.
 */

const int LED_PIN = 13;
String incoming;

void setup() {
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0){
    incoming = Serial.readString();
    Serial.print("cool and good: ");
    Serial.println(incoming);
    digitalWrite(LED_PIN, HIGH);
  }

  for (int i=0; i <= 10; i++){
    Serial.println(i);
  }

  delay(500);
  Serial.println("nice");
  delay(3000);
  digitalWrite(LED_PIN, LOW);
  delay(2000);

}
