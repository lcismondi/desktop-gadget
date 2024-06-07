// Rotary Encoder Inputs injectora
/*
Implementa:
  * Press
  * Released
  * Longpress
  * Scroll simple
  * Scroll press

*/

#define VCC 53
//define GND GND

// Rotary Encoder Inputs
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

String currentDir = "";

unsigned long lastButtonPress = 0;
unsigned long lastButtonRelease = 0;  //Antirebote botón al soltar
unsigned long countLongPress = 0;     //Tiempo botón presionado


void setup() {

  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(VCC, OUTPUT);

  digitalWrite(VCC, HIGH);

  // Setup Serial Monitor
  Serial.begin(9600);

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

      if(btnState == LOW){
      counter2++;
      countLongPress = millis();
      }
      else{
      counter++;
      }

      currentDir = "CW";
      flagCLK = LOW;
      Serial.print("Direction: ");
      Serial.print(currentDir);
      Serial.print(" | Counter: ");
      Serial.println(counter);

    } else if (currentStateDT == HIGH && currentStateDT != lastStateDT) {

      if(btnState == LOW){
      counter2--;
      countLongPress = millis();
      }
      else{
      counter--;
      }

      currentDir = "CCW";
      flagCLK = LOW;
      Serial.print("Direction: ");
      Serial.print(currentDir);
      Serial.print(" | Counter: ");
      Serial.println(counter);
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

  //Efectos sobre el botón
  if (flagPress == HIGH) {
    flagPress = LOW;
    Serial.println("Button pressed!");
  }
  if (flagRelease == HIGH && activeLongpress == LOW) {
    flagRelease = LOW;
    Serial.print("Button released! ");
    Serial.print(millis() - countLongPress);
    Serial.println(" mseg");
  } else {
    flagRelease = LOW;
  }
  if (flagLongpress == HIGH) {
    if (millis() - countLongPress > 3000) {
      flagLongpress = LOW;
      activeLongpress = HIGH;
      Serial.println("Button pressed > 3seg");
    }
  }
}
