// ###########################################################################################################################################
// #
// # WordClock code for the printables WordClock 16x16 matrix project: https://www.printables.com/de/model/331012-wordclock-16x16-led-matrix
// #
// # Code by https://github.com/AWSW-de
// #
// # Released under license: GNU General Public License v3.0: https://github.com/AWSW-de/WordClock-16x16-LED-matrix/blob/main/LICENSE
// #
// ###########################################################################################################################################
/*
      ___           ___           ___           ___           ___           ___       ___           ___           ___     
     /\__\         /\  \         /\  \         /\  \         /\  \         /\__\     /\  \         /\  \         /\__\    
    /:/ _/_       /::\  \       /::\  \       /::\  \       /::\  \       /:/  /    /::\  \       /::\  \       /:/  /    
   /:/ /\__\     /:/\:\  \     /:/\:\  \     /:/\:\  \     /:/\:\  \     /:/  /    /:/\:\  \     /:/\:\  \     /:/__/     
  /:/ /:/ _/_   /:/  \:\  \   /::\~\:\  \   /:/  \:\__\   /:/  \:\  \   /:/  /    /:/  \:\  \   /:/  \:\  \   /::\__\____ 
 /:/_/:/ /\__\ /:/__/ \:\__\ /:/\:\ \:\__\ /:/__/ \:|__| /:/__/ \:\__\ /:/__/    /:/__/ \:\__\ /:/__/ \:\__\ /:/\:::::\__\
 \:\/:/ /:/  / \:\  \ /:/  / \/_|::\/:/  / \:\  \ /:/  / \:\  \  \/__/ \:\  \    \:\  \ /:/  / \:\  \  \/__/ \/_|:|~~|~   
  \::/_/:/  /   \:\  /:/  /     |:|::/  /   \:\  /:/  /   \:\  \        \:\  \    \:\  /:/  /   \:\  \          |:|  |    
   \:\/:/  /     \:\/:/  /      |:|\/__/     \:\/:/  /     \:\  \        \:\  \    \:\/:/  /     \:\  \         |:|  |    
    \::/  /       \::/  /       |:|  |        \::/__/       \:\__\        \:\__\    \::/  /       \:\__\        |:|  |    
     \/__/         \/__/         \|__|         ~~            \/__/         \/__/     \/__/         \/__/         \|__|    
*/


// ###########################################################################################################################################
// # Includes:
// #
// # You will need to add the following libraries to your Arduino IDE to use the project:
// # - Adafruit NeoPixel  // by Adafruit:                     https://github.com/adafruit/Adafruit_NeoPixel
// # - WiFiManager        // by tablatronix / tzapu:          https://github.com/tzapu/WiFiManager
// # - AsyncTCP           // by me-no-dev:                    https://github.com/me-no-dev/AsyncTCP
// # - ESPAsyncWebServer  // by me-no-dev:                    https://github.com/me-no-dev/ESPAsyncWebServer
// # - ESPUI              // by s00500:                       https://github.com/s00500/ESPUI
// # - ArduinoJson        // by bblanchon:                    https://github.com/bblanchon/ArduinoJson
// # - LITTLEFS           // by lorol:                        https://github.com/lorol/LITTLEFS        
// #
// ###########################################################################################################################################
#include <WiFi.h>               // Used to connect the ESP32 to your WiFi
#include <WiFiManager.h>        // Used for the WiFi Manager option to be able to connect the WordClock to your WiFi without code changes
#include <Adafruit_NeoPixel.h>  // Used to drive the NeoPixel LEDs
#include "time.h"               // Used for NTP time requests
#include <AsyncTCP.h>           // Used for the internal web server
#include <ESPAsyncWebServer.h>  // Used for the internal web server
#include <DNSServer.h>          // Used for the internal web server
#include <ESPUI.h>              // Used for the internal web server
#include <Preferences.h>        // Used to save the configuration to the ESP32 flash
//#include <WiFiClient.h>         // Used for update function
//#include <WebServer.h>          // Used for update function
//#include <Update.h>             // Used for update function
#include "settings.h"  // Settings are stored in a seperate file to make to code better readable and to be able to switch to other settings faster
// #include "languages.h"       // Translation for texts for the HTML page


