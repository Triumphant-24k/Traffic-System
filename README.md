AI Smart Street Lighting System

An AI-enabled adaptive smart street lighting system using ESP32, RTC, LDR, and IR/radar-based vehicle detection. The system integrates IoT monitoring, machine learning-based traffic prediction, and multi-state adaptive illumination for energy-efficient smart-city applications.

---

Features

- Adaptive brightness control
- ML-based traffic prediction
- IoT web dashboard
- RTC-based time awareness
- Parent-child smart streetlight architecture
- Real-time vehicle detection
- Smooth LED transition control
- Energy-efficient illumination

---

Hardware Used

- ESP32
- DS3231 RTC Module
- LDR Sensor
- IR/Radar Sensor
- LED + Resistor
- Breadboard + Jumper Wires

---

Technologies Used

- Embedded C++
- ESP32 WiFi
- Python
- Decision Tree Machine Learning
- IoT Dashboard

---

Control System Concept

The project operates as a closed-loop adaptive control system where environmental sensing, traffic prediction, and real-time detection dynamically regulate streetlight brightness.

---

Machine Learning

Historical traffic data is used to train a lightweight Decision Tree model that predicts busy traffic hours and enables predictive illumination control.

---

Future Scope

- TinyML deployment
- Cloud integration
- Smart city networking
- Multi-node parent-child architecture
- Real-time traffic analytics

---

Demo Features

| State | Condition | Brightness |
|------|------|------|
| OFF | Daytime | 0% |
| DIM | Quiet Night | 20% |
| ML MODE | Predicted Traffic | 50% |
| FULL | Vehicle Detection | 100% |

---

## Author

Shiv Sriram V
