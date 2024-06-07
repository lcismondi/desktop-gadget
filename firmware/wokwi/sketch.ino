#include <Adafruit_SSD1306.h>
#include <Wire.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Rotary Encoder Inputs
#define SW 5
#define DT 6
#define CLK 7


bool flagCLK;
bool flagPress = LOW;        //Flag de presión de botón
bool flagRelease = LOW;      //Flag de liberación de botón
bool flagLongpress = LOW;    //Flag de presión larga
bool activeLongpress = LOW;  //Flag de activación presión larga
bool btnState = HIGH;        //LOW is pressed
bool currentStateCLK, currentStateDT;
bool lastStateCLK;
bool lastStateBTN = HIGH;
bool lastStateDT;

int counter, counter2 = 0;

String currentDir = "";

unsigned long lastButtonPress = 0;
unsigned long lastButtonRelease = 0;  //Antirebote botón al soltar
unsigned long countLongPress = 0;     //Tiempo botón presionado



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32-C3!");

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(SCREEN_WIDTH/3, SCREEN_HEIGHT/2);
  display.print("Digit");
  display.display();

  // Set encoder pins as inputs
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);

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

      //Scroll con presión
      if(btnState == LOW){
      counter2++;
      alapantalla("CW",counter2);
      countLongPress = millis();
      }
      //Scroll simple
      else{
      counter++;
      alapantalla("CW",counter);
      }

      

    } else if (currentStateDT == HIGH && currentStateDT != lastStateDT) {

      //Scroll con presión
      if(btnState == LOW){
      counter2--;
      alapantalla("CCW",counter2);
      countLongPress = millis();
      }
      //Scroll simple
      else{
      counter--;
      alapantalla("CCW",counter);
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

  lastStateBTN = btnState;


  //Efectos sobre el botón
  if (flagPress == HIGH) {
    flagPress = LOW;
    Serial.println("Button pressed!");

    display.clearDisplay();
    display.setCursor(SCREEN_WIDTH / 6, SCREEN_HEIGHT / 2);
    display.print("Button pressed!");
    display.display();

  }

  if (flagRelease == HIGH && activeLongpress == LOW) {
    flagRelease = LOW;
    Serial.print("Button released! ");
    Serial.print(millis() - countLongPress);
    Serial.println(" mseg");

    display.clearDisplay();
    display.setCursor(SCREEN_WIDTH / 6, SCREEN_HEIGHT / 2);
    display.print("Button released!");
    display.display();

  } else {
    flagRelease = LOW;
  }

  if (flagLongpress == HIGH) {
    if (millis() - countLongPress > 3000) {
      flagLongpress = LOW;
      activeLongpress = HIGH;
      Serial.println("Button pressed > 3seg");

      display.clearDisplay();
      display.setCursor(1, SCREEN_HEIGHT / 2);
      display.print("Button pressed > 3seg");
      display.display();

    }
  }




}

void alapantalla(String dir, int cont){
      currentDir = dir;
      flagCLK = LOW;
      Serial.print("Direction: ");
      Serial.print(currentDir);
      Serial.print(" | Counter: ");
      Serial.println(cont);

      display.clearDisplay();
      display.setCursor(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
      display.print(cont);
      display.display();
}