#include <WiFi.h>  // برای اتصال به وای‌فای
#include <DHT.h>    // برای کار با سنسور دما DHT
#include <HTTPClient.h>  // برای ارسال داده‌ها به سرور

#define DHTPIN 17       // پین داده سنسور دما
#define DHTTYPE DHT22   // نوع سنسور (DHT11 یا DHT22)

const char* ssid = "your-SSID";           // نام شبکه وای‌فای
const char* password = "your-PASSWORD";   // رمز عبور شبکه وای‌فای
const char* server = "http://your-server.com/update"; // آدرس سرور برای ارسال داده‌ها

DHT dht(DHTPIN, DHTTYPE);  // راه‌اندازی سنسور دما

void setup() {
  Serial.begin(115200);  // شروع ارتباط سریال
  WiFi.begin(ssid, password);  // اتصال به وای‌فای
  
  // تلاش برای اتصال به وای‌فای
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi!");
  dht.begin();  // شروع به کار سنسور دما
}

void loop() {
  // خواندن دمای محیط از سنسور DHT
  float temperature = dht.readTemperature();
  
  // چک کردن اگر دما به درستی خوانده شده
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // نمایش دما در سریال مانیتور
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // ارسال داده‌ها به سرور
  HTTPClient http;
  
  // اتصال به سرور
  http.begin(server);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");  // تعیین نوع داده‌ها

  // داده‌هایی که به سرور ارسال می‌کنیم
  String httpRequestData = "temp=" + String(temperature);
  
  // ارسال داده‌ها به سرور
  int httpResponseCode = http.POST(httpRequestData);

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error on sending POST request: ");
    Serial.println(httpResponseCode);
  }

  http.end();  // پایان ارتباط با سرور

  // تاخیر 10 ثانیه‌ای قبل از ارسال دوباره داده‌ها
  delay(10000);
}
