#ifndef ADMINSCRIPT_H
#define ADMINSCRIPT_H

#include <Arduino.h>

const char admin_script[] PROGMEM = R"=====(
<script>

const calibrateVoltageForm = document.getElementById("calibrateVoltageForm");
const calibrateCurrentForm = document.getElementById("calibrateCurrentForm");
const setNomCapacityForm = document.getElementById("setNomCapacityForm");

const calibrateVoltageInput = document.getElementById("calibrate-voltage-input");
const calibrateCurrentInput = document.getElementById("calibrate-current-input");
const setCapacityInput = document.getElementById("set-capacity-input");

calibrateVoltageForm.onsubmit = function(e){
  e.preventDefault();
  const voltage = calibrateVoltageInput.value;
  const req = new XMLHttpRequest();

  req.onreadystatechange = function() {
          if(this.readyState == 4 && this.status == 200){
             const res = this.responseText;
             console.log(res);
           }
      };
  
  req.open("POST", "calibratevoltage", true);
  if(voltage){
    req.send(voltage);
  }
}

calibrateCurrentForm.onsubmit = function(e){
  e.preventDefault();
  const current = calibrateCurrentInput.value;
  const req = new XMLHttpRequest();

  req.onreadystatechange = function() {
          if(this.readyState == 4 && this.status == 200){
             const res = this.responseText;
             console.log(res);
           }
      };
  
  req.open("POST", "calibratecurrent", true);
  if(current){
    req.send(current);
  }
}

setNomCapacityForm.onsubmit = function(e){
  e.preventDefault();
  const capacity = setCapacityInput.value;
  const req = new XMLHttpRequest();

  req.onreadystatechange = function() {
          if(this.readyState == 4 && this.status == 200){
             const res = this.responseText;
             console.log(res);
           }
      };
  
  req.open("POST", "setcapacity", true);
  if(capacity){
    req.send(capacity);
  }
}

</script>
)=====";

#endif