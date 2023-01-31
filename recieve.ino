//===================
// Using Serial to send and receive structs between two Arduinos
//      the Tx is sending at 500msec intervals
//      it is assumed that this Rx program can loop in less than half that time

//===================


        // data to be received

struct RxStruct {
    char textB[16];         // 16 bytes
    int valC;               //  2
    unsigned long valD;     //  4
    byte padding[10];       // 10
                            //------
                            // 32
};

RxStruct rxData;

bool newData = false;

const byte startMarker = 255;
const byte rxDataLen = sizeof(rxData);

#include <SoftwareSerial.h>
SoftwareSerial recvSerial(6,7); // Rx and Tx


//=================================

void setup() {
    Serial.begin(9600);
    Serial.println("\nStarting SerialStructReceive.ino\n");

        // set up Serial for communication
    recvSerial.begin(9600);
}

//============

void loop() {

    recvStructWithKnownLength();
        // this bit checks if a message has been received
    if (newData == true) {
        showNewData();
        newData = false;
    }
}

//=============

void showNewData() {

    Serial.print("This just in    ");
    Serial.print(rxData.textB);
    Serial.print(' ');
    Serial.print(rxData.valC);
    Serial.print(' ');
    Serial.println(rxData.valD);
}

//============

void recvStructWithKnownLength() {
    static byte recvData[rxDataLen];
    byte rb;
    byte * structStart;
    structStart = reinterpret_cast <byte *> (&rxData);

    if (recvSerial.available() >= rxDataLen + 1 and newData == false) {
        rb = recvSerial.read();

        if (rb == startMarker) {
                // copy the bytes to the struct
            for (byte n = 0; n < rxDataLen; n++) {
                *(structStart + n) = recvSerial.read();
            }
                // make sure there is no garbage left in the buffer
            while (recvSerial.available() > 0) {
                byte dumpTheData = recvSerial.read();
            }

            newData = true;
        }
    }
}

//============
