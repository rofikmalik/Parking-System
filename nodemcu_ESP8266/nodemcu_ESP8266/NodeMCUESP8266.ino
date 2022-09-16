//firebase===================================
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
//firebase set
#define FIREBASE_HOST "systemparking-7c9e8-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "XwTGKu5Yr3pEmdfbLp7ucwp2sBQhxlJs8JFSgno3"
//WiFi set
#define WIFI_SSID "tabun"
#define WIFI_PASSWORD "hahahaha"
//led set 
#define ledPin 5 //(D0)

//softwareserial=============================
#include <SoftwareSerial.h>
//variabel softwareserial (Tx(D6),Rx(D7))
SoftwareSerial DataSerial(12,13);

//millis pengganti delay
unsigned long previousMillis = 0;
const long interval = 2000;
//variabel array untuk pecahan
String arrData[7];


void setup(){
  Serial.begin(9600);
  pinMode (ledPin, OUTPUT);

  //firebase
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  digitalWrite(ledPin, HIGH);
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  //connect to firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  //serialbegin=================================
  DataSerial.begin(9600); 
}

void loop(){
  //Software Serial=================================
  //konfigurasi Millis
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval){
    //update previousMillis
    previousMillis = currentMillis;
    //prioritas pembaca arduino (hasil baca sensor)
    String data = "";
    while(DataSerial.available()>0){
      data += char(DataSerial.read());
    }
    data.trim();
    
    //uji data==============================================
    if (data != ""){
      //format data "terisi#kosong"
      //pecahan data
      int index = 0;
      for(int i= 0; i<= data.length(); i++){
        char delimiter = '#';
        if(data[i] != delimiter)
          arrData[index] += data[i];
        else 
          index++; //variabel bertambah 1
      }
    }
//==================================================================

      Serial.println("Parkir1 : "+arrData[0]); // parkiran 1
      Serial.println("Parkir2 : "+arrData[1]); // parkiran 2
      Serial.println("Parkir3 : "+arrData[2]); // parkiran 3
      Serial.println("Parkir4 : "+arrData[3]); // parkiran 4
      Serial.println("Parkir5 : "+arrData[4]); // parkiran 5
      Serial.println("Parkir6 : "+arrData[5]); // parkiran 6
      Serial.println("Slot    : "+arrData[6]); // slot parkir
      Serial.println("");

      Firebase.setString("Parkir1", arrData[0]);
      Firebase.setString("Parkir2", arrData[1]);
      Firebase.setString("Parkir3", arrData[2]);
      Firebase.setString("Parkir4", arrData[3]);
      Firebase.setString("Parkir5", arrData[4]);
      Firebase.setString("Parkir6", arrData[5]);
      Firebase.setString("Slot", arrData[6]);

      arrData[0] = "";
      arrData[1] = "";
      arrData[2] = "";
      arrData[3] = "";
      arrData[4] = "";
      arrData[5] = "";
      arrData[6] = "";

      if(Firebase.failed()) {
          Serial.print("Pengiriman Data Gagal: ");
          Serial.println(Firebase.error());
          return;
         }
      
      Serial.println("Data Terkirim");
      
//==================================================================

 //   minta data arduino
    DataSerial.println("Ya");
  }
}
