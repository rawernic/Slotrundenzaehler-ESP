#include <ESP8266WiFi.h>
#include <time.h>

#include <ArduinoJson.h>
#include <ESPASyncWebServer.h>
#include <Wire.h>


const char* ssid = "DW-Network II";
const char* password = "MGQzODI3YTBmODlj";

const char * swaggerJSON = "{\"swagger\":\"2.0\",\"info\":{\"description\":\"This is a sample server Petstore server.\",\"version\":\"1.0.0\",\"title\":\"IoT application\"},\"host\":\"192.168.178.63\",\"tags\":[{\"name\":\"Temperature\",\"description\":\"Getting temperature measurements\"}],\"paths\":{\"/temperature\":{\"get\":{\"tags\":[\"Temperature\"],\"summary\":\"Endpoint for getting temperature measurements\",\"description\":\"\",\"operationId\":\"getTemperature\",\"responses\":{\"200\":{\"description\":\"A list of temperature measurements\",\"schema\":{\"$ref\":\"#/definitions/temperatureMeasurement\"}}}}}},\"definitions\":{\"temperatureMeasurement\":{\"type\":\"object\",\"properties\":{\"value\":{\"type\":\"string\"},\"timestamp\":{\"type\":\"string\"}}}}}";
const char * swaggerUI = "<!DOCTYPE html><html><head> <meta charset=\"UTF-8\"> <meta http-equiv=\"x-ua-compatible\" content=\"IE=edge\"> <title>Swagger UI</title> <link href='https://cdnjs.cloudflare.com/ajax/libs/meyer-reset/2.0/reset.min.css' media='screen' rel='stylesheet' type='text/css'/> <link href='https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/2.2.10/css/screen.css' media='screen' rel='stylesheet' type='text/css'/> <script>if (typeof Object.assign !='function'){(function (){Object.assign=function (target){'use strict'; if (target===undefined || target===null){throw new TypeError('Cannot convert undefined or null to object');}var output=Object(target); for (var index=1; index < arguments.length; index++){var source=arguments[index]; if (source !==undefined && source !==null){for (var nextKey in source){if (Object.prototype.hasOwnProperty.call(source, nextKey)){output[nextKey]=source[nextKey];}}}}return output;};})();}</script> <script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/1.8.0/jquery-1.8.0.min.js' type='text/javascript'></script> <script>(function(b){b.fn.slideto=function(a){a=b.extend({slide_duration:\"slow\",highlight_duration:3E3,highlight:true,highlight_color:\"#FFFF99\"},a);return this.each(function(){obj=b(this);b(\"body\").animate({scrollTop:obj.offset().top},a.slide_duration,function(){a.highlight&&b.ui.version&&obj.effect(\"highlight\",{color:a.highlight_color},a.highlight_duration)})})}})(jQuery); </script> <script>jQuery.fn.wiggle=function(o){var d={speed:50,wiggles:3,travel:5,callback:null};var o=jQuery.extend(d,o);return this.each(function(){var cache=this;var wrap=jQuery(this).wrap(' <div class=\"wiggle-wrap\"></div>').css(\"position\",\"relative\");var calls=0;for(i=1;i<=o.wiggles;i++){jQuery(this).animate({left:\"-=\"+o.travel},o.speed).animate({left:\"+=\"+o.travel*2},o.speed*2).animate({left:\"-=\"+o.travel},o.speed,function(){calls++;if(jQuery(cache).parent().hasClass('wiggle-wrap')){jQuery(cache).parent().replaceWith(cache);}if(calls==o.wiggles&&jQuery.isFunction(o.callback)){o.callback();}});}});}; </script> <script src='https://cdnjs.cloudflare.com/ajax/libs/jquery.ba-bbq/1.2.1/jquery.ba-bbq.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/handlebars.js/4.0.5/handlebars.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/lodash-compat/3.10.1/lodash.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/backbone.js/1.1.2/backbone-min.js' type='text/javascript'></script> <script>Backbone.View=(function(View){return View.extend({constructor: function(options){this.options=options ||{}; View.apply(this, arguments);}});})(Backbone.View); </script> <script src='https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/2.2.10/swagger-ui.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.10.0/highlight.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.10.0/languages/json.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/json-editor/0.7.28/jsoneditor.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/marked/0.3.6/marked.min.js' type='text/javascript'></script> <script type=\"text/javascript\">$(function (){url=\"http://192.168.178.63/swagger.json\"; hljs.configure({highlightSizeThreshold: 5000}); window.swaggerUi=new SwaggerUi({url: url, dom_id: \"swagger-ui-container\", supportedSubmitMethods: ['get', 'post', 'put', 'delete', 'patch'], validatorUrl: null, onComplete: function(swaggerApi, swaggerUi){}, onFailure: function(data){log(\"Unable to Load SwaggerUI\");}, docExpansion: \"none\", jsonEditor: false, defaultModelRendering: 'schema', showRequestHeaders: false, showOperationIds: false}); window.swaggerUi.load(); function log(){if ('console' in window){console.log.apply(console, arguments);}}}); </script></head> <body class=\"swagger-section\"><div id='header'><div class=\"swagger-ui-wrap\"> <a id=\"logo\" href=\"http://swagger.io\"><img class=\"logo__img\" alt=\"swagger\" height=\"30\" width=\"30\" src=\"https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/2.2.10/images/logo_small.png\"/><span class=\"logo__title\">swagger</span></a><form id='api_selector'></form></div></div><div id=\"message-bar\" class=\"swagger-ui-wrap\" data-sw-translate>&nbsp;</div><div id=\"swagger-ui-container\" class=\"swagger-ui-wrap\"></div></body></html>";
const char * answer = "[{\"value\":\"10.5\",\"timestamp\":\"22/10/2017 10:10\"},{\"value\":\"11.0\",\"timestamp\":\"22/10/2017 10:20\"}]";


