// ###########################################################################################################################################
// #
// # WordClock code for the printables WordClock 16x16 matrix project: https://www.printables.com/de/model/331012-wordclock-16x16-led-matrix
// #
// # Code by https://github.com/AWSW-de
// #
// # Released under licenses:
// # GNU General Public License v3.0: https://github.com/AWSW-de/WordClock-16x16-LED-matrix/blob/main/LICENSE and
// # Creative Commons Attribution-NonCommercial 3.0 Unported License http://creativecommons.org/licenses/by-nc/3.0
// # Important: NonCommercial — You may not use the material for commercial purposes !
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
// # - AsyncTCP               // by me-no-dev:                    https://github.com/me-no-dev/AsyncTCP
// # - ESPAsyncWebServer      // by me-no-dev:                    https://github.com/me-no-dev/ESPAsyncWebServer
// # - ESPUI                  // by s00500:                       https://github.com/s00500/ESPUI/archive/refs/tags/2.2.3.zip (!!! MAKE SURE NOT TO UPDATE TO >=2.2.4, ESP32 will permanetelly reboot !!!)
// # - ArduinoJson            // by bblanchon:                    https://github.com/bblanchon/ArduinoJson
// # - LITTLEFS               // by lorol:                        https://github.com/lorol/LITTLEFS
// # - UniversalTelegramBot   // by witnessmenow:                 https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
// # - ESP32Time              // by fbiego:                       https://github.com/fbiego/ESP32Time
// #
// ###########################################################################################################################################
#include <WiFi.h>                  // Used to connect the ESP32 to your WiFi
#include <WebServer.h>             // ESP32 OTA update function
#include <Update.h>                // ESP32 OTA update function
#include <Adafruit_NeoPixel.h>     // Used to drive the NeoPixel LEDs
#include "time.h"                  // Used for NTP time requests
#include <AsyncTCP.h>              // Used for the internal web server
#include <ESPAsyncWebServer.h>     // Used for the internal web server
#include <DNSServer.h>             // Used for the internal web server
#include <ESPUI.h>                 // Used for the internal web server
#include <Preferences.h>           // Used to save the configuration to the ESP32 flash
#include <WiFiClientSecure.h>      // Telegram support
#include <UniversalTelegramBot.h>  // Telegram support
#include <ArduinoJson.h>           // Telegram support
#include <ESP32Time.h>             // Used for the Offline Mode ESP32 time function
#include "settings.h"              // Settings are stored in a seperate file to make to code better readable and to be able to switch to other settings faster


// ###########################################################################################################################################
// # Version number of the code:
// ###########################################################################################################################################
const char* WORD_CLOCK_VERSION = "V4.0.0";


// ###########################################################################################################################################
// # Internal web server settings:
// ###########################################################################################################################################
AsyncWebServer server(80);       // Web server for config
WebServer otaserver(8080);       // Web OTA ESP32 update server
AsyncWebServer ledserver(2023);  // Web server for LED updates
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
DNSServer dnsServer;


// ###########################################################################################################################################
// # Declartions and variables used in the functions:
// ###########################################################################################################################################
ESP32Time rtc;            // Setup Offline ESP32 time function:
Preferences preferences;  // ESP32 flash storage
int langLEDlayout, iHour, iMinute, iSecond, redVal_back, greenVal_back, blueVal_back, redVal_time, greenVal_time, blueVal_time, ew1, ew2, ew3, ew4, ew5, ew6, ew7, ew8, ew9, ew10, ew11, ew12;
int redVal_ew1, greenVal_ew1, blueVal_ew1, redVal_ew2, greenVal_ew2, blueVal_ew2, redVal_ew3, greenVal_ew3, blueVal_ew3, redVal_ew4, greenVal_ew4, blueVal_ew4, redVal_ew5, greenVal_ew5, blueVal_ew5, redVal_ew6, greenVal_ew6, blueVal_ew6;
int redVal_ew7, greenVal_ew7, blueVal_ew7, redVal_ew8, greenVal_ew8, blueVal_ew8, redVal_ew9, greenVal_ew9, blueVal_ew9, redVal_ew10, greenVal_ew10, blueVal_ew10, redVal_ew11, greenVal_ew11, blueVal_ew11, redVal_ew12, greenVal_ew12, blueVal_ew12;
int intensity, intensity_day, intensity_night, intensity_web, usenightmode, statusNightMode, useshowip, statusLabelID, usesinglemin, useStartupText, useTelegram, useTelegramID;
int statusNightModeID, sliderBrightnessDayID, switchNightModeID, statusNightModeWarnID, sliderBrightnessNightID, call_day_time_startID, call_day_time_stopID, intensity_web_HintID, ew_web_HintID, statusLanguageID, DayNightSectionID;
int OfflineCurrentHourOffset, iHourOffset, statusTimeFromDevice, statusTimeSetOffline, OfflineModeHint1, OfflineModeHint2, OfflineModeHint3, showOMhints;
int UseOnlineMode, OfflineCurrentHour, OfflineCurrentMinute, mySetTimeZone, mySetTimeZoneID, mySetTimeServer, mySetTimeServerID, showTeleData, TeleBotID, TeleChatID, TeleShowDataID, TeleNoteUnknown, TeleNoteUnknownID;
int set_web_intensity = 0;
int set_web_colors = 0;
int set_web_ew_color = 0;
bool updatedevice = true;
bool updatenow = false;
bool updatemode = false;
bool changedvalues = false;
bool ResetExtraWords = false;
bool WiFIsetup = false;
String ewtext1, ewtext2, ewtext3, ewtext4, ewtext5, ewtext6, ewtext7, ewtext8, ewtext9, ewtext10, ewtext11, ewtext12 = "-";  // Set texts for the extra words 9 to 12:
String iStartTime, selectLangTXT, myTimeZone, myTimeServer, Timezone, NTPserver, day_time_start, day_time_stop, statusNightModeIDtxt;
String BOTtoken = "XXXXXXXXXX:YYYYYYYYYYYYYYY-ZZZZZZZZZZZZZZZZZZZZ";
String CHAT_ID = "1234512345";
String chat_id = CHAT_ID;
uint16_t TelegramSwitcher, TelegramSwitcherID, text_colour_background, text_colour_time, selectLang, timeId;
// WiFi REconnect function:
unsigned long WiFi_currentMillis = 0;
unsigned long WiFi_previousMillis = 0;
unsigned long WiFi_interval = 60000;
int WiFi_retry_counter = 0;
int useWiFiReCon, WiFiReConHint;
int WiFi_FlashLEDs = 10;
int WiFi_Restart = 30;


// ###########################################################################################################################################
// # WiFi, Telegram and other settings:
// ###########################################################################################################################################
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOTtoken, secured_client);
unsigned long bot_lasttime;           // last time messages' scan has been done
const unsigned long BOT_MTBS = 5000;  // mean time between scan messages --> 5s delay because of GUI stability issues at 1s


// ###########################################################################################################################################
// # Setup function that runs once at startup of the ESP:
// ###########################################################################################################################################
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("######################################################################");
  Serial.println("# WordClock startup of version: " + String(WORD_CLOCK_VERSION));
  Serial.println("######################################################################");
  preferences.begin("wordclock", false);         // Init ESP32 flash
  getFlashValues();                              // Read settings from flash
  strip.begin();                                 // Init the LEDs
  strip.show();                                  // Init the LEDs --> Set them to OFF
  intensity = intensity_day;                     // Set the intenity to day mode for startup
  strip.setBrightness(intensity);                // Set LED brightness
  startup();                                     // Run startup actions
  if (UseOnlineMode == 1) WIFI_SETUP();          // ONLINE MODE WiFi login and startup of web services
  if (UseOnlineMode == 0) {                      // OFFLINE MODE
    iHour = 9;                                   // Default hour in Offline Mode
    iMinute = 41;                                // Default minute in Offline Mode
    OfflinePotalSetup();                         // Offline mode setup access point
    rtc.setTime(0, iMinute, iHour, 1, 1, 2024);  // Set time: (ss, mm, hh, DD, MM, YYYY) --> 17th Jan 2021 09:41:00
    updatenow = true;                            // Update the display 1x after startup
    update_display();                            // Update LED display
    Serial.println("######################################################################");
    Serial.println("# WordClock startup in OFFLINE MODE finished...");
    Serial.println("######################################################################");
  }
}


// ###########################################################################################################################################
// # Loop function which runs all the time after the startup was done:
// ###########################################################################################################################################
void loop() {
  if (UseOnlineMode == 1) {                        // Offline Mode actions:
    if ((WiFIsetup == true) || (testTime == 1)) {  // WiFi mode or test time actions:
      WiFi_currentMillis = millis();
      if ((useWiFiReCon == true) && (WiFIsetup == true) && (WiFi.status() != WL_CONNECTED) && (WiFi_currentMillis - WiFi_previousMillis >= WiFi_interval)) {  // Device offline during runtime? > Reconnect now!
        WiFi_Lost();
      }
      if ((useWiFiReCon == true) && (WiFIsetup == true) && (WiFi.status() == WL_CONNECTED) && (WiFi_retry_counter >= 1)) {  // Device was offline during runtime? > Reconnect was successfull!
        Serial.println("Reconnecting to WiFi = SUCCESS");
        WiFi_retry_counter = 0;
        updatedevice = true;
        updatenow = true;
      }
      printLocalTime();                                 // Get the current time
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
    }
    if (updatemode == true) otaserver.handleClient();    // ESP32 OTA update
  } else {                                               // Offline Mode actions:
    if (debugtexts == 1) Serial.println(rtc.getTime());  // Time string as e.g. 15:24:38
    struct tm timeinfo = rtc.getTimeStruct();
    iHour = timeinfo.tm_hour;
    iMinute = timeinfo.tm_min;
    iSecond = timeinfo.tm_sec;
    if (updatedevice == true) {
      if (changedvalues == true) setFlashValues();  // Write settings to flash
      update_display();
    }
    delay(1000);
  }
  dnsServer.processNextRequest();  // Update web server
}


