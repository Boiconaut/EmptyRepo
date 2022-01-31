#ifndef STYLE_H
#define STYLE_H

#include <Arduino.h>

const char webheader[] PROGMEM = R"=====(
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1.0, user-scalable=no, charset="utf-8"">
        <title>Aspil Sensor</title>
        <style>
        html { 
          font-family: Helvetica; 
          display: inline-block; 
          margin: 0px auto; 
          text-align: center;
       }
       body {
          text-align: center;
          font-family: "Trebuchet MS", Arial;
        }
        h1 {
            color: #444444;
            margin: 50px auto 30px;
        } 
        h3 {
            color: #444444;
            margin-bottom: 20px;
        }
        p {
            font-size: 14px;
            color: #888;
            margin-bottom: 10px;
        }
        td {
          border: solid 1px black;
        }
        .sensor {
          color:white;
          font-weight: bold;
          background-color: #bcbcbc;
          padding: 8px;
        }
        tr {
          border: solid 1px black;
          padding: 16px;
        }
        .no-errors{
           background-color: greenyellow;
        }
        .error{
           background-color: red;
        }
        table {
          border: solid 1px black;
          width:60%;
          margin-left:auto;
          margin-right:auto;
          margin-top: 10px;
        }
        th {
          padding: 16px;
          background-color: #0043af;
          color: white;
        }
        :root {
            --primary-color: #17a2b8;
            --dark-color: #343a40;
            --light-color: #f4f4f4;
            --danger-color: #dc3545;
            --success-color: #28a745;
        }
        
        body {
            font-family: 'Raleway', sans-serif;
            font-size: 1rem;
            line-height: 1.6;
            background-color: #fff;
            color: #333;
        }
        
        a {
            color: var(--primary-color);
            text-decoration: none;
        }
        
        
        /* Utilities */
        
        .container {
            max-width: 1100px;
            margin: auto;
            overflow: hidden;
            padding: 0 2rem;
            margin-top: 6rem;
            margin-bottom: 3rem;
        }
        
        
        /* Text Styles*/
        
        .x-large {
            font-size: 4rem;
            line-height: 1.2;
        }
        
        .large {
            font-size: 3rem;
            line-height: 1.2;
        }
        
        .lead {
            font-size: 1.5rem;
        }
        
        .text-center {
            text-align: center;
        }
        
        .text-primary {
            color: var(--primary-color);
        }
        
        .btn {
            display: inline-block;
            background: var(--light-color);
            color: #333;
            padding: 0.4rem 1.3rem;
            font-size: 1rem;
            border: none;
            cursor: pointer;
            margin-right: 0.5rem;
            transition: opacity 0.2s ease-in;
            outline: none;
        }
        
        .alert {
            padding: 0.8rem;
            margin: 1rem 0;
            opacity: 0.9;
            background: var(--light-color);
            color: #333;
        }
        
        .btn-primary,
        .bg-primary,
        .badge-primary,
        .alert-primary {
            background: var(--primary-color);
            color: #fff;
        }
        
        .btn-danger,
        .bg-danger,
        .badge-danger,
        .alert-danger {
            background: var(--danger-color);
            color: #fff;
        }
        
        .btn:hover {
            opacity: 0.8;
        }
        
        /* Forms */
        
        .form .form-group {
            margin: 1.2rem 0;
        }
        
        .form .form-text {
            display: block;
            margin-top: 0.3rem;
            color: #888;
        }
        
        .form input[type='text'],
        .form input[type='email'],
        .form input[type='password'],
        .form input[type='date'],
        .form select,
        .form textarea {
            display: block;
            width: 100%;
            padding: 0.4rem;
            font-size: 1.2rem;
            border: 1px solid #ccc;
        }
        
        .form input[type='submit'],
        button {
            font: inherit;
        }
        
        .table th,
        .table td {
            padding: 1rem;
            text-align: left;
        }
        
        .table th {
            background: var(--light-color);
        }
        </style>
    </head>
)=====";

#endif