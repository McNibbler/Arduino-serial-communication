/* Serial Communication Through USB
 * Version: Alpha 0.1
 * Thomas Kaunzinger
 * May 3, 2018
 * 
 * My take on creating a sort of protocol for sending and recieving code through serial input and executing
 * code based on the received data.
 * 
 * Acknowledgements:
 * codementor.io for their excellent explanation on implementation of linked lists in C++
 */

///////////////////////////////////
// CONSTANTS AND INITIALIZATIONS //
///////////////////////////////////

// Initializes pin 13 because it's the one that also controlls the on-board LED, so I can see the code executing
// without needing to use any actual hardware attached to any pinouts
const int LED_PIN = 13;

/////////////////////////
// LINKED LIST METHODS //
/////////////////////////
// Huge credits to codementor for their comprehensive guide to implementation of this in C++
// Struct creation
struct cons{
  char data;
  cons *next;
};

// Lists class and all associated methods
class LinkedList{
  private:
  cons *head, *tail;  // important pointers (tail isn't really required, but useful)

  // Useful constructor to point to NULL to avoid unwanted address locations upon creation
  public:
  LinkedList(){
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


  char getHead(){
    return head->data;
  }
  char getTail(){
    return tail->data;
  }


};

// Initializes the linked list of the queue of bytes being sent through the arduino.
LinkedList dataQueue;

// Initializes the current command to be executed until the execution byte is sent
LinkedList currentCommand;

///////////
// SETUP //
///////////

void setup() {
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Serial.begin(9600);
}

////////////////////
// EXECUTION LOOP //
////////////////////

void loop() {

  if (Serial.available() > 0){
    dataQueue.newCons(char(Serial.read()));
    currentCommand.newCons(char(Serial.read()));
  }
  if (currentCommand.getHead() == char('!')){
    execute(currentCommand);
  }

}

//////////////////////
// HELPER FUNCTIONS //
//////////////////////

// I'll make this do something. It has to keep running through from the head to the tail ('!'), running a pre-set command based
// on each char that is in this linked list buffer
void execute(LinkedList command){
  
}




