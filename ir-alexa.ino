  //#include "esp_camera.h"
  //#define DEBUG_FAUXMO_VERBOSE true
  #include "fauxmoESP.h"
  #include <WiFi.h>
  #include <IRremote.h>
  
  #define TV_POWER (0xFE50AF)
  #define TV_MUTE (0xFE0AF5)
  #define TVVOL_UP (0xFE7887)
  #define TVVOL_DN (0xFEFA05)
  #define TV_UP (0xFE7A85)
  #define TV_DN (0xFE6A95)
  #define TV_LFT (0xFEDA25)
  #define TV_RGT (0xFE1AE5)
  #define TV_OK (0xFE5AA5)
  #define TV_UTUB (0xFE56A9)
  #define TV_NETFX (0xFE7689)
  #define TV_BACK (0xFEB649)
  #define TV_EXIT (0xFE728D)
  
  #define MAX_CMD 13
  const char *voice_command[]={"TV Power","TV Mute","TV Voulume UP","TV Voulume Down","TV UP","TV Down","TV Left","TV Right","TV OK","TV Youtube","TV Netflix","TV Back","TV Exit"};
  const long int ircommand[]={TV_POWER,TV_MUTE ,TVVOL_UP,TVVOL_DN,TV_UP ,TV_DN ,TV_LFT,TV_RGT,TV_OK ,TV_UTUB ,TV_NETFX,TV_BACK ,TV_EXIT };
  
  
  const int RECV_PIN = 13;
  
  byte SEND_PIN = 2;
  IRrecv irrecv(RECV_PIN);
  IRsend irsend (SEND_PIN);
  decode_results results;
  #define SERIAL_BAUDRATE               (115200)
  
  fauxmoESP fauxmo;
  const char* ssid = "Error";
  const char* password = "ReshmiAthul7";
  int i=0, ir_cmd_no=-1;
  void setup() {
    
    Serial.begin(SERIAL_BAUDRATE);
    pinMode(SEND_PIN, OUTPUT);
    Serial.println();
    Serial.println();
    Serial.println("IR demo sketch");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) { 
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("My Ip");
    Serial.print(WiFi.localIP());
    fauxmo.createServer(true); // not needed, this is the default value
    delay(100);
    fauxmo.setPort(80); // This is required for gen3 devices
    delay(100);
    fauxmo.enable(true);
    delay(100);
    for(i=0;i<MAX_CMD;i++){
      fauxmo.addDevice(voice_command[i]);
      Serial.println(voice_command[i]);
      delay(10);
    }
    
    //fauxmo.onMessage(callback);
    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    
    
      Serial.print("New CMD\r\n");
      state=true;
      for(i=0;i<MAX_CMD;i++){
      
      if ( (strcmp(device_name, voice_command[i]) == 0) ) {
      
        ir_cmd_no=i;
        delay(10);
        break;
      }
      
      }
      
      });
    
  
  }
  
  void loop() {
    
    fauxmo.handle();
    delay(500);
    if(ir_cmd_no!=-1)
      irsend.sendNEC(ircommand[ir_cmd_no], 32);
    ir_cmd_no=-1;
  }
