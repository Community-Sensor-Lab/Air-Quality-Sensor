# Air Quality Sensor Assembly Guide - Detailed Version

## Introduction

This step-by-step guide walks you through building an air quality monitoring system using Arduino-compatible components. The finished device will measure temperature, humidity, CO2, VOC/NOX, and particulate matter, log data to an SD card, display readings on an OLED screen, and optionally upload data to Google Sheets via WiFi.

This project is designed for students and educators in STEM programs, requiring basic electronics skills that will be developed throughout the assembly process. By completion, you'll have hands-on experience with soldering, Arduino programming, sensor integration, and data visualization.

**What you'll build:** A portable air quality sensor that can monitor indoor/outdoor air conditions and track environmental data over time.

**Time required:** Approximately 6-8 hours across multiple sessions

**Prerequisites:** Basic familiarity with computers and willingness to learn electronics fundamentals

> 🚀 **Looking for a quick overview?** Check out the [streamlined guide](Readme.md).

## Detailed Contents

**0.** [Parts List](/Contents/Parts-List.md) *(Mark)*
- Students assemble all the parts in their own boxes

**1.** [Required Skills and Tools](/Contents/1.%20Required-Skills-and-Tools.md) *(Lorena)*
- Safety procedures and guidelines
- Materials and Tools overview
- Practice soldering techniques:
  - Using breadboard for practice
  - Freestyle soldering methods
- Wire stripping techniques
- Component mounting on breadboard
- Other essential tool usage
- Using multimeter for continuity testing (beep function)

**2.** [Installing Arduino and Getting Started](/Contents/2.%20Installing-Arduino-and-Getting-Started.md) *(Vincent)*
- Downloading and installing Arduino IDE
- Managing libraries and board packages
- Connecting Feather M0 and running Blink example:
  - USB connection setup
  - Finding the correct port
  - Running example Blink code to test connection

**3.** [Sensor Boards Soldering and Cables](/Contents/3.%20Soldering-Pins-and-Headers-on-Boards.md)
- BME280 sensor header pin soldering
- SCD41 sensor header pin installation
- Rewiring JST connector cable for SEN5x sensor
- OLED display header pin soldering
- Feather M0 stacking header installation:
  - Proper soldering techniques
  - Trimming excess pin length
- Data logger stacking header setup:
  - Soldering best practices
  - Pin trimming for proper fit

**3.5.** [Feather M0 WiFi Setup Procedure](/Contents/3.5%20Feather-M0-WiFi-Setup-Procedure.md)
- WiFi provisioning procedures
- Network configuration setup

**4.** [Setting Up Components](/Contents/4%20Breadboard-Assembly-and-Component-Code-Testing.md) *(Lorena)*
- Component placement on breadboard and USB connection
- Finding and selecting the active communication port
- Uploading and compiling the Blink test program
- SD card configuration:
  - Formatting SD card with SD Formatter tool
  - Running SD card example code for testing
- Real Time Clock (RTC) setup:
  - Installing coin battery for RTC backup power
  - Running RTC example to set current time
- Sensor explanation with datasheet references
- Step-by-step sensor placement on breadboard:
  - BME280 temperature/humidity sensor installation (must be installed first)
  - SCD30 CO2 sensor placement (note: BME280 dependency)

**5.** [Code Activity](/Contents/5.%20Code-Activity.md) *(Lorena)*
- Learning exercise: merging two example code bases
- Understanding code integration principles

**6.** [Downloading CSL AQS Code from GitHub](/Contents/6.%20Downloading-CSL-AQS-Code-from-GitHub.md) *(Mark)*
- Step-by-step GitHub download process
- Running V4 firmware with offline capabilities
- Sensor troubleshooting using:
  - Serial monitor output analysis
  - OLED display diagnostic information
- WiFi and network ID (SSID) provisioning through hotspot setup

**7.** [Posting to Google Logging Spreadsheet](/Contents/7.%20Posting-to-Google-Logging-Spreadsheet.md) *(Vincent)*
- Setting up Google Sheets integration
- Configuring automatic data logging
- Troubleshooting cloud connectivity issues

**8.** [How to Make Data Graphs - Lines and Time Series](/Contents/8.%20How-to-Make-Data-Graphs-Lines-and-Time-Series.md) *(Ricardo)*
- Creating effective data visualizations
- Time series graph configuration
- Analyzing air quality trends over time

**9.** [Design and Assembly of PCB](/Contents/9.%20Design-and-Assembly-of-PCB.md) *(Mark)*
- PCB design file access and required software
- PCB ordering process and specifications
- Professional sensor assembly on custom PCB

**10.** [AQS PCB Mount](/Contents/10.%20AQS-PCB-Mount.md) *(Vincent)*
- 3D printing PCB mounting components
- Installing PCB mounts and housing in PVC tube enclosure

<!-- Internal Notes:
-make doc headers smaller
-gray pages on docs
-make 3d printed or wood chunk for trimming
-JST cables for pcb sen5 is different sequential order than breadboard connector
-add screw and jst cable to parts list
-new fritzing with jst ordering
-update markdown on github readme page & links to docs
-kendra compile docs as is 

Readme Extras:
-Cable making
-->
