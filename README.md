# Air Quality Sensor V5 
### With: provisioning, SEN55 PM and SCD41 CO<sub>2</sub> sensors, PVC enclosure and PCB mounting

Welcome to our ASRC Community Sensor Lab! We're still building this space but feel free to use the materials available. Contact [kkrueger@gc.cuny.edu](mailto:kkrueger@gc.cuny.edu) if you have any questions. We'll try to answer as soon as possible!
 
The Air Quaity Sensor (AQS) is a **low-cost, low-power** DIY device with sensors for **CO<sub>2</sub>, temperature, pressure, relative humidity, PM2.5** (dust or smoke), **VOC** (Volitale Organic Compound), and **NOx** (nitric oxide NO and nitrogen dioxide NO2). It has an on-board logger to a memory SD card and a WiFi radio chip to connect to Google Sheets for real time data reporting.<br>
The Air Quality Sensor V5 has a provisioning function to upload the WiFi name, password and Google Sheets information to the sensor without having to modify and upload the code. It keeps the last input used to connect to the most recently connected WiFi and Google Sheet. Provisioning can be changed or initiated with a button.

<img src="images/CSL-AQS-V4.0 close up.jpg" width="400" height = "300"> <img src="images/CSL-AQS-V4.0 front view.jpg" width="400" height = "300"> 

<img src="images/CSL-AQS-V4.0 back view.jpg" width="400" height = "300"> <img src="images/CSL-AQS-V4.0 top view.jpg" width="400" height = "300">

<br>

## Contents:
### 1. Required skills and tools [How to Solder](https://docs.google.com/document/d/1l6jeO-1NOw7vDZXchC9iGBB8vkmnokzwbz_wixmtFyU/edit?usp=sharing)
* Safety
* Materials and Tools
* Practice soldering
* Stripping Wires
* Mounting on breadboard
* Other tools
* Using multimeter to beep

### 2. Part list [Excel Document](https://docs.google.com/spreadsheets/d/1TGWcEC6WSEFVkZWzWj3JGiwqsN0nAnGTTTdQC7kgLis/edit?gid=0#gid=0)
### 3. Installing Arduino on your computer [arduino install](https://docs.google.com/document/d/16o8Np3GaZfSjQlxNWIgi12ycb2CosCJ2blwEGiTplbA/edit?usp=sharing) 
* Downloading Arduino
* Libraries & Board Manager