// Set web server port number to 80
AsyncWebServer server(80);

// Variable to store the HTTP request
String header;

// JSON data buffer
StaticJsonDocument<2000> jsonDocument;
char buffer[2000];

#define max_laps  20
volatile unsigned long lap_1[max_laps] = { 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 };
volatile unsigned long fastest_lap_1 = 0;
volatile unsigned long average_lap_1 = 0;
volatile unsigned long lap_2[max_laps] = { 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 };
volatile unsigned long fastest_lap_2 = 0;
volatile unsigned long average_lap_2 = 0;
volatile int count_laps_1 = 0;
volatile int count_laps_2 = 0;
volatile unsigned long total_time_1 = 0;
volatile unsigned long total_time_2 = 0;
unsigned long previousTime_1 = 0;
unsigned long previousTime_2 = 0;
#define MIN_RUNDEN_ZEIT 1000 // 3000 /1000 = 3Sek

#define play  1
#define pause 0
boolean zaehlpause = true;
unsigned long startzeit  =  0;
unsigned long laufzeit   =  0;
unsigned long zielzeit   =  0;
unsigned long zielrunden =  0;

String fahrer_1 = "Fahrer1";
String fahrer_2 = "Fahrer2";


// GPIO pins
const int led = 2;


#define RX   3
#define TX   1
#define D1  16
#define D1   4
#define D2   5
#define D3   0
#define D4   2
#define D6  12
#define D7  13
#define D5  14
#define D8  15
#define S3  10
#define S2   9

