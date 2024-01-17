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
// # Compatible with WordClock version: V3.2.0
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
// # LED language layout default: !!! SET YOUR DEFAULT LANGUAGE HERE !!!
// ###########################################################################################################################################
int langLEDlayout_default = 1;  // Can be selected in the configuraration portal later too. This is just the default for the 1st startup
// Available languages:
// ####################
// 0 = German
// 1 = English
// 2 = Dutch
// 3 = French
// NOTE: You may need to use the "Reset WordClock settings"-button to update the value on the device


// ###########################################################################################################################################
// # Hardware settings:
// ###########################################################################################################################################
#define LEDPIN 32                                                                      // Arduino-Pin connected to the NeoPixels
#define NUMPIXELS 256                                                                  // How many NeoPixels are attached to the Arduino
#define ROWPIXELS 16                                                                   // How many NeoPixels per row
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);  // Init LED matrix


// ###########################################################################################################################################
// # Various default settings:
// ###########################################################################################################################################
int useshowip_default = 1;        // Show the current ip at boot
int useTelegram_default = 0;      // Use Telegram support
int useTelegramID_default = 1;    // React to your own Telegram CHAT_ID only
int showTeleData_default = 0;     // Show your own Telegram bot token and chat id on the configuration page directly visible
int TeleNoteUnknown_default = 1;  // Send notifications about messages from unknown users
int maxWiFiconnctiontries = 30;   // Maximum connection tries to logon to the set WiFi. After the amount of tries is reached the WiFi settings will be deleted!
int iHourOffset_default = 1;      // Offset of hours in Offline Mode
int useStartupText_default = 1;   // Show the "WordClock" text at boot


// ###########################################################################################################################################
// # Variables declaration:
// ###########################################################################################################################################
#define DEFAULT_AP_NAME "WordClock"  // WiFi access point name of the ESP32


// ###########################################################################################################################################
// # Variables declaration in Offline Mode:
// ###########################################################################################################################################
// NOTE: When you try this setting "UseOfflineModeWithPassword" you may need to remove the known WiFi entries to see the change after some
// seconds and meanwhile disabling WiFi on your device. Some operating systems seem to cache the settings, so you might not see the change first
int UseOfflineModeWithPassword = 1;                      // Choose to open the interal WiFi access point with (1) or without (0) password protection
const char* Offline_SSID = "WordClock in Offline Mode";  // SSID of the internal WiFi access point used in Offline Mode
const char* Offline_PW = "Matrix-16x16";                 // Access point with password protection. Minimum 8 characters needed!
int showOMhints_default = 0;                             // Show or hide the Operation Mode hints on the page


// ###########################################################################################################################################
// # Telegram support:
// ###########################################################################################################################################
// Telegram emoticons used in the messages:           // Codes see here: https://apps.timwhitlock.info/emoji/tables/unicode
String emoStartup = "\xF0\x9F\x98\x8A";
String emoActive = "\xF0\x9F\x91\x8D";
String emoInactive = "\xF0\x9F\x91\x8E";
// Telegram new BOT creation:
// ###########################################################################################################################################
// 1.: In Telegram search user "BotFather" and use the "Start" button
// 2.: Then type: "/newbot"
// 3.: Set a name for the new bot when requested
// 4.: Set a username for the new bot when requested
// 5.: Check the "Done! Congratulations" message for the line after "Use this token to access the HTTP API:" and add this "bot token" string 1:1 during the initial WordClock setup where you set the WiFi settings too.
// 6.: Leave the "BotFather" and search for your new bot name.
// 7.: Select your new created bot and use the "/start" command or the "Start"-button to activate the bot
// ###########################################################################################################################################
// 8.: Get your "chat id" for your own Telegram messanger:
// 9.: In Telegram search user "IDBot" and use the "Start" button
// 10.: Then type: "/getid"
// 11.: Copy your own Telegram ID and add this number 1:1 during the initial WordClock setup where you set the WiFi settings too.
// 12.: Activate the Telegram support in the web config and restart the device
// 13.: Add this to the Telegram menu, if you like to have a menu with all the available commands just edit the created bot using BotFather again:
// - In Telegram search user "BotFather" and use the "Start" button
// - /mybots
// - Select your bot
// - Select "Edit bot"
// - Select "Edit Commands"
// - Follow the instruction to set up the menu entries - example (send without the "/* LANGUAGE:" and "*/"):
// ###########################################################################################################################################

