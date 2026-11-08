#include <M5Cardputer.h>
#include <WiFi.h>  //allows for wifi scaning
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

enum AppState {
  STATE_BOOT,
  STATE_MENU,
  STATE_SCANER,
  STATE_SPIRIT_BOX,
  STATE_BT_SCANNER,
  STATE_MOTION,
  STATE_SETTINGS
};

AppState currentState = STATE_BOOT;

const char* menuItems[] = {
  "1. WIFI Scanner",
  "2. Spirit Board",
  "3. Spooktooth Radar",
  "4. Motion Detector",
  "5. Settings"
};
const int menuSize = 5;
int currentIndex = 0;
//graph variables
const int graphWidth = 100;
int rssiHistory[graphWidth];
int historyIndex = 0;

int screenBrightness = 128;
int lastBrightness = -1;

BLEScan* pBLEScan;
int bluetoothDevicesFound = 0;

const char* ouijaMessages[] = {
  "Hello...",
  "I am here",
  "Behind you",
  "Cold",
  "Sad",
  "Dark",
  "Go, GO NOW!",
  "Fouuundd Youuuu!",
  "Look to the left"
};
String activeMessage = "PRESS ANY KEY...";

//helping functions 
// menu
void drawMenu() {
  auto& d = M5Cardputer.Display;
  d.fillScreen(BLACK);
  d.setTextSize(1);
  d.setCursor(10, 5);
  d.print("--- PHANTOM MENU ---");
  for (int i = 0; i < menuSize; i++) {
    d.setCursor(20, 25 + (i * 15));
    if (i == currentIndex) {
      d.print("> ");
    } else {
      d.print(" ");
    }
    d.print(menuItems[i]);
  }
}
//scanner
void runScanner() {
  auto& d = M5Cardputer.Display;
  d.setCursor(10, 5);
  d.setTextSize(1);
  d.print("--- PARANORMAL SCANNER ---");

  int n = WiFi.scanComplete();

  if (n != -1) {
    d.fillRect(10, 20, 220, 60, BLACK);
    d.setCursor(10, 20);
    d.setTextColor(CYAN, BLACK);

    if (n == 0) {
      d.print("No spirits found.");
      WiFi.scanDelete();
      WiFi.scanNetworks(true, true);
    }
    else if (n > 0) {
      d.print("Entities: ");
      d.print(n);
      d.print(" | RSSI: ");
      d.print(WiFi.RSSI(0));
      d.print(" dBm");

      int currentRssi = WiFi.RSSI(0);
      int mappedHeight = map(currentRssi, -90, -30, 98, 46);
      mappedHeight = constrain(mappedHeight, 46, 98);
      rssiHistory[historyIndex] = mappedHeight;
      historyIndex = (historyIndex + 1) % graphWidth;
       
      WiFi.scanDelete();
      WiFi.scanNetworks(true, true);
    }
  } else {
    d.setCursor(10, 20);
    d.setTextColor(CYAN, BLACK);
    d.print("Scanning airwaves...");
  }
  d.drawRect(10, 45, 115, 55, CYAN);
  d.setCursor(130, 45);
  d.print("-30dBm");
  d.setCursor(130, 92);
  d.print("-90dBm");
  d.fillRect(11, 46, 113, 53, BLACK);

  for (int i = 0; i < graphWidth; i++) {
    int idx1 = (historyIndex + i) % graphWidth;
    int idx2 = (historyIndex + i + 1) % graphWidth;

    int x1 = 12 + i;
    int x2 = 13 + i;
    int y1 = rssiHistory[idx1];
    int y2 = rssiHistory[idx2];

    if (y1 == 0) y1 = 98;
    if (y2 == 0) y2 = 98;

    d.drawLine(x1, y1, x2, y2, GREEN);
  }
  d.setCursor(10, 110);
  d.setTextColor(RED, BLACK);
  d.print("ESC: Menu");
  d.setTextColor(CYAN, BLACK);
}
//ouija board
void runSpiritBoard() {
  auto& d = M5Cardputer.Display;
  d.setCursor(10, 5);
  d.setTextSize(1);
  d.print("--- DIGITAL OUIJA BOARD ---");
  d.drawRect(15, 30, 210, 60, MAGENTA);

    d.fillRect(17, 32, 206, 56, BLACK);
    d.setCursor(30, 52);
    d.setTextSize(2);
    d.setTextColor(MAGENTA, BLACK);
    d.print(activeMessage);

    d.setTextSize(1);
    d.setCursor(10, 110);
    d.setTextColor(RED, BLACK);
    d.print("ESC: Menu | Type to Ask");
    d.setTextColor(CYAN, BLACK); 
    }
