#ifndef ADMIN_H
#define ADMIN_H

#include <Arduino.h>

const char admin_html[] PROGMEM = R"=====(
<form class="form" id="calibrateVoltageForm">
                <div>
                  <h2>Калибровка напряжения</h2>
                  <h3>Введите фактическое напряжение АКБ, В:</h3>
                  <input 
                    type="text" 
                    id="calibrate-voltage-input" />
                </div>
                <input type="submit" class="btn btn-primary" value="Установить" />
          </form>

<form class="form" id="calibrateCurrentForm">
                <div>
                  <h2>Калибровка тока</h2>
                  <h3>Введите фактическое значение тока через шунт, А:</h3>
                  <input 
                    type="text" 
                    id="calibrate-current-input" />
                </div>
                <input type="submit" class="btn btn-primary" value="Установить" />
          </form>

<form class="form" id="setNomCapacityForm">
                <div>
                  <h2>Установка номинальной ёмкости АКБ</h2>
                  <h3>Введите значение ёмкости АКБ, Ач:</h3>
                  <input 
                    type="text" 
                    id="set-capacity-input" />
                </div>
                <input type="submit" class="btn btn-primary" value="Установить" />
          </form>
 )=====";

#endif