// ###########################################################################################################################################
// # Setup the internal web server configuration page:
// ###########################################################################################################################################
void setupWebInterface() {
  dnsServer.start(DNS_PORT, "*", apIP);
  if (UseOnlineMode == 0) ESPUI.captivePortal = true;  // Offline Mode Captive Portal



  // Section General:
  // ################
  ESPUI.separator("General:");

  // Welcome label:
  ESPUI.label("WordClock configuration", ControlColor::None, "Welcome to the WordClock configuration. Here you can adjust your WordClock settings to your personal needs. Enjoy your WordClock =)");

  // WordClock version:
  ESPUI.label("WordClock software version", ControlColor::None, WORD_CLOCK_VERSION);



  // Section Operation Mode:
  // #######################
  ESPUI.separator("Operation mode:");

  // Status label:
  if (UseOnlineMode == 0) {
    statusLabelID = ESPUI.label("Operation mode", ControlColor::None, "WordClock is used in Offline Mode. Please check time value.");
  } else {
    statusLabelID = ESPUI.label("Operation mode", ControlColor::Dark, "WordClock is used in Online Mode. All functions are available.");
  }

  // Add the invisible "Time" control to update to the current time of your device (PC, smartphone, tablet, ...)
  timeId = ESPUI.addControl(Time, "", "", None, 0, timeCallback);

  // Use online or offline mode:
  ESPUI.switcher("Use WordClock in Online Mode (change forces restart)", &switchOffline, ControlColor::Dark, UseOnlineMode);

  if (UseOnlineMode == 0) {
    // Time read from your device:
    ESPUI.button("Get time from your device", &getTimeCallback, ControlColor::Dark, "Get time from your computer, phone, tablet");
    statusTimeFromDevice = ESPUI.label("Time received from your device", ControlColor::Dark, "-");

    // Hour offset:
    OfflineCurrentHourOffset = ESPUI.addControl(ControlType::Select, "Current hour offset", String(iHourOffset), ControlColor::Dark, Control::noParent, SetOfflineHourOffset);
    ESPUI.addControl(ControlType::Option, "- 9 hours", "-9", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "- 8 hours", "-8", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "- 7 hours", "-7", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "- 6 hours", "-6", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "- 5 hours", "-5", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "- 4 hours", "-4", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "- 3 hours", "-3", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "- 2 hours", "-2", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "- 1 hours", "-1", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "+ 0 hours", "0", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "+ 1 hours", "1", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "+ 2 hours", "2", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "+ 3 hours", "3", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "+ 4 hours", "4", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "+ 5 hours", "5", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "+ 6 hours", "6", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "+ 7 hours", "7", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "+ 8 hours", "8", ControlColor::Alizarin, OfflineCurrentHourOffset);
    ESPUI.addControl(ControlType::Option, "+ 9 hours", "9", ControlColor::Alizarin, OfflineCurrentHourOffset);

    // Time set on WordClock with the calculated offset:
    statusTimeSetOffline = ESPUI.label("Time set on WordClock", ControlColor::Dark, "-");
  }

  // Operation Mode hints:
  ESPUI.switcher("Show the Operation Mode hints", &switchOMhints, ControlColor::Dark, showOMhints);
  OfflineModeHint1 = ESPUI.label("What is 'Online Mode' ?", ControlColor::Dark, "WordClock uses your set local WiFi to update the time and can use all of the smart functions in your local network. (Normal and recommended operation mode)");
  ESPUI.setPanelStyle(OfflineModeHint1, "width: 95%;");
  OfflineModeHint2 = ESPUI.label("What is 'Offline Mode' ?", ControlColor::Dark, "WordClock does not use your WiFi and it sets up an own, internal access point '" + String(Offline_SSID) + "' you can connect to to control all functions that do not require your network. All smart functions will be disabled and you need to set the time manually after each startup, but you can use the time piece in environments without a local WiFi.");
  ESPUI.setPanelStyle(OfflineModeHint2, "width: 95%;");
  OfflineModeHint3 = ESPUI.label("General usage hints", ControlColor::Dark, "You can switch between both modes without lost of data. In case your browser does not open the WordClock configuration page automatically in any mode after connecting to the access point, please navigate to this URL manually: <a href='http://" + IpAddress2String(WiFi.softAPIP()) + "' target='_blank' style='color:#ffffff;'>" + IpAddress2String(WiFi.softAPIP()) + "</a>");
  ESPUI.setPanelStyle(OfflineModeHint3, "width: 95%;");
  if (showOMhints == 0) {
    ESPUI.updateVisibility(OfflineModeHint1, false);
    ESPUI.updateVisibility(OfflineModeHint2, false);
    ESPUI.updateVisibility(OfflineModeHint3, false);
  }



  // Section LED settings:
  // #####################
  ESPUI.separator("LED settings:");

  // Time color selector:
  char hex_time[7] = { 0 };
  sprintf(hex_time, "#%02X%02X%02X", redVal_time, greenVal_time, blueVal_time);
  text_colour_time = ESPUI.text("Time", colCallTIME, ControlColor::Dark, hex_time);
  ESPUI.setInputType(text_colour_time, "color");

  // Background color selector:
  char hex_back[7] = { 0 };
  sprintf(hex_back, "#%02X%02X%02X", redVal_back, greenVal_back, blueVal_back);
  text_colour_background = ESPUI.text("Background", colCallBACK, ControlColor::Dark, hex_back);
  ESPUI.setInputType(text_colour_background, "color");

  // Show single minutes to display the minute exact time function:
  ESPUI.switcher("Show single minutes to display the minute exact time", &switchSingleMinutes, ControlColor::Dark, usesinglemin);

  // Show note when intensity is currently controlled via web-url usage and these internal settings get disabled:
  intensity_web_HintID = ESPUI.label("Manual settings disabled due to web URL usage:", ControlColor::Alizarin, "Restart WordClock or deactivate web control usage via <a href='http://" + IpAddress2String(WiFi.localIP()) + ":2023/config?INTENSITYviaWEB=0' target='_blank' style='color:#ffffff;'>http://" + IpAddress2String(WiFi.localIP()) + ":2023/config?INTENSITYviaWEB=0</a>");
  ESPUI.updateVisibility(intensity_web_HintID, false);



  // Section LED night mode settings:
  // ################################
  DayNightSectionID = ESPUI.separator("Day/Night LED brightness mode settings:");

  // Use night mode function:
  switchNightModeID = ESPUI.switcher("Use night mode to reduce brightness", &switchNightMode, ControlColor::Dark, usenightmode);

  // Intensity DAY slider selector:
  sliderBrightnessDayID = ESPUI.slider("Brightness during the day", &sliderBrightnessDay, ControlColor::Dark, intensity_day, 0, LEDintensityLIMIT);

  // Intensity NIGHT slider selector:
  sliderBrightnessNightID = ESPUI.slider("Brightness at night", &sliderBrightnessNight, ControlColor::Dark, intensity_night, 0, LEDintensityLIMIT);

  // Warning if day/night time setting is wrong:
  statusNightModeWarnID = ESPUI.label("Night mode settings error", ControlColor::Alizarin, "Wrong setting! 'Day time starts at' needs to be before 'Day time ends after'. Please correct values.");
  ESPUI.updateVisibility(statusNightModeWarnID, false);

  // Night mode status:
  statusNightModeID = ESPUI.label("Night mode status", ControlColor::Dark, "Night mode not used");

  // Day mode start time:
  call_day_time_startID = ESPUI.text("Day time starts at", call_day_time_start, ControlColor::Dark, String(day_time_start));
  ESPUI.setInputType(call_day_time_startID, "time");

  // Day mode stop time:
  call_day_time_stopID = ESPUI.text("Day time ends after", call_day_time_stop, ControlColor::Dark, String(day_time_stop));
  ESPUI.setInputType(call_day_time_stopID, "time");



  // Section Startup:
  // ################
  if (UseOnlineMode == 1) {
    ESPUI.separator("Startup:");

    // Startup WordClock text function:
    ESPUI.switcher("Show the 'WordClock' text on startup", &switchStartupText, ControlColor::Dark, useStartupText);

    // Show IP-address on startup:
    ESPUI.switcher("Show IP-address on startup", &switchShowIP, ControlColor::Dark, useshowip);
  }



  // Section Extra Words:
  // ####################
  if (UseOnlineMode == 1) {
    ESPUI.separator("Extra Words:");

    // ################################################################## DE:
    if (langLEDlayout == 0) {  // DE:
      ewtext1 = "1: ALARM";
      ewtext2 = "2: GEBURTSTAG";
      ewtext3 = "3: MÜLL RAUS BRINGEN";
      ewtext4 = "4: AUTO";
      ewtext5 = "5: FEIERTAG";
      ewtext6 = "6: FORMEL1";
      ewtext7 = "7: GELBER SACK";
      ewtext8 = "8: URLAUB";
      ewtext9 = "9: WERKSTATT";
      ewtext10 = "10: ZEIT ZUM ZOCKEN";
      ewtext11 = "11: FRISEUR";
      ewtext12 = "12: TERMIN";
    }

    // ################################################################## EN:
    if (langLEDlayout == 1) {  // EN:
      ewtext1 = "1: COME HERE";
      ewtext2 = "2: LUNCH TIME";
      ewtext3 = "3: ALARM";
      ewtext4 = "4: GARBAGE";
      ewtext5 = "5: HOLIDAY";
      ewtext6 = "6: TEMPERATURE";
      ewtext7 = "7: DATE";
      ewtext8 = "8: BIRTHDAY";
      ewtext9 = "9: DOORBELL";
    }

    // ################################################################## NL:
    if (langLEDlayout == 2) {  // NL:
      ewtext1 = "1: KOM HIER";
      ewtext2 = "2: LUNCH TIJD";
      ewtext3 = "3: ALARM";
      ewtext4 = "4: AFVAL";
      ewtext5 = "5: VAKANTIE";
      ewtext6 = "6: TEMPERATUUR";
      ewtext7 = "7: DATUM";
      ewtext8 = "8: VERJAARDAG";
      ewtext9 = "9: DEURBEL";
    }

    // ################################################################## FR:
    if (langLEDlayout == 3) {  // FR:
      ewtext1 = "1: ALARME";
      ewtext2 = "2: ANNIVERSAIRE";
      ewtext3 = "3: POUBELLE";
      ewtext4 = "4: A TABLE";
      ewtext5 = "5: VACANCES";
      ewtext6 = "6: VIENS ICI";
      ewtext7 = "7: SONNETTE";
      ewtext8 = "8: TEMPERATURE";
      ewtext9 = "9: DATE";
    }

    // ################################################################## IT:
    if (langLEDlayout == 4) {  // IT:
      ewtext1 = "1: VIENI QUI";
      ewtext2 = "2: ORA DI PRANZO";
      ewtext3 = "3: ALLARME";
      ewtext4 = "4: VACANZA";
      ewtext5 = "5: TEMPERATURA";
      ewtext6 = "6: DATA";
      ewtext7 = "7: COMPLEANNO";
      ewtext8 = "8: CAMPANELLO";
    }


    // ################################################################## SWE:
    if (langLEDlayout == 5) {     // SWE:
      ewtext1 = "1: FÖDELSEDAG";  // Geburtstag
      ewtext2 = "2: LARM";        // Alarm
      ewtext3 = "3: HÖGTID";      // Feiertag
      ewtext4 = "4: SEMESTER";    // Urlaub
      ewtext5 = "5: LADDA NER";   // Müll Tag
      ewtext6 = "6: LUNCHTID";    // Mittgszeit
      ewtext7 = "7: KOM HIT";     // Komm her
      ewtext8 = "8: DÖRRKLOCKA";  // Türklingel
      ewtext9 = "9: TEMPERATUR";  // Temperatur
    }


    // ################################################################## ES:
    if (langLEDlayout == 6) {           // ES:
      ewtext1 = "1: CUMPLEAÑOS";        // Geburtstag
      ewtext2 = "2: ALARMA";            // Alarm
      ewtext3 = "3: VACACIONES";        // Feiertag/Urlaub
      ewtext4 = "4: DÍA DE BASURA	";    // Müll Tag
      ewtext5 = "5: FECHA";             // Termin
      ewtext6 = "6: HORA DE ALMUERZO";  // Mittgszeit
      ewtext7 = "7: VEN AQUÍ";          // Komm her
      ewtext8 = "8: TIMBRE";            // Türklingel
      ewtext9 = "9: TEMPERATURA";       // Temperatur
    }

    // Get the selected colors for the extra words 1 to 12:

    // Color Extra Word ew1:
    char hex_ew1[7] = { 0 };
    sprintf(hex_ew1, "#%02X%02X%02X", redVal_ew1, greenVal_ew1, blueVal_ew1);
    uint16_t text_colour_ew1;
    text_colour_ew1 = ESPUI.text(ewtext1.c_str(), colCallew, ControlColor::Dark, hex_ew1, (void*)1);
    ESPUI.setInputType(text_colour_ew1, "color");

    // Color Extra Word ew2:
    char hex_ew2[7] = { 0 };
    sprintf(hex_ew2, "#%02X%02X%02X", redVal_ew2, greenVal_ew2, blueVal_ew2);
    uint16_t text_colour_ew2;
    text_colour_ew2 = ESPUI.text(ewtext2.c_str(), colCallew, ControlColor::Dark, hex_ew2, (void*)2);
    ESPUI.setInputType(text_colour_ew2, "color");

    // Color Extra Word ew3:
    char hex_ew3[7] = { 0 };
    sprintf(hex_ew3, "#%02X%02X%02X", redVal_ew3, greenVal_ew3, blueVal_ew3);
    uint16_t text_colour_ew3;
    text_colour_ew3 = ESPUI.text(ewtext3.c_str(), colCallew, ControlColor::Dark, hex_ew3, (void*)3);
    ESPUI.setInputType(text_colour_ew3, "color");

    // Color Extra Word ew4:
    char hex_ew4[7] = { 0 };
    sprintf(hex_ew4, "#%02X%02X%02X", redVal_ew4, greenVal_ew4, blueVal_ew4);
    uint16_t text_colour_ew4;
    text_colour_ew4 = ESPUI.text(ewtext4.c_str(), colCallew, ControlColor::Dark, hex_ew4, (void*)4);
    ESPUI.setInputType(text_colour_ew4, "color");

    // Color Extra Word ew5:
    char hex_ew5[7] = { 0 };
    sprintf(hex_ew5, "#%02X%02X%02X", redVal_ew5, greenVal_ew5, blueVal_ew5);
    uint16_t text_colour_ew5;
    text_colour_ew5 = ESPUI.text(ewtext5.c_str(), colCallew, ControlColor::Dark, hex_ew5, (void*)5);
    ESPUI.setInputType(text_colour_ew5, "color");

    // Color Extra Word ew6:
    char hex_ew6[7] = { 0 };
    sprintf(hex_ew6, "#%02X%02X%02X", redVal_ew6, greenVal_ew6, blueVal_ew6);
    uint16_t text_colour_ew6;
    text_colour_ew6 = ESPUI.text(ewtext6.c_str(), colCallew, ControlColor::Dark, hex_ew6, (void*)6);
    ESPUI.setInputType(text_colour_ew6, "color");

    // Color Extra Word ew7:
    char hex_ew7[7] = { 0 };
    sprintf(hex_ew7, "#%02X%02X%02X", redVal_ew7, greenVal_ew7, blueVal_ew7);
    uint16_t text_colour_ew7;
    text_colour_ew7 = ESPUI.text(ewtext7.c_str(), colCallew, ControlColor::Dark, hex_ew7, (void*)7);
    ESPUI.setInputType(text_colour_ew7, "color");

    // Color Extra Word ew8:
    char hex_ew8[7] = { 0 };
    sprintf(hex_ew8, "#%02X%02X%02X", redVal_ew8, greenVal_ew8, blueVal_ew8);
    uint16_t text_colour_ew8;
    text_colour_ew8 = ESPUI.text(ewtext8.c_str(), colCallew, ControlColor::Dark, hex_ew8, (void*)8);
    ESPUI.setInputType(text_colour_ew8, "color");

    if (langLEDlayout != 4) {  // IT has 8 extra words only:
      // Color Extra Word ew9:
      char hex_ew9[7] = { 0 };
      sprintf(hex_ew9, "#%02X%02X%02X", redVal_ew9, greenVal_ew9, blueVal_ew9);
      uint16_t text_colour_ew9;
      text_colour_ew9 = ESPUI.text(ewtext9.c_str(), colCallew, ControlColor::Dark, hex_ew9, (void*)9);
      ESPUI.setInputType(text_colour_ew9, "color");
    }

    if (langLEDlayout == 0) {  // DE has 12 extra words only:
      // Color Extra Word ew10:
      char hex_ew10[7] = { 0 };
      sprintf(hex_ew10, "#%02X%02X%02X", redVal_ew10, greenVal_ew10, blueVal_ew10);
      uint16_t text_colour_ew10;
      text_colour_ew10 = ESPUI.text(ewtext10.c_str(), colCallew, ControlColor::Dark, hex_ew10, (void*)10);
      ESPUI.setInputType(text_colour_ew10, "color");

      // Color Extra Word ew11:
      char hex_ew11[7] = { 0 };
      sprintf(hex_ew11, "#%02X%02X%02X", redVal_ew11, greenVal_ew11, blueVal_ew11);
      uint16_t text_colour_ew11;
      text_colour_ew11 = ESPUI.text(ewtext11.c_str(), colCallew, ControlColor::Dark, hex_ew11, (void*)11);
      ESPUI.setInputType(text_colour_ew11, "color");

      // Color Extra Word ew12:
      char hex_ew12[7] = { 0 };
      sprintf(hex_ew12, "#%02X%02X%02X", redVal_ew12, greenVal_ew12, blueVal_ew12);
      uint16_t text_colour_ew12;
      text_colour_ew12 = ESPUI.text(ewtext12.c_str(), colCallew, ControlColor::Dark, hex_ew12, (void*)12);
      ESPUI.setInputType(text_colour_ew12, "color");
    }

    // Show note when extra word color is currently controlled via web-url usage and these internal settings are not in sync (yet):
    ew_web_HintID = ESPUI.label("Manual extra word color settings may not be in sync (yet):", ControlColor::Alizarin, "Due to to web URL usage of the extra word colors the here shown color settings may not be in sync. Restart the WordClock to update the values.");
    ESPUI.updateVisibility(ew_web_HintID, false);
  }



  // Section WiFi:
  // #############
  if (UseOnlineMode == 1) {
    ESPUI.separator("WiFi:");

    // WiFi SSID:
    ESPUI.label("SSID", ControlColor::Dark, WiFi.SSID());

    // WiFi signal strength:
    ESPUI.label("Signal", ControlColor::Dark, String(WiFi.RSSI()) + "dBm");

    // Hostname:
    ESPUI.label("Hostname in your router", ControlColor::Dark, "<a href='http://" + String(WiFi.getHostname()) + "' target='_blank' style='color:#ffffff;'>" + String(WiFi.getHostname()) + "</a>");

    // WiFi MAC-address:
    ESPUI.label("MAC address", ControlColor::Dark, WiFi.macAddress());

    // WiFi ip-address:
    ESPUI.label("IP-address", ControlColor::Dark, "<a href='http://" + IpAddress2String(WiFi.localIP()) + "' target='_blank' style='color:#ffffff;'>" + IpAddress2String(WiFi.localIP()) + "</a>");

    // WiFi DNS address:
    ESPUI.label("DNS address", ControlColor::Dark, IpAddress2String(WiFi.dnsIP()));

    // WiFi Gateway address:
    ESPUI.label("Gateway address", ControlColor::Dark, IpAddress2String(WiFi.gatewayIP()));

    // Maximum retries to reach the configured WiFi:
    ESPUI.label("Max. tries on startup until the WiFi config is reset", ControlColor::Dark, String(maxWiFiconnctiontries));
  }



  // Section WiFi reconnect:
  // #######################
  if (UseOnlineMode == 1) {
    ESPUI.separator("Active WiFi reconnect during runtime:");

    // Use WIFi reconnect function during runtime:
    ESPUI.switcher("Use active WIFi reconnect function during runtime", &switchWiFiReConnect, ControlColor::Dark, useWiFiReCon);

    // Notes about active WiFi reconnect:
    WiFiReConHint = ESPUI.label("Notes about active WiFi reconnect", ControlColor::Dark, "WordClock will check its connection to your WiFi every " + String(WiFi_interval / 1000) + " seconds (" + String((WiFi_interval / 1000) / 60) + " minute(s)). If the WiFi cannot be reached anymore it will continue to work offline for " + String((WiFi_interval / 1000) * WiFi_FlashLEDs) + " seconds (" + String(((WiFi_interval / 1000) * WiFi_FlashLEDs) / 60) + " minutes), but will flash the WiFi LEDs in blue, yellow and red then. If after " + String((WiFi_interval / 1000) * WiFi_Restart) + " seconds (" + String(((WiFi_interval / 1000) * WiFi_Restart) / 60) + " minutes) the WiFi can still not be reconnected, WordClock will reboot automatically to solve the connection problem. In case the WiFi then still not is able to reach for " + String(maxWiFiconnctiontries) + " tries, the configuration will be set to default. It is expected then, that the WiFi will not be available anymore, e.g. due to a change of your router, etc...");
    ESPUI.setPanelStyle(WiFiReConHint, "width: 60%;");
  }



  // Section Time settings:
  // ######################
  if (UseOnlineMode == 1) {
    ESPUI.separator("Time settings:");

    // WordClock startup time:
    ESPUI.label("WordClock startup time", ControlColor::Dark, iStartTime);

    // NTP time server: (All of these were successfully tested on 06.01.2024)
    mySetTimeServer = ESPUI.addControl(ControlType::Select, "Choose your time server (change forces restart)", String(myTimeServer), ControlColor::Dark, Control::noParent, SetMyTimeServer);
    ESPUI.addControl(ControlType::Option, "Best choice: Your local router (WiFi gateway address)", "Your local router", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "Special: Use value set in 'settings.h'", NTPserver_default, ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "Special: Your local 'Speedport IP' router", "speedport.ip", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "Special: Your local 'Fritz!Box' router", "fritz.box", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "Default: pool.ntp.org", "pool.ntp.org", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "AS: asia.pool.ntp.org", "asia.pool.ntp.org", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "AT: asynchronos.iiss.at", "asynchronos.iiss.at", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "BE: ntp1.oma.be", "ntp1.oma.be", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "BR: ntps1.pads.ufrj.br", "ntps1.pads.ufrj.br", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "CA: time.nrc.ca", "time.nrc.ca", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "CH: ntp.neel.ch", "ntp.neel.ch", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "CL: ntp.shoa.cl", "ntp.shoa.cl", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "CN: ntp.neu.edu.cn", "ntp.neu.edu.cn", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "CZ: ntp.nic.cz", "ntp.nic.cz", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "DE: time.fu-berlin.de", "time.fu-berlin.de", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "ES: hora.roa.es", "hora.roa.es", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "EU: europe.pool.ntp.org", "europe.pool.ntp.org", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "EUS: ntp.i2t.ehu.eus", "ntp.i2t.ehu.eus", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "HU: ntp.atomki.mta.hu", "ntp.atomki.mta.hu", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "IT: ntp1.inrim.it", "ntp1.inrim.it", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "JP: ntp.nict.jp", "ntp.nict.jp", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "MX: cronos.cenam.mx", "cronos.cenam.mx", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "NL: ntp.vsl.nl", "ntp.vsl.nl", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "PL: tempus1.gum.gov.pl", "tempus1.gum.gov.pl", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "RO: ntp1.usv.ro", "ntp1.usv.ro", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "SE: time1.stupi.se", "time1.stupi.se", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "UK: uk.pool.ntp.org", "uk.pool.ntp.org", ControlColor::Alizarin, mySetTimeServer);
    ESPUI.addControl(ControlType::Option, "US: north-america.pool.ntp.org", "north-america.pool.ntp.org", ControlColor::Alizarin, mySetTimeServer);
    mySetTimeServerID = ESPUI.label("Used NTP time server", ControlColor::Dark, NTPserver);

    // Time zone:
    mySetTimeZone = ESPUI.addControl(ControlType::Select, "Choose your time zone (change forces restart)", String(myTimeZone), ControlColor::Dark, Control::noParent, SetMyTimeZone);
    ESPUI.addControl(ControlType::Option, "Use value set in 'settings.h'", Timezone_default, ControlColor::Alizarin, mySetTimeZone);
    ESPUI.addControl(ControlType::Option, "Asia: EET-2EEST,M3.5.5/0,M10.5.5/0", "EET-2EEST,M3.5.5/0,M10.5.5/0", ControlColor::Alizarin, mySetTimeZone);
    ESPUI.addControl(ControlType::Option, "Australia: ACST-9:30ACDT,M10.1.0,M4.1.0/3", "ACST-9:30ACDT,M10.1.0,M4.1.0/3", ControlColor::Alizarin, mySetTimeZone);
    ESPUI.addControl(ControlType::Option, "Central Europe: CET-1CEST,M3.5.0,M10.5.0/3 (Austria,Denmark,France,Germany,Italy,Netherlands,Poland,Switzerland)", "CET-1CEST,M3.5.0,M10.5.0/3", ControlColor::Alizarin, mySetTimeZone);
    ESPUI.addControl(ControlType::Option, "Most of Europe: MET-2METDST,M3.5.0/01,M10.5.0/02", "MET-2METDST,M3.5.0/01,M10.5.0/02", ControlColor::Alizarin, mySetTimeZone);
    ESPUI.addControl(ControlType::Option, "New Zealand: NZST-12NZDT,M9.5.0,M4.1.0/3 (Auckland)", "NZST-12NZDT,M9.5.0,M4.1.0/3", ControlColor::Alizarin, mySetTimeZone);
    ESPUI.addControl(ControlType::Option, "UK: GMT0BST,M3.5.0/01,M10.5.0/02 (London)", "GMT0BST,M3.5.0/01,M10.5.0/02", ControlColor::Alizarin, mySetTimeZone);
    ESPUI.addControl(ControlType::Option, "USA EST: EST5EDT,M3.2.0,M11.1.0", "EST5EDT,M3.2.0,M11.1.0", ControlColor::Alizarin, mySetTimeZone);
    ESPUI.addControl(ControlType::Option, "USA CST: CST6CDT,M3.2.0,M11.1.0", "CST6CDT,M3.2.0,M11.1.0", ControlColor::Alizarin, mySetTimeZone);
    ESPUI.addControl(ControlType::Option, "USA MST: MST7MDT,M4.1.0,M10.5.0", "MST7MDT,M4.1.0,M10.5.0", ControlColor::Alizarin, mySetTimeZone);
    ESPUI.addControl(ControlType::Option, "Vietnam: ICT-7", "ICT-7", ControlColor::Alizarin, mySetTimeZone);
    mySetTimeZoneID = ESPUI.label("Used time zone value", ControlColor::Dark, Timezone);
  }



  // Section Telegram:
  // #################
  if (UseOnlineMode == 1) {
    ESPUI.separator("Telegram support:");
    if ((String(BOTtoken) == "XXXXXXXXXX:YYYYYYYYYYYYYYY-ZZZZZZZZZZZZZZZZZZZZ") || (String(CHAT_ID) == "1234512345")) {
      // Show note when Telegram was not configured yet:
      ESPUI.label("Status:", ControlColor::Dark, "Telegram not configured yet. See intructions in settings.h");
    } else {
      // Use Telegram support:
      TelegramSwitcher = ESPUI.switcher("Use Telegram support (change forces restart)", &switchTelegram, ControlColor::Dark, useTelegram);

      // Reduce Telegram support to your own CHAT_ID:
      TelegramSwitcherID = ESPUI.switcher("React to your own Telegram ID only", &switchTelegramID, ControlColor::Dark, useTelegramID);


      // Show or hide your own Telegram BOTtoken and CHAT_ID:
      TeleNoteUnknownID = ESPUI.switcher("Send notifications about messages from unknown users", &switchTeleShowNote, ControlColor::Dark, TeleNoteUnknown);

      // Show or hide your own Telegram BOTtoken and CHAT_ID:
      TeleShowDataID = ESPUI.switcher("Show your own Telegram bot token and chat id", &switchTeleShowData, ControlColor::Dark, showTeleData);

      // Your Telegram BOTtoken
      TeleBotID = ESPUI.label("Your Telegram bot token", ControlColor::Dark, BOTtoken);

      // Your Telegram chat id:
      TeleChatID = ESPUI.label("Your Telegram chat id", ControlColor::Dark, CHAT_ID);
    }
    if (showTeleData == 0) {
      ESPUI.updateVisibility(TeleBotID, false);
      ESPUI.updateVisibility(TeleChatID, false);
    }
  }



  // Section smart home control via web URLs:
  // ########################################
  if (UseOnlineMode == 1) {
    ESPUI.separator("Smart home control via web URLs:");

    // About note:
    ESPUI.label("About note", ControlColor::Dark, "Control WordClock from your smart home environment via web URLs.");

    // Functions note:
    ESPUI.label("Functions", ControlColor::Dark, "You can configure colors, the LED intensity and the extra words and get the status of each named element.");

    // Usage note:
    ESPUI.label("Usage hints and examples", ControlColor::Dark, "<a href='http://" + IpAddress2String(WiFi.localIP()) + ":2023' target='_blank' style='color:#ffffff;'>http://" + IpAddress2String(WiFi.localIP()) + ":2023</a> or <a href='http://" + String(WiFi.getHostname()) + ":2023' target='_blank' style='color:#ffffff;'>http://" + String(WiFi.getHostname()) + ":2023</a>");
  }



  // Section Update:
  // ###############
  if (UseOnlineMode == 1) {
    ESPUI.separator("Update:");

    // Update WordClock:
    ESPUI.button("Activate update mode", &buttonUpdate, ControlColor::Dark, "Activate update mode", (void*)1);

    // Update URL
    ESPUI.label("Update URLs", ControlColor::Dark, "<a href='http://" + IpAddress2String(WiFi.localIP()) + ":8080' target='_blank' style='color:#ffffff;'>http://" + IpAddress2String(WiFi.localIP()) + ":8080</a> or <a href='http://" + String(WiFi.getHostname()) + ":8080' target='_blank' style='color:#ffffff;'>http://" + String(WiFi.getHostname()) + ":8080</a>");

    // AWSW software GitHub repository:
    ESPUI.label("Download newer software updates here", ControlColor::Dark, "<a href='https://github.com/AWSW-de/WordClock-16x16-LED-matrix' target='_blank' style='color:#ffffff;'>https://github.com/AWSW-de/WordClock-16x16-LED-matrix</a>");
  }



  // Section Language:
  // #################
  ESPUI.separator("Language:");
  // Set layout language in drop down field:
  if (langLEDlayout == 0) selectLangTXT = "German";
  if (langLEDlayout == 1) selectLangTXT = "English";
  if (langLEDlayout == 2) selectLangTXT = "Dutch";
  if (langLEDlayout == 3) selectLangTXT = "French";
  if (langLEDlayout == 4) selectLangTXT = "Italian (2024 models only)";
  if (langLEDlayout == 5) selectLangTXT = "Swedish (2024 models only)";
  if (langLEDlayout == 6) selectLangTXT = "Spanish (2024 models only)";
  Serial.print("Selected language: ");
  Serial.println(selectLangTXT);

  // Change language:
  selectLang = ESPUI.addControl(ControlType::Select, "Change layout language", String(langLEDlayout), ControlColor::Dark, Control::noParent, call_langauge_select);
  ESPUI.addControl(ControlType::Option, "German", "0", ControlColor::Alizarin, selectLang);
  ESPUI.addControl(ControlType::Option, "English", "1", ControlColor::Alizarin, selectLang);
  ESPUI.addControl(ControlType::Option, "Dutch", "2", ControlColor::Alizarin, selectLang);
  ESPUI.addControl(ControlType::Option, "French", "3", ControlColor::Alizarin, selectLang);
  ESPUI.addControl(ControlType::Option, "Italian (2024 models only)", "4", ControlColor::Alizarin, selectLang);
  ESPUI.addControl(ControlType::Option, "Swedish (2024 models only)", "5", ControlColor::Alizarin, selectLang);
  ESPUI.addControl(ControlType::Option, "Spanish (2024 models only)", "6", ControlColor::Alizarin, selectLang);

  // Current language:
  statusLanguageID = ESPUI.label("Current layout language", ControlColor::Dark, selectLangTXT);



  // Section Maintenance:
  // ####################
  ESPUI.separator("Maintenance:");

  // Restart WordClock:
  ESPUI.button("Restart WordClock", &buttonRestart, ControlColor::Dark, "Restart WordClock", (void*)1);

  if (UseOnlineMode == 1) {
    // Reset WiFi and initial settings:
    ESPUI.button("Reset settings to enter initial values again", &buttonWiFiReset, ControlColor::Dark, "Reset WiFi, language, Telegram settings", (void*)2);
  }

  // Reset WiFi and initial settings:
  ESPUI.button("Reset all Extra Words", &buttonResetExtraWords, ControlColor::Dark, "Reset all Extra Words", (void*)3);

  // Reset WordClock settings:
  ESPUI.button("Reset settings (except WiFi, language, Telegram & operation mode)", &buttonWordClockReset, ControlColor::Dark, "Reset WordClock settings", (void*)3);



  // Section License:
  // ####################
  ESPUI.separator("License information:");

  // License information:
  ESPUI.label("License information", ControlColor::Dark, "NonCommercial — You may not use the project for commercial purposes!");



  checkforNightMode();       // Check for night mode settings on startup
  ESPUI.begin("WordClock");  // Deploy the page
}


