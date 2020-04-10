#include <SD_E80.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Pin Instantiation:
const int tempPin = 4;
const int probePin = A0;
const int usbDigOutput = 10;

// Sampling Paramters:
const int samplingRate = 2; //sampling rate in milliseconds
const unsigned int samplingTime = 60; //length of sample in seconds
int loopStartTime = 0;

// SD Card File Name:
String fileName = "datalog.txt";

// Temperature and Conductivity Variables:
double tempReading = 0;
String tempString = "Temperature (C)";
int probeReading = 0;
String probeString = "Voltage (Arduino Units)";

// Setup libraries for temperature readings:
OneWire oneWire(tempPin);
DallasTemperature sensors(&oneWire);

void setup() {
 Serial.begin(9600);
 // Start up the temp. sensor libraries
 sensors.begin();
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

  // Conductivity Measurements:
  probeReading = analogRead(probePin);
  probeString = String(probeReading);

  // Temperature Measurements: 
  sensors.requestTemperatures(); 
  tempReading = sensors.getTempCByIndex(0); // Celcius
  // tempReading = sensors.getTempFByIndex(0); // Fahrenheit
  tempString = String(tempReading);

  
  // Update Serial Monitor:
  Serial.print("Conductivity: ");
  Serial.print(probeString);
  Serial.print(", Temperature: ");
  Serial.print(tempString);

  // Upload into SD file:
        // I couldn't find a better way than opening and closing the file
        // for every iteration of the loop
  auto logFile = SD.open(fileName, FILE_WRITE);
  String printLine = probeString + " " + tempString;
  logFile.println(printLine);
  logFile.close();
  }
  delay(samplingRate);
}
