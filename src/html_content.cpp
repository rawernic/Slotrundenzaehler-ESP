#include "html_content.h"

//////////////////////////////////////////////////////////////////////////////////////////
// Swagger API Dokumentation
//////////////////////////////////////////////////////////////////////////////////////////

const char* SWAGGER_JSON = "{\"swagger\":\"2.0\",\"info\":{\"description\":\"This is a sample server Petstore server.\",\"version\":\"1.0.0\",\"title\":\"IoT application\"},\"host\":\"192.168.178.63\",\"tags\":[{\"name\":\"Temperature\",\"description\":\"Getting temperature measurements\"}],\"paths\":{\"/temperature\":{\"get\":{\"tags\":[\"Temperature\"],\"summary\":\"Endpoint for getting temperature measurements\",\"description\":\"\",\"operationId\":\"getTemperature\",\"responses\":{\"200\":{\"description\":\"A list of temperature measurements\",\"schema\":{\"$ref\":\"#/definitions/temperatureMeasurement\"}}}}}},\"definitions\":{\"temperatureMeasurement\":{\"type\":\"object\",\"properties\":{\"value\":{\"type\":\"string\"},\"timestamp\":{\"type\":\"string\"}}}}}";

const char* SWAGGER_UI = "<!DOCTYPE html><html><head> <meta charset=\"UTF-8\"> <meta http-equiv=\"x-ua-compatible\" content=\"IE=edge\"> <title>Swagger UI</title> <link href='https://cdnjs.cloudflare.com/ajax/libs/meyer-reset/2.0/reset.min.css' media='screen' rel='stylesheet' type='text/css'/> <link href='https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/2.2.10/css/screen.css' media='screen' rel='stylesheet' type='text/css'/> <script>if (typeof Object.assign !='function'){(function (){Object.assign=function (target){'use strict'; if (target===undefined || target===null){throw new TypeError('Cannot convert undefined or null to object');}var output=Object(target); for (var index=1; index < arguments.length; index++){var source=arguments[index]; if (source !==undefined && source !==null){for (var nextKey in source){if (Object.prototype.hasOwnProperty.call(source, nextKey)){output[nextKey]=source[nextKey];}}}}return output;};})();}</script> <script src='https://cdnjs.cloudflare.com/ajax/libs/jquery/1.8.0/jquery-1.8.0.min.js' type='text/javascript'></script> <script>(function(b){b.fn.slideto=function(a){a=b.extend({slide_duration:\"slow\",highlight_duration:3E3,highlight:true,highlight_color:\"#FFFF99\"},a);return this.each(function(){obj=b(this);b(\"body\").animate({scrollTop:obj.offset().top},a.slide_duration,function(){a.highlight&&b.ui.version&&obj.effect(\"highlight\",{color:a.highlight_color},a.highlight_duration)})})}})(jQuery); </script> <script>jQuery.fn.wiggle=function(o){var d={speed:50,wiggles:3,travel:5,callback:null};var o=jQuery.extend(d,o);return this.each(function(){var cache=this;var wrap=jQuery(this).wrap(' <div class=\"wiggle-wrap\"></div>').css(\"position\",\"relative\");var calls=0;for(i=1;i<=o.wiggles;i++){jQuery(this).animate({left:\"-=\"+o.travel},o.speed).animate({left:\"+=\"+o.travel*2},o.speed*2).animate({left:\"-=\"+o.travel},o.speed,function(){calls++;if(jQuery(cache).parent().hasClass('wiggle-wrap')){jQuery(cache).parent().replaceWith(cache);}if(calls==o.wiggles&&jQuery.isFunction(o.callback)){o.callback();}});}});}; </script> <script src='https://cdnjs.cloudflare.com/ajax/libs/jquery.ba-bbq/1.2.1/jquery.ba-bbq.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/handlebars.js/4.0.5/handlebars.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/lodash-compat/3.10.1/lodash.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/backbone.js/1.1.2/backbone-min.js' type='text/javascript'></script> <script>Backbone.View=(function(View){return View.extend({constructor: function(options){this.options=options ||{}; View.apply(this, arguments);}});})(Backbone.View); </script> <script src='https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/2.2.10/swagger-ui.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.10.0/highlight.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/highlight.js/9.10.0/languages/json.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/json-editor/0.7.28/jsoneditor.min.js' type='text/javascript'></script> <script src='https://cdnjs.cloudflare.com/ajax/libs/marked/0.3.6/marked.min.js' type='text/javascript'></script> <script type=\"text/javascript\">$(function (){url=\"http://192.168.178.63/swagger.json\"; hljs.configure({highlightSizeThreshold: 5000}); window.swaggerUi=new SwaggerUi({url: url, dom_id: \"swagger-ui-container\", supportedSubmitMethods: ['get', 'post', 'put', 'delete', 'patch'], validatorUrl: null, onComplete: function(swaggerApi, swaggerUi){}, onFailure: function(data){log(\"Unable to Load SwaggerUI\");}, docExpansion: \"none\", jsonEditor: false, defaultModelRendering: 'schema', showRequestHeaders: false, showOperationIds: false}); window.swaggerUi.load(); function log(){if ('console' in window){console.log.apply(console, arguments);}}}); </script></head> <body class=\"swagger-section\"><div id='header'><div class=\"swagger-ui-wrap\"> <a id=\"logo\" href=\"http://swagger.io\"><img class=\"logo__img\" alt=\"swagger\" height=\"30\" width=\"30\" src=\"https://cdnjs.cloudflare.com/ajax/libs/swagger-ui/2.2.10/images/logo_small.png\"/><span class=\"logo__title\">swagger</span></a><form id='api_selector'></form></div></div><div id=\"message-bar\" class=\"swagger-ui-wrap\" data-sw-translate>&nbsp;</div><div id=\"swagger-ui-container\" class=\"swagger-ui-wrap\"></div></body></html>";

