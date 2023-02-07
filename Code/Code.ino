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
// # - Adafruit NeoPixel      // by Adafruit:                     https://github.com/adafruit/Adafruit_NeoPixel
// # - WiFiManager            // by tablatronix / tzapu:          https://github.com/tzapu/WiFiManager
// # - AsyncTCP               // by me-no-dev:                    https://github.com/me-no-dev/AsyncTCP
// # - ESPAsyncWebServer      // by me-no-dev:                    https://github.com/me-no-dev/ESPAsyncWebServer
// # - ESPUI                  // by s00500:                       https://github.com/s00500/ESPUI
// # - ArduinoJson            // by bblanchon:                    https://github.com/bblanchon/ArduinoJson
// # - LITTLEFS               // by lorol:                        https://github.com/lorol/LITTLEFS
// # - UniversalTelegramBot   // by witnessmenow:                 https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
// #
// ###########################################################################################################################################
#include <WiFi.h>                  // Used to connect the ESP32 to your WiFi
#include <WiFiManager.h>           // Used for the WiFi Manager option to be able to connect the WordClock to your WiFi without code changes
#include <Adafruit_NeoPixel.h>     // Used to drive the NeoPixel LEDs
#include "time.h"                  // Used for NTP time requests
#include <AsyncTCP.h>              // Used for the internal web server
#include <ESPAsyncWebServer.h>     // Used for the internal web server
#include <DNSServer.h>             // Used for the internal web server
#include <ESPUI.h>                 // Used for the internal web server
#include <Preferences.h>           // Used to save the configuration to the ESP32 flash
#include <WiFiClient.h>            // Used for update function
#include <WebServer.h>             // Used for update function
#include <Update.h>                // Used for update function
#include <WiFiClientSecure.h>      // Telegram support
#include <UniversalTelegramBot.h>  // Telegram support
#include <ArduinoJson.h>           // Telegram support
#include "settings.h"              // Settings are stored in a seperate file to make to code better readable and to be able to switch to other settings faster


// ###########################################################################################################################################
// # Version number of the code:
// ###########################################################################################################################################
const char* WORD_CLOCK_VERSION = "V1.3.1";


// ###########################################################################################################################################
// # Internal web server settings:
// ###########################################################################################################################################
AsyncWebServer server(80);  // Web server for config
WebServer ewserver(8080);   // Web server for extra words
WebServer updserver(2022);  // Web server for OTA updates
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 44, 1);
DNSServer dnsServer;


// ###########################################################################################################################################
// # Declartions and variables used in the functions:
// ###########################################################################################################################################
Preferences preferences;
int langLEDlayout;
int iHour = 0;
int iMinute = 0;
int iSecond = 0;
bool updatedevice = true;
bool updatenow = false;
bool updatemode = false;
bool changedvalues = false;
int WiFiManFix = 0;
String iStartTime = "Failed to obtain time on startup... Please restart...";
int redVal_back, greenVal_back, blueVal_back;
int redVal_time, greenVal_time, blueVal_time;
int ew1, ew2, ew3, ew4, ew5, ew6, ew7, ew8, ew9, ew10, ew11, ew12;
int redVal_ew1, greenVal_ew1, blueVal_ew1;
int redVal_ew2, greenVal_ew2, blueVal_ew2;
int redVal_ew3, greenVal_ew3, blueVal_ew3;
int redVal_ew4, greenVal_ew4, blueVal_ew4;
int redVal_ew5, greenVal_ew5, blueVal_ew5;
int redVal_ew6, greenVal_ew6, blueVal_ew6;
int redVal_ew7, greenVal_ew7, blueVal_ew7;
int redVal_ew8, greenVal_ew8, blueVal_ew8;
int redVal_ew9, greenVal_ew9, blueVal_ew9;
int redVal_ew10, greenVal_ew10, blueVal_ew10;
int redVal_ew11, greenVal_ew11, blueVal_ew11;
int redVal_ew12, greenVal_ew12, blueVal_ew12;
int intensity, intensity_day, intensity_night;
int usenightmode, day_time_start, day_time_stop, statusNightMode;
int useshowip, usesinglemin;
int statusLabelID, statusNightModeID;
int useTelegram, useTelegramID;
uint16_t TelegramSwitcher, TelegramSwitcherID;
char* selectLang;
String chat_id = CHAT_ID;


// ###########################################################################################################################################
// # WiFi, Telegram and other settings:
// ###########################################################################################################################################
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOTtoken, secured_client);
unsigned long bot_lasttime;           // last time messages' scan has been done
const unsigned long BOT_MTBS = 1000;  // mean time between scan messages


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
  getFlashValues();                                     // Read settings from flash
  strip.begin();                                        // Init the LEDs
  intensity = intensity_day;                            // Set the intenity to day mode for startup
  strip.setBrightness(intensity);                       // Set LED brightness
  startup();                                            // Run startup actions
  WIFI_login();                                         // WiFiManager
  WiFiManager1stBootFix();                              // WiFi Manager 1st connect fix
  ShowIPaddress();                                      // Display the current IP-address
  configNTPTime();                                      // NTP time setup
  setupWebInterface();                                  // Generate the configuration page
  updatenow = true;                                     // Update the display 1x after startup
  update_display();                                     // Update LED display
  handleOTAupdate();                                    // Start the ESP32 OTA update server
  handleExtraWords();                                   // Start the extra words web access server
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
  if (useTelegram) {                                    // Telegram support
    Serial.println("Send initial Telegram message: WordClock " + String(WORD_CLOCK_VERSION) + " start finished");
    bot.sendMessage(CHAT_ID, "WordClock " + String(WORD_CLOCK_VERSION) + " start finished " + emoStartup, "");
  }
  Serial.println("######################################################################");
  Serial.println("# WordClock startup finished...");
  Serial.println("######################################################################");
}


// ###########################################################################################################################################
// # Loop function which runs all the time after the startup was done:
// ###########################################################################################################################################
void loop() {
  printLocalTime();
  if (updatedevice == true) {                       // Allow display updates (normal usage)
    if (millis() - bot_lasttime > BOT_MTBS) {       // Update only after timeout
      if (changedvalues == true) setFlashValues();  // Write settings to flash
      update_display();                             // Update display (1x per minute regulary)
      if (useTelegram) {                            // Telegram chat action command usage:
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        while (numNewMessages) {
          handleNewMessages(numNewMessages);
          numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }
      }
      bot_lasttime = millis();
    }
  }
  dnsServer.processNextRequest();                    // Update web server
  ewserver.handleClient();                           // Extra words web server access update
  if (updatemode == true) updserver.handleClient();  // ESP32 OTA updates
}