const char MAIN_page[] PROGMEM = R"=="==(
<!DOCTYPE html>
<html>
<HEAD>
    <title>Rundenz&aumlhler
    </title>
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
    <link rel="stylesheet"
          href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />
</HEAD>
<style>
.card{
     min-width: 500px;
     min-height: 400px;
     background: #02b875;
     box-sizing: border-box;
     font-family: Arial, Helvetica, sans-serif;
     font-size: 40px;
     color: #FFF;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
     padding: 10px 15px;
     margin: 8px 8px;
  border-radius: 12px;
}

.headcard{
    width = 100%;
     background: #02b875;
     box-sizing: border-box;
     font-family: Arial, Helvetica, sans-serif;
     color: #FFF;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
     padding: 10px 15px;
     margin: 8px 8px;
  border-radius: 12px;
}

table, tr, td, th {
  //border: 1px solid black;
  vertical-align: middle;
  horizontal-align: middle;
  color: #FFF;
  text-align: left;
  font-family: Arial, Helvetica, sans-serif;
}
table {
  width: 100%
}
td {
  font-size: 35px;
}
th {
  font-size: 50px;
  background: #02b875;
}
tr {
  width: 100%
}
.td-left {
  text-align: left;
}
.td-right {
  text-align: right;
}
.td-center {
  text-align: center;
}
.background_bild2 {
  background-image: url("http://192.168.178.69/capture");
  background-clip: padding-box;
  background-origin: padding-box
  box-sizing: border-box;
  background-repeat: no-repeat;
  background-position: center center;
}
input{
  border-radius: 12px;
  font-size: 30px;
  background-color: #02a875;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
     margin: 0px 20px;
}
.button {
  border-radius: 12px;
  font-size: 36px;
  background-color: #02b875;
  border: 0px solid white;
     box-sizing: border-box;
     font-family: Arial, Helvetica, sans-serif;
     color: #FFF;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
  color: white;
  text-align: center;
  text-decoration: none;
  display: inline-block;
     margin: 10px 8px;
     padding: 2px 8px;
}
.material-button {
  vertical-align:middle;
  border: 0px solid white;
  //border-radius: 12px;
  background-color: #02b875;
  color: white;
  text-align: center;
  text-decoration: none;
}
.nbsp  { font-size: 15px; }
.links  { float: left; width: 50%; }
.rechts { float: left; }
.material-symbols-outlined {
  font-variation-settings:
  'FILL' 0,
  'wght' 600,
  'GRAD' 200,
  'opsz' 48
  background-color: #02a875;
}
i.md40px {
  font-size: 40px;
}
i.md50px {
  font-size: 44px;
}
i.md60px {
  font-size: 60px;
}

</STYLE>

<body>

<CENTER>

<TABLE>
<TR><TD><div class="headcard">

<TABLE>
  <col style="width:48%">
  <col style="width:2%">
  <col style="width:2%">
  <col style="width:48%">
<TR>
<TH><span id="modus"></span></TH>
<TH><button class="material-button" onclick="togglePlayPause()">
            <i class="material-symbols-outlined md60px" id="MODUS">play_circle</i>
    </button></TH>
<TH><button class="material-button" onclick="resetLanes()">
            <i class="material-symbols-outlined md60px" id="MODUS">stop_circle</i>
    </button></TH>
<TH class="td-right"><span id="message"></span></TH>
</TR>
</TABLE>
</TD></TR>
</TABLE>

<TABLE>
<TR><TD><div id="background_bild2" class="card background_bild2"/>
<TABLE>
  <col style="width:20%">
  <col style="width:60%">
  <col style="width:20%">
<TR><TH class="td-left"><span id="lane1_lap_count">0</span></TH>
    <TH class="td-center"><span id="fahrer_1"></span></TH>
    <TH class="td-right"><span id="lane1_last_lap">0.000</span></TH></TR>
<TR><TD class="nbsp">&nbsp</TD></TR>
</TABLE>
<TABLE>
  <col style="width:60%">
  <col style="width:40%">
<TR><TD>Fahrzeit</TD><TD class="td-right"><span id="lane1_total_time">0.000</span></TD></TR>
<TR><TD>Beste Runde</TD><TD class="td-right"><span id="lane1_fast_lap">0.000</span></TD></TR>
<TR><TD>Durchschnitt</TD><TD class="td-right"><span id="lane1_average_lap">0.000</span></TD></TR>
<TR><TD class="nbsp">&nbsp</TD></TR>
</TABLE>
<TABLE>
  <col style="width:33%">
  <col style="width:34%">
  <col style="width:33%">
<TR><TD class="td-center"><span id="lane1_lap0">0.001</span></TD>
    <TD class="td-center"><span id="lane1_lap5">0.000</span></TD>
    <TD class="td-center"><span id="lane1_lap10">0.000</span></TD></TR>
<TR><TD class="td-center"><span id="lane1_lap1">0.000</span></TD>
    <TD class="td-center"><span id="lane1_lap6">0.000</span></TD>
    <TD class="td-center"><span id="lane1_lap11">0.000</span></TD></TR>
<TR><TD class="td-center"><span id="lane1_lap2">0.000</span></TD>
    <TD class="td-center"><span id="lane1_lap7">0.000</span></TD>
    <TD class="td-center"><span id="lane1_lap12">0.000</span></TD></TR>
<TR><TD class="td-center"><span id="lane1_lap3">0.000</span></TD>
    <TD class="td-center"><span id="lane1_lap8">0.000</span></TD>
    <TD class="td-center"><span id="lane1_lap13">0.000</span></TD></TR>
<TR><TD class="td-center"><span id="lane1_lap4">0.000</span></TD>
    <TD class="td-center"><span id="lane1_lap9">0.000</span></TD>
    <TD class="td-center"><span id="lane1_lap14">0.000</span></TD></TR>
</TABLE>

</TD><TD><div class="card">

<TABLE>
  <col style="width:20%">
  <col style="width:60%">
  <col style="width:20%">
<TR><TH class="td-left"><span id="lane2_lap_count">0</span></TH>
    <TH class="td-center"><span id="fahrer_2"></span></TH>
    <TH class="td-right"><span id="lane2_last_lap">0.000</span></TH></TR>
<TR><TD class="nbsp">&nbsp</TD></TR>
</TABLE>
<TABLE>
  <col style="width:60%">
  <col style="width:40%">
<TR><TD>Fahrzeit</TD><TD class="td-right"><span id="lane2_total_time">0.000</span></TD></TR>
<TR><TD>Beste Runde</TD><TD class="td-right"><span id="lane2_fast_lap">0.000</span></TD></TR>
<TR><TD>Durchschnitt</TD><TD class="td-right"><span id="lane2_average_lap">0.000</span></TD></TR>
<TR><TD class="nbsp">&nbsp</TD></TR>
</TABLE>
<TABLE>
  <col style="width:33%">
  <col style="width:34%">
  <col style="width:33%">
<TR><TD class="td-right"><span id="lane2_lap0">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap5">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap10">0.000</span></TD></TR>
<TR><TD class="td-right"><span id="lane2_lap1">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap6">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap11">0.000</span></TD></TR>
<TR><TD class="td-right"><span id="lane2_lap2">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap7">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap12">0.000</span></TD></TR>
<TR><TD class="td-right"><span id="lane2_lap3">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap8">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap13">0.000</span></TD></TR>
<TR><TD class="td-right"><span id="lane2_lap4">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap9">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap14">0.000</span></TD></TR>
</TABLE>
</TABLE>
<div class="rechts"> <button class="button" onclick="toggleZielminuten()">
  <i class="material-symbols-outlined md50px" id="MODUS1">alarm</i>
 5 Minuten</button> </div>
<div class="rechts"> <button class="button" onclick="toggleZielrunden()">
  <i class="material-symbols-outlined md50px" id="MODUS2">update</i>
20 Runden</button> </div>
<div class="rechts"> <button class="button" onclick="resetLanes()">
  <i class="material-symbols-outlined md50px" id="MODUS2">speed</i>
Schnellste Runde</button> </div>
<div class="rechts"> <button class="button" onclick="updateBild2()">
  <i class="material-symbols-outlined md50px" id="MODUS3">all_inclusive</i>
Training</button> </div>
    <TH class="td-center"><input id="inFahrer2" name="inFahrer2"></input></TH>
/CENTER>
</body>

<script>
setInterval(function() {
  // Call a function repetatively with 1 Second interval
  updateData();
}, 1000); //1000mSeconds update rate

function updateData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {

      var data = JSON.parse(this.responseText);

      document.getElementById("modus").innerHTML = data.Lane_1.modus;
      document.getElementById("message").innerHTML = data.Lane_1.message;

      document.getElementById("fahrer_1").innerHTML = data.Lane_1.fahrer;
      document.getElementById("fahrer_2").innerHTML = data.Lane_2.fahrer;

      document.getElementById("lane1_lap_count").innerHTML = data.Lane_1.lap_count;
      document.getElementById("lane1_total_time").innerHTML = data.Lane_1.total_time;
      document.getElementById("lane1_fast_lap").innerHTML = data.Lane_1.fast_lap;
      document.getElementById("lane1_last_lap").innerHTML = data.Lane_1.last_lap;
      document.getElementById("lane1_average_lap").innerHTML = data.Lane_1.average_lap;
      document.getElementById("lane1_lap0").innerHTML = data.Lane_1.lap0;
      document.getElementById("lane1_lap1").innerHTML = data.Lane_1.lap1;
      document.getElementById("lane1_lap2").innerHTML = data.Lane_1.lap2;
      document.getElementById("lane1_lap3").innerHTML = data.Lane_1.lap3;
      document.getElementById("lane1_lap4").innerHTML = data.Lane_1.lap4;
      document.getElementById("lane1_lap5").innerHTML = data.Lane_1.lap5;
      document.getElementById("lane1_lap6").innerHTML = data.Lane_1.lap6;
      document.getElementById("lane1_lap7").innerHTML = data.Lane_1.lap7;
      document.getElementById("lane1_lap8").innerHTML = data.Lane_1.lap8;
      document.getElementById("lane1_lap9").innerHTML = data.Lane_1.lap9;
      document.getElementById("lane1_lap10").innerHTML = data.Lane_1.lap10;
      document.getElementById("lane1_lap11").innerHTML = data.Lane_1.lap11;
      document.getElementById("lane1_lap12").innerHTML = data.Lane_1.lap12;
      document.getElementById("lane1_lap13").innerHTML = data.Lane_1.lap13;
      document.getElementById("lane1_lap14").innerHTML = data.Lane_1.lap14;

      document.getElementById("lane2_lap_count").innerHTML = data.Lane_2.lap_count;
      document.getElementById("lane2_total_time").innerHTML = data.Lane_2.total_time;
      document.getElementById("lane2_fast_lap").innerHTML = data.Lane_2.fast_lap;
      document.getElementById("lane2_last_lap").innerHTML = data.Lane_2.last_lap;
      document.getElementById("lane2_average_lap").innerHTML = data.Lane_2.average_lap;
      document.getElementById("lane2_lap0").innerHTML = data.Lane_2.lap0;
      document.getElementById("lane2_lap1").innerHTML = data.Lane_2.lap1;
      document.getElementById("lane2_lap2").innerHTML = data.Lane_2.lap2;
      document.getElementById("lane2_lap3").innerHTML = data.Lane_2.lap3;
      document.getElementById("lane2_lap4").innerHTML = data.Lane_2.lap4;
      document.getElementById("lane2_lap5").innerHTML = data.Lane_2.lap5;
      document.getElementById("lane2_lap6").innerHTML = data.Lane_2.lap6;
      document.getElementById("lane2_lap7").innerHTML = data.Lane_2.lap7;
      document.getElementById("lane2_lap8").innerHTML = data.Lane_2.lap8;
      document.getElementById("lane2_lap9").innerHTML = data.Lane_2.lap9;
      document.getElementById("lane2_lap10").innerHTML = data.Lane_2.lap10;
      document.getElementById("lane2_lap11").innerHTML = data.Lane_2.lap11;
      document.getElementById("lane2_lap12").innerHTML = data.Lane_2.lap12;
      document.getElementById("lane2_lap13").innerHTML = data.Lane_2.lap13;
      document.getElementById("lane2_lap14").innerHTML = data.Lane_2.lap14;
    }
  };
  xhttp.open("GET", "data", true);
  xhttp.send();
}

