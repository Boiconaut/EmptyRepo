#ifndef NOTFOUND_H
#define NOTFOUND_H

#include <Arduino.h>

const char not_found_html[] PROGMEM = R"=====(
    <body>
        <h1 class="x-large text-primary">
            <i class="fas fa-exclamation-triangle" /> Page Not Found
        </h1>
        <p class="large">Sorry, this page does not exist</p>
    </body>
</html>
       )=====";

#endif