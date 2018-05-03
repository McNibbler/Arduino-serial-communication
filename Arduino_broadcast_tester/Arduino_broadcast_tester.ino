/* Serial Communication Through USB Test
 * Thomas Kaunzinger
 * May 1, 2018
 * 
 * Garbage code that I can use to see if I can read from the arduino.
 */

const int LED_PIN = 13;

void setup() {
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.begin(9600);
}

void loop() {

  if (Serial.available() > 0){
    //incoming = Serial.read();
    Serial.print("cool and good: ");
    //Serial.println(incoming);
    digitalWrite(LED_PIN, HIGH);
  }

  delay(3000);
  digitalWrite(LED_PIN, LOW);
  delay(2000);

}

// Linked list structure and methods
// Credit to hackerearth for tips on implementing this in C, since I've never done it in this language
struct cons{
  char data;
  cons *next;
};

class list{
  private:
  cons *head, *tail;  // important pointers (tail isn't really required, but useful)

  public:
  list(){
    head = NULL;
    tail = NULL;
  }

  // Creates the tail node of the cons linked list
  void newCons(char dataByte){
    cons *temporary = new cons;
    temporary->data = dataByte;
    temporary->next = NULL;
    if(head==NULL){
      head = temporary;
      tail = temporary;
      temporary = NULL;
    }
    else{
      tail->next = temporary;
      tail = temporary;
    }
  }

  // Creates a new cons at the front of the linked list and moves the header to point to this new node
  void frontInstert(char dataByte){
    cons *temporary = new cons;
    temporary->data = dataByte;
    temporary->next = head;
    head = temporary;
  }

  // Creates a temporary cons to reassign the header pointer to point to the next item in a list and deletes the temporary node
  void frontDelete(){
    cons *temporary = new cons;
    temporary = head;
    head = head->next;
    delete temporary;    
  }
  
};






