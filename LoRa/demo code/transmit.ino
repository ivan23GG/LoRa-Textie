#include <SoftwareSerial.h>
#include <Keypad.h>
const byte ROWS = 5; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
  {'F','f','#','*'},
  {'1','2','3','u'},
  {'4','5','6','d'},
  {'7','8','9','E'},
  {'l','0','r','e'}
};
byte rowPins[ROWS] = {12, 11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
SoftwareSerial sendSerial(6,7);
struct TxStruct {
    char cases;
    char coordinatesX[8];
    char coordinatesY[8];
    int cycle;                                           
};

TxStruct txData{'a', "1", "1", 1};
bool newTxData = false;
const byte startMarker = 18;
const byte stopMarker = 20;
const byte txDataLen = sizeof(txData);
unsigned long prevUpdateTime = 0;
unsigned long updateInterval = 200;

void setup() {
    Serial.begin(9600);
    Serial.println("\nStarting SerialStructSend.ino\n");
    sendSerial.begin(9600);
}
void loop() {
    updateDataToSend();
    transmitData();
}
void updateDataToSend() {
    if (millis() - prevUpdateTime >= updateInterval) {
        prevUpdateTime = millis();
        if (newTxData == false) { 
        char x[]="23.4029";
        char y[]="42.6506";
        char customKey = customKeypad.getKey();
        if (customKey>0){
         txData.cases=customKey;
         }
            strcpy(txData.coordinatesX, x);
            strcpy(txData.coordinatesY, y);
            txData.cycle++;
            newTxData = true;
        }
    }
}
void transmitData() {
    if (newTxData == true) {
        sendSerial.write(startMarker);
        sendSerial.write((byte*) &txData, txDataLen);
        sendSerial.write(stopMarker);
        Serial.print("Sent ");
        Serial.println(txDataLen);
        Serial.println(txData.coordinatesX);
        Serial.println(txData.coordinatesY);
        Serial.println(txData.cases);
        Serial.println(txData.cycle);
        newTxData = false;
    }
}