function setFahrer() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {

      var data = JSON.parse(this.responseText);

      document.getElementById("fahrer_1").innerHTML = data.Fahrer.name1;
      document.getElementById("fahrer_2").innerHTML = data.Fahrer.name2;
    }
  };
  //const fahrer_1 = document.querySelector("input[name='inFahrer1']").value;
  const fahrer_1 = document.getElementById("inFahrer1").value;
  const fahrer_2 = document.getElementById("inFahrer2").value;
  xhttp.open("GET", "fahrer?fahrer1="+fahrer_1+"&fahrer2="+fahrer_2, true);
  xhttp.send();
}

function resetLanes() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("MODUS").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "reset", true);
  xhttp.send();
}

function togglePlayPause() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("MODUS").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "playpause", true);
  xhttp.send();
}

function updateBild2() {
  var url = document.getElementById("background_bild2").style["background-image","url"];
  document.getElementById("background_bild2").style["background-image","url"] = "http://192.168.178.69/capture?"+currentTime;
}


function toggleZielrunden() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      //document.getElementById("MODUS").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "zielrunden", true);
  xhttp.send();
}

function toggleZielminuten() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      //document.getElementById("MODUS").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "zielminuten", true);
  xhttp.send();
}

function toggleLed() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      //document.getElementById("LEDState").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "led", true);
  xhttp.send();
}

