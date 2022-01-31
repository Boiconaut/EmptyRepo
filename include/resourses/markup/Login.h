#ifndef LOGIN_H
#define LOGIN_H

#include <Arduino.h>

const char login_html[] PROGMEM = R"=====(
      <body>
        <h1 class="large text-primary">Aspil Energy</h1>
            <p class="lead"><i class="fas fa-user"></i> Sign in to continue </p>
            <form class="form" id="loginForm">
                <div class="form-group">
                    <input 
                    type="text" 
                    placeholder="Login" 
                    id="email" />
                </div>
                <div class="form-group">
                    <input
                        type="password"
                        placeholder="Password"
                        id="password"
                    />
                </div>
                <input type="submit" class="btn btn-primary" value="Login" />
            </form>
    )=====";

#endif