//////////////////////////////////////////////////////////////////////////////////////////
// Hauptseite HTML
//////////////////////////////////////////////////////////////////////////////////////////

const char MAIN_PAGE[] PROGMEM = R"=="==(
<!DOCTYPE html>
<html>
<HEAD>
    <title>Rundenz&aumlhler
    </title>
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
    <link rel="stylesheet"
          href="https://fonts.googleapis.com/css2?family=Material+Symbols+Outlined:opsz,wght,FILL,GRAD@20..48,100..700,0..1,-50..200" />
</HEAD>

<STYLE>

.html{
    width = 100%;
    margin: 0px 0px;
}

.card{
     min-width: 500px;
     min-height: 400px;
     background: #02b875;
     box-sizing: border-box;
     font-family: Arial, Helvetica, sans-serif;
     font-size: 40px;
     color: #FFF;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
     padding: 10px 12px;
     margin: 3px 3px;
  border-radius: 12px;
}

.headcard{
    width = 100%;
     background: #02b875;
     box-sizing: border-box;
     font-family: Arial, Helvetica, sans-serif;
     color: #FFF;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
     padding: 0px 12px;
     margin: 3px 3px;
  border-radius: 12px;
}

table, tr, td, th {
  vertical-align: middle;
  horizontal-align: middle;
  color: #FFF;
  text-align: left;
  font-family: Arial, Helvetica, sans-serif;
  margin: 0px 0px;
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
  text-decoration: none;
  display: inline-flex;
    align-items: center;
    justify-content: space-around;
  margin: 10px 8px;
  padding:4px 10px;
}
.material-button {
  vertical-align:middle;
  border: 0px solid white;
  background-color: #02b875;
  color: white;
  text-align: center;
  text-decoration: none;
}
.nbsp  { font-size: 10px; }
.links  { float: left; width: 50%; }
.float_links  { float: left; }
.rechts { float: right; }
.material-symbols-outlined {
  font-variation-settings:
  'FILL' 0,
  'wght' 600,
  'GRAD' 200,
  'opsz' 48
  background-color: #02a875;
}

/* The Modal (background) */
.modal {
  display: none;
  position: fixed;
  z-index: 1;
  padding-top: 200px;
  left: 0;
  top: 0;
  width: 100%;
  height: 100%;
  overflow: auto;
  background-color: rgb(0,0,0);
  background-color: rgba(0,0,0,0.4);
}

/* Modal Content */
.modal-content {
  margin: auto;
  padding: 20px;
  border: 1px solid #888;
  width: 60%;
     background: #02b875;
  background-color: #02b875;
     box-sizing: border-box;
     font-family: Arial, Helvetica, sans-serif;
     font-size: 40px;
     color: #FFF;
     box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
     padding: 10px 15px;
     margin: 8px 8px;
  border-radius: 12px;
}

/* The Close Button */
.close {
  color: #aaaaaa;
  float: right;
  font-size: 28px;
  font-weight: bold;
}

.close:hover,
.close:focus {
  color: #000;
  text-decoration: none;
  cursor: pointer;
}

i.md40px { font-size: 40px; }
i.md50px { font-size: 44px; }
i.md60px { font-size: 60px; }
</STYLE>

<BODY>

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
  <col style="width:33%">
  <col style="width:34%">
  <col style="width:33%">
<TR><TD>Gesamt</TD>
    <TD class="td-center"><span id="lane1_total_time">0.000</span></TD>
    <TD class="td-right"><span id="lane1_lap_part_0">0.000</span></TD></TR>
<TR><TD>Best</TD>
    <TD class="td-center"><span id="lane1_fast_lap">0.000</span></TD>
    <TD class="td-right"><span id="lane1_lap_part_1">0.000</span></TD></TR>
<TR><TD>Schnitt</TD>
    <TD class="td-center"><span id="lane1_average_lap">0.000</span></TD>
    <TD class="td-right"><span id="lane1_lap_part_2">0.000</span></TD></TR>
<TR><TD class="nbsp">&nbsp</TD></TR>
</TABLE>
<TABLE>
  <col style="width:33%">
  <col style="width:34%">
  <col style="width:33%">
<TR><TD class="td-center"><span id="lane1_lap0">0.000</span></TD>
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
  <col style="width:33%">
  <col style="width:34%">
  <col style="width:33%">
<TR><TD>Gesamt</TD>
    <TD class="td-center"><span id="lane2_total_time">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap_part_0">0.000</span></TD></TR>
<TR><TD>Best</TD>
    <TD class="td-center"><span id="lane2_fast_lap">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap_part_1">0.000</span></TD></TR>
<TR><TD>Schnitt</TD>
    <TD class="td-center"><span id="lane2_average_lap">0.000</span></TD>
    <TD class="td-right"><span id="lane2_lap_part_2">0.000</span></TD></TR>
<TR><TD class="nbsp">&nbsp</TD></TR>
</TABLE>
<TABLE>
  <col style="width:33%">
  <col style="width:34%">
  <col style="width:33%">
<TR><TD class="td-center"><span id="lane2_lap0">0.000</span></TD>
    <TD class="td-center"><span id="lane2_lap5">0.000</span></TD>
    <TD class="td-center"><span id="lane2_lap10">0.000</span></TD></TR>
<TR><TD class="td-center"><span id="lane2_lap1">0.000</span></TD>
    <TD class="td-center"><span id="lane2_lap6">0.000</span></TD>
    <TD class="td-center"><span id="lane2_lap11">0.000</span></TD></TR>
<TR><TD class="td-center"><span id="lane2_lap2">0.000</span></TD>
    <TD class="td-center"><span id="lane2_lap7">0.000</span></TD>
    <TD class="td-center"><span id="lane2_lap12">0.000</span></TD></TR>
<TR><TD class="td-center"><span id="lane2_lap3">0.000</span></TD>
    <TD class="td-center"><span id="lane2_lap8">0.000</span></TD>
    <TD class="td-center"><span id="lane2_lap13">0.000</span></TD></TR>
<TR><TD class="td-center"><span id="lane2_lap4">0.000</span></TD>
    <TD class="td-center"><span id="lane2_lap9">0.000</span></TD>
    <TD class="td-center"><span id="lane2_lap14">0.000</span></TD></TR>
</TABLE>
</TABLE>

<div class="float_links"> <button class="button" id="fahrer1Btn">
  <i class="material-symbols-outlined md50px">face</i>
  <i>&nbsp</i>
Fahrer1</button> </div>
<div class="float_links"> <button class="button" onclick="toggleZielminuten()">
  <i class="material-symbols-outlined md50px" id="MODUS1">alarm</i>
  <i>&nbsp</i>
5 Minuten</button> </div>
<div class="float_links"> <button class="button" onclick="toggleZielrunden()">
  <i class="material-symbols-outlined md50px" id="MODUS2">update</i>
  <i>&nbsp</i>
20 Runden</button> </div>
<div class="float_links"> <button class="button" onclick="resetLanes()">
  <i class="material-symbols-outlined md50px" id="MODUS2">speed</i>
  <i>&nbsp</i>
Schnellste Runde</button> </div>
<div class="float_links"> <button class="button" onclick="updateBild2()">
  <i class="material-symbols-outlined md50px" id="MODUS3">all_inclusive</i>
  <i>&nbsp</i>
Training</button> </div>
<div class="float_links"> <button class="button" id="fahrer2Btn">
  <i class="material-symbols-outlined md50px">face</i>
  <i>&nbsp</i>
Fahrer2</button> </div>

<!-- The Modal -->
<div id="myModalFahrer1" class="modal">
  <div class="modal-content">
    <span class="close">&times;</span>
    <label for="inputFahrer1">Fahrer 1:</label>
    <input id=inputFahrer1 type="text" maxlength="15" width=100px ></input>
    <button id="setNameFahrer1Btn" class="button" onclick="setFahrer1">Speichern</button>
  </div>
</div>

<!-- The Modal -->
<div id="myModalFahrer2" class="modal">
  <div class="modal-content">
    <span class="close">&times;</span>
    <label for="inputFahrer2">Fahrer 2:</label>
    <input id=inputFahrer2 type="text" maxlength="15"></input>
    <button id="setNameFahrer2Btn" class="button" onclick="setFahrer2">Speichern</button>
  </div>
</div>

</BODY>

<SCRIPT>

// Get the modal
var modalFahrer1 = document.getElementById("myModalFahrer1");
var modalFahrer2 = document.getElementById("myModalFahrer2");

// Get the button that opens the modal
var btn1 = document.getElementById("fahrer1Btn");
var btn2 = document.getElementById("fahrer2Btn");

btn1.onclick = function() {
  modalFahrer1.style.display = "block";
  document.getElementById("inputFahrer1").focus();
}

btn2.onclick = function() {
  modalFahrer2.style.display = "block";
  document.getElementById("inputFahrer2").focus();
}

window.onclick = function(event) {
  if (event.target == modalFahrer1) {
    modalFahrer1.style.display = "none";
  }
  if (event.target == modalFahrer2) {
    modalFahrer2.style.display = "none";
  }
}

var btnSaveFahrer1 = document.getElementById("setNameFahrer1Btn");
var btnSaveFahrer2 = document.getElementById("setNameFahrer2Btn");

btnSaveFahrer1.onclick = function() {
   document.getElementById("fahrer_1").innerHTML = document.getElementById("inputFahrer1").value;
   setFahrer();
   modalFahrer1.style.display = "none";
}
btnSaveFahrer2.onclick = function() {
   document.getElementById("fahrer_2").innerHTML = document.getElementById("inputFahrer2").value;
   setFahrer();
   modalFahrer2.style.display = "none";
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
  const fahrer_1 = document.getElementById("inputFahrer1").value;
  const fahrer_2 = document.getElementById("inputFahrer2").value;
  xhttp.open("GET", "fahrer?fahrer1="+fahrer_1+"&fahrer2="+fahrer_2, true);
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

function toggleZielrunden() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {}
  };
  xhttp.open("GET", "zielrunden", true);
  xhttp.send();
}