//BLE scanner
void runBtScanner() {
  auto& d = M5Cardputer.Display;
  d.setTextColor(GREEN, BLACK);

  static bool btFirstRun = true;
  if (btFirstRun) {
    d.fillScreen(BLACK);
    d.setCursor(10, 5);
    d.setTextSize(1);
    d.print("--- SPOOKTOOTH RADAR ---");

    d.drawCircle(180, 70, 35, GREEN);
    d.drawCircle(180, 70, 20, GREEN);
    d.drawLine(180, 35, 180, 105, GREEN);
    d.drawLine(145, 70, 215, 70, GREEN);

    d.setCursor(10, 110);
    d.setTextColor(RED, BLACK);
    d.print("ESC: Menu");
    d.setTextColor(GREEN, BLACK);
    btFirstRun = false;
  }

  BLEScanResults* foundDevices = pBLEScan->start(1, false);
  bluetoothDevicesFound = foundDevices->getCount();
  pBLEScan->clearResults();

  d.fillRect(10, 50, 120, 40, BLACK);
  d.setCursor(10, 45);
  d.setTextSize(1);
  d.setTextColor(GREEN, BLACK);
  d.print("Nearby BT:");

  d.setCursor(10, 65);
  d.setTextSize(2);
  d.setTextColor(GREEN, BLACK);
  d.print(bluetoothDevicesFound);

  if (bluetoothDevicesFound > 0) {
    for (int i = 0; i < min(bluetoothDevicesFound, 5); i++) {
     int blipX = 180 + random(-28, 29);
     int blipY = 70  + random(-28, 29);

     d.fillCircle(blipX, blipY, 2, RED);
     delay (200);
     d.fillCircle(blipX, blipY, 2, BLACK);
    }
    d.drawLine(180, 35, 180, 105, DARKGREEN);
    d.drawLine(145, 70, 215, 70, DARKGREEN);
  }
}
// motiondetector
float lastAccelDisplay = -1.0;

void runMotionDetector() {
  auto& d = M5Cardputer.Display;

  static bool firstRun = true;
  if (firstRun) {
    d.fillScreen(BLACK);
    d.setCursor(10, 5);
    d.setTextSize(1);
    d.print("--- IMU MOTION SENSOR ---");
    d.setCursor(10, 30);
    d.print("Tap/Vibration Sensor Active");

    d.setCursor(10, 110);
    d.setTextColor(RED, BLACK);
    d.print("ESC: Menu");
    d.setTextColor(CYAN, BLACK);
    firstRun = false;
  }
 
  M5.Imu.update();
  auto imuData = M5.Imu.getImuData();
  float accelMagnetude = sqrt ((imuData.accel.x * imuData.accel.x) +
                               (imuData.accel.y * imuData.accel.y) +
                               (imuData.accel.z * imuData.accel.z));
if (abs(accelMagnetude - lastAccelDisplay) > 0.05) {
    d.fillRect(10, 55, 220, 35, BLACK);
    d.setCursor(10, 55);
    d.setTextSize(1);
    d.print("Force Level: ");
    d.print(accelMagnetude, 2);

  if (accelMagnetude > 1.3) {
    d.setCursor(10, 75); 
    d.setTextColor(RED, BLACK);
    d.print(">>> DISTURBANCE DETECTED! <<<");
    d.setTextColor(CYAN, BLACK);
    delay(200);
  }
  lastAccelDisplay = accelMagnetude;
}
}
//settings
void runSettings() {
  auto& d = M5Cardputer.Display;

  if (screenBrightness != lastBrightness) {
  d.fillRect(10, 25, 220, 80, BLACK);
  d.setCursor(10, 25);
  d.setTextSize(1);
  d.print("--- DEVICE SETTINGS ---");
  d.setCursor(10, 50);
  d.print("Brightness: ");
  d.print(screenBrightness);
  d.print(" / 255");

  d.setCursor(10, 110);
  d.setTextColor(RED, BLACK);
  d.print("ESC: Menu");
  d.setTextColor(CYAN, BLACK);
  lastBrightness = screenBrightness;
  }
}


