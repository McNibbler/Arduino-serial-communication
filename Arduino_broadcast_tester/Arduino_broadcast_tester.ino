/* Serial Communication Through USB Test
 * Thomas Kaunzinger
 * May 1, 2018
 * 
 * Garbage code that I can use to see if I can read from the arduino.
 */


void setup() {
  Serial.begin(9600);
}

void loop() {
  
  for (int i=0; i <= 10; i++){
    Serial.println(i);
    delay(10);
  }

  delay(200);
  Serial.println("nice");
  delay(3000);

}