</script>
</html>
)=="==";

//////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
    Serial.begin(115200);

    // Initialize the output variables as outputs
    pinMode(led, OUTPUT);

    wifi_connect();
    start_rest_api();

    pinMode( D7, INPUT_PULLUP );
    pinMode( D6, INPUT_PULLUP );

    attachInterrupt( D7, ZeitMessung_2, RISING ); // FALLING
    attachInterrupt( D6, ZeitMessung_1, RISING ); // FALLING

    digitalWrite(led, HIGH);
}

//////////////////////////////////////////////////////////////////////////////////////////

IRAM_ATTR void ZeitMessung_1()
{
    if (zaehlpause) {
        previousTime_2 = 0;
        return;
    }
    unsigned long currentTime = millis();
    unsigned long tausendstel = currentTime -previousTime_1;

    if (previousTime_1 == 0) {
        previousTime_1 = currentTime;
        return;
    }
    if (tausendstel < MIN_RUNDEN_ZEIT) return;

    for (int k=max_laps-1; k>=0; k--) {
        lap_1[k+1] = lap_1[k];
        average_lap_1 = average_lap_1 + lap_1[k];
    }
    lap_1[0] = tausendstel;
    average_lap_1 = (average_lap_1 + lap_1[0]) /max_laps;

    total_time_1 = total_time_1 +tausendstel;
    count_laps_1 = count_laps_1 +1;
    average_lap_1  = total_time_1 /count_laps_1;

    if (fastest_lap_1 == 0 || fastest_lap_1 > tausendstel)
        fastest_lap_1 = tausendstel;

    previousTime_1 = currentTime;

    if (count_laps_1 == zielrunden) {
        zaehlpause = true;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

IRAM_ATTR void ZeitMessung_2()
{
    if (zaehlpause) {
        previousTime_2 = 0;
        return;
    }
    unsigned long currentTime = millis();
    unsigned long tausendstel = millis() -previousTime_2;

    if (previousTime_2 == 0) {
        previousTime_2 = currentTime;
        return;
    }
    if (tausendstel < MIN_RUNDEN_ZEIT) return;

    for (int k=max_laps-1; k>=0; k--) {
        lap_2[k+1] = lap_2[k];
    }
    lap_2[0] = tausendstel;

    total_time_2 = total_time_2 +tausendstel;
    count_laps_2 = count_laps_2 +1;
    average_lap_2 = total_time_2 /count_laps_2;

    if (fastest_lap_2 == 0 || fastest_lap_2 > tausendstel)
        fastest_lap_2 = tausendstel;

    previousTime_2 = currentTime;

    if (count_laps_2 == zielrunden) {
        zaehlpause = true;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
    server.handleClient();
}


//////////////////////////////////////////////////////////////////////////////////////////

void wifi_connect()
{
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.println("Waiting to connect...");
    }
    Serial.println("\nWiFi connected\n\n");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

//////////////////////////////////////////////////////////////////////////////////////////

void start_rest_api()
{
    server.on("/", handleRoot);
    server.on("/data", handleGetData);
    server.on("/led", handleLed);
    server.on("/reset", handleResetLanes);
    server.on("/playpause", handlePlayPause);
    server.on("/zielrunden", handleZielRunden);
    server.on("/zielminuten", handleZielMinuten);
    //server.on("/resetall", handleReset);
    server.on("/fahrer", handleFahrer);
    server.on("/swagger.json", handleSwaggerJson);
    server.on("/swaggerUI", handleSwaggerUI);

    server.begin();
    Serial.println("Server listening");
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleRoot()
{
    server.send( 200, "text/html", MAIN_page );
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleResetLanes() {

    for (int k=0; k<max_laps; k++) {
        lap_1[k] = 0;
    }
    total_time_1 = 0;
    count_laps_1 = 0;
    fastest_lap_1 = 0;
    average_lap_1 = 0;
    previousTime_1 = 0;

    for (int k=0; k<max_laps; k++) {
        lap_2[k] = 0;
    }
    total_time_2 = 0;
    count_laps_2 = 0;
    fastest_lap_2 = 0;
    average_lap_2 = 0;
    previousTime_2 = 0;

    zaehlpause = true;
    zielrunden = 0;
    zielzeit  = 0;
    startzeit = 0;
    laufzeit  = 0;
    server.send(200, "text/plane", "play_circle");

    //Redirect to Root
    //server.sendHeader("Location", "/");
    //server.send(302, "text/plain", "Updated-- Press Back Button");  //This Line Keeps It on Same Page
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleZielRunden() {
    Serial.println("Get handleZielRunden");

    //if (!zaehlpause)    server.send(400, "text/plane", "laeuft");
    if (zielzeit > 0)   server.send(400, "text/plane", "laeuft");
    if (zielrunden > 0) server.send(400, "text/plane", "laeuft");

    zielrunden = 20;
    zielzeit  = 0;
    startzeit = 0;
    laufzeit  = 0;
    server.send(200, "text/plane", "play_circle");
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleZielMinuten() {
    Serial.println("Get handleZielMinuten");

    //if (!zaehlpause)    server.send(400, "text/plane", "laeuft");
    if (zielzeit > 0)   server.send(400, "text/plane", "laeuft");
    if (zielrunden > 0) server.send(400, "text/plane", "laeuft");

    zielrunden = 0;
    zielzeit = 5 *60 *1000;
    startzeit = 0;
    laufzeit  = 0;
    server.send(200, "text/plane", "play_circle");
}

//////////////////////////////////////////////////////////////////////////////////////////

void handlePlayPause() {
    Serial.println("Get handlePlayPause");

    if (!zaehlpause) {
        zaehlpause = true;
        server.send(200, "text/plane", "play_circle");
        return;
    } else {

        startzeit = millis();
        zaehlpause = false;
        server.send(200, "text/plane", "pause_circle");
        return;
    }
}


//////////////////////////////////////////////////////////////////////////////////////////

void handleGetData() { //Handler for the body path
    Serial.println("Get data");
    jsonDocument.clear();

    String message = getTimeString( laufzeit );
    String modus = "Training";

    if (!zaehlpause) {
        laufzeit = laufzeit +(millis() -startzeit);
        startzeit = millis();
        message = getTimeString( laufzeit );
    }

    if (zielzeit > 0) {
        message = getTimeString( laufzeit );
        modus = getTimeString( zielzeit );

        if (laufzeit >= zielzeit) {
            zaehlpause = true;
            if (previousTime_2 > previousTime_2) {
                message = "((: the winner is "+fahrer_2+" :))";
            } else {
                message = "((: the winner is "+fahrer_1+" :))";
            }
        }
    }

    if (zielrunden > 0) {
        message = getTimeString( laufzeit );
        modus = String(zielrunden) +" Runden";
        if (count_laps_1 == zielrunden) {
            zaehlpause = true;
            message = "((: the winner is "+fahrer_1+" :))";
        }
        if (count_laps_2 == zielrunden) {
            zaehlpause = true;
            message = "((: the winner is "+fahrer_2+" :))";
        }
    }

    JsonObject lane_1 = jsonDocument.createNestedObject("Lane_1");
    lane_1["modus"] = modus;
    lane_1["message"] = message;
    lane_1["fahrer"] = fahrer_1;

    lane_1["lap_count"] = count_laps_1;
    lane_1["total_time"] = getTimeString( total_time_1 );
    lane_1["fast_lap"] = getSecString( fastest_lap_1 );
    lane_1["last_lap"] = getSecString( lap_1[0]);
    lane_1["average_lap"] =getSecString( average_lap_1 );
    lane_1["lap0"] = getSecString( lap_1[0] );
    lane_1["lap1"] = getSecString( lap_1[1] );
    lane_1["lap2"] = getSecString( lap_1[2] );
    lane_1["lap3"] = getSecString( lap_1[3] );
    lane_1["lap4"] = getSecString( lap_1[4] );
    lane_1["lap5"] = getSecString( lap_1[5] );
    lane_1["lap6"] = getSecString( lap_1[6] );
    lane_1["lap7"] = getSecString( lap_1[7] );
    lane_1["lap8"] = getSecString( lap_1[8] );
    lane_1["lap9"] = getSecString( lap_1[9] );
    lane_1["lap10"] = getSecString( lap_1[10] );
    lane_1["lap11"] = getSecString( lap_1[11] );
    lane_1["lap12"] = getSecString( lap_1[12] );
    lane_1["lap13"] = getSecString( lap_1[13] );
    lane_1["lap14"] = getSecString( lap_1[14] );
    lane_1["lap15"] = getSecString( lap_1[15] );

    JsonObject lane_2 = jsonDocument.createNestedObject("Lane_2");
    lane_2["fahrer"] = fahrer_2;
    lane_2["lap_count"] = count_laps_2;
    lane_2["total_time"] = getTimeString( total_time_2 );
    lane_2["fast_lap"] = getSecString( fastest_lap_2 );
    lane_2["last_lap"] = getSecString( lap_2[0] );
    lane_2["average_lap"] =getSecString( average_lap_2 );
    lane_2["lap0"] = getSecString( lap_2[0] );
    lane_2["lap1"] = getSecString( lap_2[1] );
    lane_2["lap2"] = getSecString( lap_2[2] );
    lane_2["lap3"] = getSecString( lap_2[3] );
    lane_2["lap4"] = getSecString( lap_2[4] );
    lane_2["lap5"] = getSecString( lap_2[5] );
    lane_2["lap6"] = getSecString( lap_2[6] );
    lane_2["lap7"] = getSecString( lap_2[7] );
    lane_2["lap8"] = getSecString( lap_2[8] );
    lane_2["lap9"] = getSecString( lap_2[9] );
    lane_2["lap10"] = getSecString( lap_2[10] );
    lane_2["lap11"] = getSecString( lap_2[11] );
    lane_2["lap12"] = getSecString( lap_2[12] );
    lane_2["lap13"] = getSecString( lap_2[13] );
    lane_2["lap14"] = getSecString( lap_2[14] );
    lane_2["lap15"] = getSecString( lap_2[15] );
    /*
    JsonArray laps1 = jsonDocument.createNestedArray("Lane_1_laps");
    for (int k=0; k<max_laps; k++) {
      laps1.add( lap_1[k] );
    }

    JsonArray laps2 = jsonDocument.createNestedArray("Lane_2_laps");
    for (int k=0; k<max_laps; k++) {
      laps2.add( lap_2[k] );
    }*/

    serializeJsonPretty(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleLed()
{
    Serial.println("handleLed");
    // toggle LED
    digitalWrite( led, !digitalRead(led) );
    server.send(200, "text/html", "LED toggled");
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleFahrer()
{
    Serial.println("handleFahrer");
    Serial.println( server.arg("fahrer1") );
    Serial.println( server.arg("fahrer2") );

    fahrer_1 = server.arg("fahrer1");
    fahrer_2 = server.arg("fahrer2");

    jsonDocument.clear();
    JsonObject fahrer = jsonDocument.createNestedObject("Fahrer");
    fahrer["name1"] = fahrer_1;
    fahrer["name2"] = fahrer_2;

    serializeJsonPretty(jsonDocument, buffer);
    server.send(200, "application/json", buffer);
}


//////////////////////////////////////////////////////////////////////////////////////////

void handleSwaggerJson() { //Handler for the body path
    server.send(200, "application/json", swaggerJSON);
}

//////////////////////////////////////////////////////////////////////////////////////////

void handleSwaggerUI() { //Handler for the body path
    server.send(200, "text/html", swaggerUI);
}

//////////////////////////////////////////////////////////////////////////////////////////

String getTimeString( unsigned long millis_) {

    unsigned long allSeconds=millis_/1000;
    int milliSecs = millis_%1000;
    int runHours= allSeconds/3600;
    int secsRemaining=allSeconds%3600;
    int runMinutes=secsRemaining/60;
    int runSeconds=secsRemaining%60;

    char buf[30];
    sprintf(buf,"%02d:%02d:%02ds",runHours,runMinutes,runSeconds);
    return buf;
}

//////////////////////////////////////////////////////////////////////////////////////////

String getSecString( unsigned long millis_) {

    unsigned long allSeconds=millis_/1000;
    int milliSecs = millis_%1000;

    char buf[30];
    sprintf(buf,"%d.%03ds",allSeconds,milliSecs);
    return buf;
}