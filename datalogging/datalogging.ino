#include <SD.h>

const int tempPin = 2;
const int probePin = A0;
const int usbDigOutput = 10;

int tempReading = 0;
String tempString = "";
int probeReading = 0;
String probeString = "";

String fileName = "datalog.txt";


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 Serial.print("Initializing SD card...");
 
 pinMode(usbDigOutput, OUTPUT);
 
 if (!SD.begin(usbDigOutput)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  logFile = SD.open(fileName, FILE_WRITE);
  logFile.println("Temperature, Analog Probe Voltage");
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);
  //may need to implement a 1 second pause depending
  //on whether we choose to use a digital sensor

  probeReading = analogRead(probePin);
  probeString = String(probeReading);
  Serial.print("Analog Probe Voltage:");
  Serial.println(probeString);
  String printLine = probeString + "," + tempString;
  logFile.println(printLine);
  
  delay(2);
}
