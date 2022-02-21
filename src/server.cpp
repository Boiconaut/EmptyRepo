#include <SD.h>
#include "server.h"
#include "resourses/markup/Data.h"
#include "resourses/markup/Login.h"
#include "resourses/markup/Alert.h"
#include "resourses/markup/NotFound.h"
#include "resourses/styles/Style.h"
#include "resourses/scripts/DataScript.h"
#include "resourses/scripts/LoginScript.h"
#include "config.h"

ServerHandler::ServerHandler(){

}

ServerHandler::~ServerHandler(){
  
}

String ServerHandler::getAuthToken(){
    unsigned long token = random(998) + 1;
    return String(token);
}

String ServerHandler::htmlData(){
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

String ServerHandler::htmlInvalid(){
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

String ServerHandler::htmlLogin(){
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

String ServerHandler::htmlNotFound(){
    String ptr = "<!DOCTYPE html> <html>\n";
    String h = webheader;
    ptr += h;
    String nf = not_found_html;
    ptr += nf;
    return ptr;
}

void ServerHandler::handle_onAuth(AsyncWebServerRequest *request, String data){
    String token;
    String body = data;
  
    String correct = "{\"login\":\"user\",\"password\":\"bms\"}";
  
    if(body == correct){
      token = getAuthToken();
      valid_token = token;
      IsAuth = true;
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

void ServerHandler::handle_Prognosis(AsyncWebServerRequest *request, String data, SensorsHandler *sensors){
    String body = data;
    int str_length = body.length() + 1;
    char char_array[str_length];
    body.toCharArray(char_array, str_length);
    
    float capacity = atof(char_array);
    sensors->SetNomCapacity((int)(capacity));
    float time_past = sensors->GetSeconds() + (sensors->GetMinutes() * 60) + (sensors->GetHours() * 3600);
    float time_all = (capacity / (0 - sensors->GetAverageCurrent())) * 3600;
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
    if(sensors->GetAverageCurrent() > 0 || time_past < 60){
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

void ServerHandler::assembleData(SensorsHandler *sensors, MotoHandler *moto, ClockHandler *clk, ErrorHandler *error){
    #ifndef RELEASE
    #endif

    json["voltage"] = String(sensors->GetVoltage());
    json["current"] = String(sensors->GetCurrent());
    json["power"] = String(sensors->GetPower());
    json["soc"] = String(sensors->GetSOC());

    String date_hour = String(clk->GetHours());
    String date_minute = clk->GetMinutes() < 10 ? "0" + String(clk->GetMinutes()) : String(clk->GetMinutes());
    String date_second = clk->GetSeconds() < 10 ? "0" + String(clk->GetSeconds()) : String(clk->GetSeconds());
    String datetime = date_hour + ":" + date_minute + ":" + date_second;
    json["datetime"] = datetime;

    String hour = String(moto->GetHours());
    String minute = moto->GetMinutes() < 10 ? "0" + String(moto->GetMinutes()) : String(moto->GetMinutes());
    String second = moto->GetSeconds() < 10 ? "0" + String(moto->GetSeconds()) : String(moto->GetSeconds());
    String mototime = hour + ":" + minute + ":" + second;
    json["time"] = mototime;

    String state_time = String(sensors->GetHours()) + ":" + String(sensors->GetMinutes()) 
                          + ":" + String(sensors->GetSeconds());
    json["state_time"] = state_time;

    String current_state;
    if(sensors->STATE == STATE_CHARGE) current_state = "В заряде:";
    else if (sensors->STATE == STATE_DISCHARGE) current_state = "В разряде:";
    else current_state = "Релаксация";
    json["state"] = current_state;

    json["temperature"] = clk->GetTemperature();
    json["capacity"] = String(sensors->GetCapacity()).substring(0, String(sensors->GetCapacity()).length() - 3);

    String error_state = error->ERROR_CODE ? "Ошибка" : "Ошибок нет";
    json["error_state"] = error_state;

    String error_code = "";
    if(bitRead(error->ERROR_CODE, ERROR_LOW_VOLTAGE)) { error_code += "<h3>Низкое напряжение</h3>"; }
    if(bitRead(error->ERROR_CODE, ERROR_OVERCURRENT)) { error_code += "<h3>Перегрузка по току</h3>"; }
    if(bitRead(error->ERROR_CODE, ERROR_NO_SD)) { error_code += "<h3>Отсутствует SD-карта</h3>"; }
    json["error"] = error_code;
}

void ServerHandler::GetCredentials(ClockHandler *clk, Screen *screen, SensorsHandler *sensors){
    File f = SD.open("/config.json", FILE_READ);
    if(f){
        DynamicJsonDocument doc(1024);
        String s = f.readString();
        deserializeJson(doc, s);
        JsonObject obj = doc.as<JsonObject>();
        String s1 = obj["ssid"];
        String s2 = obj["password"];
        net_ssid = s1.c_str();
        net_password = s2.c_str();
        f.close();
        
        #ifndef RELEASE
          Serial.print("Connecting to ");
          Serial.print(net_ssid);
          Serial.print(" with password ");
          Serial.println(net_password);
        #endif

        WiFi.begin(net_ssid, net_password);

        screen->setTextSize(0);
        screen->setTextColor(1);
        uint8_t n = 0;

        while (WiFi.status() != WL_CONNECTED) 
        {
          delay(1000);
          screen->clearDisplay();
          screen->setCursor(1, 1);
          screen->print("Network: ");
          screen->print(net_ssid);
          screen->setCursor(1, 10);
          for(uint8_t i = 0; i < n; i++) screen->print(".");
          n++;
          if(n > 5) n = 0;
          screen->display();

          #ifndef RELEASE
            Serial.print(".");
          #endif
        }
        #ifndef RELEASE
          Serial.println("");
          Serial.println("WiFi connected..!");
          Serial.print("Got IP: ");
        #endif 

        screen->clearDisplay();
        screen->setCursor(1, 1);
        screen->print("Network: ");
        screen->print(net_ssid);
        screen->setCursor(1, 10);
        screen->print("Wifi connected!");
    
        timeClient.begin();
        timeClient.setTimeOffset(10800);
        timeClient.update();
        clk->SyncTime(&timeClient, sensors);
    }
}

NTPClient* ServerHandler::NTP(){
  return &timeClient;
}

void ServerHandler::SetupServer(Screen *screen){
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
          handle_Prognosis(request, dat, getSensors());
        }
      });
    
    server.on("/", HTTP_GET, [&](AsyncWebServerRequest *request) mutable{
      IsAuth = false;
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
        else{
          request->send(200, "text/html", htmlInvalid());
        }
    });

    server.on("/getdata", HTTP_GET, [&](AsyncWebServerRequest *request){
        assembleData(getSensors(), getMoto(), getClock(), getError());
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
      
      screen->setCursor(1, 19);
      screen->print("IP: ");
      screen->print(WiFi.localIP());
      screen->setCursor(1, 28);
      screen->println("HTTP server started");
      screen->setCursor(1, 37);
      screen->print("Mac address: ");
      screen->setCursor(1, 46);
      screen->print(WiFi.macAddress());
      screen->display();

      #ifndef RELEASE
        Serial.println("HTTP server started");
        Serial.print("Mac address: ");
        Serial.print(WiFi.macAddress());
        Serial.print("\n");
      #endif
}