// ###########################################################################################################################################
// # Version number of the code:
// ###########################################################################################################################################
const char* WORD_CLOCK_VERSION = "V1.0.3.1";


// ###########################################################################################################################################
// # Internal web server settings:
// ###########################################################################################################################################
AsyncWebServer server(80);  // Web server for config
// WebServer updserver(8080);  // Web server for OTA updates
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 44, 1);
DNSServer dnsServer;


// ###########################################################################################################################################
// # Declartions and variables used in the functions:
// ###########################################################################################################################################
Preferences preferences;
int iHour = 0;
int iMinute = 0;
int iSecond = 0;
bool updatedevice = true;
bool updatemode = false;
int WiFiManFix = 0;
String iStartTime = "-";


// ###########################################################################################################################################
// # Setup function that runs once at startup of the ESP:
// ###########################################################################################################################################
void setup() {
  Serial.begin(115200);
  delay(500);
  preferences.begin("wordclock", false);  // Init ESP32 flash
  Serial.println("######################################################################");
  Serial.print("# WordClock startup of version: ");
  Serial.println(WORD_CLOCK_VERSION);
  Serial.println("######################################################################");
  getFlashValues();                 // Read settings from flash
  strip.begin();                    // Init the LEDs
  strip.setBrightness(intensity);   // Set LED brightness
  DisplayTest();                    // Perform the LED test
  startup();                        // Run startup actions
  SetWLAN(strip.Color(0, 0, 255));  // Show WLAN text
  WIFI_login();                     // WiFiManager
  WiFiManager1stBootFix();          // WiFi Manager 1st connect fix
  ShowIPaddress();                  // Display the current IP-address
  configNTPTime();                  // NTP time setup
  setupWebInterface();              // Generate the configuration page
  // handleOTAupdate();                // Start the ESP32 OTA update server
  Serial.println("######################################################################");
  Serial.println("# WordClock startup finished...");
  Serial.println("######################################################################");
}


// ###########################################################################################################################################
// # Loop function which runs all the time after the startup was done:
// ###########################################################################################################################################
void loop() {
  if (updatedevice == true) {
    update_display();  // Update the display (time and events)
  }
  dnsServer.processNextRequest();  // Update web server
  // if (updatemode == true) updserver.handleClient();  // ESP32 OTA updates
}


// ###########################################################################################################################################
// # Read settings from flash:
// ###########################################################################################################################################
void getFlashValues() {
  redVal_time = preferences.getUInt("redVal_time", redVal_time);
  greenVal_time = preferences.getUInt("greenVal_time", greenVal_time);
  blueVal_time = preferences.getUInt("blueVal_time", blueVal_time);
  redVal_back = preferences.getUInt("redVal_back", redVal_back);
  greenVal_back = preferences.getUInt("greenVal_back", greenVal_back);
  blueVal_back = preferences.getUInt("blueVal_back", blueVal_back);
  intensity = preferences.getUInt("intensity", intensity);
}


// ###########################################################################################################################################
// # Show the IP-address on the display:
// ###########################################################################################################################################
void ShowIPaddress() {
  if (useshowip == 1) {
    // TODO
    // IpAddress2String(WiFi.localIP());
    // (ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3]);
  }
}


