# Air Quality Sensor Assembly Guide - Detailed Version

## Introduction

This step-by-step guide walks you through building an air quality monitoring system using Arduino-compatible components. The finished device will measure temperature, humidity, CO2, VOC/NOX, and particulate matter, log data to an SD card, display readings on an OLED screen, and optionally upload data to Google Sheets via WiFi.

This project is designed for students and educators in STEM programs, requiring basic electronics skills that will be developed throughout the assembly process. By completion, you'll have hands-on experience with soldering, Arduino programming, sensor integration, and data visualization.

**What you'll build:** A portable air quality sensor that can monitor indoor/outdoor air conditions and track environmental data over time.

**Time required:** Approximately 6-8 hours across multiple sessions

**Prerequisites:** Basic familiarity with computers and willingness to learn electronics fundamentals

> 🚀 **Looking for a quick overview?** Check out the [streamlined guide](Readme.md).

## Detailed Contents

**0.** [Parts List](https://docs.google.com/document/d/1eruTOVNQ70D9B6PCWNkDp4wKfiiPEReuvoAi7Rmay4g/edit?usp=drive_link) *(Mark)*
- Students assemble all the parts in their own boxes

**1.** [Required Skills and Tools](https://docs.google.com/document/d/1mvQsTc32bjuC7l7ijtVF-_ARMnHwnhWfpPXzsSneSdQ/edit?usp=drive_link) *(Lorena)*
- Safety procedures and guidelines
- Materials and Tools overview
- Practice soldering techniques:
  - Using breadboard for practice
  - Freestyle soldering methods
- Wire stripping techniques
- Component mounting on breadboard
- Other essential tool usage
- Using multimeter for continuity testing (beep function)

**2.** [Installing Arduino and Getting Started](https://docs.google.com/document/d/1yBIu31yoNl-S-uk0izQ7rGxLrPd03uEbaEyvdvf0bEY/edit?usp=drive_link) *(Vincent)*
- Downloading and installing Arduino IDE
- Managing libraries and board packages
- Connecting Feather M0 and running Blink example:
  - USB connection setup
  - Finding the correct port
  - Running example Blink code to test connection

**3.** [Sensor Boards Soldering and Cables](https://docs.google.com/document/d/1qe-Vhb9ftH12_VpeKCsfD-ODc87dPtZxIyc3WNPK15M/edit?usp=drive_link)
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

**3.5.** [Feather M0 WiFi Setup Procedure](https://docs.google.com/document/d/1W8ZJhmlNl1G_N5n0yRctXMS5erIFklMhOZFekRmV0GQ/edit?usp=sharing)
- WiFi provisioning procedures
- Network configuration setup

**4.** [Setting Up Components](https://docs.google.com/document/d/1WFvF_7xLqt54y61l6RR8TfNEejDlFR1TU_Uz_Xx3njM/edit?usp=drive_link) *(Lorena)*
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

**5.** [Code Activity](https://docs.google.com/document/d/18pwJ7u2U9X6tMacqrYKDeCemcAeE9zQeEivDSDVQ8Qs/edit?usp=drive_link) *(Lorena)*
- Learning exercise: merging two example code bases
- Understanding code integration principles

**6.** [Downloading CSL AQS Code from Github](https://docs.google.com/document/d/1TifAqv9jKfEEz7CfKPhilbDQkyi2U0I-H0AbGaviCVQ/edit?usp=drive_link) *(Mark)*
- Step-by-step GitHub download process
- Running V4 firmware with offline capabilities
- Sensor troubleshooting using:
  - Serial monitor output analysis
  - OLED display diagnostic information
- WiFi and network ID (SSID) provisioning through hotspot setup

**7.** [Posting to Google Logging Spreadsheet](https://docs.google.com/document/d/1lNlVMkligkhTjbFhVWIEB8BZCj9COPIhkLUQ6nwG8GI/edit?usp=drive_link) *(Vincent)*
- Setting up Google Sheets integration
- Configuring automatic data logging
- Troubleshooting cloud connectivity issues

**8.** [How to Make Data Graphs - Lines and Time Series](https://docs.google.com/document/d/1v4I3emiJqDPMkTu0Ljfq2Lmtf5U1SFGffgOTqIotLh8/edit?usp=drive_link) *(Ricardo)*
- Creating effective data visualizations
- Time series graph configuration
- Analyzing air quality trends over time

**9.** [Design and Assembly of PCB](https://docs.google.com/document/d/1sCKWZlnZ-L1BZDOygIV_YUr5N8475Aq2Q2YT3bVnMBQ/edit?usp=drive_link) *(Mark)*
- PCB design file access and required software
- PCB ordering process and specifications
- Professional sensor assembly on custom PCB

**10.** [AQS PCB Mount](https://docs.google.com/document/d/1UT8rh22DokDd82nWzjSlzTCdgBBu4pW-yA3CvoBYW14/edit?usp=drive_link) *(Vincent)*
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
