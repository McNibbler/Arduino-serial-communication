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

///////////////
// LIBRARIES //
///////////////

#include <stdint.h>   // Because I was an idiot and thought that it was a good idea to use char to represent 8 bit ints

///////////////////////////////////
// CONSTANTS AND INITIALIZATIONS //
///////////////////////////////////

// Initializes pin 13 because it's the one that also controlls the on-board LED, so I can see the code executing
// without needing to use any actual hardware attached to any pinouts
const int_fast8_t LED_PIN = 13;

/////////////////////////
// LINKED LIST METHODS //
/////////////////////////
// Huge credits to codementor for their comprehensive guide to implementation of this in C++
// Struct creation
struct cons{
  int8_t data;
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
  void newCons(int8_t dataByte){
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
  void frontInsert(int8_t dataByte){
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

  // Dumb function to purge the contents of a list. Not even sure if it works.
  void purge(){
    tail = NULL;
    head = NULL;
  }

  // Prints all values out the serial communication port
  void printList(){
    cons *temporary = new cons;
    temporary = head;
    while(temporary != NULL){
      Serial.print(temporary->data);
      temporary = temporary->next;
    }
    Serial.println();
  }

  // Getters
  char getHead(){
    return head->data;
  }
  char getTail(){
    return tail->data;
  }


};

// Initializes the linked list of the queue of bytes being sent through the arduino.
LinkedList dataQueue;

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

  // Initializes the current command to be executed until the execution byte is sent
  LinkedList currentCommand;

  if (Serial.available() > 0){
    dataQueue.newCons(int8_t(Serial.read()));
    currentCommand.newCons(int8_t(Serial.read()));
  }
  if (currentCommand.getHead() != NULL){
    execute(currentCommand);
    currentCommand.purge();
  }

  delay(3000);
}

//////////////////////
// HELPER FUNCTIONS //
//////////////////////

// I'll make this do something. It has to keep running through from the head to the tail ('!'), running a pre-set command based
// on each char that is in this linked list buffer
void execute(LinkedList command){
  command.printList();
}




