#ifndef DATASCRIPT_H
#define DATASCRIPT_H

#include <Arduino.h>

const char script[] PROGMEM = R"=====(

<script>
const prognosisForm = document.getElementById("prognosisForm");
const prognosisField = document.getElementById("prognosis-input");
const prognosisDisplay = document.getElementById("prognosis");

const state_tr = document.getElementById("state-tr");

setInterval(function(){
        getData();
    }, 1000);
    
function getData() {
    let xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
        if(this.readyState == 4 && this.status == 200){
            let res = JSON.parse(this.responseText);
            document.getElementById("time").innerHTML = res.time;
            document.getElementById("datetime").innerHTML = res.datetime;
            document.getElementById("state-time").innerHTML = res.state_time;
            document.getElementById("voltage").innerHTML = res.voltage;
            document.getElementById("current").innerHTML = res.current;
            document.getElementById("power").innerHTML = res.power;
            document.getElementById("soc").innerHTML = res.soc;
            document.getElementById("capacity").innerHTML = res.capacity;
            document.getElementById("state").innerHTML = res.state;
            document.getElementById("temperature").innerHTML = res.temperature;
            document.getElementById("error-state").innerHTML = res.error_state;
            document.getElementById("error").innerHTML = res.error;
            state_tr.className = (res.error_state == "Ошибка") ? "error" : "no-errors";
        }
    };

    xhttp.open("GET", "getdata", true);
    xhttp.send();
}

prognosisForm.onsubmit = function(e){
      e.preventDefault();
    
      const capacity = prognosisField.value;
      const req = new XMLHttpRequest();
    
      req.onreadystatechange = function() {
          if(this.readyState == 4 && this.status == 200){
             const res = this.responseText;
             prognosisDisplay.innerHTML = res;
           }
      };
      req.open("POST", "getprognosis", true);
      req.setRequestHeader("Content-Type", "text/plain");
      if(capacity){
        req.send(capacity);
      }
}

</script>
)=====";

#endif