#ifndef ALERT_H
#define ALERT_H

#include <Arduino.h>

const char alert_html[] PROGMEM = R"=====(
    <div class="alert alert-danger">
      Please type correct credentials to log in
    </div>
    )=====";

#endif