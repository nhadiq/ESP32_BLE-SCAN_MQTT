#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <WiFi.h>
#include <PubSubClient.h>
WiFiClient espClient;
PubSubClient client(espClient);
/*msg init*/
String msgk = "abcdefghijklmno12";
char msg[18]= "abcdefghijklmno12";
int i;
// Please input the SSID and password of WiFi
const char* ssid     = "Your-SSID Goes Here";
const char* password = "Your Password Goes Here";
const char* mqtt_server = "Your Server Goes Here";
int scanTime = 5; //In seconds
BLEScan* pBLEScan;
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      //Uncomment the next line if you want to print the entire Advertised BLE List
      //Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
      Serial.printf("%s \n",advertisedDevice.getAddress().toString().c_str());//Print just advertised Device id
      msgk = (advertisedDevice.getAddress().toString().c_str());
       
     for (i = 0; i < 18; i++) { 
        msg[i] = msgk[i]; }
      client.publish("test", msg);
     
    }
};
void mqttconnect() {
  /* Loop until reconnected */
  if (!client.connected()) {
    Serial.print("MQTT connecting ...");
    /* client ID */
    //Fill the Client ID 
    String clientId = "Random Client ID goes here";
    //if using authentication enter Username and Password Below
    String user = "YOUR Username Goes here";
    String pass = "YOUR Password Goes here";
   /* connect now */
    if (client.connect(clientId.c_str(), user.c_str(), pass.c_str())) {
      //If you do not use authentication comment the previous line and uncomment the next line
       if (client.connect(clientId.c_str())) {
      Serial.println("connected");
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}
void setup() {
  Serial.begin(115200);//initialise serial out
  WiFi.begin(ssid, password);//initialise wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }
    Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());//local ip of the device
  client.setServer(mqtt_server, 9001);//9001 specifies the server port change it if required

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}
void loop() {
   if (!client.connected()) {
    mqttconnect();
  }
  BLEScanResults foundDevices = pBLEScan->start(scanTime, false);
  pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory
  delay(3000);
}
