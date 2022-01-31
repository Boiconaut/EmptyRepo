#ifndef LOGINSCRIPT_H
#define LOGINSCRIPT_H

#include <Arduino.h>

const char login_script[] PROGMEM = R"=====(
<script>
    const loginForm = document.getElementById("loginForm");
    const loginField = document.getElementById("email");
    const passwordField = document.getElementById("password");

    loginForm.onsubmit = function(e){
        e.preventDefault();

        const login = loginField.value;
        const password = passwordField.value;

        const data = JSON.stringify({ login: login, password: password });
        const req = new XMLHttpRequest();
        
        req.onreadystatechange = function() {
            if(this.readyState == 4 && this.status == 200){
                const res = JSON.parse(this.responseText);
                getData(res);
            }
        };
        req.open("POST", "auth", true);
        req.setRequestHeader("Content-Type", "text/json");
        req.send(data);
    }

    function getData(_token){
       const req = new XMLHttpRequest();
       const auth_token = _token.token;
        
        req.onreadystatechange = function() {
            if(this.readyState == 4 && this.status == 200){
                console.log(auth_token);
                document.write(this.response);
            }
        };
        req.open("GET", "data", true);
        req.setRequestHeader('x-auth-token', auth_token);
        req.send();
    }
</script>
)=====";

#endif