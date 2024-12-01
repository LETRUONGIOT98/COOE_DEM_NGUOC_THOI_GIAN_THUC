#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const char* ssid = "linhkien2t.com";
const char* password = "865khavancan";


const int targetYear = 2023;   // Năm dương lịch của Tết Trung Thu
const int targetMonth = 9;     // Tháng dương lịch của Tết Trung Thu
const int targetDay = 29;      // Ngày dương lịch của Tết Trung Thu

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600); // Đặt máy chủ NTP và sửa đổi múi giờ +7

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  Serial.begin(115200);
  connectToWiFi();
  
  timeClient.begin();
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Countdown to Mid-Autumn");
  display.display();
  delay(2000);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    timeClient.update();
    // Lấy thời gian thực từ máy chủ NTP
    unsigned long epochTime = timeClient.getEpochTime();
    
    // Tính thời gian còn lại đến Tết Trung Thu 2023
    unsigned long secondsLeft = calculateTimeDifference(epochTime);
    
    int daysLeft = secondsLeft / 86400;
    secondsLeft %= 86400;
    int hoursLeft = secondsLeft / 3600;
    secondsLeft %= 3600;
    int minutesLeft = secondsLeft / 60;
    int secondsRemaining = secondsLeft % 60;
    Serial.println(getFormattedTime(epochTime));
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("BAY GIO: " + String(getFormattedTime(epochTime)));
    display.setCursor(42, 8);
    display.print("CON LAI ");
    display.setCursor(8, 16);
    display.print(daysLeft);
    display.print("d ");
    display.print(hoursLeft);
    display.print("h ");
    display.print(minutesLeft);
    display.print("m ");
    display.print(secondsRemaining);
    display.print("s");
    display.setCursor(5, 24);
    display.print("DEN TET TRUNG THU ");
    display.display();
  }
  
  delay(1000); // Đợi 1 giây trước khi cập nhật lại
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
  display.print("DANG KET NOI WIFI:");
  display.setCursor(0, 10);
  display.print("linhkien2t.com");
    display.display();
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

unsigned long calculateTimeDifference(unsigned long currentTime) {
  tm targetTimeinfo = {0, 0, 0, targetDay, targetMonth - 1, targetYear - 1900}; // Chú ý: struct tm sử dụng các giá trị 0-11 cho tháng và năm là năm-1900
  time_t targetTime = mktime(&targetTimeinfo);

  return targetTime - currentTime;
}

String getFormattedTime(unsigned long epochTime) {
  int currentHour = ((epochTime % 86400L) / 3600 + 7) % 24; // Sửa đổi múi giờ +7
  int currentMinute = (epochTime % 3600) / 60;
  int currentSecond = epochTime % 60;

  String formattedTime = String(currentHour) + ":" + (currentMinute < 10 ? "0" : "") + String(currentMinute) + ":" + (currentSecond < 10 ? "0" : "") + String(currentSecond);
  return formattedTime;
}
