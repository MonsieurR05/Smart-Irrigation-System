# 🌱 Smart Irrigation System  

An **IoT-based Smart Irrigation System** using **Arduino UNO** that monitors soil moisture, temperature, humidity, and light levels to automatically control a water pump. The system also uploads sensor data to IoT platforms like **Blynk** and **ThingSpeak** for remote monitoring.  

---

## 📌 Features  
- Monitors **soil moisture**, **temperature & humidity (DHT11)**, and **light intensity (LDR)**  
- Displays live data on a **16x2 LCD Display**  
- Automatically controls the **relay and water pump** based on soil conditions  
- Uploads data to **Blynk Dashboard** for real-time monitoring  
- Sends data to **ThingSpeak IoT Platform** for logging and analysis  

---

## 📐 System Architecture  
![Block Diagram](Smart_Irrigation/block-diagram.png)  

- **Sensors**:  
  - Soil Moisture Sensor → Detects soil water content  
  - DHT11 Sensor → Measures temperature & humidity  
  - LDR Sensor → Detects light intensity  

- **Controller**:  
  - Arduino UNO R4 (main processing unit)  

- **Actuator**:  
  - Relay Module controlling a Water Pump  

- **Display**:  
  - 16x2 LCD for live status  

- **IoT Integration**:  
  - Data uploaded to Blynk Dashboard  
  - Data uploaded to ThingSpeak  

---

## 🔌 Circuit Diagram  
![Circuit Diagram](circuit-diagram.png)  

---

## 🛠️ Components Required  
- Arduino UNO R4 (or UNO R3 compatible)  
- Soil Moisture Sensor  
- DHT11 Sensor (Temperature & Humidity)  
- LDR Sensor (Light Detection)  
- Relay Module  
- Water Pump / Motor  
- 16x2 LCD Display (with I2C module)  
- WiFi Module (built-in with UNO R4 or ESP8266 if using UNO R3)  
- Breadboard & Jumper Wires  
- Power Supply  

---

## 💻 Installation & Setup  

1. **Clone the repository**  
   ```bash
   git clone https://github.com/yourusername/Smart-Irrigation-System.git
   cd Smart-Irrigation-System