/* DE:


start - WordClock Telegram welcome message
ew1 - ALARM text
ew2 - GEBURTSTAG text
ew3 - MÜLL RAUS BRINGEN text
ew4 - AUTO text
ew5 - FEIERTAG text
ew6 - FORMEL1 text
ew7 - GELBER SACK text
ew8 - URLAUB text
ew9 - WERKSTATT text
ew10 - ZEIT ZUM ZOCKEN text
ew11 - FRISEUR text
ew12 - TERMIN text


*/


/* EN:


start - WordClock Telegram welcome message
ew1 - COME HERE text
ew2 - LUNCH TIME text
ew3 - ALARM text
ew4 - GARBAGE text
ew5 - HOLIDAY text
ew6 - TEMPERATURE text
ew7 - DATE text
ew8 - BIRTHDAY text
ew9 - DOORBELL text


*/


/* NL:


start - WordClock Telegram welcome message
ew1 - KOM HIER text
ew2 - LUNCH TIJD text
ew3 - ALARM text
ew4 - AFVAL text
ew5 - VAKANTIE text
ew6 - TEMPERATUUR text
ew7 - DATUM text
ew8 - VERJAARDAG text
ew9 - DEURBEL text


*/


/* FR:


start - WordClock Telegram welcome message
ew1 - ALARME text
ew2 - ANNIVERSAIRE text
ew3 - POUBELLE text
ew4 - A TABLE text
ew5 - VACANCES text
ew6 - VIENS ICI text
ew7 - SONNETTE text
ew8 - TEMPERATURE text
ew9 - DATE text


*/

// ###########################################################################################################################################


// ###########################################################################################################################################
// # Special NTP time server and time zone settings:
// ###########################################################################################################################################
// Time server:
String NTPserver_default = "pool.ntp.org";  // Here you can set your own specific time server address if needed. Many others can be selected in the configuraration portal
// Choose the closest one to you here: https://gist.github.com/mutin-sa/eea1c396b1e610a2da1e5550d94b0453
// PS: The closest NTP time server to you might be your local router which can be selected in the configuraration portal too =)
//
// Time zone:
String Timezone_default = "CET-1CEST,M3.5.0,M10.5.0/3";  // Here you can set your own specific time zone if needed. Many others can be selected in the configuraration portal
// Please send me a message with your time zone settings to add them to this list in future releases. Thanks in advance! =)
// You can check a list of timezone string variables here:  https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv


// ###########################################################################################################################################
// # LED defaults:
// ###########################################################################################################################################
int redVal_back_default = 0;              // Default background color RED
int greenVal_back_default = 255;          // Default background color GREEN
int blueVal_back_default = 255;           // Default background color BLUE
int redVal_time_default = 255;            // Default time color RED
int greenVal_time_default = 128;          // Default time color GREEN
int blueVal_time_default = 0;             // Default time color BLUE
int intensity_day_default = 15;           // LED intensity (0..255) in day mode   - Important note: Check power consumption and used power supply capabilities!
int intensity_night_default = 5;          // LED intensity (0..255) in day mode   - Important note: Check power consumption and used power supply capabilities!
int usenightmode_default = 1;             // Use the night mode to reduce LED intensity during set times
String day_time_start_default = "06:15";  // Define day mode start --> time before is then night mode if used
String day_time_stop_default = "22:14";   // Define day mode end --> time after is then night mode if used
int usesinglemin_default = 1;             // Show the single minutes
// Extra Words default colors:
int redVal_ew1_default = 255;    // Default ew1 color RED
int greenVal_ew1_default = 0;    // Default ew1 color GREEN
int blueVal_ew1_default = 0;     // Default ew1 color BLUE
int redVal_ew2_default = 0;      // Default ew2 color RED
int greenVal_ew2_default = 255;  // Default ew2 color GREEN
int blueVal_ew2_default = 0;     // Default ew2 color BLUE
int redVal_ew3_default = 255;    // Default ew3 color RED
int greenVal_ew3_default = 255;  // Default ew3 color GREEN
int blueVal_ew3_default = 0;     // Default ew3 color BLUE
int redVal_ew4_default = 0;      // Default ew4 color RED
int greenVal_ew4_default = 0;    // Default ew4 color GREEN
int blueVal_ew4_default = 255;   // Default ew4 color BLUE
int redVal_ew5_default = 55;     // Default ew5 color RED
int greenVal_ew5_default = 26;   // Default ew5 color GREEN
int blueVal_ew5_default = 148;   // Default ew5 color BLUE
int redVal_ew6_default = 0;      // Default ew6 color RED
int greenVal_ew6_default = 0;    // Default ew6 color GREEN
int blueVal_ew6_default = 255;   // Default ew6 color BLUE
int redVal_ew7_default = 255;    // Default ew7 color RED
int greenVal_ew7_default = 255;  // Default ew7 color GREEN
int blueVal_ew7_default = 0;     // Default ew7 color BLUE
int redVal_ew8_default = 255;    // Default ew8 color RED
int greenVal_ew8_default = 255;  // Default ew8 color GREEN
int blueVal_ew8_default = 255;   // Default ew8 color BLUE
int redVal_ew9_default = 0;      // Default ew9 color RED
int greenVal_ew9_default = 0;    // Default ew9 color GREEN
int blueVal_ew9_default = 255;   // Default ew9 color BLUE
int redVal_ew10_default = 181;   // Default ew10 color RED
int greenVal_ew10_default = 26;  // Default ew10 color GREEN
int blueVal_ew10_default = 0;    // Default ew10 color BLUE
int redVal_ew11_default = 122;   // Default ew11 color RED
int greenVal_ew11_default = 33;  // Default ew11 color GREEN
int blueVal_ew11_default = 158;  // Default ew11 color BLUE
int redVal_ew12_default = 255;   // Default ew12 color RED
int greenVal_ew12_default = 0;   // Default ew12 color GREEN
int blueVal_ew12_default = 0;    // Default ew12 color BLUE