void setup() {
  auto cfg = M5.config();
  auto& d = M5Cardputer.Display;
  M5Cardputer.begin(cfg);
  d.setRotation(1);
  d.setTextColor(CYAN, BLACK);
  d.setBrightness(screenBrightness);
//wifi
WiFi.mode(WIFI_STA);
WiFi.disconnect();
// BLE
BLEDevice::init("PhantomFinder");
pBLEScan = BLEDevice::getScan();
pBLEScan->setActiveScan(true);

d.setCursor(10, 10);
d.setTextSize(2);
d.print("Phantom Finder v0.5");
delay(500);
d.setTextSize(1);
d.setCursor(10, 40);
d.print("BOOTING...");
delay(500);
d.setCursor(10, 60);
d.print("Press OK to Start");
}
void loop () {
  M5Cardputer.update();
  auto& d = M5Cardputer.Display;
  //boot screen info
  if (currentState == STATE_BOOT) {
    if (M5Cardputer.Keyboard.isChange()) {
      if (M5Cardputer.Keyboard.isPressed()) {
        Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
        if (status.enter) {
          currentState = STATE_MENU;
          drawMenu();
        }
      }
    }
  }
else if (currentState == STATE_MENU) {
  if (M5Cardputer.Keyboard.isChange()) {
    if (M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
      
      for (auto c : status.word) {
        if (c == ';') {
          currentIndex--;
          if (currentIndex < 0) currentIndex = menuSize - 1;
          }
          else if (c == '.') {
            currentIndex++;
            if (currentIndex >= menuSize) currentIndex = 0;
          }
      }
      if (status.enter) {
        if (currentIndex == 0) {
          currentState = STATE_SCANER;
          d.fillScreen(BLACK);
          WiFi.scanNetworks(true, true);
        }
        else if (currentIndex == 1) {
          currentState = STATE_SPIRIT_BOX;
          d.fillScreen(BLACK);
        }
        else if (currentIndex == 2) {
          currentState = STATE_BT_SCANNER;
          d.fillScreen(BLACK);
        }   
       else if (currentIndex == 3) {
        currentState = STATE_MOTION;
        d.fillScreen(BLACK);
      }
      else if (currentIndex == 4) {
        currentState = STATE_SETTINGS;
        lastBrightness = -1;
        d.fillScreen(BLACK);
      }
      } else {
        drawMenu();
      }
    }
  }
}
else if (currentState == STATE_SCANER) {
   runScanner();

  if (M5Cardputer.Keyboard.isPressed()) {
     if (M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
      for (auto c : status.word) {
        if (c == '`') {
          currentState = STATE_MENU;
          drawMenu();
        }
      }
     }
  }
  delay(50);
}
else if (currentState == STATE_MOTION) {
  runMotionDetector();
 if (M5Cardputer.Keyboard.isPressed()) {
     if (M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
      for (auto c : status.word) {
        if (c == '`') {
          currentState = STATE_MENU;
          drawMenu();
        }
      }
    }
    delay(50);
  }
}
else if (currentState == STATE_SPIRIT_BOX) {
    runSpiritBoard();

  if (M5Cardputer.Keyboard.isChange()) {
    if (M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
      for (auto c : status.word) {
        if (c == '`') {
          currentState = STATE_MENU;
          drawMenu();
        }
        else {
          int randomIndex = random(0, 8);
          activeMessage = ouijaMessages[randomIndex];
        }
      }
    }
  }
  delay(30);
}
else if (currentState == STATE_BT_SCANNER) {
  runBtScanner();

  if (M5Cardputer.Keyboard.isChange()) {
    if (M5Cardputer.Keyboard.isPressed()){
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
      for (auto c : status.word) {
        if (c == '`') {
          currentState = STATE_MENU;
          drawMenu();
        }
      }
    }
    delay(50);
  }
}
else if (currentState == STATE_SETTINGS) {
  runSettings();

  if (M5Cardputer.Keyboard.isChange()) {
    if (M5Cardputer.Keyboard.isPressed()) {
      Keyboard_Class::KeysState status = M5Cardputer.Keyboard.keysState();
      for (auto c : status.word) {
        if (c == '.') {screenBrightness = max(20, screenBrightness - 25);
        d.setBrightness(screenBrightness);
        }
        else if (c == ';') {
          screenBrightness = min(255, screenBrightness + 25);
          d.setBrightness(screenBrightness);
        }
        else if (c == '`') {
          currentState = STATE_MENU;
          drawMenu();
        }
      }
    }
  }
  delay(50);
}
}