// ###########################################################################################################################################
// # Setup the internal web server configuration page:
// ###########################################################################################################################################
void setupWebInterface() {
  dnsServer.start(DNS_PORT, "*", apIP);

  // Section General:
  // ################
  ESPUI.separator("General:");

  // Status label:
  statusLabelID = ESPUI.label("Status:", ControlColor::Dark, "Operational");

  // WordClock version:
  ESPUI.label("Version", ControlColor::None, WORD_CLOCK_VERSION);

  // Set layout language:
  if (langLEDlayout == 0) selectLang = "Current layout language: German";
  if (langLEDlayout == 1) selectLang = "Current layout language: English";
  if (langLEDlayout == 2) selectLang = "Current layout language: Dutch";
  if (langLEDlayout == 3) selectLang = "Current layout language: French";
  ESPUI.button(selectLang, &buttonlangChange, ControlColor::Dark, "Change layout language", (void*)4);



  // Section LED settings:
  // #####################
  ESPUI.separator("LED settings:");

  // Time color selector:
  char hex_time[7] = { 0 };
  sprintf(hex_time, "#%02X%02X%02X", redVal_time, greenVal_time, blueVal_time);
  uint16_t text_colour_time;
  text_colour_time = ESPUI.text("Time", colCallTIME, ControlColor::Dark, hex_time);
  ESPUI.setInputType(text_colour_time, "color");

  // Background color selector:
  char hex_back[7] = { 0 };
  sprintf(hex_back, "#%02X%02X%02X", redVal_back, greenVal_back, blueVal_back);
  uint16_t text_colour_background;
  text_colour_background = ESPUI.text("Background", colCallBACK, ControlColor::Dark, hex_back);
  ESPUI.setInputType(text_colour_background, "color");

  // Intensity DAY slider selector: !!! DEFAULT LIMITED TO 128 of 255 !!!
  ESPUI.slider("Brightness during the day", &sliderBrightnessDay, ControlColor::Dark, intensity_day, 0, LEDintensityLIMIT);

  // Use night mode function:
  ESPUI.switcher("Use night mode to reduce brightness", &switchNightMode, ControlColor::Dark, usenightmode);

  // Night mode status:
  statusNightModeID = ESPUI.label("Night mode status", ControlColor::Dark, "Night mode not used");

  // Intensity NIGHT slider selector: !!! DEFAULT LIMITED TO 128 of 255 !!!
  ESPUI.slider("Brightness at night", &sliderBrightnessNight, ControlColor::Dark, intensity_night, 0, LEDintensityLIMIT);

  // Day mode start time:
  ESPUI.number("Day time starts at", call_day_time_start, ControlColor::Dark, day_time_start, 0, 11);

  // Day mode stop time:
  ESPUI.number("Day time ends after", call_day_time_stop, ControlColor::Dark, day_time_stop, 12, 23);

  // Use night mode function:
  ESPUI.switcher("Show single minutes to display the minute exact time", &switchSingleMinutes, ControlColor::Dark, usesinglemin);



  // Section Startup:
  // ################
  ESPUI.separator("Startup:");

  // Show IP-address on startup:
  ESPUI.switcher("Show IP-address on startup", &switchShowIP, ControlColor::Dark, useshowip);



  // Section Extra Words:
  // ####################
  ESPUI.separator("Extra Words:");

  // ################################################################## DE:
  if (langLEDlayout == 0) {  // DE:
    // Color Extra Word ew1:
    char hex_ew1[7] = { 0 };
    sprintf(hex_ew1, "#%02X%02X%02X", redVal_ew1, greenVal_ew1, blueVal_ew1);
    uint16_t text_colour_ew1;
    text_colour_ew1 = ESPUI.text("ALARM", colCallew1, ControlColor::Dark, hex_ew1);
    ESPUI.setInputType(text_colour_ew1, "color");

    // Color Extra Word ew2:
    char hex_ew2[7] = { 0 };
    sprintf(hex_ew2, "#%02X%02X%02X", redVal_ew2, greenVal_ew2, blueVal_ew2);
    uint16_t text_colour_ew2;
    text_colour_ew2 = ESPUI.text("GEBURTSTAG", colCallew2, ControlColor::Dark, hex_ew2);
    ESPUI.setInputType(text_colour_ew2, "color");

    // Color Extra Word ew3:
    char hex_ew3[7] = { 0 };
    sprintf(hex_ew3, "#%02X%02X%02X", redVal_ew3, greenVal_ew3, blueVal_ew3);
    uint16_t text_colour_ew3;
    text_colour_ew3 = ESPUI.text("MÜLL RAUS BRINGEN", colCallew3, ControlColor::Dark, hex_ew3);
    ESPUI.setInputType(text_colour_ew3, "color");

    // Color Extra Word ew4:
    char hex_ew4[7] = { 0 };
    sprintf(hex_ew4, "#%02X%02X%02X", redVal_ew4, greenVal_ew4, blueVal_ew4);
    uint16_t text_colour_ew4;
    text_colour_ew4 = ESPUI.text("AUTO", colCallew4, ControlColor::Dark, hex_ew4);
    ESPUI.setInputType(text_colour_ew4, "color");

    // Color Extra Word ew5:
    char hex_ew5[7] = { 0 };
    sprintf(hex_ew5, "#%02X%02X%02X", redVal_ew5, greenVal_ew5, blueVal_ew5);
    uint16_t text_colour_ew5;
    text_colour_ew5 = ESPUI.text("FEIERTAG", colCallew5, ControlColor::Dark, hex_ew5);
    ESPUI.setInputType(text_colour_ew5, "color");

    // Color Extra Word ew6:
    char hex_ew6[7] = { 0 };
    sprintf(hex_ew6, "#%02X%02X%02X", redVal_ew6, greenVal_ew6, blueVal_ew6);
    uint16_t text_colour_ew6;
    text_colour_ew6 = ESPUI.text("FORMEL1", colCallew6, ControlColor::Dark, hex_ew6);
    ESPUI.setInputType(text_colour_ew6, "color");

    // Color Extra Word ew7:
    char hex_ew7[7] = { 0 };
    sprintf(hex_ew7, "#%02X%02X%02X", redVal_ew7, greenVal_ew7, blueVal_ew7);
    uint16_t text_colour_ew7;
    text_colour_ew7 = ESPUI.text("GELBER SACK", colCallew7, ControlColor::Dark, hex_ew7);
    ESPUI.setInputType(text_colour_ew7, "color");

    // Color Extra Word ew8:
    char hex_ew8[7] = { 0 };
    sprintf(hex_ew8, "#%02X%02X%02X", redVal_ew8, greenVal_ew8, blueVal_ew8);
    uint16_t text_colour_ew8;
    text_colour_ew8 = ESPUI.text("URLAUB", colCallew8, ControlColor::Dark, hex_ew8);
    ESPUI.setInputType(text_colour_ew8, "color");

    // Color Extra Word ew9:
    char hex_ew9[7] = { 0 };
    sprintf(hex_ew9, "#%02X%02X%02X", redVal_ew9, greenVal_ew9, blueVal_ew9);
    uint16_t text_colour_ew9;
    text_colour_ew9 = ESPUI.text("WERKSTATT", colCallew9, ControlColor::Dark, hex_ew9);
    ESPUI.setInputType(text_colour_ew9, "color");

    // Color Extra Word ew10:
    char hex_ew10[7] = { 0 };
    sprintf(hex_ew10, "#%02X%02X%02X", redVal_ew10, greenVal_ew10, blueVal_ew10);
    uint16_t text_colour_ew10;
    text_colour_ew10 = ESPUI.text("ZEIT ZUM ZOCKEN", colCallew10, ControlColor::Dark, hex_ew10);
    ESPUI.setInputType(text_colour_ew10, "color");

    // Color Extra Word ew11:
    char hex_ew11[7] = { 0 };
    sprintf(hex_ew11, "#%02X%02X%02X", redVal_ew11, greenVal_ew11, blueVal_ew11);
    uint16_t text_colour_ew11;
    text_colour_ew11 = ESPUI.text("FRISEUR", colCallew11, ControlColor::Dark, hex_ew11);
    ESPUI.setInputType(text_colour_ew11, "color");

    // Color Extra Word ew12:
    char hex_ew12[7] = { 0 };
    sprintf(hex_ew12, "#%02X%02X%02X", redVal_ew12, greenVal_ew12, blueVal_ew12);
    uint16_t text_colour_ew12;
    text_colour_ew12 = ESPUI.text("TERMIN", colCallew12, ControlColor::Dark, hex_ew12);
    ESPUI.setInputType(text_colour_ew12, "color");
  }

  // ################################################################## EN:
  if (langLEDlayout == 1) {  // EN:

    // ew1 - COME HERE text
    // ew2 - LUNCH TIME text
    // ew3 - ALARM text
    // ew4 - GARBAGE text
    // ew5 - HOLIDAY text
    // ew6 - TEMPERATURE text
    // ew7 - DATE text
    // ew8 - BIRTHDAY text
    // ew9 - DOORBEL text

    // Color Extra Word ew1:
    char hex_ew1[7] = { 0 };
    sprintf(hex_ew1, "#%02X%02X%02X", redVal_ew1, greenVal_ew1, blueVal_ew1);
    uint16_t text_colour_ew1;
    text_colour_ew1 = ESPUI.text("COME HERE", colCallew1, ControlColor::Dark, hex_ew1);
    ESPUI.setInputType(text_colour_ew1, "color");

    // Color Extra Word ew2:
    char hex_ew2[7] = { 0 };
    sprintf(hex_ew2, "#%02X%02X%02X", redVal_ew2, greenVal_ew2, blueVal_ew2);
    uint16_t text_colour_ew2;
    text_colour_ew2 = ESPUI.text("LUNCH TIME", colCallew2, ControlColor::Dark, hex_ew2);
    ESPUI.setInputType(text_colour_ew2, "color");

    // Color Extra Word ew3:
    char hex_ew3[7] = { 0 };
    sprintf(hex_ew3, "#%02X%02X%02X", redVal_ew3, greenVal_ew3, blueVal_ew3);
    uint16_t text_colour_ew3;
    text_colour_ew3 = ESPUI.text("ALARM", colCallew3, ControlColor::Dark, hex_ew3);
    ESPUI.setInputType(text_colour_ew3, "color");

    // Color Extra Word ew4:
    char hex_ew4[7] = { 0 };
    sprintf(hex_ew4, "#%02X%02X%02X", redVal_ew4, greenVal_ew4, blueVal_ew4);
    uint16_t text_colour_ew4;
    text_colour_ew4 = ESPUI.text("GARBAGE", colCallew4, ControlColor::Dark, hex_ew4);
    ESPUI.setInputType(text_colour_ew4, "color");

    // Color Extra Word ew5:
    char hex_ew5[7] = { 0 };
    sprintf(hex_ew5, "#%02X%02X%02X", redVal_ew5, greenVal_ew5, blueVal_ew5);
    uint16_t text_colour_ew5;
    text_colour_ew5 = ESPUI.text("HOLIDAY", colCallew5, ControlColor::Dark, hex_ew5);
    ESPUI.setInputType(text_colour_ew5, "color");

    // Color Extra Word ew6:
    char hex_ew6[7] = { 0 };
    sprintf(hex_ew6, "#%02X%02X%02X", redVal_ew6, greenVal_ew6, blueVal_ew6);
    uint16_t text_colour_ew6;
    text_colour_ew6 = ESPUI.text("TEMPERATURE", colCallew6, ControlColor::Dark, hex_ew6);
    ESPUI.setInputType(text_colour_ew6, "color");

    // Color Extra Word ew7:
    char hex_ew7[7] = { 0 };
    sprintf(hex_ew7, "#%02X%02X%02X", redVal_ew7, greenVal_ew7, blueVal_ew7);
    uint16_t text_colour_ew7;
    text_colour_ew7 = ESPUI.text("DATE", colCallew7, ControlColor::Dark, hex_ew7);
    ESPUI.setInputType(text_colour_ew7, "color");

    // Color Extra Word ew8:
    char hex_ew8[7] = { 0 };
    sprintf(hex_ew8, "#%02X%02X%02X", redVal_ew8, greenVal_ew8, blueVal_ew8);
    uint16_t text_colour_ew8;
    text_colour_ew8 = ESPUI.text("BIRTHDAY", colCallew8, ControlColor::Dark, hex_ew8);
    ESPUI.setInputType(text_colour_ew8, "color");

    // Color Extra Word ew9:
    char hex_ew9[7] = { 0 };
    sprintf(hex_ew9, "#%02X%02X%02X", redVal_ew9, greenVal_ew9, blueVal_ew9);
    uint16_t text_colour_ew9;
    text_colour_ew9 = ESPUI.text("DOORBELL", colCallew9, ControlColor::Dark, hex_ew9);
    ESPUI.setInputType(text_colour_ew9, "color");
  }

  // ################################################################## NL:
  if (langLEDlayout == 2) {  // NL:
    // ew1 - KOM HIER text
    // ew2 - LUNCH TIJD text
    // ew3 - ALARM text
    // ew4 - AFVAL text
    // ew5 - VAKANTIE text
    // ew6 - TEMPERATUUR text
    // ew7 - DATUM text
    // ew8 - VERJAARDAG text
    // ew9 - DEURBEL text

    // Color Extra Word ew1:
    char hex_ew1[7] = { 0 };
    sprintf(hex_ew1, "#%02X%02X%02X", redVal_ew1, greenVal_ew1, blueVal_ew1);
    uint16_t text_colour_ew1;
    text_colour_ew1 = ESPUI.text("KOM HIER", colCallew1, ControlColor::Dark, hex_ew1);
    ESPUI.setInputType(text_colour_ew1, "color");

    // Color Extra Word ew2:
    char hex_ew2[7] = { 0 };
    sprintf(hex_ew2, "#%02X%02X%02X", redVal_ew2, greenVal_ew2, blueVal_ew2);
    uint16_t text_colour_ew2;
    text_colour_ew2 = ESPUI.text("LUNCH TIJD", colCallew2, ControlColor::Dark, hex_ew2);
    ESPUI.setInputType(text_colour_ew2, "color");

    // Color Extra Word ew3:
    char hex_ew3[7] = { 0 };
    sprintf(hex_ew3, "#%02X%02X%02X", redVal_ew3, greenVal_ew3, blueVal_ew3);
    uint16_t text_colour_ew3;
    text_colour_ew3 = ESPUI.text("ALARM", colCallew3, ControlColor::Dark, hex_ew3);
    ESPUI.setInputType(text_colour_ew3, "color");

    // Color Extra Word ew4:
    char hex_ew4[7] = { 0 };
    sprintf(hex_ew4, "#%02X%02X%02X", redVal_ew4, greenVal_ew4, blueVal_ew4);
    uint16_t text_colour_ew4;
    text_colour_ew4 = ESPUI.text("AFVAL", colCallew4, ControlColor::Dark, hex_ew4);
    ESPUI.setInputType(text_colour_ew4, "color");

    // Color Extra Word ew5:
    char hex_ew5[7] = { 0 };
    sprintf(hex_ew5, "#%02X%02X%02X", redVal_ew5, greenVal_ew5, blueVal_ew5);
    uint16_t text_colour_ew5;
    text_colour_ew5 = ESPUI.text("VAKANTIE", colCallew5, ControlColor::Dark, hex_ew5);
    ESPUI.setInputType(text_colour_ew5, "color");

    // Color Extra Word ew6:
    char hex_ew6[7] = { 0 };
    sprintf(hex_ew6, "#%02X%02X%02X", redVal_ew6, greenVal_ew6, blueVal_ew6);
    uint16_t text_colour_ew6;
    text_colour_ew6 = ESPUI.text("TEMPERATUUR", colCallew6, ControlColor::Dark, hex_ew6);
    ESPUI.setInputType(text_colour_ew6, "color");

    // Color Extra Word ew7:
    char hex_ew7[7] = { 0 };
    sprintf(hex_ew7, "#%02X%02X%02X", redVal_ew7, greenVal_ew7, blueVal_ew7);
    uint16_t text_colour_ew7;
    text_colour_ew7 = ESPUI.text("DATUM", colCallew7, ControlColor::Dark, hex_ew7);
    ESPUI.setInputType(text_colour_ew7, "color");

    // Color Extra Word ew8:
    char hex_ew8[7] = { 0 };
    sprintf(hex_ew8, "#%02X%02X%02X", redVal_ew8, greenVal_ew8, blueVal_ew8);
    uint16_t text_colour_ew8;
    text_colour_ew8 = ESPUI.text("VERJAARDAG", colCallew8, ControlColor::Dark, hex_ew8);
    ESPUI.setInputType(text_colour_ew8, "color");

    // Color Extra Word ew9:
    char hex_ew9[7] = { 0 };
    sprintf(hex_ew9, "#%02X%02X%02X", redVal_ew9, greenVal_ew9, blueVal_ew9);
    uint16_t text_colour_ew9;
    text_colour_ew9 = ESPUI.text("DEURBEL", colCallew9, ControlColor::Dark, hex_ew9);
    ESPUI.setInputType(text_colour_ew9, "color");
  }

  // ################################################################## FR:
  if (langLEDlayout == 3) {  // FR:

    // ew1 - ALARME text
    // ew2 - ANNIVERSAIRE text
    // ew3 - POUBELLE text
    // ew4 - A TABLE text
    // ew5 - VACANCES text
    // ew6 - VIENS ICI text
    // ew7 - SONNETTE text
    // ew8 - TEMPERATURE text
    // ew9 - DATE text

    // Color Extra Word ew1:
    char hex_ew1[7] = { 0 };
    sprintf(hex_ew1, "#%02X%02X%02X", redVal_ew1, greenVal_ew1, blueVal_ew1);
    uint16_t text_colour_ew1;
    text_colour_ew1 = ESPUI.text("ALARME", colCallew1, ControlColor::Dark, hex_ew1);
    ESPUI.setInputType(text_colour_ew1, "color");

    // Color Extra Word ew2:
    char hex_ew2[7] = { 0 };
    sprintf(hex_ew2, "#%02X%02X%02X", redVal_ew2, greenVal_ew2, blueVal_ew2);
    uint16_t text_colour_ew2;
    text_colour_ew2 = ESPUI.text("ANNIVERSAIRE", colCallew2, ControlColor::Dark, hex_ew2);
    ESPUI.setInputType(text_colour_ew2, "color");

    // Color Extra Word ew3:
    char hex_ew3[7] = { 0 };
    sprintf(hex_ew3, "#%02X%02X%02X", redVal_ew3, greenVal_ew3, blueVal_ew3);
    uint16_t text_colour_ew3;
    text_colour_ew3 = ESPUI.text("POUBELLE", colCallew3, ControlColor::Dark, hex_ew3);
    ESPUI.setInputType(text_colour_ew3, "color");

    // Color Extra Word ew4:
    char hex_ew4[7] = { 0 };
    sprintf(hex_ew4, "#%02X%02X%02X", redVal_ew4, greenVal_ew4, blueVal_ew4);
    uint16_t text_colour_ew4;
    text_colour_ew4 = ESPUI.text("A TABLE", colCallew4, ControlColor::Dark, hex_ew4);
    ESPUI.setInputType(text_colour_ew4, "color");

    // Color Extra Word ew5:
    char hex_ew5[7] = { 0 };
    sprintf(hex_ew5, "#%02X%02X%02X", redVal_ew5, greenVal_ew5, blueVal_ew5);
    uint16_t text_colour_ew5;
    text_colour_ew5 = ESPUI.text("VACANCES", colCallew5, ControlColor::Dark, hex_ew5);
    ESPUI.setInputType(text_colour_ew5, "color");

    // Color Extra Word ew6:
    char hex_ew6[7] = { 0 };
    sprintf(hex_ew6, "#%02X%02X%02X", redVal_ew6, greenVal_ew6, blueVal_ew6);
    uint16_t text_colour_ew6;
    text_colour_ew6 = ESPUI.text("VIENS ICI", colCallew6, ControlColor::Dark, hex_ew6);
    ESPUI.setInputType(text_colour_ew6, "color");

    // Color Extra Word ew7:
    char hex_ew7[7] = { 0 };
    sprintf(hex_ew7, "#%02X%02X%02X", redVal_ew7, greenVal_ew7, blueVal_ew7);
    uint16_t text_colour_ew7;
    text_colour_ew7 = ESPUI.text("SONNETTE", colCallew7, ControlColor::Dark, hex_ew7);
    ESPUI.setInputType(text_colour_ew7, "color");

    // Color Extra Word ew8:
    char hex_ew8[7] = { 0 };
    sprintf(hex_ew8, "#%02X%02X%02X", redVal_ew8, greenVal_ew8, blueVal_ew8);
    uint16_t text_colour_ew8;
    text_colour_ew8 = ESPUI.text("TEMPERATURE", colCallew8, ControlColor::Dark, hex_ew8);
    ESPUI.setInputType(text_colour_ew8, "color");

    // Color Extra Word ew9:
    char hex_ew9[7] = { 0 };
    sprintf(hex_ew9, "#%02X%02X%02X", redVal_ew9, greenVal_ew9, blueVal_ew9);
    uint16_t text_colour_ew9;
    text_colour_ew9 = ESPUI.text("DATE", colCallew9, ControlColor::Dark, hex_ew9);
    ESPUI.setInputType(text_colour_ew9, "color");
  }


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



  // Section Telegram:
  // #################
  ESPUI.separator("Telegram support:");

  if ((String(BOTtoken) == "XXXXXXXXXX:YYYYYYYYYYYYYYY-ZZZZZZZZZZZZZZZZZZZZ") || (String(CHAT_ID) == "1234512345")) {

    // Show note when Telegram was not configured yet:
    ESPUI.label("Status:", ControlColor::Dark, "Telegram not configured yet. See intructions in settings.h");

  } else {

    // Use Telegram support:
    TelegramSwitcher = ESPUI.switcher("Use Telegram support", &switchTelegram, ControlColor::Dark, useTelegram);

    // Reduce Telegram support to your own CHAT_ID:
    TelegramSwitcherID = ESPUI.switcher("React to your own Telegram ID only", &switchTelegramID, ControlColor::Dark, useTelegramID);
  }


  // Section Update:
  // ###############
  ESPUI.separator("Update:");

  // Update WordClock:
  ESPUI.button("Activate update mode", &buttonUpdate, ControlColor::Dark, "Activate update mode", (void*)1);

  // Update URL
  ESPUI.label("Update URL", ControlColor::Dark, "http://" + IpAddress2String(WiFi.localIP()) + ":2022/ota");

  // Update User account
  ESPUI.label("Update account", ControlColor::Dark, "Username: WordClock   /   Password: 16x16");



  // Section Maintenance:
  // ####################
  ESPUI.separator("Maintenance:");

  // Restart WordClock:
  ESPUI.button("Restart", &buttonRestart, ControlColor::Dark, "Restart", (void*)1);

  // Reset WiFi settings:
  ESPUI.button("Reset WiFi settings", &buttonWiFiReset, ControlColor::Dark, "Reset WiFi settings", (void*)2);

  // Reset WordClock settings:
  ESPUI.button("Reset WordClock settings", &buttonWordClockReset, ControlColor::Dark, "Reset WordClock settings", (void*)3);



  // Update night mode status text on startup:
  if (usenightmode == 1) {
    if ((iHour <= day_time_stop) && (iHour >= day_time_start)) {
      ESPUI.print(statusNightModeID, "Day time");
    } else {
      ESPUI.print(statusNightModeID, "Night time");
    }
  }


  // Deploy the page:
  ESPUI.begin("WordClock");
}


// ###########################################################################################################################################
// # Read settings from flash:
// ###########################################################################################################################################
void getFlashValues() {
  if (debugmode == 1) Serial.println("Read settings from flash: START");
  langLEDlayout = preferences.getUInt("langLEDlayout", langLEDlayout_default);
  redVal_time = preferences.getUInt("redVal_time", redVal_time_default);
  greenVal_time = preferences.getUInt("greenVal_time", greenVal_time_default);
  blueVal_time = preferences.getUInt("blueVal_time", blueVal_time_default);
  redVal_back = preferences.getUInt("redVal_back", redVal_back_default);
  greenVal_back = preferences.getUInt("greenVal_back", greenVal_back_default);
  blueVal_back = preferences.getUInt("blueVal_back", blueVal_back_default);
  intensity_day = preferences.getUInt("intensity_day", intensity_day_default);
  intensity_night = preferences.getUInt("intensity_night", intensity_night_default);
  usenightmode = preferences.getUInt("usenightmode", usenightmode_default);
  day_time_start = preferences.getUInt("day_time_start", day_time_start_default);
  day_time_stop = preferences.getUInt("day_time_stop", day_time_stop_default);
  useshowip = preferences.getUInt("useshowip", useshowip_default);
  usesinglemin = preferences.getUInt("usesinglemin", usesinglemin_default);
  useTelegram = preferences.getUInt("useTelegram", useTelegram_default);
  useTelegramID = preferences.getUInt("useTelegramID", useTelegramID_default);
  redVal_ew1 = preferences.getUInt("redVal_ew1", redVal_ew1_default);
  greenVal_ew1 = preferences.getUInt("greenVal_ew1", greenVal_ew1_default);
  blueVal_ew1 = preferences.getUInt("blueVal_ew1", blueVal_ew1_default);
  redVal_ew2 = preferences.getUInt("redVal_ew2", redVal_ew2_default);
  greenVal_ew2 = preferences.getUInt("greenVal_ew2", greenVal_ew2_default);
  blueVal_ew2 = preferences.getUInt("blueVal_ew2", blueVal_ew2_default);
  redVal_ew3 = preferences.getUInt("redVal_ew3", redVal_ew3_default);
  greenVal_ew3 = preferences.getUInt("greenVal_ew3", greenVal_ew3_default);
  blueVal_ew3 = preferences.getUInt("blueVal_ew3", blueVal_ew3_default);
  redVal_ew4 = preferences.getUInt("redVal_ew4", redVal_ew4_default);
  greenVal_ew4 = preferences.getUInt("greenVal_ew4", greenVal_ew4_default);
  blueVal_ew4 = preferences.getUInt("blueVal_ew4", blueVal_ew4_default);
  redVal_ew5 = preferences.getUInt("redVal_ew5", redVal_ew5_default);
  greenVal_ew5 = preferences.getUInt("greenVal_ew5", greenVal_ew5_default);
  blueVal_ew5 = preferences.getUInt("blueVal_ew5", blueVal_ew5_default);
  redVal_ew6 = preferences.getUInt("redVal_ew6", redVal_ew6_default);
  greenVal_ew6 = preferences.getUInt("greenVal_ew6", greenVal_ew6_default);
  blueVal_ew6 = preferences.getUInt("blueVal_ew6", blueVal_ew6_default);
  redVal_ew7 = preferences.getUInt("redVal_ew7", redVal_ew7_default);
  greenVal_ew7 = preferences.getUInt("greenVal_ew7", greenVal_ew7_default);
  blueVal_ew7 = preferences.getUInt("blueVal_ew7", blueVal_ew7_default);
  redVal_ew8 = preferences.getUInt("redVal_ew8", redVal_ew8_default);
  greenVal_ew8 = preferences.getUInt("greenVal_ew8", greenVal_ew8_default);
  blueVal_ew8 = preferences.getUInt("blueVal_ew8", blueVal_ew8_default);
  redVal_ew9 = preferences.getUInt("redVal_ew9", redVal_ew9_default);
  greenVal_ew9 = preferences.getUInt("greenVal_ew9", greenVal_ew9_default);
  blueVal_ew9 = preferences.getUInt("blueVal_ew9", blueVal_ew9_default);
  redVal_ew10 = preferences.getUInt("redVal_ew10", redVal_ew10_default);
  greenVal_ew10 = preferences.getUInt("greenVal_ew10", greenVal_ew10_default);
  blueVal_ew10 = preferences.getUInt("blueVal_ew10", blueVal_ew10_default);
  redVal_ew11 = preferences.getUInt("redVal_ew11", redVal_ew11_default);
  greenVal_ew11 = preferences.getUInt("greenVal_ew11", greenVal_ew11_default);
  blueVal_ew11 = preferences.getUInt("blueVal_ew11", blueVal_ew11_default);
  redVal_ew12 = preferences.getUInt("redVal_ew12", redVal_ew12_default);
  greenVal_ew12 = preferences.getUInt("greenVal_ew12", greenVal_ew12_default);
  blueVal_ew12 = preferences.getUInt("blueVal_ew12", blueVal_ew12_default);
  ew1 = preferences.getUInt("ew1", ew1_default);
  ew2 = preferences.getUInt("ew2", ew2_default);
  ew3 = preferences.getUInt("ew3", ew3_default);
  ew4 = preferences.getUInt("ew4", ew4_default);
  ew5 = preferences.getUInt("ew5", ew5_default);
  ew6 = preferences.getUInt("ew6", ew6_default);
  ew7 = preferences.getUInt("ew7", ew7_default);
  ew8 = preferences.getUInt("ew8", ew8_default);
  ew9 = preferences.getUInt("ew9", ew9_default);
  ew10 = preferences.getUInt("ew10", ew10_default);
  ew11 = preferences.getUInt("ew11", ew11_default);
  ew12 = preferences.getUInt("ew12", ew12_default);
  if (debugmode == 1) Serial.println("Read settings from flash: END");
}