// ###########################################################################################################################################
// # Setup the internal web server configuration page:
// ###########################################################################################################################################
int statusLabelId;
void setupWebInterface() {
  dnsServer.start(DNS_PORT, "*", apIP);

  // Section General:
  // ################
  ESPUI.separator("General:");

  // Status label:
  statusLabelId = ESPUI.label("Status:", ControlColor::Dark, "Operational");

  // WordClock version:
  ESPUI.label("Version", ControlColor::None, WORD_CLOCK_VERSION);



  // Section LED settings:
  // #####################
  ESPUI.separator("LED settings:");

  // Time color selector:
  redVal_time = preferences.getUInt("redVal_time");
  greenVal_time = preferences.getUInt("greenVal_time");
  blueVal_time = preferences.getUInt("blueVal_time");
  char hex_time[7] = { 0 };
  sprintf(hex_time, "#%02X%02X%02X", redVal_time, greenVal_time, blueVal_time);
  uint16_t text_colour_time;
  text_colour_time = ESPUI.text("Time", colCallTIME, ControlColor::Dark, hex_time);
  ESPUI.setInputType(text_colour_time, "color");

  // Background color selector:
  redVal_back = preferences.getUInt("redVal_back");
  greenVal_back = preferences.getUInt("greenVal_back");
  blueVal_back = preferences.getUInt("blueVal_back");
  char hex_back[7] = { 0 };
  sprintf(hex_back, "#%02X%02X%02X", redVal_back, greenVal_back, blueVal_back);
  uint16_t text_colour_background;
  text_colour_background = ESPUI.text("Background", colCallBACK, ControlColor::Dark, hex_back);
  ESPUI.setInputType(text_colour_background, "color");

  // Intensity slider selector: !!! DEFAULT LIMITED TO 128 of 255 !!!
  intensity = preferences.getUInt("intensity");
  ESPUI.slider("Intensity", &sliderBrightness, ControlColor::Dark, intensity, 0, LEDintensityLIMIT);



  // Section Startup:
  // ################
  ESPUI.separator("Startup:");

  // Startup LED test function:
  useledtest = preferences.getUInt("useledtest", 1);
  ESPUI.switcher("Show LED test on startup", &switchLEDTest, ControlColor::Dark, useledtest);

  // Show IP-address on startup:
  // useshowip = preferences.getUInt("useshowip", 1);
  // ESPUI.switcher("Show IP-address on startup", &switchShowIP, ControlColor::Dark, useshowip);



  // Section WiFi:
  // #############
  ESPUI.separator("WiFi:");

  // WiFi SSID:
  ESPUI.label("SSID", ControlColor::Dark, WiFi.SSID());

  // WiFi signal strength:
  ESPUI.label("Signal", ControlColor::Dark, String(WiFi.RSSI()) + "dBm");

  // Hostname:
  ESPUI.label("Hostname", ControlColor::Dark, hostname);

  // WiFi ip-address:
  ESPUI.label("IP-address", ControlColor::Dark, IpAddress2String(WiFi.localIP()));

  // WiFi MAC-address:
  ESPUI.label("MAC address", ControlColor::Dark, WiFi.macAddress());



  // Section Time settings:
  // ######################
  ESPUI.separator("Time settings:");

  // NTP server:
  ESPUI.label("NTP server", ControlColor::Dark, NTPserver);

  // Time zone:
  ESPUI.label("Time zone", ControlColor::Dark, Timezone);

  // Time:
  ESPUI.label("Startup time", ControlColor::Dark, iStartTime);



  // Section Maintenance:
  // ####################
  ESPUI.separator("Maintenance:");

  // Restart WordClock:
  ESPUI.button("Restart", &buttonRestart, ControlColor::Dark, "Restart", (void*)1);

  // Reset WiFi settings:
  ESPUI.button("Reset WiFi settings", &buttonWiFiReset, ControlColor::Dark, "Reset WiFi settings", (void*)2);

  // Reset WordClock settings:
  // ESPUI.button("Reset WordClock settings", &buttonWordClockReset, ControlColor::Dark, "Reset WordClock settings", (void*)2);


  /*
  // Section Update:
  // ####################
  ESPUI.separator("Update:");

  // Update WordClock:
  ESPUI.button("Activate update mode", &buttonUpdate, ControlColor::Dark, "Activate update mode", (void*)1);

  // Update URL
  ESPUI.label("Update URL", ControlColor::Dark, "http://" + IpAddress2String(WiFi.localIP()) + ":8080");

  // Update User account
  ESPUI.label("Update account", ControlColor::Dark, "Username: WordClock   /   Password: 16x16");
*/


  // Deploy the page:
  ESPUI.begin("WordClock");
}


// ###########################################################################################################################################
// # GUI: LED test switch:
// ###########################################################################################################################################
void switchLEDTest(Control* sender, int value) {
  switch (value) {
    case S_ACTIVE:
      Serial.print("Active:");
      preferences.putUInt("useledtest", 1);
      break;
    case S_INACTIVE:
      Serial.print("Inactive");
      preferences.putUInt("useledtest", 0);
      break;
  }
  Serial.print(" ");
  Serial.println(sender->id);
}


