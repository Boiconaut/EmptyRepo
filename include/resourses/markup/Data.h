#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

const char data_html[] PROGMEM = R"=====(
    <body>
        <h1>Aspil Shunt v 1.2 Web Interface</h1>
        <table>
             <tr id="state-tr" class="no-errors">
                <td><h3>Состояние: </h3> </td>
                <td><h3 id="error-state">Ошибок нет</h3></td>
                <td><h3 id="error">-</h3></td>
            </tr>
             <tr>
                <th>Параметр</th>
                <th>Значение</th>
                <th>Ед. изм.</th>
              </tr>
            <tr>
                <td><h3>Время: </h3> </td>
                <td><h3 id="datetime">00:00:00</h3></td>
                <td><h3>h:m:s</h3></td>
            </tr>
            <tr>
                <td><h3>Моточасы: </h3> </td>
                <td><h3 id="time">00:00:00</h3></td>
                <td><h3>h:m:s</h3></td>
            </tr>
            <tr>
                <td><h3 id="state">Время в текущем состоянии: </h3> </td>
                <td><h3 id="state-time">00:00:00</h3></td>
                <td><h3>h:m:s</h3></td>
            </tr>
            <tr>
                <td><h3>Напряжение: </h3></td>
                <td><h3 id="voltage">0.00</h3></td>
                <td><h3>V</h3></td>
            </tr>
            <tr>
                <td><h3>Ток: </h3></td>
                <td><h3 id="current">0.00</h3></td>
                <td><h3>A</h3></td>
            </tr>
            <tr>
                <td><h3>Мощность: </h3></td>
                <td><h3 id="power">0.00</h3></td>
                <td><h3>W</h3></td>
            </tr>
            <tr>
                <td><h3>Отданная ёмкость: </h3></td>
                <td><h3 id="capacity">0</h3></td>
                <td><h3>mAh</h3></td>
            </tr>
            <tr>
                <td><h3>Уровень заряда: </h3></td>
                <td><h3 id="soc">0</h3></td>
                <td><h3>%</h3></td>
            </tr>
            <tr>
                <td><h3>Температура: </h3></td>
                <td><h3 id="temperature">0.00</h3></td>
                <td><h3>oC</h3></td>
            </tr>
        </table>

         <form class="form" id="prognosisForm">
                <div>
                  <h2>Расчёт остаточного времени работы АКБ</h2>
                  <h3>Введите ёмкость аккумулятора, Ah:</h3>
                  <input 
                    type="text" 
                    id="prognosis-input" />
                </div>
                <input type="submit" class="btn btn-primary" value="Рассчитать" />
          </form>
          <h3 id="prognosis"></h3>
          <a href="/downloadlog" class="btn btn-primary" download>Скачать файл с логами ошибок</a>
          <a href="/downloadlogfile" class="btn btn-primary" download>Скачать файл с логами</a>
    )=====";

#endif