// ###########################################################################################################################################
// # Write settings to flash:
// ###########################################################################################################################################
void setFlashValues() {
  if (debugmode == 1) Serial.println("Write settings to flash: START");
  changedvalues = false;
  preferences.putUInt("langLEDlayout", langLEDlayout);
  preferences.putUInt("redVal_time", redVal_time);
  preferences.putUInt("greenVal_time", greenVal_time);
  preferences.putUInt("blueVal_time", blueVal_time);
  preferences.putUInt("redVal_back", redVal_back);
  preferences.putUInt("greenVal_back", greenVal_back);
  preferences.putUInt("blueVal_back", blueVal_back);
  preferences.putUInt("intensity_day", intensity_day);
  preferences.putUInt("intensity_night", intensity_night);
  preferences.putUInt("usenightmode", usenightmode);
  preferences.putUInt("day_time_start", day_time_start);
  preferences.putUInt("day_time_stop", day_time_stop);
  preferences.putUInt("useshowip", useshowip);
  preferences.putUInt("usesinglemin", usesinglemin);
  preferences.putUInt("useTelegram", useTelegram);
  preferences.putUInt("useTelegramID", useTelegramID);
  preferences.putUInt("redVal_ew1", redVal_ew1);
  preferences.putUInt("greenVal_ew1", greenVal_ew1);
  preferences.putUInt("blueVal_ew1", blueVal_ew1);
  preferences.putUInt("redVal_ew2", redVal_ew2);
  preferences.putUInt("greenVal_ew2", greenVal_ew2);
  preferences.putUInt("blueVal_ew2", blueVal_ew2);
  preferences.putUInt("redVal_ew3", redVal_ew3);
  preferences.putUInt("greenVal_ew3", greenVal_ew3);
  preferences.putUInt("blueVal_ew3", blueVal_ew3);
  preferences.putUInt("redVal_ew4", redVal_ew4);
  preferences.putUInt("greenVal_ew4", greenVal_ew4);
  preferences.putUInt("blueVal_ew4", blueVal_ew4);
  preferences.putUInt("redVal_ew5", redVal_ew5);
  preferences.putUInt("greenVal_ew5", greenVal_ew5);
  preferences.putUInt("blueVal_ew5", blueVal_ew5);
  preferences.putUInt("redVal_ew6", redVal_ew6);
  preferences.putUInt("greenVal_ew6", greenVal_ew6);
  preferences.putUInt("blueVal_ew6", blueVal_ew6);
  preferences.putUInt("redVal_ew7", redVal_ew7);
  preferences.putUInt("greenVal_ew7", greenVal_ew7);
  preferences.putUInt("blueVal_ew7", blueVal_ew7);
  preferences.putUInt("redVal_ew8", redVal_ew8);
  preferences.putUInt("greenVal_ew8", greenVal_ew8);
  preferences.putUInt("blueVal_ew8", blueVal_ew8);
  preferences.putUInt("redVal_ew9", redVal_ew9);
  preferences.putUInt("greenVal_ew9", greenVal_ew9);
  preferences.putUInt("blueVal_ew9", blueVal_ew9);
  preferences.putUInt("redVal_ew10", redVal_ew10);
  preferences.putUInt("greenVal_ew10", greenVal_ew10);
  preferences.putUInt("blueVal_ew10", blueVal_ew10);
  preferences.putUInt("redVal_ew11", redVal_ew11);
  preferences.putUInt("greenVal_ew11", greenVal_ew11);
  preferences.putUInt("blueVal_ew11", blueVal_ew11);
  preferences.putUInt("redVal_ew12", redVal_ew12);
  preferences.putUInt("greenVal_ew12", greenVal_ew12);
  preferences.putUInt("blueVal_ew12", blueVal_ew12);
  preferences.putUInt("ew1", ew1);
  preferences.putUInt("ew2", ew2);
  preferences.putUInt("ew3", ew3);
  preferences.putUInt("ew4", ew4);
  preferences.putUInt("ew5", ew5);
  preferences.putUInt("ew6", ew6);
  preferences.putUInt("ew7", ew7);
  preferences.putUInt("ew8", ew8);
  preferences.putUInt("ew9", ew9);
  preferences.putUInt("ew10", ew10);
  preferences.putUInt("ew11", ew11);
  preferences.putUInt("ew12", ew12);
  if (debugmode == 1) Serial.println("Write settings to flash: END");
  if (usenightmode == 1) {
    if ((iHour <= day_time_stop) && (iHour >= day_time_start)) {
      ESPUI.print(statusNightModeID, "Day time");
    } else {
      ESPUI.print(statusNightModeID, "Night time");
    }
  } else {
    ESPUI.print(statusNightModeID, "Night mode not used");
  }
  updatenow = true;  // Update display now...
}


// ###########################################################################################################################################
// # GUI: Reset the WordClock settings:
// ###########################################################################################################################################
int WordClockResetCounter = 0;
void buttonWordClockReset(Control* sender, int type, void* param) {
  updatedevice = false;
  delay(1000);
  switch (type) {
    case B_DOWN:
      break;
    case B_UP:
      if (WordClockResetCounter == 1) {
        Serial.println("Status: WORDCLOCK SETTINGS RESET REQUEST EXECUTED");
        preferences.clear();
        delay(1000);
        preferences.putUInt("langLEDlayout", langLEDlayout_default);
        preferences.putUInt("redVal_time", redVal_time_default);
        preferences.putUInt("greenVal_time", greenVal_time_default);
        preferences.putUInt("blueVal_time", blueVal_time_default);
        preferences.putUInt("redVal_back", redVal_back_default);
        preferences.putUInt("greenVal_back", greenVal_back_default);
        preferences.putUInt("blueVal_back", blueVal_back_default);
        preferences.putUInt("intensity_day", intensity_day_default);
        preferences.putUInt("intensity_night", intensity_night_default);
        preferences.putUInt("useshowip", useshowip_default);
        preferences.putUInt("usesinglemin", usesinglemin_default);
        preferences.putUInt("usenightmode", usenightmode_default);
        preferences.putUInt("day_time_stop", day_time_stop_default);
        preferences.putUInt("day_time_stop", day_time_stop_default);
        preferences.putUInt("useTelegram", useTelegram_default);
        preferences.putUInt("useTelegramID", useTelegramID_default);
        preferences.putUInt("redVal_ew1", redVal_ew1_default);
        preferences.putUInt("greenVal_ew1", greenVal_ew1_default);
        preferences.putUInt("blueVal_ew1", blueVal_ew1_default);
        preferences.putUInt("redVal_ew2", redVal_ew2_default);
        preferences.putUInt("greenVal_ew2", greenVal_ew2_default);
        preferences.putUInt("blueVal_ew2", blueVal_ew2_default);
        preferences.putUInt("redVal_ew3", redVal_ew3_default);
        preferences.putUInt("greenVal_ew3", greenVal_ew3_default);
        preferences.putUInt("blueVal_ew3", blueVal_ew3_default);
        preferences.putUInt("redVal_ew4", redVal_ew4_default);
        preferences.putUInt("greenVal_ew4", greenVal_ew4_default);
        preferences.putUInt("blueVal_ew4", blueVal_ew4_default);
        preferences.putUInt("redVal_ew5", redVal_ew5_default);
        preferences.putUInt("greenVal_ew5", greenVal_ew5_default);
        preferences.putUInt("blueVal_ew5", blueVal_ew5_default);
        preferences.putUInt("redVal_ew6", redVal_ew6_default);
        preferences.putUInt("greenVal_ew6", greenVal_ew6_default);
        preferences.putUInt("blueVal_ew6", blueVal_ew6_default);
        preferences.putUInt("redVal_ew7", redVal_ew7_default);
        preferences.putUInt("greenVal_ew7", greenVal_ew7_default);
        preferences.putUInt("blueVal_ew7", blueVal_ew7_default);
        preferences.putUInt("redVal_ew8", redVal_ew8_default);
        preferences.putUInt("greenVal_ew8", greenVal_ew8_default);
        preferences.putUInt("blueVal_ew8", blueVal_ew8_default);
        preferences.putUInt("redVal_ew9", redVal_ew9_default);
        preferences.putUInt("greenVal_ew9", greenVal_ew9_default);
        preferences.putUInt("blueVal_ew9", blueVal_ew9_default);
        preferences.putUInt("redVal_ew10", redVal_ew10_default);
        preferences.putUInt("greenVal_ew10", greenVal_ew10_default);
        preferences.putUInt("blueVal_ew10", blueVal_ew10_default);
        preferences.putUInt("redVal_ew11", redVal_ew11_default);
        preferences.putUInt("greenVal_ew11", greenVal_ew11_default);
        preferences.putUInt("blueVal_ew11", blueVal_ew11_default);
        preferences.putUInt("redVal_ew12", redVal_ew12_default);
        preferences.putUInt("greenVal_ew12", greenVal_ew12_default);
        preferences.putUInt("blueVal_ew12", blueVal_ew12_default);
        preferences.putUInt("ew1", ew1_default);
        preferences.putUInt("ew2", ew2_default);
        preferences.putUInt("ew3", ew3_default);
        preferences.putUInt("ew4", ew4_default);
        preferences.putUInt("ew5", ew5_default);
        preferences.putUInt("ew6", ew6_default);
        preferences.putUInt("ew7", ew7_default);
        preferences.putUInt("ew8", ew8_default);
        preferences.putUInt("ew9", ew9_default);
        preferences.putUInt("ew10", ew10_default);
        preferences.putUInt("ew11", ew11_default);
        preferences.putUInt("ew12", ew12_default);
        delay(1000);
        preferences.end();
        back_color();
        redVal_time = 0;
        greenVal_time = 255;
        blueVal_time = 0;
        if (langLEDlayout == 0) {  // DE:
          setLED(165, 172, 1);
          setLED(144, 152, 1);
        }
        if (langLEDlayout == 1) {  // EN:
          setLED(131, 139, 1);
          setLED(24, 28, 1);
        }
        if (langLEDlayout == 2) {  // NL:
          setLED(117, 125, 1);
          setLED(27, 31, 1);
        }
        if (langLEDlayout == 3) {  // FR:
          setLED(198, 206, 1);
          setLED(101, 111, 1);
        }
        strip.show();
        Serial.println("####################################################################################################");
        Serial.println("# WORDCLOCK SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE CONFIGURE AGAIN... #");
        Serial.println("####################################################################################################");
        delay(1000);
        ESP.restart();
      } else {
        Serial.println("Status: WORDCLOCK SETTINGS RESET REQUEST");
        ESPUI.updateButton(sender->id, "! Press button once more to apply settings reset !");
        ESPUI.print(statusLabelID, "WORDCLOCK SETTINGS RESET REQUESTED");
        back_color();
        redVal_time = 255;
        greenVal_time = 0;
        blueVal_time = 0;
        if (langLEDlayout == 0) {  // DE:
          setLED(165, 172, 1);
          setLED(144, 152, 1);
        }
        if (langLEDlayout == 1) {  // EN:
          setLED(24, 28, 1);
          setLED(131, 139, 1);
        }
        if (langLEDlayout == 2) {  // NL:
          setLED(117, 125, 1);
          setLED(27, 31, 1);
        }
        if (langLEDlayout == 3) {  // FR:
          setLED(198, 206, 1);
          setLED(101, 111, 1);
        }
        strip.show();
        delay(1000);
        WordClockResetCounter = WordClockResetCounter + 1;
      }
      break;
  }
}


// ###########################################################################################################################################
// # GUI: Change LED layout language:
// ###########################################################################################################################################
int langChangeCounter = 0;
void buttonlangChange(Control* sender, int type, void* param) {
  updatedevice = false;
  delay(1000);
  switch (type) {
    case B_DOWN:
      break;
    case B_UP:
      if (langChangeCounter == 1) {
        Serial.println("WORDCLOCK LAYOUT LANGUAGE CHANGE EXECUTED");
        if (langLEDlayout == 0) preferences.putUInt("langLEDlayout", 1);  // DE to EN
        if (langLEDlayout == 1) preferences.putUInt("langLEDlayout", 2);  // EN to NL
        if (langLEDlayout == 2) preferences.putUInt("langLEDlayout", 3);  // NL to FR
        if (langLEDlayout == 3) preferences.putUInt("langLEDlayout", 0);  // FR to DE
        delay(1000);
        preferences.end();
        back_color();
        redVal_time = 0;
        greenVal_time = 255;
        blueVal_time = 0;
        if (langLEDlayout == 0) {  // DE: NEUSTART
          setLED(165, 172, 1);
        }
        if (langLEDlayout == 1) {  // EN: RESET
          setLED(24, 28, 1);
        }
        if (langLEDlayout == 2) {  // NL: HERSTART
          setLED(96, 103, 1);
        }
        if (langLEDlayout == 3) {  // FR: REDEMARRAGE
          setLED(101, 111, 1);
        }
        strip.show();
        Serial.println("##########################################################################");
        Serial.println("# WORDCLOCK LAYOUT LANGUAGE WAS CHANGED... WORDCLOCK WILL NOW RESTART... #");
        Serial.println("##########################################################################");
        delay(1000);
        ESP.restart();
      } else {
        Serial.println("WORDCLOCK LAYOUT LANGUAGE CHANGE REQUESTED");
        ESPUI.updateButton(sender->id, "! Press button once more to apply the language change and restart !");
        ESPUI.print(statusLabelID, "WORDCLOCK LAYOUT LANGUAGE CHANGE REQUESTED");
        back_color();
        redVal_time = 255;
        greenVal_time = 0;
        blueVal_time = 0;
        if (langLEDlayout == 0) {  // DE: NEUSTART
          setLED(165, 172, 1);
        }
        if (langLEDlayout == 1) {  // EN: RESET
          setLED(24, 28, 1);
        }
        if (langLEDlayout == 2) {  // NL: HERSTART
          setLED(96, 103, 1);
        }
        if (langLEDlayout == 3) {  // FR: REDEMARRAGE
          setLED(101, 111, 1);
        }
        strip.show();
        delay(1000);
        langChangeCounter = langChangeCounter + 1;
      }
      break;
  }
}


