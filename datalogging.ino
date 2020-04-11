#include <SD_E80.h> // SD Card Reader
#include <OneWire.h> // Temperature Probe
#include <DallasTemperature.h> // Temperature Probe
#include <LiquidCrystal.h> // LCD Screen

// Pin Instantiation:
const int tempPin = 2;
const int probePin = A0;
const int usbDigOutput = 10;

// Sampling Paramters:
const unsigned long samplingRate = 100; // samples per secomd
const unsigned long samplingPeriod = 240; //length of sample period in seconds
const unsigned long milliSamplingPeriod = samplingPeriod * 1000;
unsigned long startTime;

// SD Card File Name:
String fileName = "datalog.txt";

// Temperature and Conductivity:
double probeReading = 0;
String probeString = "Voltage (Arduino Units)";
double tempReading = 0;
String tempString = "Temperature (C)";

// Display Variables:
char array1[]="Volts:  ";
char array2[]="Temp:   ";
int tim = 2;  //the value of delay time

// Setup libraries for temperature readings:
OneWire oneWire(tempPin);
DallasTemperature sensors(&oneWire);

// Initialize LCD Display Pins
const int RSPin = 4;
const int EPin = 3;
const int DB7 = 8;
const int DB6 = 7;
const int DB5 = 6;
const int DB4 = 5;
LiquidCrystal lcd(RSPin, EPin, DB4, DB5, DB6, DB7);

void setup() {
 Serial.begin(9600);
 sensors.begin();  // Set up the temp sensor libraries
 lcd.begin(16, 2);  // Set up the LCD's number of columns and rows
 
 Serial.print("Initializing SD card...");

 //from the sample code I have seen, setting this probe to output
 //initializes/sets up the SD card
 pinMode(usbDigOutput, OUTPUT);
 
 if (!SD.begin(usbDigOutput)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done, Starting main loop");
  unsigned long startTime = millis();
}

void loop() {
  // Setup sampling period / sampling rate:
  unsigned long currentTime = millis() - startTime;
  if(currentTime < milliSamplingPeriod) {

  // Update LCD  Display: 
  lcd.clear();
  for (int i = 0; i < 7; i++) {
    lcd.print(array1[i]);  // "Probe:  "
    delay(tim);
  }

  lcd.setCursor(10,0); 
  for (int i = 0; i < min(probeString.length(), 6); i++) {
    lcd.print(probeString[i]);  // "< Conductivity Data >"
    delay(tim);
  }
  
  lcd.setCursor(0,1);  // set the cursor to column 0, line 1
  for (int i = 0; i < 7; i++)
  {
    lcd.print(array2[i]);  // "Temp:  "
    delay(tim);
  }

  lcd.setCursor(10,1); 
  for (int i = 0; i < min(tempString.length(), 6); i++) {
    lcd.print(tempString[i]);  // "< Temperature Data >"
    delay(tim);
  }

  // Conductivity Measurements:
  probeReading = analogRead(probePin);
  if(probeReading != 0) {
    probeReading = 5*1023/probeReading;  
  }
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
  Serial.println(tempString);
  
  // Upload into SD file:
        // I couldn't find a better way than opening and closing the file
        // for every iteration of the loop
  auto logFile = SD.open(fileName, FILE_WRITE);
  String printLine = probeString + " " + tempString;
  logFile.println(printLine);
  logFile.close();

  unsigned long loopTime = millis() - currentTime;
  unsigned long delayTime = (1000/samplingRate)-loopTime; // samplingRate (samples per second)
  // delay(delayTime);  // This generates a hugely large number, so there's probably some value wrapping going on.
                        // TODO: Figure out how to delay to keep a standard sampling rate !!
  }
}
