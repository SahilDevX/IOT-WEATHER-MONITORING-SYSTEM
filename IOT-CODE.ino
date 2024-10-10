#include <UnoWiFiDevEd.h>

/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL3I_6kDJ8I"
#define BLYNK_TEMPLATE_NAME "Weather station"
#define BLYNK_AUTH_TOKEN "pxXHYeZhbV94DaGmFMWWVxRLZdEg8tqO"
#include <DHT.h>
#include <Adafruit_BMP085.h>

#define DHTPIN 13      // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 

#define BMP_SDA 21      // I2C SDA pin
#define BMP_SCL 22      // I2C SCL pin
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "OnePlus 8";
char pass[] = "Qwerty01";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
void sendSensorData() {
  float tempDHT = dht.readTemperature();  // Read temperature from DHT sensor
  float hum = dht.readHumidity();       // Read humidity
  
  float tempBMP = bmp.readTemperature();   // Read temperature from BMP180 sensor
  float pressure = bmp.readPressure() / 100.0F; // Read pressure from BMP180 sensor (converted to hPa)

  if (!isnan(tempDHT) && !isnan(hum) && !isnan(tempBMP)) {
    Blynk.virtualWrite(V0, tempDHT);      // Send DHT temperature data to V5
    Blynk.virtualWrite(V1, hum);          // Send DHT humidity data to V6
    Blynk.virtualWrite(V2, tempBMP);      // Send BMP temperature data to V7
    Blynk.virtualWrite(V3, pressure);     // Send BMP pressure data to V8
  }
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void setup() {
  Serial.begin(115200);
  dht.begin();
  
   Wire.begin(BMP_SDA, BMP_SCL);
  if (!bmp.begin()) {
    Serial.println("Could not find BMP180 sensor, check wiring!");
    while (1);
  }

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  timer.setInterval(1000L, sendSensorData);  // Send sensor data every 5 seconds
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}

