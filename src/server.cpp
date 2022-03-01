#include <SD.h>
#include "server.h"
#include "resourses/markup/Data.h"
#include "resourses/markup/Login.h"
#include "resourses/markup/Alert.h"
#include "resourses/markup/NotFound.h"
#include "resourses/styles/Style.h"
#include "resourses/scripts/DataScript.h"
#include "resourses/scripts/LoginScript.h"
#include "resourses/markup/Admin.h"
#include "resourses/scripts/AdminScript.h"
#include "config.h"

IPAddress ip = IPAddress(192, 168, 0, 44);
IPAddress gateway = IPAddress(192, 168, 0, 1);
IPAddress subnet = IPAddress(255, 255, 255, 0);

AsyncWebServer server = AsyncWebServer(80);
WiFiUDP ntpUDP = WiFiUDP();
NTPClient timeClient = NTPClient(ntpUDP);
DynamicJsonDocument json = DynamicJsonDocument(512);

const char* user_login;
const char* user_password;
const char* net_ssid;
const char* net_password;

String valid_token;
boolean IsAuth = false;
boolean IsAdmin = false;

String getAuthToken(){
    unsigned long token = random(998) + 1;
    return String(token);
}

String htmlData(){
    String ptr = "<!DOCTYPE html> <html>\n";
    String h = webheader;
    ptr += h;
    String data = data_html;
    ptr += data;
    String s = script;
    ptr += s;
    ptr +="</body>\n";
    ptr +="</html>\n";
    return ptr;
}

String htmlDataAdmin(){
    String ptr = "<!DOCTYPE html> <html>\n";
    String h = webheader;
    ptr += h;
    String data = data_html;
    ptr += data;
    String s = script;
    ptr += s;

    String admin = admin_html;
    ptr += admin_html;
    String ads = admin_script;
    ptr += ads;
    
    ptr +="</body>\n";
    ptr +="</html>\n";
    
    return ptr;
}

String htmlInvalid(){
    String ptr = "<!DOCTYPE html> <html>\n";
    String h = webheader;
    ptr += h;
    String alert = alert_html;
    ptr += alert;
    String login = login_html;
    ptr += login;
    String s = login_script;
    ptr += s;
    ptr +="</body>\n";
    ptr +="</html>\n";
    return ptr;
}

String htmlLogin(){
    String ptr = "<!DOCTYPE html> <html>\n";
    String h = webheader;
    ptr += h;
    String login = login_html;
    ptr += login;
    String s = login_script;
    ptr += s;
    ptr +="</body>\n";
    ptr +="</html>\n";
    return ptr;
}

String htmlNotFound(){
    String ptr = "<!DOCTYPE html> <html>\n";
    String h = webheader;
    ptr += h;
    String nf = not_found_html;
    ptr += nf;
    return ptr;
}

void handle_onAuth(AsyncWebServerRequest *request, String data){
    String token;
    String body = data;
  
    String correct = "{\"login\":\"user\",\"password\":\"bms\"}";
    String correct_admin = "{\"login\":\"admin\",\"password\":\"bms\"}";
  
    if(body == correct){
      token = getAuthToken();
      valid_token = token;
      IsAuth = true;
    }
    else if(body == correct_admin){
      token = getAuthToken();
      valid_token = token;
      IsAdmin = true;
    }
    else{
      token = "0";
      valid_token = "null";
    }
  
    String json_token = "{\"token\":\"";
    json_token += token;
    json_token += "\"}";
    
    request->send(200, "text/json", json_token); 
}

void handle_Prognosis(AsyncWebServerRequest *request, String data){
    String body = data;
    int str_length = body.length() + 1;
    char char_array[str_length];
    body.toCharArray(char_array, str_length);
    
    float capacity = atof(char_array);
    _capacity = (int)(capacity);
    float time_past = state_time.seconds() + (state_time.minutes() * 60) + (state_time.hours() * 3600);
    float time_all = (capacity / (0 - average_current)) * 3600;
    float time_fore = time_all - time_past;

    if(time_fore < 0){
    time_fore = 0;
    }
    float hours_fore = floor(time_fore / 3600);
    float minutes_fore = (time_fore - (hours_fore * 3600)) / 60;

    if(String(hours_fore) == "inf" || String(minutes_fore) == "nan"){
    minutes_fore = 0.00;
    hours_fore = 999999999.00;
    }

    String prognosis = "Приблизительное остаточное время работы: ";
    if(average_current > 0 || time_past < 60){
    prognosis += "Для расчёта должно пройти не менее минуты с момента начала разряда";
    }
    else{
        prognosis += String(hours_fore).substring(0, String(hours_fore).length() - 3);
        prognosis += ":";
        if(minutes_fore < 10){
        prognosis += "0";
        }
        prognosis += String(minutes_fore).substring(0, String(minutes_fore).length() - 3);
    }

    request->send(200, "text/plain", prognosis);
}

void handle_CalibrateVoltage(AsyncWebServerRequest *request, String data){
    String body = data;
    int str_length = body.length() + 1;
    char char_array[str_length];
    body.toCharArray(char_array, str_length);
    float v = atof(char_array);

    if(voltage != 0) kVD *= (v / voltage);
    if(kVD == 0.00) kVD = 0.1;
    request->send(200, "text/plain", String(v));
}

void handle_CalibrateCurrent(AsyncWebServerRequest *request, String data){
  String body = data;
  int str_length = body.length() + 1;
  char char_array[str_length];
  body.toCharArray(char_array, str_length);
  float cur = atof(char_array);

  if(current != 0) kI *= (cur / current);
  if(kI == 0.00) kI = 0.1;
  request->send(200, "text/plain", String(cur));
}