// ###########################################################################################################################################
// # GUI: Show IP-ADdress switch:
// ###########################################################################################################################################
void switchShowIP(Control* sender, int value) {
  switch (value) {
    case S_ACTIVE:
      Serial.print("Active:");
      preferences.putUInt("useshowip", 1);
      break;
    case S_INACTIVE:
      Serial.print("Inactive");
      preferences.putUInt("useshowip", 0);
      break;
  }
  Serial.print(" ");
  Serial.println(sender->id);
}


// ###########################################################################################################################################
// # GUI: Restart the WordClock:
// ###########################################################################################################################################
int ResetCounter = 0;
void buttonRestart(Control* sender, int type, void* param) {
  preferences.end();
  delay(1000);
  updatedevice = false;
  Serial.println(String("param: ") + String(int(param)));
  switch (type) {
    case B_DOWN:
      ESPUI.print(statusLabelId, "Restart requested");
      back_color();
      redVal_time = 255;
      greenVal_time = 0;
      blueVal_time = 0;
      setLED(165, 172, 1);
      strip.show();
      break;
    case B_UP:
      if (ResetCounter == 1) {
        Serial.println("Status: Restart executed");
        ESP.restart();
      } else {
        Serial.println("Status: Restart request");
        ESPUI.updateButton(sender->id, "! Press button once more to apply restart !");
        ResetCounter = ResetCounter + 1;
      }
      break;
  }
}


// ###########################################################################################################################################
// # GUI: Reset the WiFi settings of the WordClock:
// ###########################################################################################################################################
int WIFIResetCounter = 0;
void buttonWiFiReset(Control* sender, int type, void* param) {
  preferences.putUInt("WiFiManFix", 0);  // WiFi Manager Fix Reset
  delay(1000);
  preferences.end();
  updatedevice = false;
  Serial.println(String("param: ") + String(int(param)));
  switch (type) {
    case B_DOWN:
      ESPUI.print(statusLabelId, "WIFI SETTINGS RESET REQUESTED");
      back_color();
      SetWLAN(strip.Color(255, 0, 0));
      redVal_time = 255;
      greenVal_time = 0;
      blueVal_time = 0;
      setLED(165, 172, 1);
      strip.show();
      delay(1000);
      break;
    case B_UP:
      if (WIFIResetCounter == 1) {
        Serial.println("Status: WIFI SETTINGS RESET REQUEST EXECUTED");
        delay(1000);
        WiFi.disconnect();
        delay(1000);
        WiFiManager manager;
        delay(1000);
        manager.resetSettings();
        delay(1000);
        Serial.println("####################################################################################################");
        Serial.println("# WIFI SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE CONFIGURE WIFI AGAIN... #");
        Serial.println("####################################################################################################");
        delay(1000);
        ESP.restart();
      } else {
        Serial.println("Status: WIFI SETTINGS RESET REQUEST");
        ESPUI.updateButton(sender->id, "! Press button once more to apply WiFi reset !");
        WIFIResetCounter = WIFIResetCounter + 1;
      }
      break;
  }
}


// ###########################################################################################################################################
// # GUI: Reset the WordClock settings:
// ###########################################################################################################################################
/* int WordClockResetCounter = 0;
void buttonWordClockReset(Control* sender, int type, void* param) {
  updatedevice = false;
  Serial.println(String("param: ") + String(int(param)));
  switch (type) {
    case B_DOWN:
      ESPUI.print(statusLabelId, "WORDCLOCK SETTINGS RESET REQUESTED");
      back_color();
      redVal_time = 255;
      greenVal_time = 0;
      blueVal_time = 0;
      setLED(165, 172, 1);
      strip.show();
      delay(1000);
      break;
    case B_UP:
      if (WordClockResetCounter == 1) {
        Serial.println("Status: WORDCLOCK SETTINGS RESET REQUEST EXECUTED");
        delay(1000);
        // preferences.clear(); // TODO
        delay(1000);
        preferences.end();
        Serial.println("####################################################################################################");
        Serial.println("# WORDCLOCK SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE CONFIGURE AGAIN... #");
        Serial.println("####################################################################################################");
        delay(1000);
        ESP.restart();
      } else {
        Serial.println("Status: WORDCLOCK SETTINGS RESET REQUEST");
        ESPUI.updateButton(sender->id, "! Press button once more to apply settings reset !");
        WordClockResetCounter = WordClockResetCounter + 1;
      }
      break;
  }
}*/