// ###########################################################################################################################################
// # Show the IP-address on the display:
// ###########################################################################################################################################
void ShowIPaddress() {
  if (useshowip == 1) {
    Serial.println("Show current IP-address on the display: " + IpAddress2String(WiFi.localIP()));
    int ipdelay = 2000;

    // Testing the digits:
    // for (int i = 0; i < 10; i++) {
    //   back_color();
    //   // numbers(i, 3);
    //   // numbers(i, 2);
    //   numbers(i, 1);
    //   strip.show();
    //   delay(ipdelay);
    // }

    // Octet 1:
    back_color();
    numbers(getDigit(int(WiFi.localIP()[0]), 2), 3);
    numbers(getDigit(int(WiFi.localIP()[0]), 1), 2);
    numbers(getDigit(int(WiFi.localIP()[0]), 0), 1);
    setLED(191, 191, 1);
    setLED(0, 3, 1);
    setLED(240, 243, 1);
    strip.show();
    delay(ipdelay);

    // // Octet 2:
    back_color();
    numbers(getDigit(int(WiFi.localIP()[1]), 2), 3);
    numbers(getDigit(int(WiFi.localIP()[1]), 1), 2);
    numbers(getDigit(int(WiFi.localIP()[1]), 0), 1);
    setLED(191, 191, 1);
    setLED(0, 7, 1);
    setLED(240, 247, 1);
    strip.show();
    delay(ipdelay);

    // // Octet 3:
    back_color();
    numbers(getDigit(int(WiFi.localIP()[2]), 2), 3);
    numbers(getDigit(int(WiFi.localIP()[2]), 1), 2);
    numbers(getDigit(int(WiFi.localIP()[2]), 0), 1);
    setLED(191, 191, 1);
    setLED(0, 11, 1);
    setLED(240, 251, 1);
    strip.show();
    delay(ipdelay);

    // // Octet 4:
    back_color();
    numbers(getDigit(int(WiFi.localIP()[3]), 2), 3);
    numbers(getDigit(int(WiFi.localIP()[3]), 1), 2);
    numbers(getDigit(int(WiFi.localIP()[3]), 0), 1);
    setLED(0, 15, 1);
    setLED(240, 255, 1);
    strip.show();
    delay(ipdelay);
  }
}


// ###########################################################################################################################################
// # Set the numbers on the display in each single row:
// ###########################################################################################################################################
void numbers(int wert, int segment) {

  // Serial.println(wert);

  switch (segment) {
    case 3:
      {
        switch (wert) {
          case 0:
            {
              setLED(76, 79, 1);
              setLED(80, 80, 1);
              setLED(83, 83, 1);
              setLED(108, 108, 1);
              setLED(111, 111, 1);
              setLED(112, 112, 1);
              setLED(115, 115, 1);
              setLED(140, 140, 1);
              setLED(143, 143, 1);
              setLED(144, 144, 1);
              setLED(147, 147, 1);
              setLED(172, 172, 1);
              setLED(175, 179, 1);
              break;
            }
          case 1:
            {
              setLED(76, 76, 1);
              setLED(83, 83, 1);
              setLED(108, 108, 1);
              setLED(115, 115, 1);
              setLED(140, 140, 1);
              setLED(147, 147, 1);
              setLED(172, 172, 1);
              setLED(179, 179, 1);
              break;
            }
          case 2:
            {
              setLED(76, 79, 1);
              setLED(83, 83, 1);
              setLED(108, 108, 1);
              setLED(112, 115, 1);
              setLED(143, 144, 1);
              setLED(175, 179, 1);
              break;
            }
          case 3:
            {
              setLED(76, 79, 1);
              setLED(83, 83, 1);
              setLED(108, 108, 1);
              setLED(112, 115, 1);
              setLED(140, 140, 1);
              setLED(147, 147, 1);
              setLED(172, 172, 1);
              setLED(176, 179, 1);
              break;
            }
          case 4:
            {
              setLED(76, 76, 1);
              setLED(79, 79, 1);
              setLED(80, 80, 1);
              setLED(83, 83, 1);
              setLED(108, 108, 1);
              setLED(111, 111, 1);
              setLED(112, 115, 1);
              setLED(140, 140, 1);
              setLED(147, 147, 1);
              setLED(172, 172, 1);
              setLED(179, 179, 1);
              break;
            }
          case 5:
            {
              setLED(76, 79, 1);
              setLED(80, 80, 1);
              setLED(111, 111, 1);
              setLED(112, 115, 1);
              setLED(140, 140, 1);
              setLED(147, 147, 1);
              setLED(172, 172, 1);
              setLED(176, 179, 1);
              break;
            }
          case 6:
            {
              setLED(76, 79, 1);
              setLED(80, 80, 1);
              setLED(111, 111, 1);
              setLED(112, 115, 1);
              setLED(140, 140, 1);
              setLED(143, 144, 1);
              setLED(147, 147, 1);
              setLED(172, 172, 1);
              setLED(175, 179, 1);
              break;
            }
          case 7:
            {
              setLED(76, 79, 1);
              setLED(83, 83, 1);
              setLED(108, 108, 1);
              setLED(115, 115, 1);
              setLED(140, 140, 1);
              setLED(147, 147, 1);
              setLED(172, 172, 1);
              setLED(179, 179, 1);
              break;
            }
          case 8:
            {
              setLED(76, 79, 1);
              setLED(80, 80, 1);
              setLED(83, 83, 1);
              setLED(108, 108, 1);
              setLED(111, 111, 1);
              setLED(112, 115, 1);
              setLED(140, 140, 1);
              setLED(143, 144, 1);
              setLED(147, 147, 1);
              setLED(172, 172, 1);
              setLED(175, 179, 1);
              break;
            }
          case 9:
            {
              setLED(76, 79, 1);
              setLED(80, 80, 1);
              setLED(83, 83, 1);
              setLED(108, 108, 1);
              setLED(111, 111, 1);
              setLED(112, 115, 1);
              setLED(140, 140, 1);
              setLED(147, 147, 1);
              setLED(172, 172, 1);
              setLED(176, 179, 1);
              break;
            }
        }
        break;
      }

    case 2:
      {
        switch (wert) {
          case 0:
            {
              setLED(71, 74, 1);
              setLED(85, 85, 1);
              setLED(88, 88, 1);
              setLED(103, 103, 1);
              setLED(106, 106, 1);
              setLED(117, 117, 1);
              setLED(120, 120, 1);
              setLED(135, 135, 1);
              setLED(138, 138, 1);
              setLED(149, 149, 1);
              setLED(152, 152, 1);
              setLED(167, 167, 1);
              setLED(170, 170, 1);
              setLED(181, 184, 1);
              break;
            }
          case 1:
            {
              setLED(71, 71, 1);
              setLED(88, 88, 1);
              setLED(103, 103, 1);
              setLED(120, 120, 1);
              setLED(135, 135, 1);
              setLED(152, 152, 1);
              setLED(167, 167, 1);
              setLED(184, 184, 1);
              break;
            }
          case 2:
            {
              setLED(71, 74, 1);
              setLED(88, 88, 1);
              setLED(103, 103, 1);
              setLED(117, 120, 1);
              setLED(138, 138, 1);
              setLED(149, 149, 1);
              setLED(170, 170, 1);
              setLED(181, 184, 1);
              break;
            }
          case 3:
            {
              setLED(71, 74, 1);
              setLED(88, 88, 1);
              setLED(103, 103, 1);
              setLED(117, 120, 1);
              setLED(135, 135, 1);
              setLED(152, 152, 1);
              setLED(167, 167, 1);
              setLED(181, 184, 1);
              break;
            }
          case 4:
            {
              setLED(71, 71, 1);
              setLED(74, 74, 1);
              setLED(85, 85, 1);
              setLED(88, 88, 1);
              setLED(103, 103, 1);
              setLED(106, 106, 1);
              setLED(117, 120, 1);
              setLED(135, 135, 1);
              setLED(152, 152, 1);
              setLED(167, 167, 1);
              setLED(184, 184, 1);
              break;
            }
          case 5:
            {
              setLED(71, 74, 1);
              setLED(85, 85, 1);
              setLED(106, 106, 1);
              setLED(117, 120, 1);
              setLED(135, 135, 1);
              setLED(152, 152, 1);
              setLED(167, 167, 1);
              setLED(181, 184, 1);
              break;
            }
          case 6:
            {
              setLED(71, 74, 1);
              setLED(85, 85, 1);
              setLED(106, 106, 1);
              setLED(117, 120, 1);
              setLED(135, 135, 1);
              setLED(138, 138, 1);
              setLED(149, 149, 1);
              setLED(152, 152, 1);
              setLED(167, 167, 1);
              setLED(170, 170, 1);
              setLED(181, 184, 1);
              break;
            }
          case 7:
            {
              setLED(71, 74, 1);
              setLED(88, 88, 1);
              setLED(103, 103, 1);
              setLED(120, 120, 1);
              setLED(135, 135, 1);
              setLED(152, 152, 1);
              setLED(167, 167, 1);
              setLED(184, 184, 1);
              break;
            }
          case 8:
            {
              setLED(71, 74, 1);
              setLED(85, 85, 1);
              setLED(88, 88, 1);
              setLED(103, 103, 1);
              setLED(106, 106, 1);
              setLED(117, 120, 1);
              setLED(135, 135, 1);
              setLED(138, 138, 1);
              setLED(149, 149, 1);
              setLED(152, 152, 1);
              setLED(167, 167, 1);
              setLED(170, 170, 1);
              setLED(181, 184, 1);
              break;
            }
          case 9:
            {
              setLED(71, 74, 1);
              setLED(85, 85, 1);
              setLED(88, 88, 1);
              setLED(103, 103, 1);
              setLED(106, 106, 1);
              setLED(117, 120, 1);
              setLED(135, 135, 1);
              setLED(152, 152, 1);
              setLED(167, 167, 1);
              setLED(181, 184, 1);
              break;
            }
        }
        break;
      }

    case 1:
      {
        switch (wert) {
          case 0:
            {
              setLED(66, 69, 1);
              setLED(90, 90, 1);
              setLED(93, 93, 1);
              setLED(98, 98, 1);
              setLED(101, 101, 1);
              setLED(122, 122, 1);
              setLED(125, 125, 1);
              setLED(130, 130, 1);
              setLED(133, 133, 1);
              setLED(154, 154, 1);
              setLED(157, 157, 1);
              setLED(162, 162, 1);
              setLED(165, 165, 1);
              setLED(186, 189, 1);
              break;
            }
          case 1:
            {
              setLED(66, 66, 1);
              setLED(93, 93, 1);
              setLED(98, 98, 1);
              setLED(125, 125, 1);
              setLED(130, 130, 1);
              setLED(157, 157, 1);
              setLED(162, 162, 1);
              setLED(189, 189, 1);
              break;
            }
          case 2:
            {
              setLED(66, 69, 1);
              setLED(93, 93, 1);
              setLED(98, 98, 1);
              setLED(122, 125, 1);
              setLED(133, 133, 1);
              setLED(154, 154, 1);
              setLED(165, 165, 1);
              setLED(186, 189, 1);
              break;
            }
          case 3:
            {
              setLED(66, 69, 1);
              setLED(93, 93, 1);
              setLED(98, 98, 1);
              setLED(122, 125, 1);
              setLED(130, 130, 1);
              setLED(157, 157, 1);
              setLED(162, 162, 1);
              setLED(186, 189, 1);
              break;
            }
          case 4:
            {
              setLED(66, 66, 1);
              setLED(69, 69, 1);
              setLED(90, 90, 1);
              setLED(93, 93, 1);
              setLED(98, 98, 1);
              setLED(101, 101, 1);
              setLED(122, 125, 1);
              setLED(130, 130, 1);
              setLED(157, 157, 1);
              setLED(162, 162, 1);
              setLED(189, 189, 1);
              break;
            }
          case 5:
            {
              setLED(66, 69, 1);
              setLED(90, 90, 1);
              setLED(101, 101, 1);
              setLED(122, 125, 1);
              setLED(130, 130, 1);
              setLED(157, 157, 1);
              setLED(162, 162, 1);
              setLED(186, 189, 1);
              break;
            }
          case 6:
            {
              setLED(66, 69, 1);
              setLED(90, 90, 1);
              setLED(101, 101, 1);
              setLED(122, 125, 1);
              setLED(130, 130, 1);
              setLED(133, 133, 1);
              setLED(154, 154, 1);
              setLED(157, 157, 1);
              setLED(162, 162, 1);
              setLED(165, 165, 1);
              setLED(186, 189, 1);
              break;
            }
          case 7:
            {
              setLED(66, 69, 1);
              setLED(93, 93, 1);
              setLED(98, 98, 1);
              setLED(125, 125, 1);
              setLED(130, 130, 1);
              setLED(157, 157, 1);
              setLED(162, 162, 1);
              setLED(189, 189, 1);
              break;
            }
          case 8:
            {
              setLED(66, 69, 1);
              setLED(90, 90, 1);
              setLED(93, 93, 1);
              setLED(98, 98, 1);
              setLED(101, 101, 1);
              setLED(122, 125, 1);
              setLED(130, 130, 1);
              setLED(133, 133, 1);
              setLED(154, 154, 1);
              setLED(157, 157, 1);
              setLED(162, 162, 1);
              setLED(165, 165, 1);
              setLED(186, 189, 1);
              break;
            }
          case 9:
            {
              setLED(66, 69, 1);
              setLED(90, 90, 1);
              setLED(93, 93, 1);
              setLED(98, 98, 1);
              setLED(101, 101, 1);
              setLED(122, 125, 1);
              setLED(130, 130, 1);
              setLED(157, 157, 1);
              setLED(162, 162, 1);
              setLED(186, 189, 1);
              break;
            }
        }
        break;
      }
  }
}


// ###########################################################################################################################################
// # Get a digit from a number at position pos: (Split IP-address octets in single digits)
// ###########################################################################################################################################
int getDigit(int number, int pos) {
  return (pos == 0) ? number % 10 : getDigit(number / 10, --pos);
}


// ###########################################################################################################################################
// # GUI: Restart the WordClock:
// ###########################################################################################################################################
int ResetCounter = 0;
void buttonRestart(Control* sender, int type, void* param) {
  updatedevice = false;
  delay(1000);
  if (changedvalues == true) {
    setFlashValues();  // Write settings to flash
    delay(1000);
  }
  preferences.end();
  if (ResetCounter == 0) ResetTextLEDs(strip.Color(255, 0, 0));
  if (ResetCounter == 1) ResetTextLEDs(strip.Color(0, 255, 0));
  switch (type) {
    case B_DOWN:
      break;
    case B_UP:
      if (ResetCounter == 1) {
        delay(1000);
        ESP.restart();
      } else {
        Serial.println("Status: Restart request");
        ESPUI.print(statusLabelID, "RESTART REQUESTED");
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
  updatedevice = false;
  if (WIFIResetCounter == 0) ResetTextLEDs(strip.Color(255, 0, 0));
  if (WIFIResetCounter == 1) ResetTextLEDs(strip.Color(0, 255, 0));
  if (WIFIResetCounter == 0) SetWLAN(strip.Color(255, 0, 0));
  if (WIFIResetCounter == 1) SetWLAN(strip.Color(0, 255, 0));
  switch (type) {
    case B_DOWN:
      break;
    case B_UP:
      if (WIFIResetCounter == 1) {
        Serial.println("Status: WIFI SETTINGS RESET REQUEST EXECUTED");
        WiFi.disconnect();
        delay(1000);
        WiFiManager manager;
        manager.resetSettings();
        delay(1000);
        Serial.println("####################################################################################################");
        Serial.println("# WIFI SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE CONFIGURE WIFI AGAIN... #");
        Serial.println("####################################################################################################");
        delay(1000);
        ESP.restart();
      } else {
        ESPUI.print(statusLabelID, "WIFI SETTINGS RESET REQUESTED");
        ESPUI.updateButton(sender->id, "! Press button once more to apply WiFi reset !");
        Serial.println("Status: WIFI SETTINGS RESET REQUEST");
        preferences.putUInt("WiFiManFix", 0);  // WiFi Manager Fix Reset
        preferences.putUInt("useshowip", 1);   // Show IP-address again
        delay(250);
        preferences.end();
        WIFIResetCounter = WIFIResetCounter + 1;
      }
      break;
  }
}


// ###########################################################################################################################################
// # Show a LED output for RESET in the different languages:
// ###########################################################################################################################################
void ResetTextLEDs(uint32_t color) {
  updatedevice = false;
  delay(1000);
  back_color();

  if (langLEDlayout == 0) {      // DE:
    setLEDcol(165, 172, color);  // NEUSTART
  }

  if (langLEDlayout == 1) {    // EN:
    setLEDcol(24, 28, color);  // RESET
  }

  if (langLEDlayout == 2) {     // NL:
    setLEDcol(96, 103, color);  // HERSTART
  }

  if (langLEDlayout == 3) {  // FR:
    setLED(101, 111, 1);     //  REDEMARRAGE
  }

  strip.show();
  delay(1000);
}


// ###########################################################################################################################################
// # Actual function, which controls 1/0 of the LED with color value:
// ###########################################################################################################################################
void setLEDcol(int ledNrFrom, int ledNrTo, uint32_t color) {
  if (ledNrFrom > ledNrTo) {
    setLED(ledNrTo, ledNrFrom, 1);  // Sets LED numbers in correct order
  } else {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, color);
    }
  }
}