function toggleZielminuten() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {}
  };
  xhttp.open("GET", "zielminuten", true);
  xhttp.send();
}

setInterval(function() {
  updateData();
}, 1000);

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

      document.getElementById("lane1_lap_part_0").innerHTML = data.Lane_1.lap_part_0;
      document.getElementById("lane1_lap_part_1").innerHTML = data.Lane_1.lap_part_1;
      document.getElementById("lane1_lap_part_2").innerHTML = data.Lane_1.lap_part_2;

      for (var i = 0; i < 15; i++) {
        document.getElementById("lane1_lap"+i).innerHTML = data.Lane_1["lap"+i];
      }

      document.getElementById("lane2_lap_count").innerHTML = data.Lane_2.lap_count;
      document.getElementById("lane2_total_time").innerHTML = data.Lane_2.total_time;
      document.getElementById("lane2_fast_lap").innerHTML = data.Lane_2.fast_lap;
      document.getElementById("lane2_last_lap").innerHTML = data.Lane_2.last_lap;
      document.getElementById("lane2_average_lap").innerHTML = data.Lane_2.average_lap;

      document.getElementById("lane2_lap_part_0").innerHTML = data.Lane_2.lap_part_0;
      document.getElementById("lane2_lap_part_1").innerHTML = data.Lane_2.lap_part_1;
      document.getElementById("lane2_lap_part_2").innerHTML = data.Lane_2.lap_part_2;

      for (var i = 0; i < 15; i++) {
        document.getElementById("lane2_lap"+i).innerHTML = data.Lane_2["lap"+i];
      }
    }
  };
  xhttp.open("GET", "data", true);
  xhttp.send();
}

function toggleLed() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {}
  };
  xhttp.open("GET", "led", true);
  xhttp.send();
}

function updateBild2() {
  var url = document.getElementById("background_bild2").style["background-image","url"];
  document.getElementById("background_bild2").style["background-image","url"] = "http://192.168.178.69/capture?"+currentTime;
}

document.addEventListener("keypress", function(event) {
  if (event.key === "Enter") {
    event.preventDefault();
    document.getElementById("setNameFahrer1Btn").click();
    document.getElementById("setNameFahrer2Btn").click();
  }
  if (event.key === "Escape") {
    resetLanes();
  }
  if (event.key === " ") {
    togglePlayPause();
  }
});

</SCRIPT>
</html>
)=="==";

