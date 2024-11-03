// Rotary Encoder Inputs
/*
Implementa:
  * Press
  * Released
  * Longpress
  * Scroll simple
  * Scroll press
  * Doble click

*/

#define VCC 53
//define GND GND

// Rotary Encoder Inputs
/*
#define SW 13
#define DT 12
#define CLK 16
*/

// Rotary enclosure
#define SW 13
#define DT 12
#define CLK 16


bool flagCLK;
bool flagPress = LOW;        //Flag de presión de botón
bool flagRelease = LOW;      //Flag de liberación de botón
bool flagLongpress = LOW;    //Flag de presión larga
bool activeLongpress = LOW;  //Flag de activación presión larga
bool btnState = HIGH;        //LOW is pressed

int counter, counter2 = 0;
int currentStateCLK, currentStateDT;
int lastStateCLK;
int lastStateBTN = HIGH;
int lastStateDT;
int dobleClickTime = 150;
int dobleClickStage = 0;

String currentDir = "";

unsigned long lastButtonPress = 0;
unsigned long lastButtonRelease = 0;                            //Antirebote botón al soltar
unsigned long countLongPress = 0;                               //Tiempo botón presionado
unsigned long dobleClickSt1, dobleClickSt2, dobleClickSt3 = 0;  //Transición del doble click

void setup() {

  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(VCC, OUTPUT);

  digitalWrite(VCC, HIGH);

  // Setup Serial Monitor
  Serial.begin(115200);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
}

void loop() {


  //**********************************************************************
  //                          Control del encoder
  //**********************************************************************

  currentStateCLK = digitalRead(CLK);
  currentStateDT = digitalRead(DT);

  //Serial plotter
  // Serial.print("CLK:");
  // Serial.print(currentStateCLK);
  // Serial.print(",");
  // Serial.print("DT:");
  // Serial.println(currentStateDT);

  if (currentStateCLK == LOW && currentStateCLK != lastStateCLK) {

    flagCLK = HIGH;
    lastStateCLK = currentStateCLK;

  } else if (currentStateCLK == HIGH && currentStateCLK != lastStateCLK) {

    flagCLK = LOW;
    lastStateCLK = currentStateCLK;

  } else {
    //lastStateCLK = currentStateCLK;
  }

  if (flagCLK == HIGH) {

    if (currentStateDT == LOW && currentStateDT != lastStateDT) {

      if (btnState == LOW) {
        counter2++;
        countLongPress = millis();

        currentDir = "CW";
        flagCLK = LOW;
        Serial.print("Direction: ");
        Serial.print(currentDir);
        Serial.print(" | Counter2: ");
        Serial.println(counter2);

      } else {
        counter++;

        currentDir = "CW";
        flagCLK = LOW;
        Serial.print("Direction: ");
        Serial.print(currentDir);
        Serial.print(" | Counter: ");
        Serial.println(counter);
      }



    } else if (currentStateDT == HIGH && currentStateDT != lastStateDT) {

      if (btnState == LOW) {
        counter2--;
        countLongPress = millis();

        currentDir = "CCW";
        flagCLK = LOW;
        Serial.print("Direction: ");
        Serial.print(currentDir);
        Serial.print(" | Counter2: ");
        Serial.println(counter2);

      } else {
        counter--;

        currentDir = "CCW";
        flagCLK = LOW;
        Serial.print("Direction: ");
        Serial.print(currentDir);
        Serial.print(" | Counter: ");
        Serial.println(counter);
      }
    }

    //lastStateDT = currentStateDT;
  } else {
    lastStateDT = currentStateDT;
  }


  //**********************************************************************
  //                          Botón
  //**********************************************************************

  // Read the button state
  btnState = digitalRead(SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW && lastStateBTN == HIGH) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      flagPress = HIGH;
      flagLongpress = HIGH;
      activeLongpress = LOW;
      //flagRelease = LOW;
      countLongPress = millis();
    }

    // Remember last button press event
    lastButtonPress = millis();
  }
  //Si detecta señal HIGH y antes estaba presionado, el botón se liberó
  else if (btnState == HIGH && lastStateBTN == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonRelease > 50) {
      flagRelease = HIGH;
      flagLongpress = LOW;
    }
    // Remember last button release event
    lastButtonRelease = millis();
  }

  // Remember last button state
  lastStateBTN = btnState;


  //**********************
  //Efectos sobre el botón
  //**********************

  if (flagPress == HIGH) {
    flagPress = LOW;
    //----------------------------------
    //Serial.println("Button pressed!");
    //----------------------------------
    if (dobleClickStage == 0) {

      dobleClickStage = 1;
      //Serial.print(dobleClickStage);
      dobleClickSt1 = millis();
      //Serial.print(" | ");
      //Serial.println("Inicio doble click");

    } else if (dobleClickStage == 2) {

      dobleClickSt3 = millis();
      dobleClickSt2 = dobleClickSt3 - dobleClickSt2;

      if (dobleClickSt2 > dobleClickTime) {

        Serial.println("Simple press");
        dobleClickStage = 0;

      } else {

        dobleClickStage = 3;
        //Serial.print(dobleClickStage);
        //Serial.print(" | ");
        //Serial.print(dobleClickSt2);
        //Serial.println("mseg");
      }

    } else {
      Serial.print(dobleClickStage);
      Serial.println(" | Error 1");
      dobleClickStage = 0;
    }
  }
  if (flagRelease == HIGH && activeLongpress == LOW) {
    flagRelease = LOW;
    //----------------------------------
    //Serial.print("Button released!");
    //----------------------------------
    //Serial.print(millis() - countLongPress);
    //Serial.println(" mseg");

    if (dobleClickStage == 1) {

      dobleClickSt2 = millis();
      dobleClickSt1 = dobleClickSt2 - dobleClickSt1;

      if (dobleClickSt1 > dobleClickTime) {

        Serial.println("Simple press");
        dobleClickStage = 0;

      } else {

        dobleClickStage = 2;
        //Serial.print(dobleClickStage);
        //Serial.print(" | ");
        //Serial.print(dobleClickSt1);
        //Serial.println("mseg");
      }

    } else if (dobleClickStage == 3) {

      dobleClickSt3 = millis() - dobleClickSt3;

      if (dobleClickSt3 > dobleClickTime) {

        Serial.println("Simple press");
        dobleClickStage = 0;

      } else {

        dobleClickStage = 4;
        //Serial.print(dobleClickStage);
        //Serial.print(" | ");
        //Serial.print(dobleClickSt3);
        //Serial.println("mseg");
        Serial.println("Doble press");
        dobleClickStage = 0;
      }

    } else {
      Serial.print(dobleClickStage);
      Serial.println(" | Error 2");
      dobleClickStage = 0;
    }

  } else {
    flagRelease = LOW;
  }
  //----------------------------------
  //Long press
  //----------------------------------
  if (flagLongpress == HIGH) {
    if (millis() - countLongPress > 3000) {
      flagLongpress = LOW;
      activeLongpress = HIGH;
      Serial.println("Button pressed > 3seg");
      dobleClickStage = 0;
    }
  }
}
