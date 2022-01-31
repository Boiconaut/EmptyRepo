#ifndef DATASCRIPT_H
#define DATASCRIPT_H

#include <Arduino.h>

const char script[] PROGMEM = R"=====(

<script>
const prognosisForm = document.getElementById("prognosisForm");
const prognosisField = document.getElementById("prognosis-input");
const prognosisDisplay = document.getElementById("prognosis");

const set12 = document.getElementById("set12");
const set24 = document.getElementById("set24");
const set36 = document.getElementById("set36");
const set48 = document.getElementById("set48");
let voltages = [set12, set24, set36, set48];

const state_tr = document.getElementById("state-tr");

setInterval(function(){
        getData();
    }, 1000);

voltages.forEach(function(item, index, array){
  item.onclick = function(){
    let voltage;

    if(index == 0){
      voltage = "12";
    }
    if(index == 1){
      voltage = "24";
    }
    if(index == 2){
      voltage = "36";
    }
    if(index == 3){
      voltage = "48";
    }

    const req = new XMLHttpRequest();

    req.onreadystatechange = function() {
          if(this.readyState == 4 && this.status == 200){
             const res = this.responseText;
             let button;

             set12.className = "btn btn-primary";
             set24.className = "btn btn-primary";
             set36.className = "btn btn-primary";
             set48.className = "btn btn-primary";

             if(res == "12.00"){
               set12.className = "btn btn-danger";
             }
             if(res == "24.00"){
               set24.className = "btn btn-danger";
             }
             if(res == "36.00"){
               set36.className = "btn btn-danger";
             }
             if(res == "48.00"){
               set48.className = "btn btn-danger";
             }
           }
      };
    
    req.open("POST", "setvoltage", true);
    req.setRequestHeader("Content-Type", "text/plain");
    req.send(voltage);
  }
});

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