// ###########################################################################################################################################
// # GUI: Update the WordClock:
// ###########################################################################################################################################
void buttonUpdate(Control* sender, int type, void* param) {
  Serial.println("Status: Update request");
  updatedevice = false;
  preferences.end();
  updatemode = true;
  back_color();
  delay(500);
  redVal_time = 0;
  greenVal_time = 0;
  blueVal_time = 255;
  if (langLEDlayout == 0) setLED(52, 57, 1);  // DE
  if (langLEDlayout == 1) setLED(48, 53, 1);  // EN
  if (langLEDlayout == 2) setLED(16, 21, 1);  // NL
  if (langLEDlayout == 3) setLED(0, 13, 1);   // FR
  strip.show();
  ESPUI.print(statusLabelID, "Update requested");
  ESPUI.updateButton(sender->id, "Update mode active now - Use the update url: >>>");
  Serial.println("Status: Update executed");
}


// ###########################################################################################################################################
// # GUI: Night mode switch:
// ###########################################################################################################################################
void switchNightMode(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      usenightmode = 1;
      if ((iHour <= day_time_stop) && (iHour >= day_time_start)) {
        intensity = intensity_day;
      } else {
        intensity = intensity_night;
      }
      break;
    case S_INACTIVE:
      intensity = intensity_day;
      usenightmode = 0;
      break;
  }
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Show IP-ADdress switch:
// ###########################################################################################################################################
void switchShowIP(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      useshowip = 1;
      break;
    case S_INACTIVE:
      useshowip = 0;
      break;
  }
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Use Telegram switch:
// ###########################################################################################################################################
void switchTelegram(Control* sender, int value) {
  updatedevice = false;
  Serial.println("Status: Restart request");
  ResetTextLEDs(strip.Color(255, 0, 0));
  switch (value) {
    case S_ACTIVE:
      useTelegram = 1;
      break;
    case S_INACTIVE:
      useTelegram = 0;
      break;
  }
  changedvalues = true;
  setFlashValues();  // Save values!
  ResetTextLEDs(strip.Color(0, 255, 0));
  Serial.println("Status: Perform restart now");
  delay(1000);
  ESP.restart();
}


// ###########################################################################################################################################
// # GUI: React to your own Telegram CHAT_ID only switch:
// ###########################################################################################################################################
void switchTelegramID(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      useTelegramID = 1;
      break;
    case S_INACTIVE:
      useTelegramID = 0;
      break;
  }
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Single minutes switch:
// ###########################################################################################################################################
void switchSingleMinutes(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      usesinglemin = 1;
      break;
    case S_INACTIVE:
      usesinglemin = 0;
      break;
  }
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # Update the display / time on it:
// ###########################################################################################################################################
void update_display() {
  if (usenightmode == 1) {
    if ((iHour <= day_time_stop) && (iHour >= day_time_start)) {
      intensity = intensity_day;
    } else {
      intensity = intensity_night;
    }
  } else {
    intensity = intensity_day;
  }
  strip.setBrightness(intensity);
  if (testTime == 0) {  // Show the current time:
    show_time(iHour, iMinute);
  } else {  // TEST THE DISPLAY TIME OUTPUT:
    strip.setBrightness(33);
    for (int i = 0; i <= 12; i++) {  // 12 hours only to show all hour texts
      show_time(i, 0);
      delay(500);
    }
    for (int i = 0; i <= 23; i++) {  // Hours 0 to 23 with all hour and minute texts:
      for (int y = 0; y < 60; y++) {
        show_time(i, y);
        delay(5);
      }
    }
  }
}


// ###########################################################################################################################################
// # Display hours and minutes text function:
// ###########################################################################################################################################
void show_time(int hours, int minutes) {
  static int lastHourSet = -1;
  static int lastMinutesSet = -1;
  if ((lastHourSet == hours && lastMinutesSet == minutes) && updatenow == false) {  // Reduce display updates to new minutes and new config updates
    return;
  }
  String currentTime = String(hours) + ":" + String(minutes) + ":" + String(iSecond);
  if (updatenow = true) Serial.println("Update LED display... " + currentTime);
  updatenow = false;
  lastHourSet = hours;
  lastMinutesSet = minutes;


  // Set background color:
  back_color();

  // Display time:
  iHour = hours;
  iMinute = minutes;

  // Test a special time:
  // iHour = 23;
  // iMinute = 35;


  // divide minute by 5 to get value for display control
  int minDiv = iMinute / 5;
  if (usesinglemin == 1) showMinutes(iMinute);


  // ########################################################### DE:
  if (langLEDlayout == 0) {  // DE:

    // ES IST:
    setLED(62, 63, 1);
    setLED(70, 72, 1);

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
  }


  // ########################################################### EN:
  if (langLEDlayout == 1) {  // EN:

    // IT IS:
    setLED(14, 15, 1);
    setLED(45, 46, 1);

    // FIVE: (Minutes)                         // x:05 + x:25 + x:35 + x:55
    if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) setLED(140, 143, 1);
    // QUARTER:                                // x:15 + X:45
    if ((minDiv == 3) || (minDiv == 9)) setLED(104, 110, 1);
    // A:
    if ((minDiv == 3) || (minDiv == 9)) setLED(51, 51, 1);
    // TEN: (Minutes)                          // x:10 + x:50
    if ((minDiv == 2) || (minDiv == 10)) setLED(101, 103, 1);
    // TWENTY:                                 // x:20 + x:25 + x:35 + x:40
    if ((minDiv == 4) || (minDiv == 5) || (minDiv == 7) || (minDiv == 8)) setLED(58, 63, 1);
    // PAST:                                   // x:05 + x:10 + x:15 + x:20 + x:25 + x:30
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 5) || (minDiv == 6)) setLED(144, 147, 1);
    // TO:                                     // x:35 + x:40 + x:45 + x:50 + x:55
    if ((minDiv == 7) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) setLED(129, 130, 1);
    // HALF:                                   // x:30
    if ((minDiv == 6)) setLED(54, 57, 1);


    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 35 hour needs to be counted up:
    // Twenty five to two = 13:35
    if (iMinute >= 35) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }


    switch (xHour) {
      case 1:
        {
          if (xHour == 1) setLED(208, 210, 1);  // ONE
          break;
        }
      case 2:
        {
          setLED(160, 162, 1);  // TWO
          break;
        }
      case 3:
        {
          setLED(196, 200, 1);  // THREE
          break;
        }
      case 4:
        {
          setLED(156, 159, 1);  // FOUR
          break;
        }
      case 5:
        {
          setLED(188, 191, 1);  // FIVE
          break;
        }
      case 6:
        {
          setLED(192, 194, 1);  // SIX
          break;
        }
      case 7:
        {
          setLED(202, 206, 1);  // SEVEN
          break;
        }
      case 8:
        {
          setLED(162, 166, 1);  // EIGHT
          break;
        }
      case 9:
        {
          setLED(184, 187, 1);  // NINE
          break;
        }
      case 10:
        {
          setLED(173, 175, 1);  // TEN
          break;
        }
      case 11:
        {
          setLED(167, 172, 1);  // ELEVEN
          break;
        }
      case 12:
        {
          setLED(177, 182, 1);  // TWELVE
          break;
        }
    }

    if (iMinute < 5) setLED(212, 218, 1);  // O'CLOCK
  }


  // ########################################################### NL:
  if (langLEDlayout == 2) {  // NL:

    // HET IS:
    setLED(13, 15, 1);
    setLED(41, 42, 1);

    // VIJF: (Minuten) x:05, x:25, x:35, x:55
    if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) {
      setLED(104, 107, 1);
    }
    // KWART: x:15, x:45
    if ((minDiv == 3) || (minDiv == 9)) {
      setLED(51, 55, 1);
    }
    // TIEN: (Minuten) x:10, x:50
    if ((minDiv == 2) || (minDiv == 10)) {
      setLED(108, 111, 1);
    }
    // TIEN: (TIEN VOOR HALF, TIEN OVER HALF) x:20, x:40 (on request not set to TWINTIG OVER)
    if ((minDiv == 4) || (minDiv == 8)) {
      setLED(108, 111, 1);
    }
    // OVER: x:05, x:10, x:15, x:35, x:40
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 7) || (minDiv == 8)) {
      setLED(140, 143, 1);
    }
    // VOOR: x:20, x:25, x:45, x:50, x:55
    if ((minDiv == 4) || (minDiv == 5) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLED(135, 138, 1);
    }
    // HALF:
    if ((minDiv == 4) || (minDiv == 5) || (minDiv == 6) || (minDiv == 7) || (minDiv == 8)) {
      setLED(130, 133, 1);
    }


    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 20 hour needs to be counted up:
    // tien voor half 2 = 13:20
    if (iMinute >= 20) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }


    switch (xHour) {
      case 1:
        {
          setLED(189, 191, 1);  // EEN
          break;
        }
      case 2:
        {
          setLED(182, 185, 1);  // TWEE
          break;
        }
      case 3:
        {
          setLED(186, 189, 1);  // DRIE
          break;
        }
      case 4:
        {
          setLED(156, 159, 1);  // VIER
          break;
        }
      case 5:
        {
          setLED(166, 169, 1);  // VIJF
          break;
        }
      case 6:
        {
          setLED(170, 172, 1);  // ZES
          break;
        }
      case 7:
        {
          setLED(161, 165, 1);  // ZEVEN
          break;
        }
      case 8:
        {
          setLED(145, 148, 1);  // ACHT
          break;
        }
      case 9:
        {
          setLED(151, 155, 1);  // NEGEN
          break;
        }
      case 10:
        {
          setLED(148, 151, 1);  // TIEN (Stunden)
          break;
        }
      case 11:
        {
          setLED(173, 175, 1);  // ELF
          break;
        }
      case 12:
        {
          setLED(176, 181, 1);  // TWAALF
          break;
        }
    }

    if (iMinute < 5) {
      setLED(192, 194, 1);  // UUR
    }
  }



  // ########################################################### FR:
  if (langLEDlayout == 3) {  // FR:

    // IL EST:
    setLED(14, 15, 1);  // IL
    setLED(21, 23, 1);  // EST

    // CINQ: (Minutes) x:05, x:25, x:35, x:55
    if ((minDiv == 1) || (minDiv == 5) || (minDiv == 7) || (minDiv == 11)) {
      setLED(214, 217, 1);
    }
    // ET QUART: x:15,
    if ((minDiv == 3)) {
      setLED(153, 154, 1);  // ET
      setLED(176, 180, 1);  // QUART
    }
    // LE QUART: x:45
    if ((minDiv == 9)) {
      setLED(160, 161, 1);  // LE
      setLED(176, 180, 1);  // QUART
    }
    // DIX: (Minutes) x:10, x:50
    if ((minDiv == 2) || (minDiv == 10)) {
      setLED(181, 183, 1);
    }
    // VINGT: x:20, x:40
    if ((minDiv == 4) || (minDiv == 8)) {
      setLED(208, 212, 1);
    }
    // VINGT-: x:25, x:35
    if ((minDiv == 5) || (minDiv == 7)) {
      setLED(208, 213, 1);
    }
    // MOINS: x:35, x:40 x:45, x:50, x:55
    if ((minDiv == 7) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLED(155, 159, 1);
    }
    // ET DEMIE: x:30
    if ((minDiv == 6)) {
      setLED(153, 154, 1);  // ET
      setLED(192, 196, 1);  // DEMIE
    }


    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 35 hour needs to be counted up:
    if (iMinute >= 35) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }


    switch (xHour) {
      case 1:
        {
          setLED(52, 54, 1);    // UNE
          setLED(129, 133, 1);  // HEURE
          break;
        }
      case 2:
        {
          setLED(117, 120, 1);  // DEUX
          setLED(128, 133, 1);  // HEURES
          break;
        }
      case 3:
        {
          setLED(65, 69, 1);    // TROIS
          setLED(128, 133, 1);  // HEURES
          break;
        }
      case 4:
        {
          setLED(55, 60, 1);    // QUATRE
          setLED(128, 133, 1);  // HEURES
          break;
        }
      case 5:
        {
          setLED(48, 51, 1);    // CINQ
          setLED(128, 133, 1);  // HEURES
          break;
        }
      case 6:
        {
          setLED(45, 47, 1);    // SIX
          setLED(128, 133, 1);  // HEURES
          break;
        }
      case 7:
        {
          setLED(88, 91, 1);    // SEPT
          setLED(128, 133, 1);  // HEURES
          break;
        }
      case 8:
        {
          setLED(139, 142, 1);  // HUIT
          setLED(128, 133, 1);  // HEURES
          break;
        }
      case 9:
        {
          setLED(70, 73, 1);    // NEUF
          setLED(128, 133, 1);  // HEURES
          break;
        }
      case 10:
        {
          setLED(61, 63, 1);    // DIX
          setLED(128, 133, 1);  // HEURES
          break;
        }
      case 11:
        {
          setLED(135, 138, 1);  // ONZE
          setLED(128, 133, 1);  // HEURES
          break;
        }
      case 12:
        {
          // MINUIT (0) or MIDI (12)
          if (iHour == 0 || (iHour == 23 && iMinute >= 35)) setLED(74, 79, 1);   // MINUIT (0)
          if (iHour == 12 || (iHour == 11 && iMinute >= 35)) setLED(80, 83, 1);  // MIDI (12)
          break;
        }
    }
  }


  // Handle extra words:
  set_extra_words();

  strip.show();
}


// ###########################################################################################################################################
// # Display extra minutes function:
// ###########################################################################################################################################
void showMinutes(int minutes) {
  int minMod = (minutes % 5);
  // Serial.println(minMod);

  // ##################################################### DE:
  if (langLEDlayout == 0) {  // DE:

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
          setLED(245, 245, 1);  // 4
          setLED(248, 254, 1);  // MINUTEN
          break;
        }
    }
  }

  // ##################################################### EN:
  if (langLEDlayout == 1) {  // EN:
    switch (minMod) {
      case 1:
        {
          setLED(241, 241, 1);  // +
          setLED(243, 243, 1);  // 1
          setLED(248, 253, 1);  // MINUTE
          break;
        }
      case 2:
        {
          setLED(241, 241, 1);  // +
          setLED(244, 244, 1);  // 2
          setLED(248, 254, 1);  // MINUTEN
          break;
        }
      case 3:
        {
          setLED(241, 241, 1);  // +
          setLED(245, 245, 1);  // 3
          setLED(248, 254, 1);  // MINUTEN
          break;
        }
      case 4:
        {
          setLED(241, 241, 1);  // +
          setLED(246, 246, 1);  // 4
          setLED(248, 254, 1);  // MINUTEN
          break;
        }
    }
  }

  // ##################################################### NL:
  if (langLEDlayout == 2) {  // NL:

    switch (minMod) {
      case 1:
        {
          setLED(241, 241, 1);  // +
          setLED(243, 243, 1);  // 1
          setLED(248, 254, 1);  // MINUTEN
          break;
        }
      case 2:
        {
          setLED(241, 241, 1);  // +
          setLED(244, 244, 1);  // 2
          setLED(248, 254, 1);  // MINUTEN
          break;
        }
      case 3:
        {
          setLED(241, 241, 1);  // +
          setLED(245, 245, 1);  // 3
          setLED(248, 254, 1);  // MINUTEN
          break;
        }
      case 4:
        {
          setLED(241, 241, 1);  // +
          setLED(246, 246, 1);  // 4
          setLED(248, 254, 1);  // MINUTEN
          break;
        }
    }
  }

  // ##################################################### FR:
  if (langLEDlayout == 3) {  // FR:
    switch (minMod) {
      case 1:
        {
          setLED(241, 241, 1);  // +
          setLED(243, 243, 1);  // 1
          setLED(248, 253, 1);  // MINUTE
          break;
        }
      case 2:
        {
          setLED(241, 241, 1);  // +
          setLED(244, 244, 1);  // 2
          setLED(248, 254, 1);  // MINUTES
          break;
        }
      case 3:
        {
          setLED(241, 241, 1);  // +
          setLED(245, 245, 1);  // 3
          setLED(248, 254, 1);  // MINUTES
          break;
        }
      case 4:
        {
          setLED(241, 241, 1);  // +
          setLED(246, 246, 1);  // 4
          setLED(248, 254, 1);  // MINUTES
          break;
        }
    }
  }
}