void handle_SetCapacity(AsyncWebServerRequest *request, String data){
  String body = data;
  int str_length = body.length() + 1;
  char char_array[str_length];
  body.toCharArray(char_array, str_length);
  float capacity = atof(char_array);
  
  NOM_CAPACITY = (uint8_t)(capacity);
  request->send(200, "text/plain", String(NOM_CAPACITY));
}

void assembleData(){
    #ifndef RELEASE
    #endif

    json["voltage"] = String(voltage);
    json["current"] = String(current);
    json["power"] = String(power);
    json["soc"] = String(SOC);

    String date_hour = String(_now.hour());
    String date_minute = _now.minute() < 10 ? "0" + String(_now.minute()) : String(_now.minute());
    String date_second = _now.second() < 10 ? "0" + String(_now.second()) : String(_now.second());
    String datetime = date_hour + ":" + date_minute + ":" + date_second;
    json["datetime"] = datetime;

    String hour = String(motohour);
    String minute = moto_time.minutes() < 10 ? "0" + String(moto_time.minutes()) : String(moto_time.minutes());
    String second = moto_time.seconds() < 10 ? "0" + String(moto_time.seconds()) : String(moto_time.seconds());
    String moto = hour + ":" + minute + ":" + second;
    json["time"] = moto;

    String state_t = String(state_hour) + ":" + String(state_time.minutes()) 
                          + ":" + String(state_time.seconds());
    json["state_time"] = state_t;

    String current_state;
    if(STATE == STATE_CHARGE) current_state = "В заряде:";
    else if (STATE == STATE_DISCHARGE) current_state = "В разряде:";
    else current_state = "Релаксация";
    json["state"] = current_state;

    json["temperature"] = temperature;
    json["capacity"] = String(_capacity).substring(0, String(_capacity).length() - 3);

    String error_state = ERROR_CODE ? "Ошибка" : "Ошибок нет";
    json["error_state"] = error_state;

    String error_code = "";
    if(bitRead(ERROR_CODE, ERROR_LOW_VOLTAGE)) { error_code += "<h3>Низкое напряжение</h3>"; }
    if(bitRead(ERROR_CODE, ERROR_OVERCURRENT)) { error_code += "<h3>Перегрузка по току</h3>"; }
    if(bitRead(ERROR_CODE, ERROR_NO_SD)) { error_code += "<h3>Отсутствует SD-карта</h3>"; }
    json["error"] = error_code;
}

void SetupServer(){
    WiFi.config(ip, gateway, subnet);
    #ifndef RELEASE
      Serial.println(WiFi.localIP());
    #endif
    
    server.onRequestBody([&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) mutable{
        String string_data = "";
        String dat = "";
        dat += (const char*)data;
        
        if(request->url() == "/auth"){
          string_data = dat.substring(0, dat.indexOf("}") + 1);
          handle_onAuth(request, string_data);
        }
        if(request->url() == "/getprognosis"){
          handle_Prognosis(request, dat);
        }
        if(request->url() == "/calibratevoltage"){
          handle_CalibrateVoltage(request, dat);
        }
        if(request->url() == "/calibratecurrent"){
          handle_CalibrateCurrent(request, dat);
        }
        if(request->url() == "/setcapacity"){
          handle_SetCapacity(request, dat);
        }
      });
    
    server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request) mutable{
      IsAuth = false;
      IsAdmin = false;
      request->send(200, "text/html", htmlLogin());
      #ifndef RELEASE
        Serial.println("Called: server.on(\"/\")");
      #endif
    });

    server.on("/data", HTTP_GET, [&](AsyncWebServerRequest *request) mutable{
        #ifndef RELEASE
          Serial.println("Called: server.on(\"/data\")");
         #endif
        
        if(IsAuth){
          request->send(200, "text/html", htmlData());
        }
        else if(IsAdmin){
          request->send(200, "text/html", htmlDataAdmin());
        }
        else{
          request->send(200, "text/html", htmlInvalid());
        }
    });

    server.on("/getdata", HTTP_GET, [&](AsyncWebServerRequest *request){
        assembleData();
        String json_string; 
        serializeJson(json, json_string);
        request->send(200, "text/json", json_string);
        json.clear(); 
      });

      server.on("/downloadlog", HTTP_GET, [](AsyncWebServerRequest *request){
        File file = SD.open("/errors.txt");
        
        if(file){
          if(file.available()){
            AsyncWebServerResponse *response = request->beginResponse(SD, "/errors.txt");
            request->send(response);
            file.close();
          }
        }
        else{
          request->send(404, "text/plain", "File not found");
        }
      });

      server.on("/downloadlogfile", HTTP_GET, [](AsyncWebServerRequest *request){
        File file = SD.open("/datalog.txt");
        
        if(file){
          if(file.available()){
            AsyncWebServerResponse *response = request->beginResponse(SD, "/datalog.txt");
            request->send(response);
            file.close();
          }
        }
        else{
          request->send(404, "text/plain", "File not found");
        }
      });

      server.onNotFound([&](AsyncWebServerRequest *request) mutable{
          request->send(404, "text/html", htmlNotFound());
      });
      server.begin();
      
      lcd.setCursor(1, 19);
      lcd.print("IP: ");
      lcd.print(WiFi.localIP());
      lcd.setCursor(1, 28);
      lcd.println("HTTP server started");
      lcd.setCursor(1, 37);
      lcd.print("Mac address: ");
      lcd.setCursor(1, 46);
      lcd.print(WiFi.macAddress());
      lcd.display();

      #ifndef RELEASE
        Serial.println("HTTP server started");
        Serial.print("Mac address: ");
        Serial.print(WiFi.macAddress());
        Serial.print("\n");
      #endif
}