// ###########################################################################################################################################
// # Read settings from flash:
// ###########################################################################################################################################
void getFlashValues() {
  if (debugtexts == 1) Serial.println("Read settings from flash: START");
  UseOnlineMode = preferences.getUInt("UseOnlineMode", 1);
  showOMhints = preferences.getUInt("showOMhints", showOMhints_default);
  iHourOffset = preferences.getUInt("iHourOffset", iHourOffset_default);
  useStartupText = preferences.getUInt("useStartupText", useStartupText_default);
  langLEDlayout = preferences.getUInt("langLEDlayout", langLEDlayout_default);
  myTimeZone = preferences.getString("myTimeZone", Timezone_default);
  Timezone = myTimeZone;
  myTimeServer = preferences.getString("myTimeServer", NTPserver_default);
  NTPserver = myTimeServer;
  redVal_time = preferences.getUInt("redVal_time", redVal_time_default);
  greenVal_time = preferences.getUInt("greenVal_time", greenVal_time_default);
  blueVal_time = preferences.getUInt("blueVal_time", blueVal_time_default);
  redVal_back = preferences.getUInt("redVal_back", redVal_back_default);
  greenVal_back = preferences.getUInt("greenVal_back", greenVal_back_default);
  blueVal_back = preferences.getUInt("blueVal_back", blueVal_back_default);
  intensity_day = preferences.getUInt("intensity_day", intensity_day_default);
  intensity_night = preferences.getUInt("intensity_night", intensity_night_default);
  usenightmode = preferences.getUInt("usenightmode", usenightmode_default);
  day_time_start = preferences.getString("day_time_start", day_time_start_default);
  day_time_stop = preferences.getString("day_time_stop", day_time_stop_default);
  useshowip = preferences.getUInt("useshowip", useshowip_default);
  usesinglemin = preferences.getUInt("usesinglemin", usesinglemin_default);
  useTelegram = preferences.getUInt("useTelegram", useTelegram_default);
  useTelegramID = preferences.getUInt("useTelegramID", useTelegramID_default);
  TeleNoteUnknown = preferences.getUInt("TeleNoteUnknown", TeleNoteUnknown_default);
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
  ew1 = preferences.getUInt("ew1", 0);
  ew2 = preferences.getUInt("ew2", 0);
  ew3 = preferences.getUInt("ew3", 0);
  ew4 = preferences.getUInt("ew4", 0);
  ew5 = preferences.getUInt("ew5", 0);
  ew6 = preferences.getUInt("ew6", 0);
  ew7 = preferences.getUInt("ew7", 0);
  ew8 = preferences.getUInt("ew8", 0);
  ew9 = preferences.getUInt("ew9", 0);
  ew10 = preferences.getUInt("ew10", 0);
  ew11 = preferences.getUInt("ew11", 0);
  ew12 = preferences.getUInt("ew12", 0);
  BOTtoken = preferences.getString("iBotToken", "");
  CHAT_ID = preferences.getString("iChatID", "");
  showTeleData = preferences.getUInt("showTeleData", showTeleData_default);
  bot.updateToken(String(BOTtoken));
  useWiFiReCon = preferences.getUInt("useWiFiReCon", useWiFiReCon_default);
  if (debugtexts == 1) Serial.println("Read settings from flash: END");
}


// ###########################################################################################################################################
// # Write settings to flash:
// ###########################################################################################################################################
void setFlashValues() {
  if (debugtexts == 1) Serial.println("Write settings to flash: START");
  changedvalues = false;
  preferences.putUInt("UseOnlineMode", UseOnlineMode);
  preferences.putUInt("showOMhints", showOMhints);
  preferences.putUInt("iHourOffset", iHourOffset);
  preferences.putUInt("useStartupText", useStartupText);
  preferences.putString("myTimeZone", myTimeZone);
  preferences.putString("myTimeServer", myTimeServer);
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
  preferences.putString("day_time_start", day_time_start);
  preferences.putString("day_time_stop", day_time_stop);
  preferences.putUInt("useshowip", useshowip);
  preferences.putUInt("usesinglemin", usesinglemin);
  preferences.putUInt("useTelegram", useTelegram);
  preferences.putUInt("useTelegramID", useTelegramID);
  preferences.putUInt("showTeleData", showTeleData);
  preferences.putUInt("TeleNoteUnknown", TeleNoteUnknown);
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
  preferences.putUInt("useWiFiReCon", useWiFiReCon);
  if (debugtexts == 1) Serial.println("Write settings to flash: END");
  checkforNightMode();
  updatenow = true;  // Update display now...
}


// ###########################################################################################################################################
// # GUI: Reset the WordClock settings:
// ###########################################################################################################################################
void buttonWordClockReset(Control* sender, int type, void* param) {
  updatedevice = false;
  delay(1000);
  Serial.println("Status: WORDCLOCK SETTINGS RESET REQUEST EXECUTED");
  // Save stored values for WiFi, language and Operation Mode:
  String tempDelWiFiSSID = preferences.getString("WIFIssid");
  String tempDelWiFiPASS = preferences.getString("WIFIpass");
  int tempDelLANG = preferences.getUInt("langLEDlayout");
  int tempOfflineMode = preferences.getUInt("UseOnlineMode");
  preferences.clear();
  delay(100);
  preferences.putUInt("UseOnlineMode", tempOfflineMode);  // Offline Mode
  preferences.putString("WIFIssid", tempDelWiFiSSID);     // Restore entered WiFi SSID
  preferences.putString("WIFIpass", tempDelWiFiPASS);     // Restore entered WiFi password
  preferences.putUInt("langLEDlayout", tempDelLANG);      // Restore entered language
  preferences.putUInt("showOMhints", showOMhints_default);
  preferences.putUInt("useStartupText", useStartupText_default);
  preferences.putString("myTimeZone", Timezone_default);
  preferences.putString("myTimeServer", NTPserver_default);
  preferences.putString("iBotToken", BOTtoken);
  preferences.putString("iChatID", CHAT_ID);
  preferences.putUInt("useTelegram", useTelegram_default);
  preferences.putUInt("useTelegramID", useTelegramID_default);
  preferences.putUInt("showTeleData", showTeleData_default);
  preferences.putUInt("TeleNoteUnknown", TeleNoteUnknown_default);
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
  preferences.putString("day_time_start", day_time_start_default);
  preferences.putString("day_time_stop", day_time_stop_default);
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
  preferences.putUInt("ew1", 0);
  preferences.putUInt("ew2", 0);
  preferences.putUInt("ew3", 0);
  preferences.putUInt("ew4", 0);
  preferences.putUInt("ew5", 0);
  preferences.putUInt("ew6", 0);
  preferences.putUInt("ew7", 0);
  preferences.putUInt("ew8", 0);
  preferences.putUInt("ew9", 0);
  preferences.putUInt("ew10", 0);
  preferences.putUInt("ew11", 0);
  preferences.putUInt("ew12", 0);
  preferences.putUInt("useWiFiReCon", useWiFiReCon_default);
  delay(1000);
  preferences.end();
  Serial.println("####################################################################################################");
  Serial.println("# WORDCLOCK SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE CONFIGURE AGAIN... #");
  Serial.println("####################################################################################################");
  ClearDisplay();
  strip.show();
  delay(250);
  ESP.restart();
}