// ###########################################################################################################################################
// # Startup function:
// ###########################################################################################################################################
void startup() {
  uint32_t c0 = strip.Color(redVal_back, greenVal_back, blueVal_back);  // Background color
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, c0);
  }

  // Display AWSW WORDCLOCK text:
  uint32_t c1 = strip.Color(redVal_time, greenVal_time, blueVal_time);

  if (langLEDlayout == 0) {  // DE:
    strip.setPixelColor(0, c1);
    strip.setPixelColor(15, c1);
    strip.setPixelColor(240, c1);
    strip.setPixelColor(255, c1);
    for (uint16_t i = 144; i < 153; i++) {
      strip.setPixelColor(i, c1);
    }
  }

  if (langLEDlayout == 1) {  // EN:
    strip.setPixelColor(80, c1);
    strip.setPixelColor(81, c1);
    strip.setPixelColor(82, c1);
    strip.setPixelColor(83, c1);
    for (uint16_t i = 131; i < 140; i++) {
      strip.setPixelColor(i, c1);
    }
  }

  if (langLEDlayout == 2) {  // NL:
    strip.setPixelColor(72, c1);
    strip.setPixelColor(73, c1);
    strip.setPixelColor(74, c1);
    strip.setPixelColor(75, c1);
    for (uint16_t i = 117; i < 126; i++) {
      strip.setPixelColor(i, c1);
    }
  }


  if (langLEDlayout == 3) {  // FR:
    strip.setPixelColor(92, c1);
    strip.setPixelColor(93, c1);
    strip.setPixelColor(94, c1);
    strip.setPixelColor(95, c1);
    for (uint16_t i = 198; i < 207; i++) {
      strip.setPixelColor(i, c1);
    }
  }


  SetWLAN(strip.Color(0, 0, 255));  // Show WLAN text
}


// ###########################################################################################################################################
// # Background color function:
// ###########################################################################################################################################
void back_color() {
  uint32_t c0 = strip.Color(redVal_back, greenVal_back, blueVal_back);
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, c0);
  }
  delay(500);
}


// ###########################################################################################################################################
// # Startup WiFi text function:
// ###########################################################################################################################################
void SetWLAN(uint32_t color) {
  Serial.println("Show text WLAN...");

  if (langLEDlayout == 0) {  // DE:
    for (uint16_t i = 48; i < 52; i++) {
      strip.setPixelColor(i, color);
    }
  }

  if (langLEDlayout == 1) {  // EN:
    for (uint16_t i = 230; i < 234; i++) {
      strip.setPixelColor(i, color);
    }
  }

  if (langLEDlayout == 2) {  // NL:
    for (uint16_t i = 113; i < 117; i++) {
      strip.setPixelColor(i, color);
    }
  }

  if (langLEDlayout == 3) {  // FR:
    for (uint16_t i = 231; i < 235; i++) {
      strip.setPixelColor(i, color);
    }
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
  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(AP_TIMEOUT);
  WiFires = wifiManager.autoConnect(DEFAULT_AP_NAME);
  if (!WiFires) {
    Serial.print("Failed to connect to WiFi: ");
    Serial.println(WiFi.SSID());
  } else {
    Serial.print("Connected to WiFi: ");
    Serial.println(WiFi.SSID());
    if (langLEDlayout == 0) {  // DE:
      for (uint16_t i = 246; i < 248; i++) {
        strip.setPixelColor(i, strip.Color(0, 255, 0));
      }
    }
    if (langLEDlayout == 1) {  // EN:
      for (uint16_t i = 227; i < 229; i++) {
        strip.setPixelColor(i, strip.Color(0, 255, 0));
      }
    }
    if (langLEDlayout == 2) {  // NL:
      for (uint16_t i = 227; i < 229; i++) {
        strip.setPixelColor(i, strip.Color(0, 255, 0));
      }
    }
    if (langLEDlayout == 3) {  // FR:
      for (uint16_t i = 227; i < 229; i++) {
        strip.setPixelColor(i, strip.Color(0, 255, 0));
      }
    }

    SetWLAN(strip.Color(0, 255, 0));
    delay(1000);
  }
}


// ###########################################################################################################################################
// # WiFi Manager 1st connect fix: (Needed after the 1st login to your router - Restart the device once to be able to reach the web page...)
// ###########################################################################################################################################
void WiFiManager1stBootFix() {
  WiFiManFix = preferences.getUInt("WiFiManFix", 0);
  if (WiFiManFix == 0) {
    Serial.println("######################################################################");
    Serial.println("# ESP restart needed because of WiFi Manager Fix");
    Serial.println("######################################################################");
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
    setLED(ledNrTo, ledNrFrom, switchOn);  // Sets LED numbers in correct order
  } else {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, strip.Color(redVal_time, greenVal_time, blueVal_time));
    }
  }
  if (switchOn == 0) {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, strip.Color(redVal_back, greenVal_back, blueVal_back));
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
    back_color();
    Serial.println("Failed to obtain time");
    ESPUI.print(statusLabelID, "Failed to obtain time");
    if (langLEDlayout == 0) {  // DE:
      setLEDexCol(84, 87, 1, 255, 0, 0);
      setLEDexCol(165, 172, 1, 255, 0, 0);
    }
    if (langLEDlayout == 1) {  // EN:
      setLEDexCol(28, 31, 1, 255, 0, 0);
      setLEDexCol(120, 126, 1, 255, 0, 0);
    }
    if (langLEDlayout == 2) {  // NL:
      setLEDexCol(76, 79, 1, 255, 0, 0);
      setLEDexCol(96, 103, 1, 255, 0, 0);
    }
    if (langLEDlayout == 3) {  // FR:
      setLEDexCol(235, 239, 1, 255, 0, 0);
      setLEDexCol(101, 111, 1, 255, 0, 0);
    }
    strip.show();
    delay(3000);
    ESP.restart();
    return;
  } else {
    back_color();
    if (langLEDlayout == 0) {  // DE:
      setLEDexCol(84, 87, 1, 0, 255, 0);
    }
    if (langLEDlayout == 1) {  // EN:
      setLEDexCol(28, 31, 1, 0, 255, 0);
    }
    if (langLEDlayout == 2) {  // NL:
      setLEDexCol(76, 79, 1, 0, 255, 0);
    }
    if (langLEDlayout == 3) {  // FR:
      setLEDexCol(235, 239, 1, 0, 255, 0);
    }
    strip.show();
    delay(1000);
    Serial.println("Got the time from NTP");
    setTimezone(timezone);
  }
}
// ###########################################################################################################################################
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time ");
    return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  char timeStringBuff[50];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  iStartTime = String(timeStringBuff);
  // Serial.println(iStartTime);
  iHour = timeinfo.tm_hour;
  iMinute = timeinfo.tm_min;
  iSecond = timeinfo.tm_sec;
  // Serial.print("Time: ");
  // Serial.print(iHour);
  // Serial.print(":");
  // Serial.print(iMinute);
  // Serial.print(":");
  // Serial.println(iSecond);
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
  updatedevice = false;
  delay(1000);
  hexvalue.toUpperCase();
  char c[7];
  hexvalue.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  // Serial.print("red: ");
  // Serial.println(red);
  // Serial.print("green: ");
  // Serial.println(green);
  // Serial.print("blue: ");
  // Serial.println(blue);
  redVal_time = red;
  greenVal_time = green;
  blueVal_time = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Convert hex color value to RGB int values - BACKGROUND:
// ###########################################################################################################################################
void getRGBBACK(String hexvalue) {
  updatedevice = false;
  delay(1000);
  hexvalue.toUpperCase();
  char c[7];
  hexvalue.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  // Serial.print("red: ");
  // Serial.println(red);
  // Serial.print("green: ");
  // Serial.println(green);
  // Serial.print("blue: ");
  // Serial.println(blue);
  redVal_back = red;
  greenVal_back = green;
  blueVal_back = blue;
  changedvalues = true;
  updatedevice = true;
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
  // Serial.print("TIME Col: ID: ");
  // Serial.print(sender->id);
  // Serial.print(", Value: ");
  // Serial.println(sender->value);
  getRGBTIME(sender->value);
}


