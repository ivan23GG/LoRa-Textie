#include <UTFT.h>
extern uint8_t BigFont[];
struct RxStruct {
    char cases;
    char coordinatesX[8];
    char coordinatesY[8];
    int cycle;                                                            
};
RxStruct rxData;
UTFT    myGLCD(ILI9486,38,39,40,41);
bool newData = false;
const byte startMarker = 18;
const byte stopMarker = 20;
const byte rxDataLen = sizeof(rxData);
void setup() {
  
    Serial.begin(9600);
    Serial.println("\nStarting SerialStructReceive.ino\n");
    Serial1.begin(9600);
    myGLCD.InitLCD();
    myGLCD.clrScr();
    myGLCD.setFont(BigFont);
    myGLCD.setBackColor(0, 0, 0);
}
void loop() {
    recvStructWithKnownLength();
    if (newData == true) {
        showNewData();
        newData = false;
    }
}

void showNewData() {
    if(rxData.cases=='1')
    {
    Serial.println("Send Help!");
    myGLCD.print("Send Help!", 10, 0);
    Serial.println("My coordinates are:");
    myGLCD.print("My coordinates are:", 10, 30);
    Serial.print(rxData.coordinatesX);
    myGLCD.print(String(rxData.coordinatesX), 10, 60);
    Serial.print(" ");
    Serial.println(rxData.coordinatesY);
    myGLCD.print(String(rxData.coordinatesY), 150, 60);
    Serial.println(rxData.cycle);
    Serial.println(rxData.cases);
    }
    else if(rxData.cases=='2')
    {
    Serial.println("I am lost");
    myGLCD.print("I am lost", 10, 0);
    Serial.println("My coordinates are:");
     myGLCD.print("My coordinates are:", 10, 30);
    Serial.print(rxData.coordinatesX);
    myGLCD.print(String(rxData.coordinatesX), 10, 60);
    Serial.print(" ");
    Serial.println(rxData.coordinatesY);
    myGLCD.print(String(rxData.coordinatesY), 150, 60);
    Serial.println(rxData.cycle);
    Serial.println(rxData.cases);
    }
    else
    {
      Serial.println("No new messages");
       myGLCD.print("No new messages", 10, 0);
    }
    
    
}


void recvStructWithKnownLength() {
    static byte recvData[rxDataLen];
    byte rb;
    byte * structStart;
    structStart = reinterpret_cast <byte *> (&rxData);
    if (Serial1.available() >= rxDataLen + 1 and newData == false) {
      
        rb = Serial1.read();
        if (rb == startMarker) {
          if(rb!=stopMarker)
          {
            for (byte n = 0; n < rxDataLen; n++) {
                *(structStart + n) = Serial1.read();
            }  
            while (Serial1.available() > 0) {
                byte dumpTheData = Serial1.read();
            }
            newData = true;
          }
        }
    }
}
