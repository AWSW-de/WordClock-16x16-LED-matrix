// ###########################################################################################################################################
// #
// # WordClock code for the printables WordClock 16x16 matrix project: https://www.printables.com/de/model/331012-wordclock-16x16-led-matrix
// #
// # Code by https://github.com/AWSW-de
// #
// # Released under license: GNU General Public License v3.0: https://github.com/AWSW-de/WordClock-16x16-LED-matrix/blob/main/LICENSE
// #
// # Compatible with WordClock version: V1.0.6
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
// # Hardware settings:
// ###########################################################################################################################################
#define LEDPIN 32              // Arduino-Pin connected to the NeoPixels
#define NUMPIXELS 256          // How many NeoPixels are attached to the Arduino
#define LEDintensityLIMIT 128  // Limit the intensity level to be able to select in the configuration to avoid to much power drain. !!! Make sure to use a propper power supply !!!
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, LEDPIN, NEO_GRB + NEO_KHZ800);


// ###########################################################################################################################################
// # LED language layout default: !!! SET YOUR DEFAULT LANGUAGE HERE !!!
// ###########################################################################################################################################
int langLEDlayout_default = 0;  // LED language layout default (0 = DE; 1 = EN)
// NOTE: You may need to use the "Reset WordClock settings"-button to update the value on the device


// ###########################################################################################################################################
// # LED defaults:
// ###########################################################################################################################################
int redVal_back_default = 0;      // Default background color RED
int greenVal_back_default = 255;  // Default background color GREEN
int blueVal_back_default = 255;   // Default background color BLUE
int redVal_time_default = 255;    // Default time color RED
int greenVal_time_default = 128;  // Default time color GREEN
int blueVal_time_default = 0;     // Default time color BLUE
int intensity_day_default = 20;   // LED intensity (0..255) in day mode   - Important note: Check power consumption and used power supply capabilities!
int intensity_night_default = 5;  // LED intensity (0..255) in day mode   - Important note: Check power consumption and used power supply capabilities!
int usenightmode_default = 1;     // Use the night mode to reduce LED intensity during set times
int day_time_start_default = 7;   // Define day mode start --> time before is then night mode if used
int day_time_stop_default = 22;   // Define day mode end --> time after is then night mode if used
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
// Extra word defauls init:
int ew1_default = 0;   // Extra Word 1
int ew2_default = 0;   // Extra Word 2
int ew3_default = 0;   // Extra Word 3
int ew4_default = 0;   // Extra Word 4
int ew5_default = 0;   // Extra Word 5
int ew6_default = 0;   // Extra Word 6
int ew7_default = 0;   // Extra Word 7
int ew8_default = 0;   // Extra Word 8
int ew9_default = 0;   // Extra Word 9
int ew10_default = 0;  // Extra Word 10
int ew11_default = 0;  // Extra Word 11
int ew12_default = 0;  // Extra Word 12


// ###########################################################################################################################################
// # Various default settings:
// ###########################################################################################################################################
#define AP_TIMEOUT 240          // Timeout in seconds for AP / WLAN config
int useledtest_default = 1;     // Show start animation and display test at boot
int useshowip_default = 1;      // Show the current ip at boot
int useTelegram_default = 0;    // Use Telegram support
int useTelegramID_default = 0;  // React to your own Telegram CHAT_ID only


// ###########################################################################################################################################
// # Variables declaration:
// ###########################################################################################################################################
#define DEFAULT_AP_NAME "WordClock"  // WiFi access point name of the ESP32
const char* hostname = "WordClock";  // Hostname to be set in your router


// ###########################################################################################################################################
// # NTP time server settings:
// ###########################################################################################################################################
const char* Timezone = "CET-1CEST,M3.5.0,M10.5.0/3";  // You can check a list of timezone string variables here:  https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
const char* NTPserver = "pool.ntp.org";               // Time server address. Choose the closest one to you here: https://gist.github.com/mutin-sa/eea1c396b1e610a2da1e5550d94b0453


// ###########################################################################################################################################
// # Telegram support:
// ###########################################################################################################################################
// Telegram emoticons used in the messages:           // Codes see here: https://apps.timwhitlock.info/emoji/tables/unicode
String emoStartup = "\xF0\x9F\x98\x8A";
String emoActive = "\xF0\x9F\x91\x8D";
String emoInactive = "\xF0\x9F\x91\x8E";

// Telegram new BOT creation:
// 1.: In Telegram search user "BotFather" and use the "Start" button
// 2.: Then type: "/newbot"
// 3.: Set a name for the new bot when requested
// 4.: Set a username for the new bot when requested
// 5.: Check the "Done! Congratulations" message for the line after "Use this token to access the HTTP API:" and add this line to the setting above here:
#define BOTtoken "XXXXXXXXXX:YYYYYYYYYYYYYYY-ZZZZZZZZZZZZZZZZZZZZ" 
// 6.: Leave the "BotFather" and search for your new bot name.
// 7.: Select your new created bot and use the "/start" command or the "Start"-button to activate the bot
// ###########################################################################################################################################
// 8.: Get your chat id for your own Telegram messanger:
// 9.: In Telegram search user "IDBot" and use the "Start" button
// 10.: Then type: "/getid"
// 11.: Copy your own Telegram ID and add this number to the setting above here:
#define CHAT_ID "1234512345" 
// 12.: Use the button "Reset WordClock settings" in the web configuration to save these values into the connfiguration or add them in the web config manually.
// 13.: Activate the Telegram support in the web config and restart the device
// 14: Add this to the Telegram menu:
//   If you like to have a menu with all the available commands just edit the created bot using BotFather again:
// - In Telegram search user "BotFather" and use the "Start" button
// - /mybots
// - Select your bot
// - Select "Edit bot"
// - Select "Edit Commands"
// - Follow the instruction to set up the menu entries - example (send without the "/* DE:" and "*/" or "/* EN:" and "*/" ):

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

// ###########################################################################################################################################


// ###########################################################################################################################################
// # Test functions:
// ###########################################################################################################################################
int testTime = 0;  // LED text output test from 00:00 to 23:29 o'clock. Each minute is 1 second in the test


// ###########################################################################################################################################
// # EOF - You have successfully reached the end of the code - well done ;-)
// ###########################################################################################################################################