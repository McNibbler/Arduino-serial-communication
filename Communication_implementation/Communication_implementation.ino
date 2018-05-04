/* Serial Communication Through USB
 * Version: Alpha 0.2
 * Thomas Kaunzinger
 * May 4, 2018
 * 
 * My take on creating a sort of protocol for sending and recieving code through serial input and executing
 * code based on the received data.
 * 
 * Acknowledgements:
 * codementor.io for their excellent explanation on implementation of linked lists in C++
 * Matt Stancliff for his guide to writing clean, modern C code
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

// Constant bytes that represent the characters being sent as commands
const int8_t READ = 'r';
const int8_t WRITE = 'w';
const int8_t DAC_A = 'a';
const int8_t DAC_B = 'b';
const int8_t DAC_2 = '2';
const int8_t DONE = '!';


// DAC CONTROL CONSTANTS //

// R/W (write is active low)
const int8_t READ_BIN = 1;
const int8_t WRITE_BIN = 0;

// Registers for controlling DAC
const int8_t DAC_REGISTER_BIN = 0;  // 000

// DAC channel addresses
const int8_t DAC_A_BIN = 0;   // 000
const int8_t DAC_B_BIN = 2;   // 010
const int8_t DAC_2_BIN = 4;   // 100

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
  int8_t getHead(){
    return head->data;
  }
  int8_t getTail(){
    return tail->data;
  }
};

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

// Initializes the linked list of the queue of bytes being sent through the arduino.
LinkedList dataQueue;

// Initializes the current command to be executed until the execution byte is sent
LinkedList currentCommand;

void loop() {  

  if (Serial.available() > 0){
    dataQueue.newCons(int8_t(Serial.read()));
    currentCommand.newCons(int8_t(Serial.read()));
  }
  if (currentCommand.getTail() == DONE){
    executeCommand(currentCommand);
    currentCommand.purge();
  }

  delay(100);
}

//////////////////////
// HELPER FUNCTIONS //
//////////////////////

// I'll make this do something. It has to keep running through from the head to the tail ('!'), running a pre-set command based
// on each byte that is in this linked list buffer
void executeCommand(LinkedList command){
  int8_t header;  // Initializes the data to send
  int16_t data;

  // Appends the read or write bits to the front of the header. Purges and ignores with with invalid syntax.
  if      (command.getHead() == READ)   {header += READ_BIN<<7;}
  else if (command.getHead() == WRITE)  {header += WRITE_BIN<<7;}
  else{
    command.purge();
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    return;
  }
  
  command.frontDelete();  // Clears out the already executed piece of the command

  header += 0 << 6;                 // Appends the reserved 0 bit
  header += DAC_REGISTER_BIN << 3;  // Appends the register for the DAC since I don't want the user to access anything else

  // Appends the DAC address to the header. Purges and ignores with with invalid syntax.
  if      (command.getHead() == DAC_A)  {header += DAC_A_BIN;}
  else if (command.getHead() == DAC_B)  {header += DAC_B_BIN;}
  else if (command.getHead() == DAC_2)  {header += DAC_2_BIN;}
  else{
    command.purge();
    digitalWrite(LED_PIN, HIGH);
    delay(1000);
    digitalWrite(LED_PIN, LOW);
    return;
  }

  command.frontDelete();  // Clears out the already executed piece of the command

  data += command.getHead() << 8;   // First 8 bits of the data
  command.frontDelete();            // Clears the first half of the data command from the linked list
  data += command.getHead();        // Rest of the 8 bits of the data
  command.purge();                  // Purges whatever is left of the command.

  sendBits(header, data);   // Function to send the data to the DAC. Only reaches here if the whole command is valid.
  
}

// I think I should start implementing this in the actual DAC controller program soon
void sendBits(int8_t header, int16_t data){

  // idk it's something dumb to make sure it actually worked
  for(int_fast8_t i = 0; i < 5; i++){
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
    delay(500);
  }
  delay(4500);
}


