#include <Keypad.h>
const byte ROWS = 5; //four rows
const byte COLS = 4; //four columns
char customKey;
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'F','f','#','*'},
  {'1','2','3','u'},
  {'4','5','6','d'},
  {'7','8','9','E'},
  {'l','0','r','e'}
};
byte rowPins[ROWS] = {12, 11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 6, 5, 7}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

        // data to be sent
struct TxStruct {
    char textA[16];         // 16 bytes
    int valA;               //  2
    unsigned long valB;     //  4
    byte padding[10];       // 10
                            //------
                            // 32
};

TxStruct txData = {"x", 235, 0};

bool newTxData = false;

const byte startMarker = 255;
const byte txDataLen = sizeof(txData);

        // timing variables for sending frequency
unsigned long prevUpdateTime = 0;
unsigned long updateInterval = 500;



//=================================

void setup() {
    Serial.begin(9600);
    Serial.println("\nStarting SerialStructSend.ino\n");

        // set up Serial for communication
    Serial1.begin(9600); // to match SoftwareSerial on Uno

}

//============

void loop() {
  customKey = customKeypad.getKey(); 
  if (customKey>0){ 
    updateDataToSend();     
    transmitData();
  }
}

//============

void updateDataToSend() {

    
        if (newTxData == false) { // ensure previous message has been sent

            char sText = customKey;
            txData.textA[0]=sText;
            txData.valA += 10;
            if (txData.valA > 300) {
                txData.valA = 235;
            }
            txData.valB = millis();
            newTxData = true;
        }
    }




void transmitData() {

    if (newTxData == true) {
        Serial1.write(startMarker);
        Serial1.write((byte*) &txData, txDataLen);

            // for demo show the data that as been sent
        Serial.print("Sent ");
        Serial.print(txData.textA);
        Serial.print(' ');
        Serial.print(txData.valA);
        Serial.print(' ');
        Serial.println(txData.valB);

        newTxData = false;
    }
}