// ###########################################################################################################################################
// # Test functions:
// ###########################################################################################################################################
int debugtexts = 0;          // Show more debug information in serial monitor
int ShowTimeEachSecond = 0;  // Show current time each second in serial monitor
int testTime = 0;            // LED time text output test
int testPrintTimeTexts = 0;  // Print the time texts shown on the display in the serial monitor (Currently DE + IT only)
// Test a special time Online Mode:
int testspecialtimeON = 0;  // Use test a special time function
int test_hourON = 23;       // Test a special time: hour
int test_minuteON = 38;     // Test a special time: minute
int test_secondON = 45;     // Test a special time: second
// Test a special time Offline Mode:
int testspecialtimeOFF = 0;  // Use test a special time function
int test_hourOFF = 0;        // Test a special time: hour
int test_minuteOFF = 38;     // Test a special time: minute
int test_secondOFF = 45;     // Test a special time: second


// ###########################################################################################################################################
// #
// # !!! DANGER ZONE: DO NOT PLAY WITH THIS VALUE IF YOU ARE NOT AWARE WHAT YOU ARE DOING - SERIOSLY !!!
// #
// # !!! PLEASE READ THIS CAREFULLY TO AVOID DAMAGE TO THE ESP32 AND THE OTHER COMPONENTS !!! YOU HAVE BEEN WARNED !!!
// #
// ###########################################################################################################################################
// # LED intensity setting:
// ###########################################################################################################################################
#define LEDintensityLIMIT 50  // of 255
                              /* Do NOT change this if you don't know how much power drain this may cause to avoid damage !!! 
                              Limit the intensity level to be able to select in the configuration to avoid to much power drain and to avoid hardware failures over time. 
                              In case you want to use the device over longer times with a higher intensity or even higher, you will need to consider a much more powerful 
                              power supply, a better cable to connect the device and you have to power the matrix directly to 5V of the power supply, not over the ESP32! 
                              Otherwise the components fail directly or over time. Therefore this setting is limited and should not be raised. 
                              In case you really need a higher value, you need to make sure to power the matrix directly so that the power is not served over the ESP32 
                              to the matrix to avoid its damage over time or directly. 
                              Specifications for these matrix say that "60mA (per pixel at full brightness" (white color) is used. Multiple this by 256 and it takes 15.36A 
                              Calculation: 16x16 = 256 LEDs >>> 256x60mA = 15.36A (without the rest of the electronics = ESP32) >>> ~16A * 5V = ~80W power drain... 
                              Think of the heat emited by the LEDs then too... 
                              Conclusion: 
                              - You should not go higher here. 
                              - Measuremets showed that with the set intensity limit of 50 the 5V/3A power supply is enough to use with THIS software. That does not mean that this 
                                device will not fail, if you try to use it with other software "to shortly test other LED software" to see the result. Keep this in mind too. 
                              - Please think about if you really need such a high intensity value. The WordClock's used from me run at a maximum intensity of 22 which is 
                                really bright enoigh in my eyes and all levels above 48 i could not see really an advantage anymore that the display is better able to view... 
                              - Also useing white background color does not seem to be a good idea because to white set LEDs take the most power... 
                              - Make sure to use propper components and settings! 
                              - As allways: You are using this device at your own risk! */


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################