// ###########################################################################################################################################
// # GUI: Update the WordClock:
// ###########################################################################################################################################
void buttonUpdate(Control* sender, int type, void* param) {
  preferences.end();
  updatedevice = false;
  updatemode = true;
  delay(1000);
  Serial.println(String("param: ") + String(int(param)));
  switch (type) {
    case B_DOWN:
      Serial.println("Status: Update request");
      ESPUI.print(statusLabelId, "Update requested");
      back_color();
      redVal_time = 0;
      greenVal_time = 0;
      blueVal_time = 255;
      setLED(52, 57, 1);
      strip.show();
      break;
    case B_UP:
      Serial.println("Status: Update executed");
      ESPUI.updateButton(sender->id, "Update mode active now - Use the update url: >>>");
      break;
  }
}


// ###########################################################################################################################################
// # Update the display / time on it:
// ###########################################################################################################################################
void update_display() {
  if (testTime == 0) {  // Show the current time:
    printLocalTime();
    show_time(iHour, iMinute);
    delay(1000);
  } else {  // TEST THE DISPLAY TIME OUTPUT:
    for (int i = 0; i < 25; i++) {
      for (int y = 0; y < 60; y++) {
        show_time(i, y);
        delay(1000);
      }
    }
  }
}


// ###########################################################################################################################################
// # Display hours and minutes text function:
// ###########################################################################################################################################
void show_time(int hours, int minutes) {
  // Set background color:
  back_color();

  // Display time:
  iHour = hours;
  iMinute = minutes;
  if (testTime == 1) {
    Serial.print(iHour);
    Serial.print(":");
    Serial.println(iMinute);
  }

  // divide minute by 5 to get value for display control
  int minDiv = iMinute / 5;
  showMinutes(iMinute);


  // FÜNF: (Minuten)
  if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) setLED(140, 143, 1);
  // VIERTEL:
  if ((minDiv == 3) || (minDiv == 9)) setLED(153, 159, 1);
  // ZEHN: (Minuten)
  if ((minDiv == 2) || (minDiv == 10)) setLED(112, 115, 1);
  // ZWANZIG:
  if ((minDiv == 4) || (minDiv == 8)) setLED(88, 94, 1);
  // NACH:
  if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 7)) setLED(176, 179, 1);
  // VOR:
  if ((minDiv == 5) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) setLED(173, 175, 1);
  // HALB:
  if ((minDiv == 5) || (minDiv == 6) || (minDiv == 7)) setLED(181, 184, 1);


  //set hour from 1 to 12 (at noon, or midnight)
  int xHour = (iHour % 12);
  if (xHour == 0)
    xHour = 12;
  // at minute 25 hour needs to be counted up:
  // fuenf vor halb 2 = 13:25
  if (iMinute >= 25) {
    if (xHour == 12)
      xHour = 1;
    else
      xHour++;
  }


  // ES IST:
  setLED(62, 63, 1);
  setLED(70, 72, 1);


  switch (xHour) {
    case 1:
      {
        if (xHour == 1) setLED(208, 210, 1);                     // EIN
        if ((xHour == 1) && (iMinute > 4)) setLED(208, 211, 1);  // EINS (S in EINS) (just used if not point 1 o'clock)
        break;
      }
    case 2:
      {
        setLED(220, 223, 1);  // ZWEI
        break;
      }
    case 3:
      {
        setLED(212, 215, 1);  // DREI
        break;
      }
    case 4:
      {
        setLED(216, 219, 1);  // VIER
        break;
      }
    case 5:
      {
        setLED(228, 231, 1);  // FUENF
        break;
      }
    case 6:
      {
        setLED(203, 207, 1);  // SECHS
        break;
      }
    case 7:
      {
        setLED(186, 191, 1);  // SIEBEN
        break;
      }
    case 8:
      {
        setLED(236, 239, 1);  // ACHT
        break;
      }
    case 9:
      {
        setLED(199, 202, 1);  // NEUN
        break;
      }
    case 10:
      {
        setLED(198, 195, 1);  // ZEHN (Stunden)
        break;
      }
    case 11:
      {
        setLED(192, 194, 1);  // ELF
        break;
      }
    case 12:
      {
        setLED(231, 235, 1);  // ZWÖLF
        break;
      }
  }

  if (iMinute < 5) setLED(224, 226, 1);  // UHR
  strip.show();
}