// ###########################################################################################################################################
// # GUI: Language selection
// ###########################################################################################################################################
void call_langauge_select(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  langLEDlayout = sender->value.toInt();
  // Set layout language in drop down field:
  if (langLEDlayout == 0) selectLangTXT = "German";
  if (langLEDlayout == 1) selectLangTXT = "English";
  if (langLEDlayout == 2) selectLangTXT = "Dutch";
  if (langLEDlayout == 3) selectLangTXT = "French";
  if (langLEDlayout == 4) selectLangTXT = "Italian (2024 models only)";
  if (langLEDlayout == 5) selectLangTXT = "Swedish (2024 models only)";
  if (langLEDlayout == 6) selectLangTXT = "Spanish (2024 models only)";
  if (debugtexts == 1) {
    Serial.print("Selected language ID: ");
    Serial.println(langLEDlayout);
    Serial.print("Selected language: ");
    Serial.println(selectLangTXT);
  }
  ESPUI.print(statusLanguageID, selectLangTXT);
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # Clear the display:
// ###########################################################################################################################################
void ClearDisplay() {
  uint32_t c0 = strip.Color(0, 0, 0);
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, c0);
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
    ClearDisplay();
    numbers(getDigit(int(WiFi.localIP()[0]), 2), 3);
    numbers(getDigit(int(WiFi.localIP()[0]), 1), 2);
    numbers(getDigit(int(WiFi.localIP()[0]), 0), 1);
    setLED(191, 191, 1);
    setLED(0, 3, 1);
    setLED(240, 243, 1);
    strip.show();
    delay(ipdelay);

    // // Octet 2:
    ClearDisplay();
    numbers(getDigit(int(WiFi.localIP()[1]), 2), 3);
    numbers(getDigit(int(WiFi.localIP()[1]), 1), 2);
    numbers(getDigit(int(WiFi.localIP()[1]), 0), 1);
    setLED(191, 191, 1);
    setLED(0, 7, 1);
    setLED(240, 247, 1);
    strip.show();
    delay(ipdelay);

    // // Octet 3:
    ClearDisplay();
    numbers(getDigit(int(WiFi.localIP()[2]), 2), 3);
    numbers(getDigit(int(WiFi.localIP()[2]), 1), 2);
    numbers(getDigit(int(WiFi.localIP()[2]), 0), 1);
    setLED(191, 191, 1);
    setLED(0, 11, 1);
    setLED(240, 251, 1);
    strip.show();
    delay(ipdelay);

    // // Octet 4:
    ClearDisplay();
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
void buttonRestart(Control* sender, int type, void* param) {
  RestartWordClock();
}


// ###########################################################################################################################################
// # Restart the WordClock:
// ###########################################################################################################################################
void RestartWordClock() {
  updatedevice = false;
  delay(250);
  ResetTextLEDs(strip.Color(0, 255, 0));
  if (changedvalues == true) setFlashValues();  // Write settings to flash
  preferences.end();
  delay(250);
  ESP.restart();
}


// ###########################################################################################################################################
// # GUI: Reset the WiFi settings of the WordClock:
// ###########################################################################################################################################
void buttonWiFiReset(Control* sender, int type, void* param) {
  updatedevice = false;
  Serial.println("Status: WIFI SETTINGS RESET REQUEST");
  ResetTextLEDs(strip.Color(0, 255, 0));
  WiFi.disconnect();  // DISCONNECT FROM WIFI
  delay(1000);
  preferences.putString("WIFIssid", "");                // Reset WiFi SSID
  preferences.putString("WIFIpass", "");                // Reste WiFi password
  preferences.putUInt("useshowip", useshowip_default);  // Show IP-address again
  preferences.end();
  Serial.println("Status: WIFI SETTINGS RESET REQUEST EXECUTED");
  Serial.println("####################################################################################################");
  Serial.println("# WIFI SETTING WERE SET TO DEFAULT... WORDCLOCK WILL NOW RESTART... PLEASE CONFIGURE WIFI AGAIN... #");
  Serial.println("####################################################################################################");
  delay(500);
  ESP.restart();
}


// ###########################################################################################################################################
// # GUI: Reset Extra Words:
// ###########################################################################################################################################
void buttonResetExtraWords(Control* sender, int type, void* param) {
  updatedevice = false;
  delay(1000);
  ew1 = 0;
  ew2 = 0;
  ew3 = 0;
  ew4 = 0;
  ew5 = 0;
  ew6 = 0;
  ew7 = 0;
  ew8 = 0;
  ew9 = 0;
  ew10 = 0;
  ew11 = 0;
  ew12 = 0;
  ResetTextLEDs(strip.Color(0, 255, 0));
  delay(1000);
  changedvalues = true;
  updatedevice = true;
  update_display();
}


// ###########################################################################################################################################
// # GUI: Update the WordClock:
// ###########################################################################################################################################
void buttonUpdate(Control* sender, int type, void* param) {
  preferences.end();
  useWiFiReCon = 0;  // Temporary set "Active WiFi reconnect to off during the update to avoid problems during the update"
  updatedevice = false;
  delay(1000);
  ESPUI.updateButton(sender->id, "Update mode active now - Use the update url: >>>");
  if (updatemode == false) {
    updatemode = true;
    int32_t c = strip.Color(0, 0, 255);
    int TextWait = 250;
    showtext('U', TextWait, c);
    showtext('P', TextWait, c);
    showtext('D', TextWait, c);
    showtext('A', TextWait, c);
    showtext('T', TextWait, c);
    showtext('E', TextWait, c);
    showtext(' ', TextWait, c);
    showtext('U', TextWait, c);
    Serial.println("Status: Update request");
  }
}


// ###########################################################################################################################################
// # Show a LED output for RESET in the different languages:
// ###########################################################################################################################################
void ResetTextLEDs(uint32_t color) {
  updatedevice = false;
  delay(1000);
  ClearDisplay();

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

  if (langLEDlayout == 4) {  // IT:
    setLED(33, 42, 1);       // RICOMINCIA
  }

  if (langLEDlayout == 5) {  // SWE:
    setLED(163, 175, 1);     // ÅTERSTÄLLNING
  }

  if (langLEDlayout == 6) {  // ES:
    setLED(32, 36, 1);       // RESET
  }

  strip.show();
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
// # GUI: Night mode switch:
// ###########################################################################################################################################
void switchNightMode(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      usenightmode = 1;
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
// # GUI: Use WIFi reconnect function during runtime:
// ###########################################################################################################################################
void switchWiFiReConnect(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      useWiFiReCon = 1;
      break;
    case S_INACTIVE:
      useWiFiReCon = 0;
      break;
  }
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Show WordClock text switch:
// ###########################################################################################################################################
void switchStartupText(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      useStartupText = 1;
      break;
    case S_INACTIVE:
      useStartupText = 0;
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
// # GUI: Use Offline Mode  switch:
// ###########################################################################################################################################
void switchOffline(Control* sender, int value) {
  updatedevice = false;
  Serial.println("Offline Mode change: Restart request");
  switch (value) {
    case S_ACTIVE:
      UseOnlineMode = 1;  // Online
      useshowip = useshowip_default;
      break;
    case S_INACTIVE:
      UseOnlineMode = 0;  // Offline
      useshowip = 1;
      break;
  }
  changedvalues = true;
  setFlashValues();  // Save values!
  ClearDisplay();
  ResetTextLEDs(strip.Color(0, 255, 0));
  strip.show();
  Serial.println("Offline Mode change: Perform restart now");
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
// # GUI: Send notifications about messages from unknown users switch:
// ###########################################################################################################################################
void switchTeleShowNote(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      TeleNoteUnknown = 1;  // Show
      break;
    case S_INACTIVE:
      TeleNoteUnknown = 0;  // Hide
      break;
  }
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Show your own Telegram bot token and chat id on the configuration page directly visible switch:
// ###########################################################################################################################################
void switchTeleShowData(Control* sender, int value) {
  updatedevice = false;
  delay(1000);
  switch (value) {
    case S_ACTIVE:
      showTeleData = 1;  // Show
      ESPUI.updateVisibility(TeleBotID, true);
      ESPUI.updateVisibility(TeleChatID, true);
      ESPUI.jsonReload();
      break;
    case S_INACTIVE:
      showTeleData = 0;  // Hide
      ESPUI.updateVisibility(TeleBotID, false);
      ESPUI.updateVisibility(TeleChatID, false);
      break;
  }
  changedvalues = true;
  updatedevice = true;
}


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
  return uVal + lVal;
}


// ###########################################################################################################################################
// # GUI: Color change for time color:
// ###########################################################################################################################################
#define SLIDER_UPDATE_TIME 150  // Wait at least 100 ms before allowing another slider update --> Bug fix for color slider crashing ESP32
void colCallTIME(Control* sender, int type) {
  static unsigned long last_slider_update = 0;  // Track the time of the last slider update
  if ((millis() - last_slider_update >= SLIDER_UPDATE_TIME)) {
    getRGBTIME(sender->value);
    last_slider_update = millis();
    if (debugtexts == 1) {
      Serial.println(type);
      Serial.println(sender->value);
    }
  }
  return;
}


// ###########################################################################################################################################
// # GUI: Color change for background color:
// ###########################################################################################################################################
#define SLIDER_UPDATE_BACK 150  // Wait at least 100 ms before allowing another slider update --> Bug fix for color slider crashing ESP32
void colCallBACK(Control* sender, int type) {
  static unsigned long last_slider_update = 0;  // Track the time of the last slider update
  if ((type == 10) && (millis() - last_slider_update >= SLIDER_UPDATE_BACK)) {
    getRGBBACK(sender->value);
    last_slider_update = millis();
    if (debugtexts == 1) {
      Serial.println(type);
      Serial.println(sender->value);
    }
  }
  return;
}


// ###########################################################################################################################################
// # GUI: Color change for Extra Word ew1 to ew12:
// ###########################################################################################################################################
void colCallew(Control* sender, int type, void* param) {
  updatedevice = false;
  delay(1000);
  sender->value.toUpperCase();

  char c[7];
  sender->value.toCharArray(c, 8);
  int red = hexcolorToInt(c[1], c[2]);
  int green = hexcolorToInt(c[3], c[4]);
  int blue = hexcolorToInt(c[5], c[6]);

  switch ((int)param) {
    case 1:
      {
        redVal_ew1 = red;
        greenVal_ew1 = green;
        blueVal_ew1 = blue;
        break;
      }
    case 2:
      {
        redVal_ew2 = red;
        greenVal_ew2 = green;
        blueVal_ew2 = blue;
        break;
      }
    case 3:
      {
        redVal_ew3 = red;
        greenVal_ew3 = green;
        blueVal_ew3 = blue;
        break;
      }
    case 4:
      {
        redVal_ew4 = red;
        greenVal_ew4 = green;
        blueVal_ew4 = blue;
        break;
      }

    case 5:
      {
        redVal_ew5 = red;
        greenVal_ew5 = green;
        blueVal_ew5 = blue;
        break;
      }
    case 6:
      {
        redVal_ew6 = red;
        greenVal_ew6 = green;
        blueVal_ew6 = blue;
        break;
      }
    case 7:
      {
        redVal_ew7 = red;
        greenVal_ew7 = green;
        blueVal_ew7 = blue;
        break;
      }
    case 8:
      {
        redVal_ew8 = red;
        greenVal_ew8 = green;
        blueVal_ew8 = blue;
        break;
      }
    case 9:
      {
        redVal_ew9 = red;
        greenVal_ew9 = green;
        blueVal_ew9 = blue;
        break;
      }
    case 10:
      {
        redVal_ew10 = red;
        greenVal_ew10 = green;
        blueVal_ew10 = blue;
        break;
      }
    case 11:
      {
        redVal_ew11 = red;
        greenVal_ew11 = green;
        blueVal_ew11 = blue;
        break;
      }
    case 12:
      {
        redVal_ew12 = red;
        greenVal_ew12 = green;
        blueVal_ew12 = blue;
        break;
      }
  }
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
// # GUI: Manual offline hour setting
// ###########################################################################################################################################
void SetOfflineHour(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  iHour = sender->value.toInt();
  rtc.setTime(0, iMinute, iHour, 1, 1, 2024);  // Set time: (ss, mm, hh, DD, MM, YYYY) --> 17th Jan 2021 15:24:30
  updatenow = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Time Day Mode Start
// ###########################################################################################################################################
void call_day_time_start(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  day_time_start = sender->value;
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Time Day Mode Stop
// ###########################################################################################################################################
void call_day_time_stop(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  day_time_stop = sender->value;
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
// # GUI: Show or hide the Operation Mode hints:
// ###########################################################################################################################################
void switchOMhints(Control* sender, int value) {
  updatedevice = false;
  switch (value) {
    case S_ACTIVE:
      showOMhints = 1;  // Show
      ESPUI.updateVisibility(OfflineModeHint1, true);
      ESPUI.updateVisibility(OfflineModeHint2, true);
      ESPUI.updateVisibility(OfflineModeHint3, true);
      ESPUI.jsonReload();
      break;
    case S_INACTIVE:
      showOMhints = 0;  // Hide
      ESPUI.updateVisibility(OfflineModeHint1, false);
      ESPUI.updateVisibility(OfflineModeHint2, false);
      ESPUI.updateVisibility(OfflineModeHint3, false);
      break;
  }
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Manual Offline Mode offset hour setting
// ###########################################################################################################################################
void SetOfflineHourOffset(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  iHourOffset = sender->value.toInt();
  changedvalues = true;
  updatedevice = true;
}


// ###########################################################################################################################################
// # GUI: Time Zone selection:
// ###########################################################################################################################################
void SetMyTimeZone(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  myTimeZone = sender->value;
  changedvalues = true;
  updatedevice = true;
  setFlashValues();  // Save values!
  ResetTextLEDs(strip.Color(0, 255, 0));
  Serial.println("Time Zone change: Perform restart now");
  delay(1000);
  ESP.restart();
}


// ###########################################################################################################################################
// # GUI: Time Server selection:
// ###########################################################################################################################################
void SetMyTimeServer(Control* sender, int type) {
  updatedevice = false;
  delay(1000);
  if (sender->value == "Your local router") {
    myTimeServer = IpAddress2String(WiFi.gatewayIP());
  } else {
    myTimeServer = sender->value;
  }
  changedvalues = true;
  updatedevice = true;
  setFlashValues();  // Save values!
  ResetTextLEDs(strip.Color(0, 255, 0));
  Serial.println("Time Server change: Perform restart now");
  Serial.println("Time Server set to: " + String(sender->value));
  delay(1000);
  ESP.restart();
}


// ###########################################################################################################################################
// # GUI: Manual time setting from device (PC, tablet, smartphone) as long as the Offline Mode web portal is opened:
// ###########################################################################################################################################
void getTimeCallback(Control* sender, int type) {
  if (type == B_UP) {
    ESPUI.updateTime(timeId);
  }
}


// ###########################################################################################################################################
// # GUI: Manual time setting from device (PC, tablet, smartphone):
// ###########################################################################################################################################
void timeCallback(Control* sender, int type) {
  updatedevice = false;
  if (type == TM_VALUE) {
    // Serial.print("Auto Time by device: ");
    // Serial.println(sender->value);
    // ESPUI.print(statusTimeFromDevice, String(sender->value));

    String calTimestamp = String(sender->value);
    struct tm tm;
    // Test String to get all values:
    // Serial.println("Parsing " + calTimestamp);  // 2024-01-04T18:33:37.294Z
    // #######################################################
    // for (int i = 0; i <= 25; i++) {
    //   Serial.print(i);
    //   Serial.print(" = ");
    //   Serial.println(calTimestamp.substring(i).toInt());
    // }
    // #######################################################

    // Date not used yet:
    // String year = calTimestamp.substring(0, 4);
    // String month = calTimestamp.substring(4, 6);
    // if (month.startsWith("0")) {
    //   month = month.substring(1);
    // }
    // String day = calTimestamp.substring(6, 8);
    // if (day.startsWith("0")) {
    //   month = day.substring(1);
    // }
    // tm.tm_year = year.toInt() - 1900;
    // tm.tm_mon = month.toInt() - 1;
    // tm.tm_mday = day.toInt();
    tm.tm_hour = calTimestamp.substring(11).toInt();  // Hour from string
    tm.tm_min = calTimestamp.substring(14).toInt();   // Minute from string
    tm.tm_sec = calTimestamp.substring(17).toInt();   // Second from string
    iHour = tm.tm_hour;
    iMinute = tm.tm_min;
    iSecond = tm.tm_sec;

    // Test a special time:
    if (testspecialtimeOFF == 1) {
      // Serial.println("Special time test active in Offline Mode: " + String(test_hourOFF) + ":" + String(test_minuteOFF) + ":" + String(test_secondOFF));
      iHour = test_hourOFF;
      iMinute = test_minuteOFF;
      iSecond = test_secondOFF;
    }

    ESPUI.print(statusTimeFromDevice, String(iHour) + ":" + String(iMinute) + ":" + String(iSecond));  // Update GUI: Time from device
    if (iHourOffset >= 0) {
      // Serial.println("iHour OLD: " + String(iHour));
      // Serial.println("iHour + Offset: " + String(iHourOffset));  // Set stored hour offset to received hour
      iHour = iHour + iHourOffset;
      // Serial.println("iHour NEW: " + String(iHour));
    }
    if ((iHourOffset <= -1)) {
      // Serial.println("iHour OLD: " + String(iHour));
      // Serial.println("iHour - Offset: " + String(iHourOffset));
      iHour = iHourOffset + iHour;
      // Serial.println("iHour NEW: " + String(iHour));
    }
    int xHour = iHour;
    if (xHour >= 24) xHour = xHour - 24;                                                                 // Mid night corrections because of offset calculation
    if (xHour < 0) xHour = xHour + 24;                                                                   // Mid night corrections because of offset calculation
    ESPUI.print(statusTimeSetOffline, String(xHour) + ":" + String(iMinute) + ":" + String(iSecond));    // Update GUI: Calculated time with offset for negative offset
    if (debugtexts == 1) Serial.println(String(xHour) + ":" + String(iMinute) + ":" + String(iSecond));  // Test output
    rtc.setTime(iSecond, iMinute, xHour, 1, 1, 2024);                                                    // Set time on device RTC: (ss, mm, hh, DD, MM, YYYY) --> Date not used yet
    checkforNightMode();                                                                                 // Night Mode check
    updatenow = true;
    updatedevice = true;
  }
  delay(1000);
}


// ###########################################################################################################################################
// # GUI: Check if Night Mode needs to be set depending on the time:
// ###########################################################################################################################################
void checkforNightMode() {
  // Start time to int:
  String day_time_startH = getValue(day_time_start, ':', 0);
  String day_time_startM = getValue(day_time_start, ':', 1);
  if (debugtexts == 1) Serial.println("day_time_start H part = " + day_time_startH);
  if (debugtexts == 1) Serial.println("day_time_start M part = " + day_time_startM);
  int dt_start_HM = (day_time_startH.toInt() * 100) + day_time_startM.toInt();
  if (debugtexts == 1) Serial.println("dt_start_HM = " + String(dt_start_HM));
  // Stop time to int:
  String day_time_stopH = getValue(day_time_stop, ':', 0);
  String day_time_stopM = getValue(day_time_stop, ':', 1);
  if (debugtexts == 1) Serial.println("day_time_stop H part = " + day_time_stopH);
  if (debugtexts == 1) Serial.println("day_time_stop M part = " + day_time_stopM);
  int dt_stop_HM = (day_time_stopH.toInt() * 100) + day_time_stopM.toInt();
  if (debugtexts == 1) Serial.println("dt_stop_HM = " + String(dt_stop_HM));
  // Current time to int:
  int now_HM = (iHour * 100) + iMinute;
  if (debugtexts == 1) Serial.println("now_HM = " + String(now_HM));

  // Check if start time is before stop time:
  if ((dt_start_HM > dt_stop_HM) || (dt_start_HM == dt_stop_HM)) {
    if (debugtexts == 1) Serial.println("Wrong setting! 'Day time starts' needs to be before 'Day time ends after'. Please correct values.");
    ESPUI.updateVisibility(statusNightModeWarnID, true);  // Show warning
    ESPUI.jsonReload();
  } else {
    ESPUI.updateVisibility(statusNightModeWarnID, false);  // Hide warning
    // Day or Night time:
    if (usenightmode == 1) {
      if ((now_HM >= dt_start_HM) && (now_HM <= dt_stop_HM)) {
        if (debugtexts == 1) Serial.println("Day Time");
        statusNightModeIDtxt = "Day Time";
        ESPUI.print(statusNightModeID, "Day time");
        if ((iHour == 0) && (day_time_startH.toInt() == 23)) {  // Special function if day_time_stop set to 23 and time is 24, so 0...
          statusNightModeIDtxt = "Night time";
          ESPUI.print(statusNightModeID, "Night time");
        }
      } else {
        if (debugtexts == 1) Serial.println("Night Time");
        statusNightModeIDtxt = "Night time";
        ESPUI.print(statusNightModeID, "Night time");
      }
    } else {
      ESPUI.print(statusNightModeID, "Night mode not used");
    }
  }
}


// ###########################################################################################################################################
// # Update the display / time on it: // TODO 2023
// ###########################################################################################################################################
void update_display() {
  if (testTime == 0) {  // Show the current time:
    show_time(iHour, iMinute);
  } else {  // TEST THE DISPLAY TIME OUTPUT:
    strip.setBrightness(33);
    for (int i = 0; i <= 12; i++) {  // 12 hours only to show all hour texts
      show_time(i, 0);
      delay(1000);
    }
    for (int i = 9; i <= 12; i++) {  // Hours 0 to 23 with all hour and minute texts:
      for (int y = 0; y < 60; y = y + 5) {
        show_time(i, y);
        delay(3000);
      }
    }
  }
}


// ###########################################################################################################################################
// # Display hours and minutes text function: // TODO 2023
// ###########################################################################################################################################
void show_time(int hours, int minutes) {
  static int lastHourSet = -1;
  static int lastMinutesSet = -1;
  if (updatenow == false) {
    if (lastHourSet == hours && lastMinutesSet == minutes) {  // Reduce display updates to new minutes and new config updates
      return;
    }
  }
  updatenow = false;
  String currentTime = String(hours) + ":" + String(minutes) + ":" + String(iSecond);
  if (debugtexts == 1) Serial.println("Update LED display now at: " + currentTime);
  lastHourSet = hours;
  lastMinutesSet = minutes;


  // Reset ResetExtraWords at midnight:
  if (((iHour == 0) && (iMinute == 0)) && (ResetExtraWords == true)) {
    ResetExtraWords = false;
    getFlashValues();
    if (debugtexts == 1) Serial.println("ResetExtraWords set to false again... Extra words will be displayed again...");
  }


  // Set LED intensity + DAY/NIGHT MDOE:
  // ##################
  // Start time to int:
  String day_time_startH = getValue(day_time_start, ':', 0);
  String day_time_startM = getValue(day_time_start, ':', 1);
  int dt_start_HM = (day_time_startH.toInt() * 100) + day_time_startM.toInt();
  // Stop time to int:
  String day_time_stopH = getValue(day_time_stop, ':', 0);
  String day_time_stopM = getValue(day_time_stop, ':', 1);
  int dt_stop_HM = (day_time_stopH.toInt() * 100) + day_time_stopM.toInt();
  // Current time to int:
  int now_HM = (iHour * 100) + iMinute;

  // Set intensity:
  if ((usenightmode == 1) && (set_web_intensity == 0)) {
    if ((now_HM >= dt_start_HM) && (now_HM <= dt_stop_HM)) {
      intensity = intensity_day;
      if (statusNightModeIDtxt != "Day time") {
        statusNightModeIDtxt = "Day time";
        ESPUI.print(statusNightModeID, "Day time");
      }
      if ((iHour == 0) && (day_time_startH.toInt() == 23)) {  // Special function if day_time_stop set to 23 and time is 24, so 0...
        intensity = intensity_night;
        if (statusNightModeIDtxt != "Night time") {
          statusNightModeIDtxt = "Night time";
          ESPUI.print(statusNightModeID, "Night time");
        }
      }
    } else {
      intensity = intensity_night;
      if (statusNightModeIDtxt != "Night time") {
        statusNightModeIDtxt = "Night time";
        ESPUI.print(statusNightModeID, "Night time");
      }
    }
  } else {
    if (set_web_intensity == 0) intensity = intensity_day;
    if (set_web_intensity == 1) intensity = intensity_web;
  }
  strip.setBrightness(intensity);

  // Set background color:
  back_color();

  // Display time:
  iHour = hours;
  iMinute = minutes;

  // Test a special time:
  if (testspecialtimeON == 1) {
    // Serial.println("Special time test active in Online Mode: " + String(test_hourON) + ":" + String(test_minuteON) + ":" + String(test_secondON));
    iHour = test_hourON;
    iMinute = test_minuteON;
    iSecond = test_secondON;
  }

  // divide minute by 5 to get value for display control
  int minDiv = iMinute / 5;
  if (usesinglemin == 1) showMinutes(iMinute);


  // Handle extra words:
  set_extra_words();


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

  // ########################################################### IT:
  if (langLEDlayout == 4) {  // IT:

    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0) xHour = 12;
    // At minute 40 hour needs to be counted up:
    if (iMinute >= 40) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }

    // SONO LE:
    if (testPrintTimeTexts == 1) {
      Serial.println("");
      Serial.print(hours);
      Serial.print(":");
      Serial.print(minutes);
    }
    if (xHour > 1) {      // NOTE: Displayed only from 2 to 23
      setLED(48, 51, 1);  // SONO
      setLED(84, 85, 1);  // LE
      if (testPrintTimeTexts == 1) {
        Serial.print(" --> SONO LE ");
      }
    }

    switch (xHour) {
      case 1:
        {
          setLED(94, 94, 1);   // È
          setLED(99, 103, 1);  // L’UNA
          if (testPrintTimeTexts == 1) Serial.print("È L’UNA ");
          break;
        }
      case 2:
        {
          setLED(91, 93, 1);  // DUE
          if (testPrintTimeTexts == 1) Serial.print("DUE ");
          break;
        }
      case 3:
        {
          setLED(96, 98, 1);  // TRE
          if (testPrintTimeTexts == 1) Serial.print("TRE ");
          break;
        }
      case 4:
        {
          setLED(112, 118, 1);  // QUATTRO
          if (testPrintTimeTexts == 1) Serial.print("QUATTRO ");
          break;
        }
      case 5:
        {
          setLED(132, 137, 1);  // CINQUE
          if (testPrintTimeTexts == 1) Serial.print("CINQUE ");
          break;
        }
      case 6:
        {
          setLED(104, 106, 1);  // SEI
          if (testPrintTimeTexts == 1) Serial.print("SEI ");
          break;
        }
      case 7:
        {
          setLED(107, 111, 1);  // SETTE
          if (testPrintTimeTexts == 1) Serial.print("SETTE ");
          break;
        }
      case 8:
        {
          setLED(149, 152, 1);  // OTTO
          if (testPrintTimeTexts == 1) Serial.print("OTTO ");
          break;
        }
      case 9:
        {
          setLED(128, 131, 1);  // NOVE
          if (testPrintTimeTexts == 1) Serial.print("NOVE ");
          break;
        }
      case 10:
        {
          setLED(144, 148, 1);  // DIECI
          if (testPrintTimeTexts == 1) Serial.print("DIECI ");
          break;
        }
      case 11:
        {
          setLED(119, 124, 1);  // UNDICI
          if (testPrintTimeTexts == 1) Serial.print("UNDICI ");
          break;
        }
      case 12:
        {
          setLED(138, 143, 1);  // DODICI
          if (testPrintTimeTexts == 1) Serial.print("DODICI ");
          break;
        }
    }

    // E:
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 5) || (minDiv == 6) || (minDiv == 7)) {
      setLED(162, 162, 1);
      if (testPrintTimeTexts == 1) Serial.print("E ");
    }
    // MENO:
    if ((minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLED(160, 163, 1);
      if (testPrintTimeTexts == 1) Serial.print("MENO ");
    }
    // 5/55: CINQUE
    if ((minDiv == 1) || (minDiv == 11)) {
      setLED(183, 188, 1);
      if (testPrintTimeTexts == 1) Serial.print("CINQUE ");
    }
    // 15/45: UN QUARTO
    if ((minDiv == 3) || (minDiv == 9)) {
      setLED(190, 191, 1);  // UN
      setLED(209, 214, 1);  // QUARTO
      if (testPrintTimeTexts == 1) Serial.print("UN QUARTO ");
    }
    // 10/50: DIECI
    if ((minDiv == 2) || (minDiv == 10)) {
      setLED(192, 196, 1);
      if (testPrintTimeTexts == 1) Serial.print("DIECI ");
    }
    // 20/40: VENTI
    if ((minDiv == 4) || (minDiv == 8)) {
      setLED(234, 238, 1);
      if (testPrintTimeTexts == 1) Serial.print("VENTI ");
    }
    // 25: VENTICINQUE
    if (minDiv == 5) {
      setLED(228, 238, 1);
      if (testPrintTimeTexts == 1) Serial.print("VENTICINQUE ");
    }
    // 30: TRENTA
    if (minDiv == 6) {
      setLED(177, 182, 1);
      if (testPrintTimeTexts == 1) Serial.print("TRENTA ");
    }
    // 35: TRENTACINQUE
    if (minDiv == 7) {
      setLED(177, 188, 1);
      if (testPrintTimeTexts == 1) Serial.print("TRENTACINQUE ");
    }
  }

  // ########################################################### SWE:
  if (langLEDlayout == 5) {  // SWE:

    // KLOCKAN ÄR = ES IST:
    setLED(6, 7, 1);
    setLED(9, 15, 1);
    if (testPrintTimeTexts == 1) {
      Serial.println("");
      Serial.print(hours);
      Serial.print(":");
      Serial.print(minutes);
      Serial.print(" --> KLOCKAN ÄR ");
    }

    // FEM - FÜNF: (Minuten)
    if ((minDiv == 1) || (minDiv == 7) || (minDiv == 11)) {
      setLED(136, 138, 1);
      if (testPrintTimeTexts == 1) Serial.print("FEM ");
    }
    // TIO - ZEHN: (Minuten)
    if ((minDiv == 2) || (minDiv == 10)) {
      setLED(125, 127, 1);
      if (testPrintTimeTexts == 1) Serial.print("TIO ");
    }
    // KVART - VIERTEL:
    if ((minDiv == 3) || (minDiv == 9)) {
      setLED(32, 36, 1);
      if (testPrintTimeTexts == 1) Serial.print("KVART ");
    }
    // TJUGO - ZWANZIG:
    if ((minDiv == 4) || (minDiv == 8)) {
      setLED(139, 143, 1);
      if (testPrintTimeTexts == 1) Serial.print("TJUGO ");
    }
    // TJUGOFEM - FÜNFUNDZWANZIG:
    if (minDiv == 5) {
      setLED(136, 143, 1);
      if (testPrintTimeTexts == 1) Serial.print("TJUGOFEM ");
    }
    // ÖVER - NACH:
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 5) || (minDiv == 7)) {
      setLED(131, 134, 1);
      if (testPrintTimeTexts == 1) Serial.print("ÖVER ");
    }
    // I - VOR:
    if ((minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLED(145, 145, 1);
      if (testPrintTimeTexts == 1) Serial.print("I ");
    }
    // HALV - HALB:
    if ((minDiv == 6) || (minDiv == 7)) {
      setLED(148, 151, 1);
      if (testPrintTimeTexts == 1) Serial.print("HALV ");
    }



    //set hour from 1 to 12 (at noon, or midnight)
    int xHour = (iHour % 12);
    if (xHour == 0)
      xHour = 12;
    // at minute 25 hour needs to be counted up:
    // fuenf vor halb 2 = 13:25
    if (iMinute >= 30) {
      if (xHour == 12)
        xHour = 1;
      else
        xHour++;
    }


    switch (xHour) {
      case 1:
        {
          setLED(208, 202, 1);  // ETT
          if (testPrintTimeTexts == 1) Serial.print("ETT ");
          break;
        }
      case 2:
        {
          setLED(198, 200, 1);  // TVÅ
          if (testPrintTimeTexts == 1) Serial.print("TVÅ ");
          break;
        }
      case 3:
        {
          setLED(160, 162, 1);  // TRE
          if (testPrintTimeTexts == 1) Serial.print("TRE ");
          break;
        }
      case 4:
        {
          setLED(153, 156, 1);  // FYRA
          if (testPrintTimeTexts == 1) Serial.print("FYRA ");
          break;
        }
      case 5:
        {
          setLED(189, 191, 1);  // FEM
          if (testPrintTimeTexts == 1) Serial.print("FEM ");
          break;
        }
      case 6:
        {
          setLED(157, 159, 1);  // SEX
          if (testPrintTimeTexts == 1) Serial.print("SEX ");
          break;
        }
      case 7:
        {
          setLED(179, 181, 1);  // SJU
          if (testPrintTimeTexts == 1) Serial.print("SJU ");
          break;
        }
      case 8:
        {
          setLED(204, 207, 1);  // ÅTTA
          if (testPrintTimeTexts == 1) Serial.print("ÅTTA ");
          break;
        }
      case 9:
        {
          setLED(186, 188, 1);  // NIO
          if (testPrintTimeTexts == 1) Serial.print("NIO ");
          break;
        }
      case 10:
        {
          setLED(176, 178, 1);  // TIO (Stunden)
          if (testPrintTimeTexts == 1) Serial.print("TIO ");
          break;
        }
      case 11:
        {
          setLED(194, 197, 1);  // ELVA
          if (testPrintTimeTexts == 1) Serial.print("ELVA ");
          break;
        }
      case 12:
        {
          setLED(182, 185, 1);  // TOLV
          if (testPrintTimeTexts == 1) Serial.print("TOLV ");
          break;
        }
    }
  }


  // ########################################################### ES:
  if (langLEDlayout == 6) {  // ES:

    // SON LA at 1 or 13 or SON LAS for all other hours:
    if ((iHour == 1) || (iHour == 13)) {
      setLED(0, 2, 1);
      setLED(16, 17, 1);
      if (testPrintTimeTexts == 1) {
        Serial.println("");
        Serial.print(hours);
        Serial.print(":");
        Serial.print(minutes);
        Serial.print(" --> SON LA ");
      }
    } else {
      setLED(0, 2, 1);
      setLED(16, 18, 1);
      if (testPrintTimeTexts == 1) {
        Serial.println("");
        Serial.print(hours);
        Serial.print(":");
        Serial.print(minutes);
        Serial.print(" --> SON LAS ");
      }
    }

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

    switch (xHour) {  // Hour numbers in 12 hour format:
      case 1:
        {
          setLED(35, 37, 1);  // UNA / 1
          if (testPrintTimeTexts == 1) Serial.print("UNA ");
          break;
        }
      case 2:
        {
          setLED(29, 31, 1);  // DOS / 2
          if (testPrintTimeTexts == 1) Serial.print("DOS ");
          break;
        }
      case 3:
        {
          setLED(102, 105, 1);  // TRES / 3
          if (testPrintTimeTexts == 1) Serial.print("TRES ");
          break;
        }
      case 4:
        {
          setLED(80, 85, 1);  // CUATRO / 4
          if (testPrintTimeTexts == 1) Serial.print("CUATRO ");
          break;
        }
      case 5:
        {
          setLED(96, 100, 1);  // CINCO / 5
          if (testPrintTimeTexts == 1) Serial.print("CINCO ");
          break;
        }
      case 6:
        {
          setLED(67, 70, 1);  // SEIS / 6
          if (testPrintTimeTexts == 1) Serial.print("SEIS ");
          break;
        }
      case 7:
        {
          setLED(139, 143, 1);  // SIETE / 7
          if (testPrintTimeTexts == 1) Serial.print("SIETE ");
          break;
        }
      case 8:
        {
          setLED(43, 46, 1);  // OCHO / 8
          if (testPrintTimeTexts == 1) Serial.print("OCHO ");
          break;
        }
      case 9:
        {
          setLED(107, 111, 1);  // NUEVE / 9
          if (testPrintTimeTexts == 1) Serial.print("NUEVE ");
          break;
        }
      case 10:
        {
          setLED(112, 115, 1);  // DIEZ / 10
          if (testPrintTimeTexts == 1) Serial.print("DIEZ ");
          break;
        }
      case 11:
        {
          setLED(40, 43, 1);  // ONCE / 11
          if (testPrintTimeTexts == 1) Serial.print("ONCE ");
          break;
        }
      case 12:
        {
          setLED(116, 119, 1);  // DOCE / 12
          if (testPrintTimeTexts == 1) Serial.print("DOCE ");
          break;
        }
    }

    if (iMinute < 5) {
      setLED(145, 146, 1);   // EN
      setLED(197, 201, 1);  // PUNCTO
      if (testPrintTimeTexts == 1) Serial.print("EN PUNCTO ");
    }


    // Y / PAST: // x:05 + x:10 + x:15 + x:20 + x:25 + x:30
    if ((minDiv == 1) || (minDiv == 2) || (minDiv == 3) || (minDiv == 4) || (minDiv == 5) || (minDiv == 6)) {
      setLED(128, 128, 1);
      if (testPrintTimeTexts == 1) Serial.print("Y ");
    }

    // MENOS / TO: // x:35 + x:40 + x:45 + x:50 + x:55
    if ((minDiv == 7) || (minDiv == 8) || (minDiv == 9) || (minDiv == 10) || (minDiv == 11)) {
      setLED(144, 148, 1);
      if (testPrintTimeTexts == 1) Serial.print("MENOS ");
    }

    // VEINTE / TWENTY: // x:20 + x:40
    if ((minDiv == 4) || (minDiv == 8)) {
      setLED(160, 165, 1);
      if (testPrintTimeTexts == 1) Serial.print("VEINTE");
    }

    // CINCO / FIVE: (Minutes) // x:05 + x:55
    if ((minDiv == 1) || (minDiv == 11)) {
      setLED(228, 232, 1);
      if (testPrintTimeTexts == 1) Serial.print("CINCO");
    }

    // VEINTICINCO / TWENTYFIVE: (Minutes) // x:25 + x:35
    if ((minDiv == 5) || (minDiv == 7)) {
      setLED(228, 238, 1);
      if (testPrintTimeTexts == 1) Serial.print("VEINTICINCO");
    }

    // DIEZ / TEN: (Minutes) // x:10 + x:50
    if ((minDiv == 2) || (minDiv == 10)) {
      setLED(219, 222, 1);
      if (testPrintTimeTexts == 1) Serial.print("DIEZ ");
    }

    // CUARTO / QUARTER: // x:15 + X:45
    if ((minDiv == 3) || (minDiv == 9)) {
      setLED(202, 207, 1);
      if (testPrintTimeTexts == 1) Serial.print("CUARTO ");
    }

    // MEDIA / HALF: // x:30
    if ((minDiv == 6)) {
      setLED(192, 196, 1);
      if (testPrintTimeTexts == 1) Serial.print("MEDIA ");
    }
  }

  // Display time output:
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

  // ##################################################### IT:
  if (langLEDlayout == 4) {  // IT:
    switch (minMod) {
      case 1:
        {
          setLED(241, 241, 1);  // +
          setLED(243, 243, 1);  // 1
          setLED(248, 253, 1);  // MINUTI
          break;
        }
      case 2:
        {
          setLED(241, 241, 1);  // +
          setLED(244, 244, 1);  // 2
          setLED(248, 253, 1);  // MINUTI
          break;
        }
      case 3:
        {
          setLED(241, 241, 1);  // +
          setLED(245, 245, 1);  // 3
          setLED(248, 253, 1);  // MINUTI
          break;
        }
      case 4:
        {
          setLED(241, 241, 1);  // +
          setLED(246, 246, 1);  // 4
          setLED(248, 253, 1);  // MINUTI
          break;
        }
    }
  }

  // ##################################################### SWE:
  if (langLEDlayout == 5) {  // SWE:
    switch (minMod) {
      case 1:
        {
          setLED(241, 241, 1);  // +
          setLED(243, 243, 1);  // 1
          setLED(247, 251, 1);  // MINUT
          break;
        }
      case 2:
        {
          setLED(241, 241, 1);  // +
          setLED(244, 244, 1);  // 2
          setLED(247, 253, 1);  // MINUTER
          break;
        }
      case 3:
        {
          setLED(241, 241, 1);  // +
          setLED(245, 245, 1);  // 3
          setLED(247, 253, 1);  // MINUTER
          break;
        }
      case 4:
        {
          setLED(241, 241, 1);  // +
          setLED(246, 246, 1);  // 4
          setLED(247, 253, 1);  // MINUTER
          break;
        }
    }
  }

  // ##################################################### ES:
  if (langLEDlayout == 6) {  // ES:
    switch (minMod) {
      case 1:
        {
          setLED(240, 240, 1);  // +
          setLED(242, 242, 1);  // 1
          setLED(248, 253, 1);  // MINUTO
          break;
        }
      case 2:
        {
          setLED(240, 240, 1);  // +
          setLED(243, 243, 1);  // 2
          setLED(248, 254, 1);  // MINUTOS
          break;
        }
      case 3:
        {
          setLED(240, 240, 1);  // +
          setLED(244, 244, 1);  // 3
          setLED(248, 254, 1);  // MINUTOS
          break;
        }
      case 4:
        {
          setLED(240, 240, 1);  // +
          setLED(245, 245, 1);  // 4
          setLED(248, 254, 1);  // MINUTOS
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


  if (langLEDlayout == 4) {  // IT:
    strip.setPixelColor(43, c1);
    strip.setPixelColor(44, c1);
    strip.setPixelColor(45, c1);
    strip.setPixelColor(46, c1);
    for (uint16_t i = 215; i < 224; i++) {
      strip.setPixelColor(i, c1);
    }
  }


  if (langLEDlayout == 5) {  // SWE:
    strip.setPixelColor(107, c1);
    strip.setPixelColor(108, c1);
    strip.setPixelColor(109, c1);
    strip.setPixelColor(110, c1);
    for (uint16_t i = 116; i < 125; i++) {
      strip.setPixelColor(i, c1);
    }
  }


  if (langLEDlayout == 6) {  // ES:
    for (uint16_t i = 52; i < 56; i++) {
      strip.setPixelColor(i, c1);
    }
    for (uint16_t i = 86; i < 95; i++) {
      strip.setPixelColor(i, c1);
    }
  }


  strip.show();
  delay(3000);

  if (UseOnlineMode == 1) SetWLAN(strip.Color(0, 0, 255));  // Show WLAN text
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
  delay(1000);
}
// ###########################################################################################################################################
void initTime(String timezone) {
  Serial.println("Setting up time from: " + NTPserver);

  // Flash TIME LEDs 3x blue:
  for (int i = 0; i < 3; i++) {
    ClearDisplay();
    strip.show();
    delay(500);

    if (langLEDlayout == 0) {  // DE:
      setLEDexCol(84, 87, 1, 0, 0, 255);
    }
    if (langLEDlayout == 1) {  // EN:
      setLEDexCol(28, 31, 1, 0, 0, 255);
    }
    if (langLEDlayout == 2) {  // NL:
      setLEDexCol(76, 79, 1, 0, 0, 255);
    }
    if (langLEDlayout == 3) {  // FR:
      setLEDexCol(235, 239, 1, 0, 0, 255);
    }
    if (langLEDlayout == 4) {  // IT:
      setLEDexCol(75, 79, 1, 0, 0, 255);
    }
    if (langLEDlayout == 5) {  // SWE:
      setLEDexCol(61, 63, 1, 0, 0, 255);
    }
    if (langLEDlayout == 6) {  // ES:
      setLEDexCol(120, 123, 1, 0, 0, 255);
    }
    strip.show();
    delay(500);
  }
  ClearDisplay();

  struct tm timeinfo;
  configTime(0, 0, NTPserver.c_str());
  delay(500);

  while (!getLocalTime(&timeinfo)) {
    ClearDisplay();
    strip.show();
    delay(250);

    // Time server could not be reached --> show red TIME texts:
    if (langLEDlayout == 0) {  // DE:
      setLEDexCol(84, 87, 1, 255, 0, 0);
    }
    if (langLEDlayout == 1) {  // EN:
      setLEDexCol(28, 31, 1, 255, 0, 0);
    }
    if (langLEDlayout == 2) {  // NL:
      setLEDexCol(76, 79, 1, 255, 0, 0);
    }
    if (langLEDlayout == 3) {  // FR:
      setLEDexCol(235, 239, 1, 255, 0, 0);
    }
    if (langLEDlayout == 4) {  // IT:
      setLEDexCol(75, 79, 1, 255, 0, 0);
    }
    if (langLEDlayout == 5) {  // SWE:
      setLEDexCol(61, 63, 1, 255, 0, 0);
    }
    if (langLEDlayout == 6) {  // ES:
      setLEDexCol(120, 123, 1, 255, 0, 0);
    }
    strip.show();
    delay(250);
    ClearDisplay();
    delay(250);
    ResetTextLEDs(strip.Color(0, 255, 0));
    Serial.println("! Failed to obtain time - Time server could not be reached ! --> RESTART THE DEVICE NOW...");
    ESP.restart();
  }

  // Time successfully received --> green TIME text:
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
  if (langLEDlayout == 4) {  // IT:
    setLEDexCol(75, 79, 1, 0, 255, 0);
  }
  if (langLEDlayout == 5) {  // SWE:
    setLEDexCol(61, 63, 1, 0, 255, 0);
  }
  if (langLEDlayout == 6) {  // ES:
    setLEDexCol(120, 123, 1, 0, 255, 0);
  }
  strip.show();
  delay(1000);
  Serial.println("Got the time from NTP server: " + NTPserver);
  setTimezone(timezone);
}
// ###########################################################################################################################################
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return;
  }
  // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
  char timeStringBuff[50];
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  iStartTime = String(timeStringBuff);
  iHour = timeinfo.tm_hour;
  iMinute = timeinfo.tm_min;
  iSecond = timeinfo.tm_sec;
  if (ShowTimeEachSecond == 1) {
    Serial.print("Current time: ");
    Serial.print(iHour);
    Serial.print(":");
    Serial.print(iMinute);
    Serial.print(":");
    Serial.println(iSecond);
  }
  delay(1000);
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
    // You will be motified in case an unknown sender id tries to control the WordClock if the notification is set active.
    if (useTelegramID == 1) {
      if (String(CHAT_ID) != chat_id) {
        // if (String("123") != chat_id) {  // TEST WITH UNKNOWN OWN FAKE ID 123
        Serial.println(String("Message received from unknown user: " + chat_id + " with text: " + text + " ==> Your WordClock will not react to this message"));
        if (TeleNoteUnknown == 1) bot.sendMessage(CHAT_ID, "Message received from unknown Telegram user: " + chat_id + "\xE2\x9D\x97\nMessage text: " + text + "\nYour WordClock will not react to this message " + "\xF0\x9F\x98\x8F");
        break;
      }
    }


    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    //##################################################################
    // Send respond messages:
    //##################################################################
    if (text == "/ew1") {
      if (ew1 == 0) {
        ew1 = 1;
        bot.sendMessage(chat_id, ewtext1 + " text set active " + emoActive);
      } else {
        ew1 = 0;
        bot.sendMessage(chat_id, ewtext1 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew2") {
      if (ew2 == 0) {
        ew2 = 1;
        bot.sendMessage(chat_id, ewtext2 + "text set active " + emoActive);
      } else {
        ew2 = 0;
        bot.sendMessage(chat_id, ewtext2 + "text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew3") {
      if (ew3 == 0) {
        ew3 = 1;
        bot.sendMessage(chat_id, ewtext3 + " text set active " + emoActive);
      } else {
        ew3 = 0;
        bot.sendMessage(chat_id, ewtext3 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew4") {
      if (ew4 == 0) {
        ew4 = 1;
        bot.sendMessage(chat_id, ewtext4 + " text set active " + emoActive);
      } else {
        ew4 = 0;
        bot.sendMessage(chat_id, ewtext4 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew5") {
      if (ew5 == 0) {
        ew5 = 1;
        bot.sendMessage(chat_id, ewtext5 + " text set active " + emoActive);
      } else {
        ew5 = 0;
        bot.sendMessage(chat_id, ewtext5 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew6") {
      if (ew6 == 0) {
        ew6 = 1;
        bot.sendMessage(chat_id, ewtext6 + " text set active " + emoActive);
      } else {
        ew6 = 0;
        bot.sendMessage(chat_id, ewtext6 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew7") {
      if (ew7 == 0) {
        ew7 = 1;
        bot.sendMessage(chat_id, ewtext7 + " text set active " + emoActive);
      } else {
        ew7 = 0;
        bot.sendMessage(chat_id, ewtext7 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew8") {
      if (ew8 == 0) {
        ew8 = 1;
        bot.sendMessage(chat_id, ewtext8 + " text set active " + emoActive);
      } else {
        ew8 = 0;
        bot.sendMessage(chat_id, ewtext8 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew9") {
      if (ew9 == 0) {
        ew9 = 1;
        bot.sendMessage(chat_id, ewtext9 + " text set active " + emoActive);
      } else {
        ew9 = 0;
        bot.sendMessage(chat_id, ewtext9 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew10") {
      if (ew10 == 0) {
        ew10 = 1;
        bot.sendMessage(chat_id, ewtext10 + " text set active " + emoActive);
      } else {
        ew10 = 0;
        bot.sendMessage(chat_id, ewtext10 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew11") {
      if (ew11 == 0) {
        ew11 = 1;
        bot.sendMessage(chat_id, ewtext11 + " text set active " + emoActive);
      } else {
        ew11 = 0;
        bot.sendMessage(chat_id, ewtext11 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (text == "/ew12") {
      if (ew12 == 0) {
        ew12 = 1;
        bot.sendMessage(chat_id, ewtext12 + " text set active " + emoActive);
      } else {
        ew12 = 0;
        bot.sendMessage(chat_id, ewtext12 + " text set inactive " + emoInactive);
      }
      changedvalues = true;
    }

    if (langLEDlayout == 0) {  // DE has 12 extra words:
      if (text == "/start") {
        String welcome = "Welcome to WordClock Telegram bot, " + from_name + ". " + "\xF0\x9F\x98\x8A" + "\n";
        welcome += "Use /ew1, /ew2, /ew3, /ew4, /ew5, /ew6, /ew7, /ew8, /ew9, /ew10, /ew11, /ew12 or the menu to set the extra words.\n\n";
        bot.sendMessage(chat_id, welcome);
      }
    }

    if ((langLEDlayout == 1) || (langLEDlayout == 2) || (langLEDlayout == 3)) {
      if (text == "/start") {  // All others except IT have 9 extra words:
        String welcome = "Welcome to WordClock Telegram bot, " + from_name + ". " + "\xF0\x9F\x98\x8A" + "\n";
        welcome += "Use /ew1, /ew2, /ew3, /ew4, /ew5, /ew6, /ew7, /ew8, /ew9 or the menu to set the extra words.\n\n";
        bot.sendMessage(chat_id, welcome);
      }
    }

    if (langLEDlayout == 4) {
      if (text == "/start") {  // IT has 8 extra words only:
        String welcome = "Welcome to WordClock Telegram bot, " + from_name + ". " + "\xF0\x9F\x98\x8A" + "\n";
        welcome += "Use /ew1, /ew2, /ew3, /ew4, /ew5, /ew6, /ew7, /ew8 or the menu to set the extra words.\n\n";
        bot.sendMessage(chat_id, welcome);
      }
    }
    //##################################################################
  }
  updatedevice = true;
}


// ###########################################################################################################################################
// # Set extra words:
// ###########################################################################################################################################
void set_extra_words() {
  // Reset extra words to off until 00:00:00:
  if (ResetExtraWords == true) {
    ew1 = 0;
    ew2 = 0;
    ew3 = 0;
    ew4 = 0;
    ew5 = 0;
    ew6 = 0;
    ew7 = 0;
    ew8 = 0;
    ew9 = 0;
    ew10 = 0;
    ew11 = 0;
    ew12 = 0;
  }

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

  // ########################################################### IT:
  if (langLEDlayout == 4) {  // IT: 8 extra words only!
    if (ew1 == 1) {
      setLEDexCol(16, 20, 1, redVal_ew1, greenVal_ew1, blueVal_ew1);    // ew1 - VIENI QUI = COME HERE text
      setLEDexCol(125, 127, 1, redVal_ew1, greenVal_ew1, blueVal_ew1);  // ew1 - VIENI QUI = COME HERE text
    } else {
      setLED(16, 20, 0);    // ew1
      setLED(125, 127, 0);  // ew1
    }

    if (ew2 == 1) {
      setLEDexCol(1, 3, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);      // ew2 - ORA DI PRANZO - LUNCH TIME text
      setLEDexCol(144, 145, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);  // ew2 - ORA DI PRANZO - LUNCH TIME text
      setLEDexCol(153, 158, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);  // ew2 - ORA DI PRANZO - LUNCH TIME text
    } else {
      setLED(1, 3, 0);      // ew2
      setLED(144, 145, 0);  // ew2
      setLED(153, 158, 0);  // ew2
    }

    if (ew3 == 1) {
      setLEDexCol(52, 58, 1, redVal_ew3, greenVal_ew3, blueVal_ew3);  // ew3 - ALLARME - ALARM text

    } else {
      setLED(52, 58, 0);  // ew3
    }

    if (ew4 == 1) {
      setLEDexCol(21, 25, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);  // ew4 - VACANZA - HOLIDAY text
    } else {
      setLED(21, 25, 0);  // ew4
    }

    if (ew5 == 1) {
      setLEDexCol(197, 207, 1, redVal_ew5, greenVal_ew5, blueVal_ew5);  // ew5 - TEMPERATURA - TEMPERATURE text
    } else {
      setLED(197, 207, 0);  // ew5
    }

    if (ew6 == 1) {
      setLEDexCol(59, 62, 1, redVal_ew6, greenVal_ew6, blueVal_ew6);  // ew6 - DATA - DATE text
    } else {
      setLED(59, 62, 0);  // ew6
    }

    if (ew7 == 1) {
      setLEDexCol(80, 89, 1, redVal_ew7, greenVal_ew7, blueVal_ew7);  // ew7 - COMPLEANNO - BIRTHDAY text
    } else {
      setLED(80, 89, 0);  // ew7
    }

    if (ew8 == 1) {
      setLEDexCol(65, 74, 1, redVal_ew8, greenVal_ew8, blueVal_ew8);  // ew8 - CAMPANELLO - DOORBELL text
    } else {
      setLED(65, 74, 0);  // ew8
    }
  }

  // ########################################################### SWE:
  if (langLEDlayout == 5) {  // SWE:
    if (ew1 == 1) {
      setLEDexCol(38, 47, 1, redVal_ew1, greenVal_ew1, blueVal_ew1);  // ew1 - FÖDELSEDAG text
    } else {
      setLED(38, 47, 0);  // ew1 - FÖDELSEDAG text
    }

    if (ew2 == 1) {
      setLEDexCol(52, 55, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);  // ew2 - LARM text
    } else {
      setLED(52, 55, 0);  // ew2 - LARM text
    }

    if (ew3 == 1) {
      setLEDexCol(90, 95, 1, redVal_ew3, greenVal_ew3, blueVal_ew3);  // ew3 - HÖGTID text
    } else {
      setLED(90, 95, 0);  // ew3 - HÖGTID text
    }

    if (ew4 == 1) {
      setLEDexCol(65, 76, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);  // ew4 - SEMESTER text
    } else {
      setLED(65, 76, 0);  // ew4 - SEMESTER text
    }

    if (ew5 == 1) {
      setLEDexCol(0, 4, 1, redVal_ew5, greenVal_ew5, blueVal_ew5);    // ew5 - LADDA  text
      setLEDexCol(26, 28, 1, redVal_ew5, greenVal_ew5, blueVal_ew5);  // ew5 - NER text
    } else {
      setLED(0, 4, 0);    // ew5 - LADDA text
      setLED(26, 28, 0);  // ew5 - NER text
    }

    if (ew6 == 1) {
      setLEDexCol(56, 63, 1, redVal_ew6, greenVal_ew6, blueVal_ew6);  // ew6 - LUNCHTID text
    } else {
      setLED(56, 63, 0);  // ew6 - LUNCHTID text
    }

    if (ew7 == 1) {
      setLEDexCol(77, 79, 1, redVal_ew7, greenVal_ew7, blueVal_ew7);    // ew7 - KOM text
      setLEDexCol(128, 130, 1, redVal_ew7, greenVal_ew7, blueVal_ew7);  // ew7 - HIT text
    } else {
      setLED(77, 79, 0);    // ew7 - KOM text
      setLED(128, 130, 0);  // ew7 - HIT text
    }

    if (ew8 == 1) {
      setLEDexCol(80, 89, 1, redVal_ew8, greenVal_ew8, blueVal_ew8);  // ew8 - DÖRRKLOCKA text
    } else {
      setLED(80, 89, 0);  // ew8 - DÖRRKLOCKA text
    }

    if (ew9 == 1) {
      setLEDexCol(16, 25, 1, redVal_ew9, greenVal_ew9, blueVal_ew9);  // ew9 - TEMPERATUR text
    } else {
      setLED(16, 25, 0);  // ew9 - TEMPERATUR text
    }
  }

  // ########################################################### ES:
  if (langLEDlayout == 6) {  // ES:
    if (ew1 == 1) {
      setLEDexCol(182, 191, 1, redVal_ew1, greenVal_ew1, blueVal_ew1);  // ew1 - CUMPLEAÑOS text
    } else {
      setLED(182, 191, 0);  // ew1 - CUMPLEAÑOS text
    }

    if (ew2 == 1) {
      setLEDexCol(57, 62, 1, redVal_ew2, greenVal_ew2, blueVal_ew2);  // ew2 - ALARMA text
    } else {
      setLED(57, 62, 0);  // ew2 - ALARMA text
    }

    if (ew3 == 1) {
      setLEDexCol(6, 15, 1, redVal_ew3, greenVal_ew3, blueVal_ew3);  // ew3 - VACACIONES text
    } else {
      setLED(6, 15, 0);  // ew3 - VACACIONES text
    }

    if (ew4 == 1) {
      setLEDexCol(64, 66, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);    // ew4 - DÍA DE BASURA text
      setLEDexCol(173, 174, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);  // ew4 - DÍA DE BASURA text
      setLEDexCol(176, 181, 1, redVal_ew4, greenVal_ew4, blueVal_ew4);  // ew4 - DÍA DE BASURA text
    } else {
      setLED(64, 66, 0);    // ew4 - DÍA DE BASURA text
      setLED(173, 174, 0);  // ew4 - DÍA DE BASURA text
      setLED(176, 181, 0);  // ew4 - DÍA DE BASURA text
    }

    if (ew5 == 1) {
      setLEDexCol(48, 52, 1, redVal_ew5, greenVal_ew5, blueVal_ew5);  // ew5 - FECHA  text
    } else {
      setLED(48, 52, 0);  // ew5 - FECHA text
    }

    if (ew6 == 1) {
      setLEDexCol(120, 123, 1, redVal_ew6, greenVal_ew6, blueVal_ew6);  // ew6 - HORA DE ALMUERZO text
      setLEDexCol(173, 174, 1, redVal_ew6, greenVal_ew6, blueVal_ew6);  // ew6 - HORA DE ALMUERZO text
      setLEDexCol(209, 216, 1, redVal_ew6, greenVal_ew6, blueVal_ew6);  // ew6 - HORA DE ALMUERZO text
    } else {
      setLED(120, 123, 0);  // ew6 - HORA DE ALMUERZO text
      setLED(173, 174, 0);  // ew6 - HORA DE ALMUERZO text
      setLED(209, 216, 0);  // ew6 - HORA DE ALMUERZO text
    }

    if (ew7 == 1) {
      setLEDexCol(3, 5, 1, redVal_ew7, greenVal_ew7, blueVal_ew7);    // ew7 - VEN AQUÍ
      setLEDexCol(19, 22, 1, redVal_ew7, greenVal_ew7, blueVal_ew7);  // ew7 - VEN AQUÍ
    } else {
      setLED(3, 5, 0);    // ew7 - VEN AQUÍ
      setLED(19, 22, 0);  // ew7 - VEN AQUÍ
    }

    if (ew8 == 1) {
      setLEDexCol(23, 28, 1, redVal_ew8, greenVal_ew8, blueVal_ew8);  // ew8 - TIMBRE
    } else {
      setLED(23, 28, 0);  // ew8 - TIMBRE
    }

    if (ew9 == 1) {
      setLEDexCol(149, 159, 1, redVal_ew9, greenVal_ew9, blueVal_ew9);  // ew9 - TEMPERATURA
    } else {
      setLED(149, 159, 0);  // ew9 - TEMPERATURA
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
// # Startup LED test function
// ###########################################################################################################################################
void callStartText() {
  Serial.println("Show 'WORDCLOCK' startup text...");
  uint32_t c = strip.Color(redVal_time, greenVal_time, blueVal_time);
  int TextWait = 500;
  showtext('W', TextWait, c);
  showtext('O', TextWait, c);
  showtext('R', TextWait, c);
  showtext('D', TextWait, c);
  showtext('C', TextWait, c);
  showtext('L', TextWait, c);
  showtext('O', TextWait, c);
  showtext('C', TextWait, c);
  showtext('K', TextWait, c);
}


// ###########################################################################################################################################
// # LED web update server:
// ###########################################################################################################################################
int ew = 0;                                                         // Current extra word
void handleLEDupdate() {                                            // LED server pages urls:
  ledserver.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {  // Show a manual how to use these links:
    String message = "WordClock web configurations and querry options examples:\n\n";
    message = message + "General:\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023 --> Shows this text\n\n";
    message = message + "Get the status of the WordClock at a glance:\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/status --> Show the status of all extra words and the color for the background and time texts\n\n";
    message = message + "Configure the WordClock colors for background and time texts:\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/config?R-Time=0&G-Time=0&B-Time=255&R-Back=255&G-Back=0&B-Back=0&INTENSITY=25&INTENSITYviaWEB=1 --> Configures time texts to blue and the background to red. Intensity is set to 25 and sets this intensity value as master setting\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/config?R-Time=0&G-Time=0&B-Time=255 --> Configures time texts to blue\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/config?R-Back=255&G-Back=0&B-Back=0&INTENSITY=5&INTENSITYviaWEB=1 --> Configures the background to red. Intensity is set to 5 and sets this intensity value as master setting\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/config?INTENSITY=0&INTENSITYviaWEB=1 --> LED intensity is set to 0 and sets this intensity value as master setting, which will turn the display off...\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/config?INTENSITY=25&INTENSITYviaWEB=0 --> LED intensity is set to 25 and sets this intensity value as NOT master setting anymore, which will switch back to the in the web configuration set intensity parameters...\n";
    message = message + "NOTE: The option INTENSITY has to be used together with the option INTENSITYviaWEB. This will ensure, that the automatic Day/Night-mode and the intensity setting that can be set in the internal WordClock web portal is not used at the same time and get disabled.\n\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/intensity --> Show the status of the value INTENSITYviaWEB parameter\n\n";
    message = message + "Set extra words:\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ew/?ew1=1 --> Turns extra word 1 to on\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ew/?ew1=0 --> Turns extra word 1 to off\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ew/?ew9=1 --> Turns extra word 9 to on\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ew/?ew9=0 --> Turns extra word 9 to off\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ew/?ew3=1&R=0&G=0&B=255 --> Turns extra word 3 on and sets it to blue color\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ew/?ew3=0 --> Turns extra word 3 to off\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ew/?ew1=1&ew2=1&ew3=1&ew4=1&ew5=1&ew6=1&ew7=1&ew8=1&ew9=1&ew10=1&ew11=1&ew12=1 --> Turns ALL extra words to on\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ew/?ew1=0&ew2=0&ew3=0&ew4=0&ew5=0&ew6=0&ew7=0&ew8=0&ew9=0&ew10=0&ew11=0&ew12=0 --> Turns ALL extra words to off\n\n";
    message = message + "Get status of a single extra word:\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ewstatus/?1 --> Status of extra word 1\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ewstatus/?3 --> Status of extra word 3\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/ewstatus/?9 --> Status of extra word 9\n\n";
    message = message + "Set all extra words off until midnight:\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/resetew1 --> Set all extra words off until midnight\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/resetew0 --> Set all extra words outputs active again\n";
    message = message + "http://" + IpAddress2String(WiFi.localIP()) + ":2023/resetewstatus --> Get the status of this switch\n";
    request->send(200, "text/plain", message);
  });

  ledserver.on("/config", HTTP_GET, [](AsyncWebServerRequest* request) {  // Configure background and time texts color and intensity:
    int paramsNr = request->params();
    // Serial.println(paramsNr);
    for (int i = 0; i < paramsNr; i++) {
      const AsyncWebParameter* p = request->getParam(i);
      // Serial.print("Param name: ");
      // Serial.println(p->name());
      // Serial.print("Param value: ");
      // Serial.println(p->value());
      // Serial.println("------------------");
      if ((p->value().toInt() >= 0) && (p->value().toInt() <= 255)) {
        if (String(p->name()) == "R-Time") redVal_time = p->value().toInt();
        if (String(p->name()) == "G-Time") greenVal_time = p->value().toInt();
        if (String(p->name()) == "B-Time") blueVal_time = p->value().toInt();
        if (String(p->name()) == "R-Back") redVal_back = p->value().toInt();
        if (String(p->name()) == "G-Back") greenVal_back = p->value().toInt();
        if (String(p->name()) == "B-Back") blueVal_back = p->value().toInt();
        if ((String(p->name()) == "INTENSITYviaWEB") && (p->value().toInt() == 1)) {
          set_web_intensity = 1;
          ESPUI.updateVisibility(intensity_web_HintID, true);
          ESPUI.updateVisibility(statusNightModeID, false);
          ESPUI.updateVisibility(sliderBrightnessDayID, false);
          ESPUI.updateVisibility(switchNightModeID, false);
          ESPUI.updateVisibility(sliderBrightnessNightID, false);
          ESPUI.updateVisibility(call_day_time_startID, false);
          ESPUI.updateVisibility(call_day_time_stopID, false);
          ESPUI.updateVisibility(text_colour_time, false);
          ESPUI.updateVisibility(text_colour_background, false);
        }
        if ((String(p->name()) == "INTENSITYviaWEB") && (p->value().toInt() == 0)) {
          set_web_intensity = 0;
          ESPUI.updateVisibility(intensity_web_HintID, false);
          ESPUI.updateVisibility(statusNightModeID, true);
          ESPUI.updateVisibility(sliderBrightnessDayID, true);
          ESPUI.updateVisibility(switchNightModeID, true);
          ESPUI.updateVisibility(sliderBrightnessNightID, true);
          ESPUI.updateVisibility(call_day_time_startID, true);
          ESPUI.updateVisibility(call_day_time_stopID, true);
          ESPUI.updateVisibility(text_colour_time, true);
          ESPUI.updateVisibility(text_colour_background, true);
        }
        if ((String(p->name()) == "INTENSITY") && (p->value().toInt() > LEDintensityLIMIT)) {
          request->send(200, "text/plain", "INVALID INTENSITY VALUE - MAXIMUM VALUE=" + String(LEDintensityLIMIT));
        } else {
          if (String(p->name()) == "INTENSITY") intensity_web = p->value().toInt();
        }
        changedvalues = true;
        updatenow = true;
      } else {
        request->send(200, "text/plain", "INVALID VALUES - MUST BE BETWEEN 0 and 255");
      }
    }
    request->send(200, "text/plain", "WordClock config received");
  });

  ledserver.on("/status", HTTP_GET, [](AsyncWebServerRequest* request) {  // Show the status of all extra words and the color for the background and time texts:
    String message = "R-Time=" + String(redVal_time) + " G-Time=" + String(greenVal_time) + " B-Time=" + String(blueVal_time) + " R-Back=" + String(redVal_back) + " G-Back=" + String(greenVal_back) + " B-Back=" + String(blueVal_back) + " INTENSITY=" + String(intensity);
    message = message + " ew1=" + String(ew1) + " ew2=" + String(ew2) + " ew3=" + String(ew3) + " ew4=" + String(ew4) + " ew5=" + String(ew5) + " ew6=" + String(ew6);
    message = message + " ew7=" + String(ew7) + " ew8=" + String(ew8) + " ew9=" + String(ew9) + " ew10=" + String(ew10) + " ew11=" + String(ew11) + " ew12=" + String(ew12);
    request->send(200, "text/plain", message);
  });

  ledserver.on("/intensity", HTTP_GET, [](AsyncWebServerRequest* request) {  // Set intensity:
    String message = String(set_web_intensity);
    request->send(200, "text/plain", message);
  });

  ledserver.on("/resetew1", HTTP_GET, [](AsyncWebServerRequest* request) {  // Set all extra words off until midnight:
    if (debugtexts == 1) Serial.println("ResetExtraWords set to true... Extra words will NOT be displayed until midnight...");
    ResetExtraWords = true;
    updatenow = true;
    String message = "Set all extra words off until midnight...";
    request->send(200, "text/plain", message);
  });

  ledserver.on("/resetew0", HTTP_GET, [](AsyncWebServerRequest* request) {  // Set all extra words outputs active again:
    if (debugtexts == 1) Serial.println("ResetExtraWords set to false... Extra words will be displayed again...");
    String message = "Set all extra words outputs active again...";
    request->send(200, "text/plain", message);
    ResetExtraWords = false;
    getFlashValues();
    updatenow = true;
  });

  ledserver.on("/resetewstatus", HTTP_GET, [](AsyncWebServerRequest* request) {  // Status of the Extra Words reset switch:
    if (debugtexts == 1) Serial.println("Status of the Extra Words reset switch: " + String(ResetExtraWords));
    String message = "Status of the Extra Words reset switch: " + String(ResetExtraWords);
    request->send(200, "text/plain", message);
  });

  ledserver.on("/ewstatus", HTTP_GET, [](AsyncWebServerRequest* request) {  // Get the status of the extra words:
    int paramsNr = request->params();

    for (int i = 0; i < paramsNr; i++) {
      const AsyncWebParameter* p = request->getParam(i);

      if ((p->name().toInt() >= 0) && (p->name().toInt() <= 12)) {
        switch (p->name().toInt()) {
          case 1:
            request->send(200, "text/plain", String(ew1));
            break;
          case 2:
            request->send(200, "text/plain", String(ew2));
            break;
          case 3:
            request->send(200, "text/plain", String(ew3));
            break;
          case 4:
            request->send(200, "text/plain", String(ew4));
            break;
          case 5:
            request->send(200, "text/plain", String(ew5));
            break;
          case 6:
            request->send(200, "text/plain", String(ew6));
            break;
          case 7:
            request->send(200, "text/plain", String(ew7));
            break;
          case 8:
            request->send(200, "text/plain", String(ew8));
            break;
          case 9:
            request->send(200, "text/plain", String(ew9));
            break;
          case 10:
            request->send(200, "text/plain", String(ew10));
            break;
          case 11:
            request->send(200, "text/plain", String(ew11));
            break;
          case 12:
            request->send(200, "text/plain", String(ew12));
            break;
        }
      } else {
        request->send(200, "text/plain", "INVALID VALUES - MUST BE BETWEEN 0 and 12");
      }
    }
  });

  ledserver.on("/ew", HTTP_GET, [](AsyncWebServerRequest* request) {  // Set the extra words:
    int paramsNr = request->params();
    // Serial.println(paramsNr);

    for (int i = 0; i < paramsNr; i++) {
      const AsyncWebParameter* p = request->getParam(i);
      if ((p->value().toInt() >= 0) && (p->value().toInt() <= 255)) {
        if (String(p->name()) == "ew1") {
          ew1 = p->value().toInt();
          ew = 1;
        }
        if (String(p->name()) == "ew2") {
          ew2 = p->value().toInt();
          ew = 2;
        }
        if (String(p->name()) == "ew3") {
          ew3 = p->value().toInt();
          ew = 3;
        }
        if (String(p->name()) == "ew4") {
          ew4 = p->value().toInt();
          ew = 4;
        }
        if (String(p->name()) == "ew5") {
          ew5 = p->value().toInt();
          ew = 5;
        }
        if (String(p->name()) == "ew6") {
          ew6 = p->value().toInt();
          ew = 6;
        }
        if (String(p->name()) == "ew7") {
          ew7 = p->value().toInt();
          ew = 7;
        }
        if (String(p->name()) == "ew8") {
          ew8 = p->value().toInt();
          ew = 8;
        }
        if (String(p->name()) == "ew9") {
          ew9 = p->value().toInt();
          ew = 9;
        }
        if (String(p->name()) == "ew10") {
          ew10 = p->value().toInt();
          ew = 10;
        }
        if (String(p->name()) == "ew11") {
          ew11 = p->value().toInt();
          ew = 11;
        }
        if (String(p->name()) == "ew12") {
          ew12 = p->value().toInt();
          ew = 12;
        }

        switch (ew) {  // Set the color to the extra words:
          case 1:
            if (String(p->name()) == "R") redVal_ew1 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew1 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew1 = p->value().toInt();
            break;
          case 2:
            if (String(p->name()) == "R") redVal_ew2 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew2 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew2 = p->value().toInt();
            break;
          case 3:
            if (String(p->name()) == "R") redVal_ew3 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew3 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew3 = p->value().toInt();
            break;
          case 4:
            if (String(p->name()) == "R") redVal_ew4 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew4 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew4 = p->value().toInt();
            break;
          case 5:
            if (String(p->name()) == "R") redVal_ew5 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew5 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew5 = p->value().toInt();
            break;
          case 6:
            if (String(p->name()) == "R") redVal_ew6 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew6 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew6 = p->value().toInt();
            break;
          case 7:
            if (String(p->name()) == "R") redVal_ew7 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew7 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew7 = p->value().toInt();
            break;
          case 8:
            if (String(p->name()) == "R") redVal_ew8 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew8 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew8 = p->value().toInt();
            break;
          case 9:
            if (String(p->name()) == "R") redVal_ew9 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew9 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew9 = p->value().toInt();
            break;
          case 10:
            if (String(p->name()) == "R") redVal_ew10 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew10 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew10 = p->value().toInt();
            break;
          case 11:
            if (String(p->name()) == "R") redVal_ew11 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew11 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew11 = p->value().toInt();
            break;
          case 12:
            if (String(p->name()) == "R") redVal_ew12 = p->value().toInt();
            if (String(p->name()) == "G") greenVal_ew12 = p->value().toInt();
            if (String(p->name()) == "B") blueVal_ew12 = p->value().toInt();
            break;
        }

        // Show note in extra word section if colors were changed - colors may be out of sync... // TODO
        if ((String(p->name()) == "R") || (String(p->name()) == "G") || (String(p->name()) == "B")) ESPUI.updateVisibility(ew_web_HintID, true);

        request->send(200, "text/plain", "Extra word " + p->name() + " set to " + p->value());
        changedvalues = true;
        updatenow = true;
      } else {
        request->send(200, "text/plain", "INVALID VALUES - MUST BE BETWEEN 0 and 255");
      }
    }
  });

  ledserver.begin();
}


// ###########################################################################################################################################
// # Startup WiFi text function:
// ###########################################################################################################################################
void SetWLAN(uint32_t color) {
  ClearDisplay();  // Clear display background

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

  if (langLEDlayout == 4) {  // IT:
    for (uint16_t i = 21; i < 25; i++) {
      strip.setPixelColor(i, color);
    }
  }

  if (langLEDlayout == 5) {  // SWE:
    for (uint16_t i = 100; i < 104; i++) {
      strip.setPixelColor(i, color);
    }
  }

  if (langLEDlayout == 6) {  // ES:
    for (uint16_t i = 124; i < 128; i++) {
      strip.setPixelColor(i, color);
    }
  }

  strip.show();
}


// ###########################################################################################################################################
// # Captive Portal web page to setup the device by AWSW:
// ###########################################################################################################################################
const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>WordClock Setup</title>
    <style>
      body {
        padding: 25px;
        font-size: 18px;
        background-color: #000;
        color: #fff;
        font-family: Arial, sans-serif;
      }
      h1, p { 
        text-align: center; 
        margin-bottom: 20px;
      }
      input, select { 
        font-size: 18px; 
        min-width: 150px;
      }
      button {
        display: inline-block;
        padding: 15px 25px;
        margin-top: 15px;
        font-size: 18px;
        cursor: pointer;
        text-align: center;
        text-decoration: none;
        outline: none;
        color: #fff;
        background-color: #4CAF50;
        border: none;
        border-radius: 15px;
        box-shadow: 0 9px #999;
      }
      button:hover {
        background-color: #3e8e41;
      }
      button:active {
        background-color: #3e8e41;
        box-shadow: 0 5px #666;
        transform: translateY(4px);
      }
    </style>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <script type="text/javascript">
      function disableButtonAndSubmit() {
        var btn = document.getElementById("submitButton");
        btn.disabled = true;
        setTimeout(function() {
          document.forms["myForm"].submit();
        }, 100);
      }
    </script>
  </head>
  <body>
    <form action="/start" name="myForm">
      <center>
        <h1>Welcome to the WordClock setup</h1>
        <p>Please add your local WiFi credentials and set your language on the next page</p>
        <p><button id="submitButton" type="submit" onclick="disableButtonAndSubmit()">Configure WordClock</button></p>
      </center>
    </form>
  </body>
  </html>
)rawliteral";


// ###########################################################################################################################################
// # Captive Portal web page to setup the device by AWSW:
// ###########################################################################################################################################
const char config_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html>
  <head>
    <title>WordClock Setup</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <script language="JavaScript">
      function updateSSIDInput() {
        var ssidSelect = document.getElementById("mySSIDSelect");
        if (ssidSelect && ssidSelect.options.length > 0) {
          document.getElementById("mySSID").value = ssidSelect.options[ssidSelect.selectedIndex].value;
        }
      }
       function validateForm() {
        var errorParagraph = document.querySelector('.error');
        errorParagraph.style.display = 'none';
        errorParagraph.innerHTML = '';
        if (document.forms["myForm"]["mySSID"].value == "") {
          errorParagraph.innerHTML = "WiFi SSID must be set. ";
          errorParagraph.style.display = 'block';
          return false;
        }
        if (document.forms["myForm"]["myPW"].value == "") {
          errorParagraph.innerHTML = "WiFi password must be set. ";
          errorParagraph.style.display = 'block'; 
          return false;
        }
        if (document.forms["myForm"]["setlanguage"].value == "99") {
          errorParagraph.innerHTML = "Language layout must be set. ";
          errorParagraph.style.display = 'block'; 
          return false;
        }
        return true;
      }
      function disableButtonAndSubmit() {
        if (validateForm()) {
          var btn = document.getElementById("submitButton");
          btn.innerText = 'Restarting WordClock...';
          btn.disabled = true;
          setTimeout(function() {
            document.forms["myForm"].submit();
          }, 1000);
        }
      }
      window.onload = function() {
        var ssidSelect = document.getElementById("mySSIDSelect");
        if (ssidSelect) {
          ssidSelect.addEventListener('change', updateSSIDInput);
        }
      };
    </script>
    <style>
      body {
        padding: 25px;
        font-size: 18px;
        background-color: #000;
        color: #fff;
        font-family: Arial, sans-serif;
      }
      h1, p { 
        text-align: center; 
        margin-bottom: 20px;
      }
      p.error { 
        color: #ff0000; 
        display: none;
      }
      input, select { 
        font-size: 18px; 
        min-width: 150px;
      }
      button {
        display: inline-block;
        padding: 15px 25px;
        margin-top: 15px;
        font-size: 18px;
        cursor: pointer;
        text-align: center;
        text-decoration: none;
        outline: none;
        color: #fff;
        background-color: #4CAF50;
        border: none;
        border-radius: 15px;
        box-shadow: 0 9px #999;
      }
      button:hover { background-color: #3e8e41 }
      button:active {
        background-color: #3e8e41;
        box-shadow: 0 5px #666;
        transform: translateY(4px);
      }
    </style>
  </head>
  <body>
    <form action="/get" name="myForm" onsubmit="return validateForm()">
      <h1>Initial WordClock setup:</h1>
      <!-- Select element will be dynamically added here -->
      <p>
        <label for="mySSID">Enter your WiFi SSID:</label><br />
        <input id="mySSID" name="mySSID" value="" />
      </p>
      <p>
        <label for="myPW">Enter your WiFi password:</label><br/>
        <input type="text" id="myPW" name="myPW" value="" />
      </p>
      <p>
        <label for="setlanguage">Select your language layout:</label><br/>
        <select id="setlanguage" name="setlanguage">
          <option value="99" disabled selected>Choose...</option>
          <option value="0">GERMAN</option>
          <option value="1">ENGLISH</option>
          <option value="2">DUTCH</option>
          <option value="3">FRENCH</option>
          <option value="4">ITALIAN (2024 models only)</option>
          <option value="5">SWEDISH (2024 models only)</option>
          <option value="6">SPANISH (2024 models only)</option>
        </select>

    <br/><br/>
    <label for="myBotToken">Enter your Telegram bot token: (Check settings.h how to get your own one)</label><br/>
    <input type="text" id="myBotToken" name="myBotToken" value="XXXXXXXXXX:YYYYYYYYYYYYYYY-ZZZZZZZZZZZZZZZZZZZZ" style="width: 400px;" /><br/><br/>
    <label for="myChatID">Enter your Telegram chat id: (Check settings.h how to get your own one)</label><br/>
    <input type="text" id="myChatID" name="myChatID" value="1234512345" style="width: 200px;" /><br/><br/>

      </p>
      <p class="error">Errors will be displayed here!</p>
      <p>
        <button id="submitButton" onclick="disableButtonAndSubmit()">Save values</button>
      </p>
    </form>
  </body>
  </html>
)rawliteral";


// ###########################################################################################################################################
// # Captive Portal web page to setup the device by AWSW:
// ###########################################################################################################################################
const char saved_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE HTML>
  <html>
  <head>
    <title>WordClock Setup</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body {
        padding: 25px;
        font-size: 18px;
        background-color: #000;
        color: #fff;
        font-family: Arial, sans-serif;
      }
      h1, p { 
        text-align: center; 
        margin-bottom: 20px;
      }
    </style>
  </head>
  <body>
    <h1>Settings saved!</h1>
    <p>WordClock is now trying to connect to the selected WiFi and display the chosen language.</p>
    <p>First the WiFi leds will be lit blue and change to green in case of a successful WiFi connection.</p>
    <p>If the connection fails the WiFi leds will flash red. Then please reconnect to the temporary access point again.</p>
    <p>Please close this page now, rejoin your selected WiFi and enjoy your WordClock. =)</p>
  </body>
  </html>
)rawliteral";


// ###########################################################################################################################################
// # Wifi scan function to help you to setup your WiFi connection
// ###########################################################################################################################################
String ScanWiFi() {
  String html = config_html;
  Serial.println("Scan WiFi networks - START");
  int n = WiFi.scanNetworks();
  Serial.println("WiFi scan done");
  Serial.println("Scan WiFi networks - END");
  Serial.println(" ");
  if (n > 0) {
    Serial.print(n);
    Serial.println(" WiFi networks found:");
    Serial.println(" ");
    String ssidList = "<p><label for=\"mySSISelect\">Found these networks:</label><br /><select id=\"mySSIDSelect\" name=\"mySSIDSelect\"><option value=\"\" disabled selected>Choose...</option>";
    for (int i = 0; i < n; ++i) {
      ssidList += "<option value=\"" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</option>";
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "" : "*");
    }
    ssidList += "</select></p>";
    html.replace("<!-- Select element will be dynamically added here -->", ssidList);
  } else {
    Serial.println("No WiFi networks found");
  }
  return html;
}


// ###########################################################################################################################################
// # Captive Portal by AWSW to avoid the usage of the WiFi Manager library to have more control
// ###########################################################################################################################################
const char* PARAM_INPUT_1 = "mySSID";
const char* PARAM_INPUT_2 = "myPW";
const char* PARAM_INPUT_3 = "setlanguage";
const char* PARAM_INPUT_4 = "myBotToken";
const char* PARAM_INPUT_5 = "myChatID";
const String captiveportalURL = "http://192.168.4.1";
void CaptivePotalSetup() {
  String htmlConfigContent = ScanWiFi();
  const char* temp_ssid = "WordClock";
  const char* temp_password = "";
  WiFi.softAP(temp_ssid, temp_password);
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("#################################################################################################################################################################################");
  Serial.print("# Temporary WiFi access point initialized. Please connect to the WiFi access point now and set your local WiFi credentials and WordClock language. Access point name: ");
  Serial.println(temp_ssid);
  Serial.print("# In case your browser does not open the WordClock setup page automatically after connecting to the access point, please navigate to this URL manually: http://");
  Serial.println(WiFi.softAPIP());
  Serial.println("#################################################################################################################################################################################");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  dnsServer.start(53, "*", WiFi.softAPIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", index_html);
  });

  server.on("/get", HTTP_GET, [](AsyncWebServerRequest* request) {
    String inputMessage;
    String inputParam;
    if (request->hasParam(PARAM_INPUT_1)) {

      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      if (debugtexts == 1) Serial.println(inputMessage);
      preferences.putString("WIFIssid", inputMessage);  // Save entered WiFi SSID

      inputMessage = request->getParam(PARAM_INPUT_2)->value();
      inputParam = PARAM_INPUT_2;
      if (debugtexts == 1) Serial.println(inputMessage);
      preferences.putString("WIFIpass", inputMessage);  // Save entered WiFi password

      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
      if (debugtexts == 1) Serial.println(inputMessage);
      preferences.putUInt("langLEDlayout", inputMessage.toInt());  // Save entered layout language

      inputMessage = request->getParam(PARAM_INPUT_4)->value();
      inputParam = PARAM_INPUT_4;
      if (debugtexts == 1) Serial.println(inputMessage);
      preferences.putString("iBotToken", inputMessage);  // Save entered Telegram bot token

      inputMessage = request->getParam(PARAM_INPUT_5)->value();
      inputParam = PARAM_INPUT_5;
      if (debugtexts == 1) Serial.println(inputMessage);
      preferences.putString("iChatID", inputMessage);  // Save entered Telegram chat id

      delay(250);
      preferences.end();
    } else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    request->send_P(200, "text/html", saved_html);
    ResetTextLEDs(strip.Color(0, 255, 0));
    delay(1000);
    ESP.restart();
  });

  server.on("/start", HTTP_GET, [htmlConfigContent](AsyncWebServerRequest* request) {
    request->send_P(200, "text/html", htmlConfigContent.c_str());
  });

  server.on("/connecttest.txt", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("msftconnecttest.com", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/fwlink", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/wpad.dat", [](AsyncWebServerRequest* request) {
    request->send(404);
  });
  server.on("/generate_204", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/redirect", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/hotspot-detect.html", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/canonical.html", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/success.txt", [](AsyncWebServerRequest* request) {
    request->send(200);
  });
  server.on("/ncsi.txt", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/chrome-variations/seed", [](AsyncWebServerRequest* request) {
    request->send(200);
  });
  server.on("/service/update2/json", [](AsyncWebServerRequest* request) {
    request->send(200);
  });
  server.on("/chat", [](AsyncWebServerRequest* request) {
    request->send(404);
  });
  server.on("/startpage", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
  });
  server.on("/favicon.ico", [](AsyncWebServerRequest* request) {
    request->send(404);
  });

  server.on("/", HTTP_ANY, [](AsyncWebServerRequest* request) {
    AsyncWebServerResponse* response = request->beginResponse(200, "text/html", index_html);
    response->addHeader("Cache-Control", "public,max-age=31536000");
    request->send(response);
    Serial.println("Served Basic HTML Page");
  });

  server.onNotFound([](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.print("onnotfound ");
    Serial.print(request->host());
    Serial.print(" ");
    Serial.print(request->url());
    Serial.print(" sent redirect to " + captiveportalURL + "\n");
  });

  server.begin();
  Serial.println("WordClock Captive Portal web server started");
}


// ###########################################################################################################################################
// # Wifi setup and reconnect function that runs once at startup and during the loop function of the ESP:
// ###########################################################################################################################################
void WIFI_SETUP() {
  Serial.println(" ");
  esp_log_level_set("wifi", ESP_LOG_WARN);  // Disable WiFi debug warnings
  if (testTime == 0) {                      // If time text test mode is NOT used:
    String WIFIssid = preferences.getString("WIFIssid");
    bool WiFiConfigEmpty = false;
    if (WIFIssid == "") {
      WiFiConfigEmpty = true;
    }
    String WIFIpass = preferences.getString("WIFIpass");
    if (WIFIpass == "") {
      WiFiConfigEmpty = true;
    }
    if (WiFiConfigEmpty == true) {
      Serial.println("Show SET WIFI...");
      uint32_t c = strip.Color(0, 255, 255);
      for (int i = 0; i < 3; i++) {
        int TextWait = 500;
        showtext('S', TextWait, c);
        showtext('E', TextWait, c);
        showtext('T', TextWait, c);
        showtext(' ', TextWait, c);
        showtext('W', TextWait, c);
        showtext('I', TextWait, c);
        showtext('F', TextWait, c);
        showtext('I', TextWait, c);
        showtext(' ', TextWait, c);
      }
      CaptivePotalSetup();
      SetWLAN(strip.Color(0, 255, 255));
    } else {
      Serial.println("Try to connect to found WiFi configuration: ");
      WiFi.disconnect();
      int tryCount = 0;
      WiFi.mode(WIFI_STA);
      WiFi.begin((const char*)WIFIssid.c_str(), (const char*)WIFIpass.c_str());
      Serial.println("Connecting to WiFi " + String(WIFIssid));
      while (WiFi.status() != WL_CONNECTED) {
        SetWLAN(strip.Color(0, 0, 255));
        tryCount = tryCount + 1;
        Serial.print("Connection try #: ");
        Serial.print(tryCount);
        Serial.print(" of maximum ");
        Serial.println(maxWiFiconnctiontries);
        if (tryCount >= maxWiFiconnctiontries - 10) {
          SetWLAN(strip.Color(255, 0, 0));
        }
        if (tryCount == maxWiFiconnctiontries) {
          Serial.println("\n\nWIFI CONNECTION ERROR: If the connection still can not be established please check the WiFi settings or location of the device.\n\n");
          preferences.putString("WIFIssid", "");  // Reset entered WiFi ssid
          preferences.putString("WIFIpass", "");  // Reset entered WiFi password
          preferences.end();
          delay(250);
          Serial.println("WiFi settings deleted because in " + String(maxWiFiconnctiontries) + " tries the WiFi connection could not be established. Temporary WordClock access point will be started to reconfigure WiFi again.");
          ESP.restart();
        }
        delay(1000);
        SetWLAN(strip.Color(0, 0, 0));
        delay(1000);
      }
      Serial.println(" ");
      WiFIsetup = true;
      Serial.print("Successfully connected now to WiFi SSID: ");
      Serial.println(WiFi.SSID());
      Serial.println("IP: " + WiFi.localIP().toString());
      Serial.println("DNS: " + WiFi.dnsIP().toString());
      Serial.println("GW: " + WiFi.gatewayIP().toString());
      Serial.println("ESP32 hostname: " + String(WiFi.getHostname()));
      SetWLAN(strip.Color(0, 255, 0));
      delay(1000);
      configNTPTime();                                      // NTP time setup
      setupWebInterface();                                  // Generate the configuration page
      handleLEDupdate();                                    // LED update via web
      setupOTAupate();                                      // ESP32 OTA update
      secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
      if (useTelegram) {                                    // Telegram support and send 1st message after startup
        Serial.println("Send initial Telegram message: WordClock " + String(WORD_CLOCK_VERSION) + " start finished");
        bot.sendMessage(CHAT_ID.c_str(), "WordClock " + String(WORD_CLOCK_VERSION) + " start finished " + emoStartup + "\n\nPlease use /start to show the commands list.\n\nWeb interface online at: http://" + IpAddress2String(WiFi.localIP()) + "\n\nSmart home web URL examples: http://" + IpAddress2String(WiFi.localIP()) + ":2023", "");
      }
      Serial.println("######################################################################");
      Serial.println("# Web interface online at: http://" + IpAddress2String(WiFi.localIP()));
      Serial.println("# Web interface online at: http://" + String(WiFi.getHostname()));
      Serial.println("# HTTP controls online at: http://" + IpAddress2String(WiFi.localIP()) + ":2023");
      Serial.println("# HTTP controls online at: http://" + String(WiFi.getHostname()) + ":2023");
      Serial.println("######################################################################");
      Serial.println("# WordClock startup finished...");
      Serial.println("######################################################################");
      if (useStartupText == 1) callStartText();  // Show "WordClock" startup text
      if (useshowip == 1) ShowIPaddress();       // Display the current IP-address
      Serial.println(" ");
      Serial.println(" ");
      Serial.println(" ");
      updatenow = true;  // Update the display 1x after startup
      update_display();  // Update LED display
    }
  }
}


// ###########################################################################################################################################
// # ESP32 OTA update:
// ###########################################################################################################################################
const char otaserverIndex[] PROGMEM = R"=====(
  <!DOCTYPE html><html><head><title>WordClock</title></head>
      <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <body>
    <form method='POST' action='/update' enctype='multipart/form-data'>
      <center><b><h1>WordClock software update</h1></b>
      <h2>Please select the in the Arduino IDE > "Sketch" ><br/>"Export Compiled Binary (Alt+Ctrl+S)"<br/>to generate the required "Code.ino.bin" file.<br/><br/>
      Select the "Code.ino.bin" file with the "Search" button.<br/><br/>
      Use the "Update" button to start the update.<br/><br/>WordClock will restart automatically.</h2><br/>
      <input type='file' name='update'>       <input type='submit' value='Update'>
     </center></form></body>
  </html>
 )=====";


const char otaNOK[] PROGMEM = R"=====(
  <!DOCTYPE html><html><head><title>WordClock</title></head>
          <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
   <style>
    .button {
      display: inline-block;
      padding: 15px 25px;
      font-size: 24px;
      cursor: pointer;
      text-align: center;
      text-decoration: none;
      outline: none;
      color: #fff;
      background-color: #4CAF50;
      border: none;
      border-radius: 15px;
      box-shadow: 0 9px #999;
    }
    .button:hover {background-color: #3e8e41}
    .button:active {
      background-color: #3e8e41;
      box-shadow: 0 5px #666;
      transform: translateY(4px);
    }
    </style>
    <body>
      <center><b><h1>WordClock software update</h1></b>
      <h2>ERROR: Software update FAILED !!!<br/><br/>WordClock will restart automatically.</h2><br/>
      </center></body>
  </html>
 )=====";


const char otaOK[] PROGMEM = R"=====(
  <!DOCTYPE html><html><head><title>WordClock</title></head>
          <style>
      body {
      padding: 25px;
      font-size: 25px;
      background-color: black;
      color: white;
      }
      </style>
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
   <style>
    .button {
      display: inline-block;
      padding: 15px 25px;
      font-size: 24px;
      cursor: pointer;
      text-align: center;
      text-decoration: none;
      outline: none;
      color: #fff;
      background-color: #4CAF50;
      border: none;
      border-radius: 15px;
      box-shadow: 0 9px #999;
    }
    .button:hover {background-color: #3e8e41}
    .button:active {
      background-color: #3e8e41;
      box-shadow: 0 5px #666;
      transform: translateY(4px);
    }
    </style>
    <body>
      <center><b><h1>WordClock software update</h1></b>
      <h2>Software update done =)<br/><br/>WordClock will restart automatically.</h2><br/>
      </center></body>
  </html>
 )=====";


void setupOTAupate() {
  otaserver.on("/", HTTP_GET, []() {
    otaserver.sendHeader("Connection", "close");
    otaserver.send(200, "text/html", otaserverIndex);
  });

  otaserver.on(
    "/update", HTTP_POST, []() {
      otaserver.sendHeader("Connection", "close");
      if (Update.hasError()) {
        otaserver.send(200, "text/html", otaNOK);
        ResetTextLEDs(strip.Color(255, 0, 0));
      } else {
        otaserver.send(200, "text/html", otaOK);
        ResetTextLEDs(strip.Color(0, 255, 0));
      }
      delay(3000);
      ESP.restart();
    },
    []() {
      HTTPUpload& upload = otaserver.upload();
      if (upload.status == UPLOAD_FILE_START) {
        Serial.setDebugOutput(true);
        Serial.printf("Update: %s\n", upload.filename.c_str());
        if (!Update.begin()) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
          Update.printError(Serial);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
          Serial.printf("Update success: %u\nRebooting...\n", upload.totalSize);
        } else {
          Update.printError(Serial);
        }
        Serial.setDebugOutput(false);
      } else {
        Serial.printf("Update failed unexpectedly (likely broken connection): status=%d\n", upload.status);
      }
    });
  otaserver.begin();
}


// ###########################################################################################################################################
// # OFFLINE MODE Captive Portal by AWSW
// ###########################################################################################################################################
void OfflinePotalSetup() {
  if (debugtexts == 1) Serial.println("\nCreating WordClock Offline Mode access point...");
  WiFi.mode(WIFI_AP);
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  if (UseOfflineModeWithPassword == 1) {  // Access point with password or not
    WiFi.softAP(Offline_SSID, Offline_PW);
  } else {
    WiFi.softAP(Offline_SSID);
  }
  Serial.println("##############################################################################################################################################################################################################");
  Serial.print("# Offline Mode WiFi access point initialized. Please connect to the WiFi access point and set the current time now. Access point name: '");
  Serial.print(Offline_SSID);
  if (UseOfflineModeWithPassword == 1) {
    Serial.print("' using the password: '");
    Serial.print(Offline_PW);
  }
  Serial.println("'");
  Serial.print("# In case your browser does not open the WordClock configuration page automatically after connecting to the access point, please navigate to this URL manually: http://");
  Serial.println(WiFi.softAPIP());
  Serial.println("##############################################################################################################################################################################################################");

  setupWebInterface();  // Generate the configuration page

  server.on("/connecttest.txt", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.println("Served /connecttest.txt");
  });
  server.on("msftconnecttest.com", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.println("Served msftconnecttest.com");
  });
  server.on("/fwlink", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.println("Served /fwlink");
  });
  server.on("/wpad.dat", [](AsyncWebServerRequest* request) {
    // request->send(404);
    request->redirect(captiveportalURL);
    Serial.println("Served wpad.dat");
  });
  server.on("/generate_204", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.println("Served /gernerate_204");
  });
  server.on("/redirect", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.println("Served /redirect");
  });
  server.on("/hotspot-detect.html", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.println("Served /hotspot-detect.html");
  });
  server.on("/canonical.html", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.println("Served /cannonical.html");
  });
  server.on("/success.txt", [](AsyncWebServerRequest* request) {
    // request->send(200);
    request->redirect(captiveportalURL);
    Serial.println("Served /success.txt");
  });
  server.on("/ncsi.txt", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.println("Served /ncsi.txt");
  });
  server.on("/chrome-variations/seed", [](AsyncWebServerRequest* request) {
    // request->send(200);
    request->redirect(captiveportalURL);
    Serial.println("Served /chrome-variations/seed");
  });
  server.on("/service/update2/json", [](AsyncWebServerRequest* request) {
    // request->send(200);
    request->redirect(captiveportalURL);
    Serial.println("Served /service/update2/json");
  });
  server.on("/chat", [](AsyncWebServerRequest* request) {
    // request->send(404);
    request->redirect(captiveportalURL);
    Serial.println("Served /chat");
  });
  server.on("/startpage", [](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.println("Served /startpage");
  });
  server.on("/favicon.ico", [](AsyncWebServerRequest* request) {
    // request->send(404);
    request->redirect(captiveportalURL);
    Serial.println("Served /favicon.ico");
  });


  server.on("/", HTTP_ANY, [](AsyncWebServerRequest* request) {
    AsyncWebServerResponse* response = request->beginResponse(200, "text/html", index_html);
    // AsyncWebServerResponse* response = request->beginResponse(200, "text/html", index_offline_html);
    response->addHeader("Cache-Control", "public,max-age=31536000");
    request->send(response);
    // request->redirect(captiveportalURL);
    Serial.println("Served Basic HTML Page");
  });

  server.onNotFound([](AsyncWebServerRequest* request) {
    request->redirect(captiveportalURL);
    Serial.print("Web page not found: ");
    Serial.print(request->host());
    Serial.print(" ");
    Serial.print(request->url());
    Serial.print(" sent redirect to " + captiveportalURL + "\n");
  });

  server.begin();
  if (debugtexts == 1) Serial.println("WordClock OFFLINE MODE captive portal web server started");
  ShowOfflineIPaddress();  // Display the current Offline Mode IP-address every time on startup
}


// ###########################################################################################################################################
// # Show the Offline Mode IP-address on the display:
// ###########################################################################################################################################
void ShowOfflineIPaddress() {
  if (useshowip == 1) {
    Serial.println("Show current IP-address on the display: " + IpAddress2String(WiFi.softAPIP()));
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
    ClearDisplay();
    numbers(getDigit(int(WiFi.softAPIP()[0]), 2), 3);
    numbers(getDigit(int(WiFi.softAPIP()[0]), 1), 2);
    numbers(getDigit(int(WiFi.softAPIP()[0]), 0), 1);
    setLED(191, 191, 1);
    setLED(0, 3, 1);
    setLED(240, 243, 1);
    strip.show();
    delay(ipdelay);

    // // Octet 2:
    ClearDisplay();
    numbers(getDigit(int(WiFi.softAPIP()[1]), 2), 3);
    numbers(getDigit(int(WiFi.softAPIP()[1]), 1), 2);
    numbers(getDigit(int(WiFi.softAPIP()[1]), 0), 1);
    setLED(191, 191, 1);
    setLED(0, 7, 1);
    setLED(240, 247, 1);
    strip.show();
    delay(ipdelay);

    // // Octet 3:
    ClearDisplay();
    numbers(getDigit(int(WiFi.softAPIP()[2]), 2), 3);
    numbers(getDigit(int(WiFi.softAPIP()[2]), 1), 2);
    numbers(getDigit(int(WiFi.softAPIP()[2]), 0), 1);
    setLED(191, 191, 1);
    setLED(0, 11, 1);
    setLED(240, 251, 1);
    strip.show();
    delay(ipdelay);

    // // Octet 4:
    ClearDisplay();
    numbers(getDigit(int(WiFi.softAPIP()[3]), 2), 3);
    numbers(getDigit(int(WiFi.softAPIP()[3]), 1), 2);
    numbers(getDigit(int(WiFi.softAPIP()[3]), 0), 1);
    setLED(0, 15, 1);
    setLED(240, 255, 1);
    strip.show();
    delay(ipdelay);
  }
}


// ###########################################################################################################################################
// # Split String into an Array (DayNightModeFunction)
// ###########################################################################################################################################
String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;
  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}


// ###########################################################################################################################################
// # Text output function: (modified and original by Dreso)
// ###########################################################################################################################################
// # Matrix treated with coordinates from 1,1 (top,left) to 16,16 (bottom, right)
// # each element with two Leds aligned above each other  (16x16 Matrix treated as 8x16 Matrix)
// #  __________________          ________________
// # | 1,1 | 1,2 | 1,3 | ....      | 1,15 | 1,16 |
// # |-----|-----|-----|            -------------|
// # | 2,1 | 2,2 |..............    ......| 2,16 |
// # |-----|-----|-----|            -------------|
// # | 3,1 |-----|
// # |-----|-----|-----|            -------------|
// # |...................
// # |-----|-----|-----|            -------------|
// # | 16,1 | 16,2 | 16,3 | ...  | 16,15 | 16,16 |
// # |_____|_____|______          _|______|______|
#define MAXROWS 16  // 8 (2023 WordClock)
#define MAXCOLUMS 16


// ##############################################################################################
// #Duocolor Pixel
// ##############################################################################################
void DrawPixelX(int row, int colum, uint32_t c1, uint32_t c2) {
  if (row > 0) row--;                                                      // internal index begins with 0 and external with 1
  if (colum > 0) colum--;                                                  // internal index begins with 0
  strip.setPixelColor(row * 2 * MAXCOLUMS + (MAXCOLUMS - 1 - colum), c1);  // upper led
  // strip.setPixelColor(row*2*MAXCOLUMS+(MAXCOLUMS+colum),c2);      // lower led // (2023 WordClock)
}


// ##############################################################################################
// # Monocolor Line Drawing
// ##############################################################################################
void DrawLine(int startrow, int startcolum, int endrow, int endcolum, uint32_t c) {
  DrawLineX(startrow, startcolum, endrow, endcolum, c, c);  //same color for both Leds
}


// ##############################################################################################
// # Monocolor Pixel
// ##############################################################################################
void DrawPixel(int row, int colum, uint32_t c) {
  DrawPixelX(row, colum, c, c);  //same color for both Leds
}


// ##############################################################################################
// # Draw Line with possible different colours on the two leds per element
// #
// # Diagonal Lines are also possible
// # Boundaries are not checked in this routine
// # Endpoints can also be smaller in number than the start
// ##############################################################################################
void DrawLineX(int startrow, int startcolum, int endrow, int endcolum, uint32_t c1, uint32_t c2) {
  int dx, dy;
  if (endcolum > startcolum) {
    dx = 1;
  } else {
    if (endcolum < startcolum) {
      dx = -1;
    } else {
      dx = 0;
    }
  }
  if (endrow > startrow) {
    dy = 1;
  } else {
    if (endrow < startrow) {
      dy = -1;
    } else {
      dy = 0;
    }
  }

  int actrow = startrow, actcolum = startcolum;
  bool bDone = false;
  while (!bDone) {
    DrawPixelX(actrow, actcolum, c1, c2);
    if (dx == 0 && dy == 0) {
      bDone = true;
    }
    actrow += dy;
    actcolum += dx;
    if (actrow == endrow) {
      dy = 0;
    }
    if (actcolum == endcolum) {
      dx = 0;
    }
  }
}


// ###########################################################################################################################################
// # Text output function:
// ###########################################################################################################################################
void showtext(char letter, int wait, uint32_t c) {
  ClearDisplay();
  uint32_t c2;
  letter = toupper(letter);  // only uppercase possible at the moment

  if (letter == 'A') {
    DrawLine(3, 6, 6, 6, c);
    DrawLine(3, 11, 6, 11, c);
    DrawLine(2, 7, 2, 10, c);
    DrawLine(4, 6, 4, 11, c);
  }
  if (letter == 'B') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(2, 7, 2, 10, c);
    DrawPixel(3, 11, c);
    DrawLine(4, 7, 4, 10, c);
    DrawPixel(5, 11, c);
    DrawLine(6, 7, 6, 10, c);
  }
  if (letter == 'C') {
    DrawLine(2, 7, 2, 11, c);
    DrawLine(3, 6, 5, 6, c);
    DrawLine(6, 7, 6, 11, c);
  }
  if (letter == 'D') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(3, 11, 5, 11, c);
    DrawLine(2, 7, 2, 10, c);
    DrawLine(6, 7, 6, 10, c);
  }
  if (letter == 'E') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(2, 6, 2, 11, c);
    DrawLine(4, 6, 4, 10, c);
    DrawLine(6, 6, 6, 11, c);
  }
  if (letter == 'F') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(2, 7, 2, 11, c);
    DrawLine(4, 7, 4, 10, c);
  }
  if (letter == 'G') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(4, 11, 6, 11, c);
    DrawLine(2, 6, 2, 11, c);
    DrawLine(4, 9, 4, 11, c);
    DrawLine(6, 6, 6, 11, c);
  }
  if (letter == 'H') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(2, 11, 6, 11, c);
    DrawLine(4, 6, 4, 11, c);
  }
  if (letter == 'I') {
    DrawLine(2, 7, 2, 9, c);
    DrawLine(3, 8, 5, 8, c);
    DrawLine(6, 7, 6, 9, c);
  }
  if (letter == 'J') {
    DrawLine(2, 6, 2, 10, c);
    DrawLine(3, 10, 5, 10, c);
    DrawLine(6, 7, 6, 9, c);
    DrawPixel(5, 6, c);
  }
  if (letter == 'K') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(4, 7, 4, 9, c);
    DrawLine(4, 9, 2, 11, c);
    DrawLine(4, 9, 6, 11, c);
  }
  if (letter == 'L') {
    //"L"
    DrawLine(2, 6, 6, 6, c);
    DrawLine(6, 6, 6, 11, c);
  }
  if (letter == 'M') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(2, 11, 6, 11, c);
    DrawPixel(3, 7, c);
    DrawLine(4, 8, 4, 9, c);
    DrawLine(3, 10, 3, 10, c);
  }
  if (letter == 'N') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(2, 11, 6, 11, c);
    DrawLine(3, 7, 6, 10, c);
  }
  if (letter == 'O') {
    DrawLine(3, 6, 5, 6, c);
    DrawLine(3, 11, 5, 11, c);
    DrawLine(2, 7, 2, 10, c);
    DrawLine(6, 7, 6, 10, c);
  }
  if (letter == 'P') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(3, 11, 3, 11, c);
    DrawLine(2, 6, 2, 10, c);
    DrawLine(4, 6, 4, 10, c);
  }
  if (letter == 'Q') {
    DrawLine(3, 6, 5, 6, c);
    DrawLine(3, 11, 5, 11, c);
    DrawLine(2, 7, 2, 10, c);
    DrawLine(6, 7, 6, 10, c);
    DrawLine(5, 10, 6, 11, c);
  }
  if (letter == 'R') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(3, 11, 4, 11, c);
    DrawLine(2, 6, 2, 10, c);
    DrawLine(4, 6, 4, 11, c);
    DrawLine(5, 10, 6, 11, c);
  }
  if (letter == 'S') {
    DrawLine(3, 6, 3, 6, c);
    DrawLine(5, 11, 5, 11, c);
    DrawLine(2, 7, 2, 11, c);
    DrawLine(4, 7, 4, 10, c);
    DrawLine(6, 6, 6, 10, c);
  }
  if (letter == 'T') {
    DrawLine(2, 6, 2, 10, c);
    DrawLine(3, 8, 6, 8, c);
  }
  if (letter == 'U') {
    DrawLine(2, 6, 5, 6, c);
    DrawLine(2, 11, 5, 11, c);
    DrawLine(6, 7, 6, 10, c);
  }
  if (letter == 'V') {
    DrawLine(2, 6, 3, 6, c);
    DrawLine(2, 11, 3, 11, c);
    DrawLine(4, 6, 6, 8, c);
    DrawLine(6, 9, 4, 11, c);
  }
  if (letter == 'W') {
    DrawLine(2, 6, 6, 6, c);
    DrawLine(2, 11, 6, 11, c);
    DrawLine(5, 7, 4, 8, c);
    DrawLine(4, 9, 5, 10, c);
  }
  if (letter == 'X') {
    DrawLine(2, 6, 4, 8, c);
    DrawLine(4, 9, 2, 11, c);
    DrawLine(6, 6, 4, 8, c);
    DrawLine(4, 9, 6, 11, c);
  }
  if (letter == 'Y') {
    DrawLine(2, 6, 4, 8, c);
    DrawLine(4, 8, 2, 10, c);
    DrawLine(5, 8, 6, 8, c);
  }
  if (letter == 'Z') {
    DrawLine(2, 6, 2, 10, c);
    DrawLine(3, 9, 5, 7, c);
    DrawLine(6, 6, 6, 10, c);
  }

  if (letter == ' ') {
    // already cleareed at top
  }
  strip.show();
  delay(wait);
}


// ###########################################################################################################################################
// # // WordClock is offline during runtime! --> Restart and reconnect now!
// ###########################################################################################################################################
void WiFi_Lost() {
  Serial.print("WordClock is offline during runtime! ");
  Serial.print(millis());
  Serial.print(" - Reconnecting to WiFi... ");
  WiFi.disconnect();
  WiFi.reconnect();
  WiFi_previousMillis = WiFi_currentMillis;
  WiFi_retry_counter = WiFi_retry_counter + 1;
  Serial.print(" Retry count: " + String(WiFi_retry_counter));

  // delay(1000);

  if ((WiFi.status() != WL_CONNECTED)) {
    Serial.println(" - Reconnecting to WiFi = FAILED - Next try in " + String(WiFi_interval / 1000) + " seconds...");
  }

  if (WiFi_retry_counter >= WiFi_FlashLEDs) {
    Serial.println("WordClock is offline during runtime for 10+ tries! --> Flash LEDs 3x now!");
    for (int i = 0; i < 3; i++) {
      updatedevice = false;
      SetWLAN(strip.Color(0, 0, 255));
      delay(1000);
      SetWLAN(strip.Color(255, 255, 0));
      delay(1000);
      SetWLAN(strip.Color(255, 0, 0));
      delay(1000);
    }
  }

  if (WiFi_retry_counter >= WiFi_Restart) {
    Serial.println("WordClock is offline during runtime for 30 tries! --> Restart and reconnect now!");
    updatedevice = false;
    delay(250);
    ResetTextLEDs(strip.Color(0, 255, 0));
    if (changedvalues == true) setFlashValues();  // Write settings to flash
    preferences.end();
    delay(1000);
    ESP.restart();
  }
}


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################