// ###########################################################################################################################################
// # GUI: Color change for background color:
// ###########################################################################################################################################
void colCallBACK(Control* sender, int type) {
  // Serial.print("BACK Col: ID: ");
  // Serial.print(sender->id);
  // Serial.print(", Value: ");
  // Serial.println(sender->value);
  getRGBBACK(sender->value);
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew1:
// ###########################################################################################################################################
void colCallew1(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew1 = red;
  greenVal_ew1 = green;
  blueVal_ew1 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew2:
// ###########################################################################################################################################
void colCallew2(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew2 = red;
  greenVal_ew2 = green;
  blueVal_ew2 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew3:
// ###########################################################################################################################################
void colCallew3(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew3 = red;
  greenVal_ew3 = green;
  blueVal_ew3 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew4:
// ###########################################################################################################################################
void colCallew4(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew4 = red;
  greenVal_ew4 = green;
  blueVal_ew4 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew5:
// ###########################################################################################################################################
void colCallew5(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew5 = red;
  greenVal_ew5 = green;
  blueVal_ew5 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew6:
// ###########################################################################################################################################
void colCallew6(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew6 = red;
  greenVal_ew6 = green;
  blueVal_ew6 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew7:
// ###########################################################################################################################################
void colCallew7(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew7 = red;
  greenVal_ew7 = green;
  blueVal_ew7 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew8:
// ###########################################################################################################################################
void colCallew8(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew8 = red;
  greenVal_ew8 = green;
  blueVal_ew8 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew9:
// ###########################################################################################################################################
void colCallew9(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew9 = red;
  greenVal_ew9 = green;
  blueVal_ew9 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew10:
// ###########################################################################################################################################
void colCallew10(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew10 = red;
  greenVal_ew10 = green;
  blueVal_ew10 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew11:
// ###########################################################################################################################################
void colCallew11(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew11 = red;
  greenVal_ew11 = green;
  blueVal_ew11 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew12:
// ###########################################################################################################################################
void colCallew12(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();
  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);
  redVal_ew12 = red;
  greenVal_ew12 = green;
  blueVal_ew12 = blue;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Slider change for LED intensity: DAY
// ###########################################################################################################################################
void sliderBrightnessDay(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  intensity_day = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Slider change for LED intensity: NIGHT
// ###########################################################################################################################################
void sliderBrightnessNight(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  intensity_night = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Time Day Mode Start
// ###########################################################################################################################################
void call_day_time_start(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  day_time_start = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Time Day Mode Stop
// ###########################################################################################################################################
void call_day_time_stop(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  day_time_stop = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Convert IP-address value to string:
// ###########################################################################################################################################
String IpAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3]);
}


// ###########################################################################################################################################
// # Telegram chat and massage handling:
// ###########################################################################################################################################
void handleNewMessages(int numNewMessages) {
  // Serial.println("handleNewMessages");
  // Serial.println(String(numNewMessages));

  updatedevice = false;

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;


    // Check if the sender id (chat_id) is the same as your own set Telegram id (CHAT_ID) to avoid unknown users control your WordClock.
    // You will be motified in case an unknown sender id tries to control the WordClock
    if (useTelegramID == 1) {
      if (String(CHAT_ID) != chat_id) {
        // if (String("123") != chat_id) { // TEST WITH UNKNOWN OWN FAKE ID 123
        Serial.println(String("Message received from UNKNOWN Telegram user: " + chat_id + " ==> WordClock will NOT react to this message"));
        bot.sendMessage(CHAT_ID, "Message received from UNKNOWN Telegram user: " + chat_id + " ==> WordClock will NOT react to this message " + "\xE2\x9D\x97");
        break;
      }
    }


    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    // ################################################################## DE:
    if (langLEDlayout == 0) {  // DE:

      if (text == "/ew1") {
        if (ew1 == 0) {
          ew1 = 1;
          bot.sendMessage(chat_id, "ALARM text set active " + emoActive);
        } else {
          ew1 = 0;
          bot.sendMessage(chat_id, "ALARM text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew2") {
        if (ew2 == 0) {
          ew2 = 1;
          bot.sendMessage(chat_id, "GEBURTSTAG text set active " + emoActive);
        } else {
          ew2 = 0;
          bot.sendMessage(chat_id, "GEBURTSTAG text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew3") {
        if (ew3 == 0) {
          ew3 = 1;
          bot.sendMessage(chat_id, "MÜLL RAUS BRINGEN text set active " + emoActive);
        } else {
          ew3 = 0;
          bot.sendMessage(chat_id, "MÜLL RAUS BRINGEN text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew4") {
        if (ew4 == 0) {
          ew4 = 1;
          bot.sendMessage(chat_id, "AUTO text set active " + emoActive);
        } else {
          ew4 = 0;
          bot.sendMessage(chat_id, "AUTO text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew5") {
        if (ew5 == 0) {
          ew5 = 1;
          bot.sendMessage(chat_id, "FEIERTAG text set active " + emoActive);
        } else {
          ew5 = 0;
          bot.sendMessage(chat_id, "FEIERTAG text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew6") {
        if (ew6 == 0) {
          ew6 = 1;
          bot.sendMessage(chat_id, "FORMEL1 text set active " + emoActive);
        } else {
          ew6 = 0;
          bot.sendMessage(chat_id, "FORMEL1 text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew7") {
        if (ew7 == 0) {
          ew7 = 1;
          bot.sendMessage(chat_id, "GELBER SACK text set active " + emoActive);
        } else {
          ew7 = 0;
          bot.sendMessage(chat_id, "GELBER SACK text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew8") {
        if (ew8 == 0) {
          ew8 = 1;
          bot.sendMessage(chat_id, "URLAUB text set active " + emoActive);
        } else {
          ew8 = 0;
          bot.sendMessage(chat_id, "URLAUB text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew9") {
        if (ew9 == 0) {
          ew9 = 1;
          bot.sendMessage(chat_id, "WERKSTATT text set active " + emoActive);
        } else {
          ew9 = 0;
          bot.sendMessage(chat_id, "WERKSTATT text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew10") {
        if (ew10 == 0) {
          ew10 = 1;
          bot.sendMessage(chat_id, "ZEIT ZUM ZOCKEN text set active " + emoActive);
        } else {
          ew10 = 0;
          bot.sendMessage(chat_id, "ZEIT ZUM ZOCKEN text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew11") {
        if (ew11 == 0) {
          ew11 = 1;
          bot.sendMessage(chat_id, "FRISEUR text set active " + emoActive);
        } else {
          ew11 = 0;
          bot.sendMessage(chat_id, "FRISEUR text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew12") {
        if (ew12 == 0) {
          ew12 = 1;
          bot.sendMessage(chat_id, "TERMIN text set active " + emoActive);
        } else {
          ew12 = 0;
          bot.sendMessage(chat_id, "TERMIN text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/start") {
        String welcome = "Welcome to WordClock Telegram bot, " + from_name + ". " + "\xF0\x9F\x98\x8A" + "\n";
        welcome += "This is a WordClock chat action test message.\n\n";
        welcome += "Use /ew1, /ew2, /ew3, /ew4, /ew5, /ew6, /ew7, /ew8, /ew9, /ew10, /ew11, /ew12 or the menu to set the extra words.\n\n";
        bot.sendMessage(chat_id, welcome);
      }
    }

    // ################################################################## EN:
    if (langLEDlayout == 1) {  // EN:
      if (text == "/ew1") {
        if (ew1 == 0) {
          ew1 = 1;
          bot.sendMessage(chat_id, "COME HERE text set active " + emoActive);
        } else {
          ew1 = 0;
          bot.sendMessage(chat_id, "COME HERE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew2") {
        if (ew2 == 0) {
          ew2 = 1;
          bot.sendMessage(chat_id, "LUNCH TIME text set active " + emoActive);
        } else {
          ew2 = 0;
          bot.sendMessage(chat_id, "LUNCH TIME text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew3") {
        if (ew3 == 0) {
          ew3 = 1;
          bot.sendMessage(chat_id, "ALARM text set active " + emoActive);
        } else {
          ew3 = 0;
          bot.sendMessage(chat_id, "ALARM text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew4") {
        if (ew4 == 0) {
          ew4 = 1;
          bot.sendMessage(chat_id, "GARBAGE text set active " + emoActive);
        } else {
          ew4 = 0;
          bot.sendMessage(chat_id, "GARBAGE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew5") {
        if (ew5 == 0) {
          ew5 = 1;
          bot.sendMessage(chat_id, "HOLIDAY text set active " + emoActive);
        } else {
          ew5 = 0;
          bot.sendMessage(chat_id, "HOLIDAY text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew6") {
        if (ew6 == 0) {
          ew6 = 1;
          bot.sendMessage(chat_id, "TEMPERATURE text set active " + emoActive);
        } else {
          ew6 = 0;
          bot.sendMessage(chat_id, "TEMPERATURE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew7") {
        if (ew7 == 0) {
          ew7 = 1;
          bot.sendMessage(chat_id, "DATE text set active " + emoActive);
        } else {
          ew7 = 0;
          bot.sendMessage(chat_id, "DATE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew8") {
        if (ew8 == 0) {
          ew8 = 1;
          bot.sendMessage(chat_id, "BIRTHDAY text set active " + emoActive);
        } else {
          ew8 = 0;
          bot.sendMessage(chat_id, "BIRTHDAY text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew9") {
        if (ew9 == 0) {
          ew9 = 1;
          bot.sendMessage(chat_id, "DOORBELL text set active " + emoActive);
        } else {
          ew9 = 0;
          bot.sendMessage(chat_id, "DOORBELL text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/start") {
        String welcome = "Welcome to WordClock Telegram bot, " + from_name + ". " + "\xF0\x9F\x98\x8A" + "\n";
        welcome += "This is a WordClock chat action test message.\n\n";
        welcome += "Use /ew1, /ew2, /ew3, /ew4, /ew5, /ew6, /ew7, /ew8, /ew9 or the menu to set the extra words.\n\n";
        bot.sendMessage(chat_id, welcome);
      }
    }

    // ################################################################## NL:
    if (langLEDlayout == 2) {  // NL:
      if (text == "/ew1") {
        if (ew1 == 0) {
          ew1 = 1;
          bot.sendMessage(chat_id, "KOM HIER text set active " + emoActive);
        } else {
          ew1 = 0;
          bot.sendMessage(chat_id, "KOM HIER text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew2") {
        if (ew2 == 0) {
          ew2 = 1;
          bot.sendMessage(chat_id, "LUNCH TIJD text set active " + emoActive);
        } else {
          ew2 = 0;
          bot.sendMessage(chat_id, "LUNCH TIJD text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew3") {
        if (ew3 == 0) {
          ew3 = 1;
          bot.sendMessage(chat_id, "ALARM text set active " + emoActive);
        } else {
          ew3 = 0;
          bot.sendMessage(chat_id, "ALARM text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew4") {
        if (ew4 == 0) {
          ew4 = 1;
          bot.sendMessage(chat_id, "AFVAL text set active " + emoActive);
        } else {
          ew4 = 0;
          bot.sendMessage(chat_id, "AFVAL text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew5") {
        if (ew5 == 0) {
          ew5 = 1;
          bot.sendMessage(chat_id, "VAKANTIE text set active " + emoActive);
        } else {
          ew5 = 0;
          bot.sendMessage(chat_id, "VAKANTIE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew6") {
        if (ew6 == 0) {
          ew6 = 1;
          bot.sendMessage(chat_id, "TEMPERATUUR text set active " + emoActive);
        } else {
          ew6 = 0;
          bot.sendMessage(chat_id, "TEMPERATUUR text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew7") {
        if (ew7 == 0) {
          ew7 = 1;
          bot.sendMessage(chat_id, "DATUM text set active " + emoActive);
        } else {
          ew7 = 0;
          bot.sendMessage(chat_id, "DATUM text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew8") {
        if (ew8 == 0) {
          ew8 = 1;
          bot.sendMessage(chat_id, "VERJAARDAG text set active " + emoActive);
        } else {
          ew8 = 0;
          bot.sendMessage(chat_id, "VERJAARDAG text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew9") {
        if (ew9 == 0) {
          ew9 = 1;
          bot.sendMessage(chat_id, "DEURBEL text set active " + emoActive);
        } else {
          ew9 = 0;
          bot.sendMessage(chat_id, "DEURBEL text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/start") {
        String welcome = "Welcome to WordClock Telegram bot, " + from_name + ". " + "\xF0\x9F\x98\x8A" + "\n";
        welcome += "This is a WordClock chat action test message.\n\n";
        welcome += "Use /ew1, /ew2, /ew3, /ew4, /ew5, /ew6, /ew7, /ew8, /ew9 or the menu to set the extra words.\n\n";
        bot.sendMessage(chat_id, welcome);
      }
    }

    // ################################################################## FR:
    if (langLEDlayout == 3) {  // FR:
      if (text == "/ew1") {
        if (ew1 == 0) {
          ew1 = 1;
          bot.sendMessage(chat_id, "ALARME text set active " + emoActive);
        } else {
          ew1 = 0;
          bot.sendMessage(chat_id, "ALARME text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew2") {
        if (ew2 == 0) {
          ew2 = 1;
          bot.sendMessage(chat_id, "ANNIVERSAIRE text set active " + emoActive);
        } else {
          ew2 = 0;
          bot.sendMessage(chat_id, "ANNIVERSAIRE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew3") {
        if (ew3 == 0) {
          ew3 = 1;
          bot.sendMessage(chat_id, "POUBELLE text set active " + emoActive);
        } else {
          ew3 = 0;
          bot.sendMessage(chat_id, "POUBELLE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew4") {
        if (ew4 == 0) {
          ew4 = 1;
          bot.sendMessage(chat_id, "A TABLE text set active " + emoActive);
        } else {
          ew4 = 0;
          bot.sendMessage(chat_id, "A TABLE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew5") {
        if (ew5 == 0) {
          ew5 = 1;
          bot.sendMessage(chat_id, "VACANCES text set active " + emoActive);
        } else {
          ew5 = 0;
          bot.sendMessage(chat_id, "VACANCES text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew6") {
        if (ew6 == 0) {
          ew6 = 1;
          bot.sendMessage(chat_id, "VIENS ICI text set active " + emoActive);
        } else {
          ew6 = 0;
          bot.sendMessage(chat_id, "VIENS ICI text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew7") {
        if (ew7 == 0) {
          ew7 = 1;
          bot.sendMessage(chat_id, "SONNETTE text set active " + emoActive);
        } else {
          ew7 = 0;
          bot.sendMessage(chat_id, "SONNETTE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew8") {
        if (ew8 == 0) {
          ew8 = 1;
          bot.sendMessage(chat_id, "TEMPERATURE text set active " + emoActive);
        } else {
          ew8 = 0;
          bot.sendMessage(chat_id, "TEMPERATURE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/ew9") {
        if (ew9 == 0) {
          ew9 = 1;
          bot.sendMessage(chat_id, "DATE text set active " + emoActive);
        } else {
          ew9 = 0;
          bot.sendMessage(chat_id, "DATE text set inactive " + emoInactive);
        }
        changedvalues = true;
      }

      if (text == "/start") {
        String welcome = "Welcome to WordClock Telegram bot, " + from_name + ". " + "\xF0\x9F\x98\x8A" + "\n";
        welcome += "This is a WordClock chat action test message.\n\n";
        welcome += "Use /ew1, /ew2, /ew3, /ew4, /ew5, /ew6, /ew7, /ew8, /ew9 or the menu to set the extra words.\n\n";
        bot.sendMessage(chat_id, welcome);
      }
    }
  }

  updatedevice = true;
}


// ###########################################################################################################################################
// # Set extra words:
// ###########################################################################################################################################
void set_extra_words() {
  // ########################################################### DE:
  if (langLEDlayout == 0) {  // DE:
    if (ew1 == 1) {
      setLEDexCol(11, 15, 1, redVal_ew1, greenVal_ew1, blueVal_ew1);  // ew1 - ALARM text
    } else {
      setLED(11, 15, 0);  // ew1 - ALARM text
    }

    if (ew2 == 1) {
      setLEDexCol(1, 10, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);  // ew2 - GEBURTSTAG text
    } else {
      setLED(1, 10, 0);  // ew2 - GEBURTSTAG text
    }

    if (ew3 == 1) {
      setLEDexCol(16, 19, 1, redVal_ew3, greenVal_ew3, blueVal_ew3);  // ew3 - MÜLL RAUS BRINGEN text
      setLEDexCol(58, 61, 1, redVal_ew3, greenVal_ew3, blueVal_ew3);  // ew3 - MÜLL RAUS BRINGEN text
      setLEDexCol(73, 79, 1, redVal_ew3, greenVal_ew3, blueVal_ew3);  // ew3 - MÜLL RAUS BRINGEN text
    } else {
      setLED(16, 19, 0);  // ew3 - MÜLL RAUS BRINGEN text
      setLED(58, 61, 0);  // ew3 - MÜLL RAUS BRINGEN text
      setLED(73, 79, 0);  // ew3 - MÜLL RAUS BRINGEN text
    }

    if (ew4 == 1) {
      setLEDexCol(20, 23, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);  // ew4 - AUTO text
    } else {
      setLED(20, 23, 0);  // ew4 - AUTO text
    }

    if (ew5 == 1) {
      setLEDexCol(24, 31, 1, redVal_ew5, greenVal_ew5, blueVal_ew5);  // ew5 - FEIERTAG text
    } else {
      setLED(24, 31, 0);  // ew5 - FEIERTAG text
    }

    if (ew6 == 1) {
      setLEDexCol(40, 46, 1, redVal_ew6, greenVal_ew6, blueVal_ew6);  // ew6 - FORMEL1 text
    } else {
      setLED(40, 46, 0);  // ew6 - FORMEL1 text
    }

    if (ew7 == 1) {
      setLEDexCol(64, 69, 1, redVal_ew7, greenVal_ew7, blueVal_ew7);  // ew7 - GELBER SACK text
      setLEDexCol(80, 83, 1, redVal_ew7, greenVal_ew7, blueVal_ew7);  // ew7 - GELBER SACK text
    } else {
      setLED(64, 69, 0);  // ew7 - GELBER SACK text
      setLED(80, 83, 0);  // ew7 - GELBER SACK text
    }

    if (ew8 == 1) {
      setLEDexCol(101, 106, 1, redVal_ew8, greenVal_ew8, blueVal_ew8);  // ew8 - URLAUB text
    } else {
      setLED(101, 106, 0);  // ew8 - URLAUB text
    }

    if (ew9 == 1) {
      setLEDexCol(116, 124, 1, redVal_ew9, greenVal_ew9, blueVal_ew9);  // ew9 - WERKSTATT text
    } else {
      setLED(116, 124, 0);  // ew9 - WERKSTATT text
    }

    if (ew10 == 1) {
      setLEDexCol(84, 87, 1, redVal_ew10, greenVal_ew10, blueVal_ew10);    // ew10 - ZEIT ZUM ZOCKEN text
      setLEDexCol(125, 127, 1, redVal_ew10, greenVal_ew10, blueVal_ew10);  // ew10 - ZEIT ZUM ZOCKEN text
      setLEDexCol(128, 133, 1, redVal_ew10, greenVal_ew10, blueVal_ew10);  // ew10 - ZEIT ZUM ZOCKEN text
    } else {
      setLED(84, 87, 0);    // ew10 - ZEIT ZUM ZOCKEN text
      setLED(125, 127, 0);  // ew10 - ZEIT ZUM ZOCKEN text
      setLED(128, 133, 0);  // ew10 - ZEIT ZUM ZOCKEN text
    }

    if (ew11 == 1) {
      setLEDexCol(134, 140, 1, redVal_ew11, greenVal_ew11, blueVal_ew11);  // ew11 - FRISEUR text
    } else {
      setLED(134, 139, 0);  // ew11 - FRISEUR text (140 = last letter of F of word FÜNF not set back to complete the word FÜNF)
    }

    if (ew12 == 1) {
      setLEDexCol(160, 165, 1, redVal_ew12, greenVal_ew12, blueVal_ew12);  // ew12 - TERMIN text
    } else {
      setLED(160, 165, 0);  // ew12 - TERMIN text
    }
  }

  // ########################################################### EN:
  if (langLEDlayout == 1) {  // EN:
    if (ew1 == 1) {
      setLEDexCol(10, 13, 1, redVal_ew1, greenVal_ew1, blueVal_ew1);  // ew1 - COME HERE text
      setLEDexCol(5, 8, 1, redVal_ew1, greenVal_ew1, blueVal_ew1);    // ew1 - COME HERE text
    } else {
      setLED(10, 13, 0);  // ew1 - COME HERE text
      setLED(5, 8, 0);    // ew1 - COME HERE text
    }

    if (ew2 == 1) {
      setLEDexCol(0, 4, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);    // ew2 - LUNCH TIME text
      setLEDexCol(28, 31, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);  // ew2 - LUNCH TIME text
    } else {
      setLED(0, 4, 0);    // ew2 - LUNCH TIME text
      setLED(28, 31, 0);  // ew2 - LUNCH TIME text
    }

    if (ew3 == 1) {
      setLEDexCol(39, 43, 1, redVal_ew3, greenVal_ew3, blueVal_ew3);  // ew3 - ALARM text

    } else {
      setLED(39, 43, 0);  // ew3 - ALARM text
    }

    if (ew4 == 1) {
      setLEDexCol(32, 38, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);  // ew4 - GARBAGE text
      setLEDexCol(68, 70, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);  // ew4 - GARBAGE text
    } else {
      setLED(32, 38, 0);  // ew4 - GARBAGE text
      setLED(68, 70, 0);  // ew4 - GARBAGE text
    }

    if (ew5 == 1) {
      setLEDexCol(65, 71, 1, redVal_ew5, greenVal_ew5, blueVal_ew5);  // ew5 - HOLIDAY text
    } else {
      setLED(65, 71, 0);  // ew5 - HOLIDAY text
    }

    if (ew6 == 1) {
      setLEDexCol(84, 94, 1, redVal_ew6, greenVal_ew6, blueVal_ew6);  // ew6 - TEMPERATURE text
    } else {
      setLED(84, 94, 0);  // ew6 - TEMPERATURE text
    }

    if (ew7 == 1) {
      setLEDexCol(96, 99, 1, redVal_ew7, greenVal_ew7, blueVal_ew7);  // ew7 - DATE text
    } else {
      setLED(96, 99, 0);  // ew7 - DATE text
    }

    if (ew8 == 1) {
      setLEDexCol(112, 119, 1, redVal_ew8, greenVal_ew8, blueVal_ew8);  // ew8 - BIRTHDAY text
    } else {
      setLED(112, 119, 0);  // ew8 - BIRTHDAY text
    }

    if (ew9 == 1) {
      setLEDexCol(148, 155, 1, redVal_ew9, greenVal_ew9, blueVal_ew9);  // ew9 - DOORBELL text
    } else {
      setLED(148, 155, 0);  // ew9 - DOORBELL text
    }
  }

  // ########################################################### NL:
  if (langLEDlayout == 2) {  // NL:

    // ew1 - KOM HIER text
    // ew2 - LUNCH TIJD text
    // ew3 - ALARM text
    // ew4 - AFVAL text
    // ew5 - VAKANTIE text
    // ew6 - TEMPERATUUR text
    // ew7 - DATUM text
    // ew8 - VERJAARDAG text
    // ew9 - DEURBEL text


    if (ew1 == 1) {
      setLEDexCol(48, 50, 1, redVal_ew1, greenVal_ew1, blueVal_ew1);  // ew1 - KOM HIER text
      setLEDexCol(68, 71, 1, redVal_ew1, greenVal_ew1, blueVal_ew1);  // ew1 - KOM HIER text
    } else {
      setLED(48, 50, 0);  // ew1 - KOM HIER text
      setLED(68, 71, 0);  // ew1 - KOM HIER text
    }

    if (ew2 == 1) {
      setLEDexCol(43, 47, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);  // ew2 - LUNCH TIJD text
      setLEDexCol(76, 79, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);  // ew2 - LUNCH TIJD text
    } else {
      setLED(43, 47, 0);  // ew2 - LUNCH TIJD text
      setLED(76, 79, 0);  // ew2 - LUNCH TIJD text
    }

    if (ew3 == 1) {
      setLEDexCol(8, 12, 1, redVal_ew3, greenVal_ew3, blueVal_ew3);  // ew3 - ALARM text

    } else {
      setLED(8, 12, 0);  // ew3 - ALARM text
    }

    if (ew4 == 1) {
      setLEDexCol(22, 27, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);  // ew4 - AFVAL text
    } else {
      setLED(22, 27, 0);  // ew4 - AFVAL text
    }

    if (ew5 == 1) {
      setLEDexCol(56, 63, 1, redVal_ew5, greenVal_ew5, blueVal_ew5);  // ew5 - VAKANTIE text
    } else {
      setLED(56, 63, 0);  // ew5 - VAKANTIE text
    }

    if (ew6 == 1) {
      setLEDexCol(196, 206, 1, redVal_ew6, greenVal_ew6, blueVal_ew6);  // ew6 - TEMPERATUUR text
    } else {
      setLED(196, 206, 0);  // ew6 - TEMPERATUUR text
    }

    if (ew7 == 1) {
      setLEDexCol(90, 94, 1, redVal_ew7, greenVal_ew7, blueVal_ew7);  // ew7 - DATUM text
    } else {
      setLED(90, 94, 0);  // ew7 - DATUM text
    }

    if (ew8 == 1) {
      setLEDexCol(80, 89, 1, redVal_ew8, greenVal_ew8, blueVal_ew8);  // ew8 - VERJAARDAG text
    } else {
      setLED(80, 89, 0);  // ew8 - VERJAARDAG text
    }

    if (ew9 == 1) {
      setLEDexCol(34, 40, 1, redVal_ew9, greenVal_ew9, blueVal_ew9);  // ew9 - DEURBEL text
    } else {
      setLED(34, 40, 0);  // ew9 - DEURBEL text
    }
  }

  // ########################################################### FR:
  if (langLEDlayout == 3) {  // FR:

    // ew1 - ALARME text
    // ew2 - ANNIVERSAIRE text
    // ew3 - POUBELLE text
    // ew4 - A TABLE text
    // ew5 - VACANCES text
    // ew6 - VIENS ICI text
    // ew7 - SONNETTE text
    // ew8 - TEMPERATURE text
    // ew9 - DATE text

    if (ew1 == 1) {
      setLEDexCol(122, 127, 1, redVal_ew1, greenVal_ew1, blueVal_ew1);  // ew1
    } else {
      setLED(122, 127, 0);  // ew1
    }

    if (ew2 == 1) {
      setLEDexCol(32, 43, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);  // ew2
    } else {
      setLED(32, 43, 0);  // ew2
    }

    if (ew3 == 1) {
      setLEDexCol(24, 31, 1, redVal_ew3, greenVal_ew3, blueVal_ew3);  // ew3

    } else {
      setLED(24, 31, 0);  // ew3
    }

    if (ew4 == 1) {
      setLEDexCol(92, 92, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);    // ew4
      setLEDexCol(219, 223, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);  // ew4
    } else {
      setLED(92, 92, 0);    // ew4
      setLED(219, 223, 0);  // ew4
    }

    if (ew5 == 1) {
      setLEDexCol(184, 191, 1, redVal_ew5, greenVal_ew5, blueVal_ew5);  // ew5
    } else {
      setLED(184, 191, 0);  // ew5
    }

    if (ew6 == 1) {
      setLEDexCol(112, 116, 1, redVal_ew6, greenVal_ew6, blueVal_ew6);  // ew6
      setLEDexCol(224, 226, 1, redVal_ew6, greenVal_ew6, blueVal_ew6);  // ew6
    } else {
      setLED(112, 116, 0);  // ew6
      setLED(224, 226, 0);  // ew6
    }

    if (ew7 == 1) {
      setLEDexCol(144, 151, 1, redVal_ew7, greenVal_ew7, blueVal_ew7);  // ew7
    } else {
      setLED(144, 151, 0);  // ew7
    }

    if (ew8 == 1) {
      setLEDexCol(164, 174, 1, redVal_ew8, greenVal_ew8, blueVal_ew8);  // ew8
    } else {
      setLED(164, 174, 0);  // ew8
    }

    if (ew9 == 1) {
      setLEDexCol(84, 87, 1, redVal_ew9, greenVal_ew9, blueVal_ew9);  // ew9
    } else {
      setLED(84, 87, 0);  // ew9
    }
  }
}


// ###########################################################################################################################################
// # Set special words with extra color
// ###########################################################################################################################################
void setLEDexCol(int ledNrFrom, int ledNrTo, int switchOn, int exRed, int exGreen, int exBlue) {
  if (ledNrFrom > ledNrTo) {
    setLED(ledNrTo, ledNrFrom, switchOn);  // Sets LED numbers in correct order
  } else {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, strip.Color(exRed, exGreen, exBlue));
    }
  }
  if (switchOn == 0) {
    for (int i = ledNrFrom; i <= ledNrTo; i++) {
      if ((i >= 0) && (i < NUMPIXELS))
        strip.setPixelColor(i, strip.Color(redVal_back, greenVal_back, blueVal_back));
    }
  }
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

void handleOTAupdate() {
  // OTA update server pages urls:
  updserver.on("/", HTTP_GET, []() {
    updserver.sendHeader("Connection", "close");
    updserver.send(200, "text/html", "WordClock web server on port 2022 is up. Please use the shown url and account credentials to update...");
  });

  updserver.on("/ota", HTTP_GET, []() {
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
          Serial.println("Status: Restart request");
          ResetTextLEDs(strip.Color(255, 0, 0));
          delay(1000);
          ResetTextLEDs(strip.Color(0, 255, 0));
          delay(1000);
        } else {
          Update.printError(Serial);
        }
      }
    });
  updserver.begin();
}


// ###########################################################################################################################################
// # Extra Words web update server:
// ###########################################################################################################################################
void handleExtraWords() {
  updatedevice = false;
  delay(1000);

  // OTA update server pages urls:
  ewserver.on("/", HTTP_GET, []() {
    ewserver.sendHeader("Connection", "close");
    ewserver.send(200, "text/html", "WordClock web server on port 8080 is up...");
  });

  // ################################################################## DE:
  if (langLEDlayout == 0) {  // DE:

    ewserver.on("/ew1", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew1 == 0) {
        ew1 = 1;
        ewserver.send(200, "text/html", "ALARM text set active");
      } else {
        ew1 = 0;
        ewserver.send(200, "text/html", "ALARM text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew2", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew2 == 0) {
        ew2 = 1;
        ewserver.send(200, "text/html", "GEBURTSTAG text set active");
      } else {
        ew2 = 0;
        ewserver.send(200, "text/html", "GEBURTSTAG text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew3", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew3 == 0) {
        ew3 = 1;
        ewserver.send(200, "text/html", "MUELL RAUS BRINGEN text set active");
      } else {
        ew3 = 0;
        ewserver.send(200, "text/html", "MUELL RAUS BRINGEN text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew4", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew4 == 0) {
        ew4 = 1;
        ewserver.send(200, "text/html", "AUTO text set active");
      } else {
        ew4 = 0;
        ewserver.send(200, "text/html", "AUTO text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew5", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew5 == 0) {
        ew5 = 1;
        ewserver.send(200, "text/html", "FEIERTAG text set active");
      } else {
        ew5 = 0;
        ewserver.send(200, "text/html", "FEIERTAG text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew6", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew6 == 0) {
        ew6 = 1;
        ewserver.send(200, "text/html", "FORMEL1 text set active");
      } else {
        ew6 = 0;
        ewserver.send(200, "text/html", "FORMEL1 text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew7", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew7 == 0) {
        ew7 = 1;
        ewserver.send(200, "text/html", "GELBER SACK text set active");
      } else {
        ew7 = 0;
        ewserver.send(200, "text/html", "GELBER SACK text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew8", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew8 == 0) {
        ew8 = 1;
        ewserver.send(200, "text/html", "URLAUB text set active");
      } else {
        ew8 = 0;
        ewserver.send(200, "text/html", "URLAUB text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew9", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew9 == 0) {
        ew9 = 1;
        ewserver.send(200, "text/html", "WERKSTATT text set active");
      } else {
        ew9 = 0;
        ewserver.send(200, "text/html", "WERKSTATT text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew10", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew10 == 0) {
        ew10 = 1;
        ewserver.send(200, "text/html", "ZEIT ZUM ZOCKEN text set active");
      } else {
        ew10 = 0;
        ewserver.send(200, "text/html", "ZEIT ZUM ZOCKEN text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew11", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew11 == 0) {
        ew11 = 1;
        ewserver.send(200, "text/html", "FRISEUR text set active");
      } else {
        ew11 = 0;
        ewserver.send(200, "text/html", "FRISEUR text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew12", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew12 == 0) {
        ew12 = 1;
        ewserver.send(200, "text/html", "TERMIN text set active");
      } else {
        ew12 = 0;
        ewserver.send(200, "text/html", "TERMIN text set inactive");
      }
      changedvalues = true;
    });
  }

  // ################################################################## EN:
  if (langLEDlayout == 1) {  // EN:

    ewserver.on("/ew1", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew1 == 0) {
        ew1 = 1;
        ewserver.send(200, "text/html", "COME HERE text set active");
      } else {
        ew1 = 0;
        ewserver.send(200, "text/html", "COME HERE text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew2", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew2 == 0) {
        ew2 = 1;
        ewserver.send(200, "text/html", "LUNCH TIME text set active");
      } else {
        ew2 = 0;
        ewserver.send(200, "text/html", "LUNCH TIME text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew3", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew3 == 0) {
        ew3 = 1;
        ewserver.send(200, "text/html", "ALARM text set active");
      } else {
        ew3 = 0;
        ewserver.send(200, "text/html", "ALARM text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew4", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew4 == 0) {
        ew4 = 1;
        ewserver.send(200, "text/html", "GARBAGE text set active");
      } else {
        ew4 = 0;
        ewserver.send(200, "text/html", "GARBAGE text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew5", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew5 == 0) {
        ew5 = 1;
        ewserver.send(200, "text/html", "HOLIDAY text set active");
      } else {
        ew5 = 0;
        ewserver.send(200, "text/html", "HOLIDAY text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew6", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew6 == 0) {
        ew6 = 1;
        ewserver.send(200, "text/html", "TEMPERATURE text set active");
      } else {
        ew6 = 0;
        ewserver.send(200, "text/html", "TEMPERATURE text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew7", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew7 == 0) {
        ew7 = 1;
        ewserver.send(200, "text/html", "DATE text set active");
      } else {
        ew7 = 0;
        ewserver.send(200, "text/html", "DATE text set inactive");
      }
      changedvalues = true;
      ;
    });


    ewserver.on("/ew8", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew8 == 0) {
        ew8 = 1;
        ewserver.send(200, "text/html", "BIRTHDAY text set active");
      } else {
        ew8 = 0;
        ewserver.send(200, "text/html", "BIRTHDAY text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew9", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew9 == 0) {
        ew9 = 1;
        ewserver.send(200, "text/html", "DOORBEL text set active");
      } else {
        ew9 = 0;
        ewserver.send(200, "text/html", "DOORBEL text set inactive");
      }
      changedvalues = true;
    });
  }

  // ################################################################## NL:
  if (langLEDlayout == 2) {  // NL:

    // ew1 - KOM HIER text
    // ew2 - LUNCH TIJD text
    // ew3 - ALARM text
    // ew4 - AFVAL text
    // ew5 - VAKANTIE text
    // ew6 - TEMPERATUUR text
    // ew7 - DATUM text
    // ew8 - VERJAARDAG text
    // ew9 - DEURBEL text

    ewserver.on("/ew1", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew1 == 0) {
        ew1 = 1;
        ewserver.send(200, "text/html", "KOM HIER text set active");
      } else {
        ew1 = 0;
        ewserver.send(200, "text/html", "KOM HIER text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew2", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew2 == 0) {
        ew2 = 1;
        ewserver.send(200, "text/html", "LUNCH TIJD text set active");
      } else {
        ew2 = 0;
        ewserver.send(200, "text/html", "LUNCH TIJD text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew3", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew3 == 0) {
        ew3 = 1;
        ewserver.send(200, "text/html", "ALARM text set active");
      } else {
        ew3 = 0;
        ewserver.send(200, "text/html", "ALARM text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew4", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew4 == 0) {
        ew4 = 1;
        ewserver.send(200, "text/html", "AFVAL text set active");
      } else {
        ew4 = 0;
        ewserver.send(200, "text/html", "AFVAL text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew5", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew5 == 0) {
        ew5 = 1;
        ewserver.send(200, "text/html", "VAKANTIE text set active");
      } else {
        ew5 = 0;
        ewserver.send(200, "text/html", "VAKANTIE text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew6", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew6 == 0) {
        ew6 = 1;
        ewserver.send(200, "text/html", "TEMPERATUUR text set active");
      } else {
        ew6 = 0;
        ewserver.send(200, "text/html", "TEMPERATUUR text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew7", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew7 == 0) {
        ew7 = 1;
        ewserver.send(200, "text/html", "DATUM text set active");
      } else {
        ew7 = 0;
        ewserver.send(200, "text/html", "DATUM text set inactive");
      }
      changedvalues = true;
      ;
    });


    ewserver.on("/ew8", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew8 == 0) {
        ew8 = 1;
        ewserver.send(200, "text/html", "VERJAARDAG text set active");
      } else {
        ew8 = 0;
        ewserver.send(200, "text/html", "VERJAARDAG text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew9", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew9 == 0) {
        ew9 = 1;
        ewserver.send(200, "text/html", "DEURBEL text set active");
      } else {
        ew9 = 0;
        ewserver.send(200, "text/html", "DEURBEL text set inactive");
      }
      changedvalues = true;
    });
  }

  // ################################################################## FR:
  if (langLEDlayout == 3) {  // FR:

    // ew1 - ALARME text
    // ew2 - ANNIVERSAIRE text
    // ew3 - POUBELLE text
    // ew4 - A TABLE text
    // ew5 - VACANCES text
    // ew6 - VIENS ICI text
    // ew7 - SONNETTE text
    // ew8 - TEMPERATURE text
    // ew9 - DATE text

    ewserver.on("/ew1", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew1 == 0) {
        ew1 = 1;
        ewserver.send(200, "text/html", "ALARME text set active");
      } else {
        ew1 = 0;
        ewserver.send(200, "text/html", "ALARME text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew2", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew2 == 0) {
        ew2 = 1;
        ewserver.send(200, "text/html", "ANNIVERSAIRE text set active");
      } else {
        ew2 = 0;
        ewserver.send(200, "text/html", "ANNIVERSAIRE text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew3", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew3 == 0) {
        ew3 = 1;
        ewserver.send(200, "text/html", "POUBELLE text set active");
      } else {
        ew3 = 0;
        ewserver.send(200, "text/html", "POUBELLE text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew4", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew4 == 0) {
        ew4 = 1;
        ewserver.send(200, "text/html", "A TABLE text set active");
      } else {
        ew4 = 0;
        ewserver.send(200, "text/html", "A TABLE text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew5", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew5 == 0) {
        ew5 = 1;
        ewserver.send(200, "text/html", "VACANCES text set active");
      } else {
        ew5 = 0;
        ewserver.send(200, "text/html", "VACANCES text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew6", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew6 == 0) {
        ew6 = 1;
        ewserver.send(200, "text/html", "VIENS ICI text set active");
      } else {
        ew6 = 0;
        ewserver.send(200, "text/html", "VIENS ICI text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew7", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew7 == 0) {
        ew7 = 1;
        ewserver.send(200, "text/html", "SONNETTE text set active");
      } else {
        ew7 = 0;
        ewserver.send(200, "text/html", "SONNETTE text set inactive");
      }
      changedvalues = true;
      ;
    });


    ewserver.on("/ew8", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew8 == 0) {
        ew8 = 1;
        ewserver.send(200, "text/html", "TEMPERATURE text set active");
      } else {
        ew8 = 0;
        ewserver.send(200, "text/html", "TEMPERATURE text set inactive");
      }
      changedvalues = true;
    });


    ewserver.on("/ew9", HTTP_GET, []() {
      ewserver.sendHeader("Connection", "close");
      if (ew9 == 0) {
        ew9 = 1;
        ewserver.send(200, "text/html", "DATE text set active");
      } else {
        ew9 = 0;
        ewserver.send(200, "text/html", "DATE text set inactive");
      }
      changedvalues = true;
    });
  }



  ewserver.begin();
  updatedevice = true;
}


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################