// ###########################################################################################################################################
// # Display extra minutes function:
// ###########################################################################################################################################
void showMinutes(int minutes) {
  int minMod = (minutes % 5);
  // Serial.println(minMod);
  switch (minMod) {
    case 1:
      {
        setLED(241, 241, 1);  // +
        setLED(242, 242, 1);  // 1
        setLED(248, 253, 1);  // MINUTE
        break;
      }
    case 2:
      {
        setLED(241, 241, 1);  // +
        setLED(243, 243, 1);  // 2
        setLED(248, 254, 1);  // MINUTEN
        break;
      }
    case 3:
      {
        setLED(241, 241, 1);  // +
        setLED(244, 244, 1);  // 3
        setLED(248, 254, 1);  // MINUTEN
        break;
      }
    case 4:
      {
        setLED(241, 241, 1);  // +
        setLED(245, 245, 1);  // 3
        setLED(248, 254, 1);  // MINUTEN
        break;
      }
  }
}


// ###########################################################################################################################################
// # Startup function:
// ###########################################################################################################################################
void startup() {
  back_color();  // Set background color
  // Display AWSW WORDCLOCK text:
  uint32_t c1 = strip.Color(redVal_time, greenVal_time, blueVal_time);
  strip.setPixelColor(0, c1);
  strip.setPixelColor(15, c1);
  strip.setPixelColor(240, c1);
  strip.setPixelColor(255, c1);
  for (uint16_t i = 144; i < 153; i++) {
    strip.setPixelColor(i, c1);
  }
  strip.show();
  delay(500);
}


// ###########################################################################################################################################
// # Background color function:
// ###########################################################################################################################################
void back_color() {
  uint32_t c0 = strip.Color(redVal_back, greenVal_back, blueVal_back);
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, c0);
  }
}


