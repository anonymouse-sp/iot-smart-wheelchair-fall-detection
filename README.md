# 🚀 IoT Smart Wheelchair Fall Detection System

## 📌 Overview

This project is designed to improve the safety of elderly and physically challenged individuals by detecting falls in real-time using IoT technology.

## 🎯 Objective

To detect sudden falls using motion sensors and send emergency alerts through the internet.

## ⚙️ Components Used

* ESP32
* MPU6050 (Accelerometer + Gyroscope)
* DFPlayer Mini (Voice Module)
* Buzzer
* Push Button (Snooze)
* Wi-Fi Module (Built-in ESP32)

## 🔧 Working

* Sensor continuously monitors movement
* If sudden jerk (>2g) is detected → fall suspected
* Buzzer starts alerting
* User has 20 seconds to cancel
* If not cancelled → emergency alert sent via Blynk
* Voice alert is played

## 🌐 Features

* Real-time fall detection
* Emergency alert system
* False alarm cancellation (snooze)
* Voice-based alert system
* IoT monitoring using Blynk

## 📂 Project Structure

* code/ → Arduino program
* hardware/ → circuit diagrams
* docs/ → report & PPT
* images/ → project photos

## 🚀 Future Scope

* Mobile app integration
* GPS tracking
* AI-based fall prediction

## 👨‍💻 Author

* Surya Prasath J A
