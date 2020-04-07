#include <SD.h>

//pins to use
const int tempPin = 2;
const int probePin = A0;
const int usbDigOutput = 10;

//sampling paramters
const int samplingRate = 2; //sampling rate in milliseconds
const unsigned int samplingTime = 60; //length of sample in seconds
int loopStartTime = 0;

//name of file to write to
String fileName = "datalog.txt";

//sampling variables --ignore
int tempReading = 0;
String tempString = "Temperature";
int probeReading = 0;
String probeString = "Analog Probe Voltage";



void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 Serial.print("Initializing SD card...");

 //from the sample code I have seen, setting this probe to output
 //initializes/sets up the SD card
 pinMode(usbDigOutput, OUTPUT);
 
 if (!SD.begin(usbDigOutput)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done, Starting main loop");
  loopStartTime = millis();
}

void loop() {
  
  unsigned int currentTime = millis() - loopStartTime;
  //delay(1000);
  //may need to implement a 1 second pause depending
  //on whether we choose to use a digital sensor

  //integers have a maximum value around 32000
  const unsigned int milliSamplingTime = samplingTime * 1000;
  if(currentTime < milliSamplingTime) {
    
  //I couldn't find a better way than opening and closing the file
  //for every iteration of the loop
  auto logFile = SD.open(fileName, FILE_WRITE);

  //the data stored in the file is the data gathered
  //from the last iteration of the loop
  String printLine = probeString + "," + tempString;

  //conductivity measurement
  probeReading = analogRead(probePin);
  probeString = String(probeReading);


  // TODO: update temperature reading (tempString variable)

  
  //update serial monitor and file
  Serial.print("Analog Probe Voltage:");
  Serial.println(probeString);
  logFile.println(printLine);
  logFile.close();
  }
  delay(samplingRate);
}