### 4. Adafruit Feather microcontroller and WiFi board set up
* Soldering Headers [1. How to Solder Air Quality Sensor](https://docs.google.com/document/d/1l6jeO-1NOw7vDZXchC9iGBB8vkmnokzwbz_wixmtFyU/edit?usp=sharing)
* Placement on breadboard & USB connection 2. How to Assemble Air Quality Sensor Boards 
* Finding Active Port
* Upload & compile Blink 2.0 Sensor Component Code Setup
* Configure SD card
	* Format with SD formatter
	* Run SD example (?)
* Configure Real Time Clock (RTC)
	* Place coin battery
	* Run RTC example to set time

### 5. Setting up the sensors [2.0 Sensor component code setup](https://docs.google.com/document/d/1QF_vD6cMe2WdAhfrvd5zp7BLdgz92-z4E1pPEGmiRQw/edit?usp=sharing)
* Explanation of sensors with datasheets
* Placement of sensors on breadboard [2. How to Assemble Air Quality Sensor Boards](https://docs.google.com/document/d/1AezJ0XiPEsc3vEHGtJqxCqGEe5GDv0d0T-DoVZDCgCY/edit?usp=sharing) 
* BME first [2.0 Sensor Component Code Setup](https://docs.google.com/document/d/1QF_vD6cMe2WdAhfrvd5zp7BLdgz92-z4E1pPEGmiRQw/edit?usp=sharing)
* SCD30 (make anote that BME has to be on the board)

	#### 5.1. Coding practice: Merging two code examples into a single Arduino sketch (program)

### 6. Finding the CSL AQS code and installing on your computer
* Download from Github
* Running V4, continue without wifi
* Troubleshooting sensors through serial monitor and OLED display
* Setting Up Google Spreadsheet and Graphs 7. Posting to Google Logging Spreadsheet
* Provisioning wifi and GSID

	#### 6.1. Data visualization practice: How to graph the data on Google Sheets

### 7. Moving to PCB board and PVC enclosure
* Assembling sensors on PCB
* 3D printing of PCB mounts
* Installing PCB mounts and placing in PVC tube

#### 7.1. Mini tutorial: 3D modeling and printing 
#### 7.2. Mini tutorial: Electronic CAD and PCB manufacturing


   * Community Sensor Lab 🌱 - Air Quality Sensor
      * [Starting From Scratch](#starting-from-scratch)
      * [Assembly](#assembly)
      * [Arduino IDE](#arduino-ide)
      * [Setting Up Google Spreadsheet](#setting-up-google-spreadsheet)
      * [How to Utilize Our Provisioning Feature](#provisioning-and-saved-state-feature)


## Starting From Scratch💡 
  * #### [List of Parts](#parts-list)

### Soldering
* [Guide On How To Solder Air Quality Sensor](https://docs.google.com/document/d/1l6jeO-1NOw7vDZXchC9iGBB8vkmnokzwbz_wixmtFyU/edit?usp=sharing)



## Assembly 
* [Air Quality Sensor Boards Assembly](https://docs.google.com/document/d/1AezJ0XiPEsc3vEHGtJqxCqGEe5GDv0d0T-DoVZDCgCY/edit?usp=sharing)

* [How to Connect Sensors to Boards](https://docs.google.com/document/d/1BrGlBl2Zau5XgyUAorEzxkr_Fa7h0j_XQgn0lQ-tBvw/edit?usp=sharing)


## Arduino IDE
<p> If you are new to Arduino, below is a guide on installing Arduino onto your computer along with the instructions on how to install the required libraries 
 and board managers needed to run our code. There are also additional setup instructions included in the guide for the WiFi and Real-Time-Clock. </p>
 
* [Installing Arduino and Getting Started](https://docs.google.com/document/d/16o8Np3GaZfSjQlxNWIgi12ycb2CosCJ2blwEGiTplbA/edit?usp=sharing)

<br>  

### Downloading Code From Github 🐈‍⬛  
If you are new to Github, **Welcome!** 
Check out our guide:
* [Downloading CSL AQS Code from Github](https://docs.google.com/document/d/1Vb9AufqkUwsVKtdozXMc4aboDjLqBNuyTXmZpECdKx0/edit?usp=sharing)

<br>

### Setting Up Your Arduino Sensor
If you are new to the process of setting up a sensor, you can check out the guide we made that allows you to test the built sensor to make sure each component of the sensor is working as intended.
* [Sensor Component Setup Guide](https://docs.google.com/document/d/1QF_vD6cMe2WdAhfrvd5zp7BLdgz92-z4E1pPEGmiRQw/edit?usp=sharing)

## Setting Up Google Spreadsheet
### HTTP-Request-to-Google-Spreadsheet 🌐
The Air Quality Sensor can post data in (almost) real-time to a spreadsheet online through HTTP methods!
For instructions on getting the AQS connected with a Google Spreadsheet, take a look at our guide: [HTTP Posting to Google Spreadsheet](https://docs.google.com/document/d/1yNolfJr7s9rs_ewV7vN-bUsllEiPPxo2xeQbsmszQKE/edit?usp=sharing).
<br>In our guide we provide:
* A template specifically for the Air Quality Sensor 
* Step-by-step instructions to using the Google Script Editor for the AQS and deploying your app script
* Managing troubleshooting issues
* Step-by-step instructions to then connecting your App Script to the AQS through the Arduino IDE

<br>

## Provisioning and Saved State Feature
Our Air-Quality Sensor allows you to input wifi and google sheet information on the fly!
For instructions on how our provisioning feature works and how to utilize it, take a look at our guide: [Intro to AQS-CSL-V4 Provisioning](https://docs.google.com/document/d/1_W05p6wplJaqzbf3onyIY-uPkPesUpoJflwmZjv0jfY/edit?usp=sharing)

<br>


## Parts List 🖥 
Here are the materials you will need with links of where to find them. 

### FeatherM0-Microcontroller-Boards

| Board | Board Image | Use | Link |
|----------------|-------------------------------|----------|:--------------------------:|
|Feather M0 WiFi (WINC1500))| <img src="images/Feather-M0-WiFi.jpeg" width="200" height = "200" >  |Connects to WiFi network. Brain of the system|[Adafruit](https://www.adafruit.com/product/3010)
|Featherwing Logger (RTC microSD)| <img src="images/Featherwing-Logger.jpeg" width="200" height = "200"> |Slot for microSD. Has a real time clock|[Adafruit](https://www.adafruit.com/product/2922)
|Featherwing OLED 128x64  | <img src="images/adafruit-featherwing.jpg" width="200" height = "200" >|Displays data values and can graph functions|[Adafruit](https://www.adafruit.com/product/4650)
|Full Sized Premium Breadboard | <img src="images/breadboard.png" width="200" height = "200" >|Foundation that connects the components together|[Adafruit](https://www.adafruit.com/product/239)

### Sensors
| Sensor | Sensor Image | Use | Purchase Link |
|----------------|-------------------------------|----------|:--------------------------:|
|Sensirion SCD30 CO2 NDIR| <img src="images/Sensirion_SD30_CO2_NDIR.jpeg" width="200" height = "200">|Measures CO2 in PPM|[Digi-Key](https://www.digikey.com/en/products/detail/sensirion-ag/SCD30/8445334?s=N4IgTCBcDaIMIHkwAIDKBRAcqhAlZAkmHMgLIIAiAQgKqrIAKA6qSALoC%2bQA)
|BME280 Temp. Press. Rel Humidity| <img src="images/BME280.jpeg" width="200" height = "200"> | Measures temperature C, pressure hPa and relative humidity %|[Digi-Key](https://www.digikey.com/en/products/detail/adafruit-industries-llc/2652/5604372)
|Sensirion SEK-SEN5X| <img src="images/SEK-SEN5X.png" width="200" height = "200" >|Measures particulate matter PM2.5+, VOC, NOx|[Digi-Key](https://www.digikey.ca/en/products/detail/sensirion-ag/SEK-SEN5X/15903862)

### Additional-Parts
| Part | Part Image | Use | Purchase Link |
|----------------|-------------------------------|----------|:--------------------------:|
|MiniBoost 5V| <img src="images/miniboost-5v.jpeg" width="200" height = "200"> |Boosts battery pawer to 5V|[Digi-Key](https://www.adafruit.com/product/4654)
|Battery Lithium Polymer LiPo 3.7V| <img src="images/Battery-Lithium-Polymer-LiPo-3.7V.jpeg" width="200" height = "200"> |Device's battery|[Digi-Key](https://www.digikey.com/en/products/detail/adafruit-industries-llc/354/5054541)
|Battery Coin cr1220| <img src="images/Battery-Coin-cr1220.jpeg" width="200" height = "200"> |Clock Battery|[Digi_Key](https://www.digikey.com/en/products/detail/renata-batteries/RENATA-CR1220-TS-1/13283109)
|Micro SD| <img src="images/Micro-SD.jpeg" width="200" height = "200">|non volatile memory|[Digi-Key](https://www.digikey.com/en/products/detail/delkin-devices-inc/USDCOEM-16GB/13882332)
|Jumper Wires| <img src="images/jumper-wire-kit.jpg" width="200" height = "200">|Connects sensor components using the breadboard|[Sparkfun](https://www.sparkfun.com/products/124)

### Diagram 🔦          
<img src="images/Sen5-LongBoard-July2023_bb.png" width="800" height = "450">


### Solar Integration ☀️ 
The Air Quality Sensor can be powered by the sun!
For instructions on getting the AQS connected to a solar panel visit our document guide: [Solar Integration for the AQS](https://docs.google.com/document/d/1Q-aV4gpCV_NFu_VYhCmv1KPlE8r25asAGBMgROmjtNk/edit?usp=sharing).


## Versions 🛠
### Air-Quality-Sensor-V4
This is currently still being worked upon. The code has been updated to allow communication from the google sheet to the sensor and allow provisioning so that the Wifi information does not need to be inputted into the code beforehand allowing it to be more versatile.[Upcoming-Version-4.0](https://github.com/Community-Sensor-Lab/Air-Quality-Sensor/tree/CSL-AQS-V4.0)
### Air-Quality-Sensor-V3
This is our current utilized version of code for our Air Quality Sensor. The code on this page is the latest code that is stable for usage. Bigger display screen, power booster, PM 2.5 and differential pressure sensors are included in this version [Version-3.0](https://github.com/Community-Sensor-Lab/Air-Quality-Sensor/tree/CSL-AQS-V3.0)
### Air-Quality-Sensor-V2.1
Code from version 2.1 of the Air Quality Sensor can be seen on the branch [Version-2.1](https://github.com/Community-Sensor-Lab/Air-Quality-Sensor/tree/CSL-AQS-V2.1)
### Air-Quality-Sensor-v1
Code from the first version of the Air Quality Sensor can be seen on the branch [Version-1](https://github.com/Community-Sensor-Lab/Air-Quality-Sensor/tree/Version-1)