// ###########################################################################################################################################
// # Startup LED test function:
// ###########################################################################################################################################
// LED test --> no blank display if WiFi was not set yet:
void DisplayTest() {
  useledtest = preferences.getUInt("useledtest", 1);
  if (useledtest) {
    Serial.println("Display test...");
    for (int i = 0; i < NUMPIXELS; i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
    }
    for (int i = 0; i < NUMPIXELS; i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
      strip.show();
      delay(10);
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.setPixelColor(NUMPIXELS, 0, 0, 0);
    strip.show();
  }
}


// ###########################################################################################################################################
// # Startup WiFi text function:
// ###########################################################################################################################################
void SetWLAN(uint32_t color) {
  Serial.println("Show text WLAN...");
  for (uint16_t i = 48; i < 52; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}


// ###########################################################################################################################################
// # Wifi Manager setup and reconnect function that runs once at startup and during the loop function of the ESP:
// ###########################################################################################################################################
void WIFI_login() {
  Serial.print("Try to connect to WiFi: ");
  Serial.println(WiFi.SSID());
  WiFi.setHostname(hostname);
  bool WiFires;
  bool DebugWifiLEDs = false;
  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(AP_TIMEOUT);
  WiFires = wifiManager.autoConnect(DEFAULT_AP_NAME);
  if (!WiFires) {
    Serial.print("Failed to connect to WiFi: ");
    Serial.println(WiFi.SSID());
    SetWLAN(strip.Color(255, 0, 0));
    for (uint16_t i = 246; i < 248; i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
    }
    strip.show();
    delay(1000);
  } else {
    Serial.print("Connected to WiFi: ");
    Serial.println(WiFi.SSID());
    SetWLAN(strip.Color(0, 255, 0));
    for (uint16_t i = 246; i < 248; i++) {
      strip.setPixelColor(i, strip.Color(0, 255, 0));
    }
    strip.show();
    delay(1000);
    back_color();
    strip.show();
  }
}


// ###########################################################################################################################################
// # WiFi Manager 1st connect fix: (Needed after the 1st login to your router - Restart the device once to be able to reach the web page...)
// ###########################################################################################################################################
void WiFiManager1stBootFix() {
  WiFiManFix = preferences.getUInt("WiFiManFix", 0);
  if (WiFiManFix == 0) {
    Serial.println("######################################################################");
    Serial.println("# ESP restart needed becaouse of WiFi Manager Fix");
    Serial.println("######################################################################");
    back_color();
    SetWLAN(strip.Color(0, 255, 0));
    redVal_time = 0;
    greenVal_time = 255;
    blueVal_time = 0;
    setLED(165, 172, 1);
    strip.show();
    delay(1000);
    preferences.putUInt("WiFiManFix", 1);
    delay(1000);
    preferences.end();
    delay(1000);
    ESP.restart();
  }
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED:
// ###########################################################################################################################################
void setLED(int ledNrFrom, int ledNrTo, int switchOn) {
  if (ledNrFrom > ledNrTo) {
    setLED(ledNrTo, ledNrFrom, switchOn);  //sets LED numbers in correct order (because of the date programming below)
  } else {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, strip.Color(redVal_time, greenVal_time, blueVal_time));
    }
  }
}


// ###########################################################################################################################################
// # NTP time functions:
// ###########################################################################################################################################
void configNTPTime() {
  initTime(Timezone);
  printLocalTime();
}
// ###########################################################################################################################################
void setTimezone(String timezone) {
  Serial.printf("Setting timezone to %s\n", timezone.c_str());
  setenv("TZ", timezone.c_str(), 1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}
// ###########################################################################################################################################
void initTime(String timezone) {
  struct tm timeinfo;
  Serial.println("Setting up time");
  configTime(0, 0, NTPserver);
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println("Got the time from NTP");
  setTimezone(timezone);
}
// ###########################################################################################################################################
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time 1");
    return;
  }
  //Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  char timeStringBuff[50];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  iStartTime = String(timeStringBuff);
  iHour = timeinfo.tm_hour;
  iMinute = timeinfo.tm_min;
  iSecond = timeinfo.tm_sec;
  /*Serial.print("Time: ");
    Serial.print(iHour);
    Serial.print(":");
    Serial.print(iMinute);
    Serial.print(":");
    Serial.println(iSecond);*/
}
// ###########################################################################################################################################
void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst) {
  struct tm tm;
  tm.tm_year = yr - 1900;  // Set date
  tm.tm_mon = month - 1;
  tm.tm_mday = mday;
  tm.tm_hour = hr;  // Set time
  tm.tm_min = minute;
  tm.tm_sec = sec;
  tm.tm_isdst = isDst;  // 1 or 0
  time_t t = mktime(&tm);
  Serial.printf("Setting time: %s", asctime(&tm));
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}
// ###########################################################################################################################################


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values - TIME:
// ###########################################################################################################################################
void getRGBTIME(String hexvalue) {
  hexvalue.toUpperCase();
  char c[7];
  hexvalue.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  Serial.print("REED: ");
  Serial.println(red);
  Serial.print("green: ");
  Serial.println(green);
  Serial.print("blue: ");
  Serial.println(blue);
  preferences.putUInt("redVal_time", red);
  preferences.putUInt("greenVal_time", green);
  preferences.putUInt("blueVal_time", blue);
  redVal_time = red;
  greenVal_time = green;
  blueVal_time = blue;
}


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values - BACKGROUND:
// ###########################################################################################################################################
void getRGBBACK(String hexvalue) {
  hexvalue.toUpperCase();
  char c[7];
  hexvalue.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  Serial.print("Read red: ");
  Serial.println(red);
  Serial.print("green: ");
  Serial.println(green);
  Serial.print("blue: ");
  Serial.println(blue);
  preferences.putUInt("redVal_back", red);
  preferences.putUInt("greenVal_back", green);
  preferences.putUInt("blueVal_back", blue);
  redVal_back = red;
  greenVal_back = green;
  blueVal_back = blue;
}


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values - helper function:
// ###########################################################################################################################################
int hexcolorToInt(char upper, char lower) {
  int uVal = (int)upper;
  int lVal = (int)lower;
  uVal = uVal > 64 ? uVal - 55 : uVal - 48;
  uVal = uVal << 4;
  lVal = lVal > 64 ? lVal - 55 : lVal - 48;
  // Serial.println(uVal+lVal);
  return uVal + lVal;
}


