// Google sheet publish code based from HTTPSRedirect examples

#include <ESP8266WiFi.h>
#include <HTTPSRedirect.h>
#include <stdio.h>
#include <time.h>

#define humidity_up 0
#define humidity_dn 100
#define airTemp_up 22
#define airTemp_dn 34
#define waterTemp_up 22
#define waterTemp_dn 34
#define waterPH_up 5
#define waterPH_dn 9
#define lightIntensity_up 0
#define lightIntensity_dn 100
#define atmCO2_up 0
#define atmCO2_dn 100

struct SensorData{
  float humidity;
  float airTemp;
  float waterTemp;
  float waterPH;
  float lightIntensity;
  float atmCO2;
};

// Place Wifi credentials here
const char* ssid = "Globehack2017";
const char* psk = "Hackathon2017";

// The ID below comes from Google Sheets.
// Towards the bottom of this page, it will explain how this can be obtained
// https://script.google.com/macros/s/AKfycbyONEtYdBb5UEWZy_njdVk-dU732gtTTvzttx1-FlVJ0xY9PYjV/exec
const char *GScriptId = "AKfycbyONEtYdBb5UEWZy_njdVk-dU732gtTTvzttx1-FlVJ0xY9PYjV";

const char* host = "script.google.com";
const char* googleRedirHost = "script.googleusercontent.com";

const int httpsPort =     443;
HTTPSRedirect client(httpsPort);

// Prepare the url (without the varying data)
String url = String("/macros/s/") + GScriptId + "/exec?";

const char* fingerprint = "F0 5C 74 77 3F 6B 25 D7 3B 66 4D 43 2F 7E BC 5B E9 28 86 AD";

int data;

void setup(){
  Serial.begin(115200);
  delay(5000);
  Serial.println("Boot up\nConnecting to Wifi");
  WiFi.begin(ssid,psk);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);Serial.print(".");
  }
  Serial.print("\nConnected. IP Address = ");
  Serial.println(WiFi.localIP());

  bool flag = false;
  while(1){
          int retval = client.connect(host, httpsPort);
          if (retval == 1) {
                      flag = true;
                      break;
          }
          else
                  Serial.println("Connection failed. Retrying…");
  }

  // Connection Status, 1 = Connected, 0 is not.
  Serial.println("Connection Status: " + String(client.connected()));
  Serial.flush();

  if (!flag){
          Serial.print("Could not connect to server: ");
          Serial.println(host);
          Serial.println("Exiting…");
          Serial.flush();
          return;
  }

  // Data will still be pushed even certification don’t match.
  if (client.verify(fingerprint, host)) {
          Serial.println("Certificate match.");
  } else {
          Serial.println("Certificate mis-match");
  }

}

// This is the main method where data gets pushed to the Google sheet
void postData(struct SensorData _sData ){

    if (!client.connected()){
            Serial.println("Connecting to client again…");
            client.stop();
            client.connect(host, httpsPort);
    }

    String urlFinal = url + "humidity=" + String(_sData.humidity)
                          + "&airTemp=" + String(_sData.airTemp)
                          + "&waterTemp=" + String(_sData.waterTemp)
                          + "&waterPH=" + String(_sData.waterPH)
                          + "&lightIntensity=" + String(_sData.lightIntensity)
                          + "&atmCO2=" + String(_sData.atmCO2);
    Serial.print("Connecting to ");
    Serial.println(urlFinal);
    if(client.GET(urlFinal, host)){
      Serial.print("Data Sent #");
      Serial.println(data++);
    }
    client.stop();
}

// Read sensor data Here
// Just generating random sensor data since cant deploy sensors here
SensorData sensorRead(){
  SensorData _sdata;

  // replace with sensor codes here if deployed in farms
  _sdata.humidity = (rand() % (humidity_up - humidity_dn)) + humidity_up;
  _sdata.airTemp = (rand() % (airTemp_up - airTemp_dn)) + airTemp_up;
  _sdata.waterTemp = (rand() % (waterTemp_up - waterTemp_dn)) + waterTemp_up;
  _sdata.waterPH = (rand() % (waterPH_up - waterPH_dn)) + waterPH_up;
  _sdata.lightIntensity = (rand() % (lightIntensity_up - lightIntensity_dn)) + lightIntensity_up;
  _sdata.atmCO2 = (rand() % (atmCO2_up - atmCO2_dn)) + atmCO2_up;

  return _sdata;
}

void loop(){
  // Generate random seed
  srand(time(NULL));
  // Data Struct Var
  SensorData sdata;

  // enter while loop
  while (1) {

    // Read sensor data
    sdata = sensorRead();
    // Post sensor data to Google sheet
    postData(sdata);

    delay(3000);
  }
}

// https://script.google.com/macros/s/AKfycbyONEtYdBb5UEWZy_njdVk-dU732gtTTvzttx1-FlVJ0xY9PYjV/exec