// ###########################################################################################################################################
// # GUI: Color change for time color:
// ###########################################################################################################################################
void colCallTIME(Control* sender, int type) {
  Serial.print("TIME Col: ID: ");
  Serial.print(sender->id);
  Serial.print(", Value: ");
  Serial.println(sender->value);
  getRGBTIME(sender->value);
}


// ###########################################################################################################################################
// # GUI: Color change for background color:
// ###########################################################################################################################################
void colCallBACK(Control* sender, int type) {
  Serial.print("BACK Col: ID: ");
  Serial.print(sender->id);
  Serial.print(", Value: ");
  Serial.println(sender->value);
  getRGBBACK(sender->value);
}


// ###########################################################################################################################################
// # GUI: Slider change for LED intensity:
// ###########################################################################################################################################
void sliderBrightness(Control* sender, int type) {
  Serial.print("Slider: ID: ");
  Serial.print(sender->id);
  Serial.print(", Value: ");
  Serial.println(sender->value);
  preferences.putUInt("intensity", (sender->value).toInt());
  strip.setBrightness((sender->value).toInt());
}


// ###########################################################################################################################################
// # GUI: Convert IP-address value to string:
// ###########################################################################################################################################
String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3]);
}


// ###########################################################################################################################################
// # ESP32 OTA update:
// ###########################################################################################################################################
const char* loginIndex =
  "<form name='loginForm'>"
  "<table width='20%' bgcolor='A09F9F' align='center'>"
  "<tr>"
  "<td colspan=2>"
  "<center><font size=4><b>WordClock Update Login Page</b></font></center>"
  "<br>"
  "</td>"
  "<br>"
  "<br>"
  "</tr>"
  "<tr>"
  "<td>Username:</td>"
  "<td><input type='text' size=25 name='userid'><br></td>"
  "</tr>"
  "<br>"
  "<br>"
  "<tr>"
  "<td>Password:</td>"
  "<td><input type='Password' size=25 name='pwd'><br></td>"
  "<br>"
  "<br>"
  "</tr>"
  "<tr>"
  "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
  "</tr>"
  "</table>"
  "</form>"
  "<script>"
  "function check(form)"
  "{"
  "if(form.userid.value=='WordClock' && form.pwd.value=='16x16')"
  "{"
  "window.open('/serverIndex')"
  "}"
  "else"
  "{"
  " alert('Error Password or Username')/*displays error message*/"
  "}"
  "}"
  "</script>";

const char* serverIndex =
  "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
  "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
  "<input type='file' name='update'>"
  "<input type='submit' value='Update'>"
  "</form>"
  "<div id='prg'>progress: 0%</div>"
  "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
  "},"
  "error: function (a, b, c) {"
  "}"
  "});"
  "});"
  "</script>";

/*void handleOTAupdate() {
  // OTA update server pages urls:
  updserver.on("/", HTTP_GET, []() {
    updserver.sendHeader("Connection", "close");
    updserver.send(200, "text/html", loginIndex);
  });
  updserver.on("/serverIndex", HTTP_GET, []() {
    updserver.sendHeader("Connection", "close");
    updserver.send(200, "text/html", serverIndex);
  });
  // handling uploading firmware file:
  updserver.on(
    "/update", HTTP_POST, []() {
      updserver.sendHeader("Connection", "close");
      updserver.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
      ESP.restart();
    },
    []() {
      HTTPUpload& upload = updserver.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin(UPDATE_SIZE_UNKNOWN)) {  //start with max available size
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        // flashing firmware to ESP
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {  //true to set the size to the current progress
          Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
          back_color();
          redVal_time = 0;
          greenVal_time = 255;
          blueVal_time = 0;
          setLED(165, 172, 1);
          setLED(52, 57, 1);
          strip.show();
          delay(3000);
        } else {
          Update.printError(Serial);
        }
      }
    });
  updserver.begin();
}*/


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################
