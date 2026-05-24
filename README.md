<div align="center">
  
  ![Phishing Simulator Banner](https://img.shields.io/badge/Instagram-Phishing_Simulator-E4405F?style=for-the-badge&logo=instagram&logoColor=white)
  
  # 🎯 Instagram Phishing Simulator
  
  **Advanced Cybersecurity Education Tool | C++ Backend | Real-time Threat Simulation**
  
  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
  [![C++ Version](https://img.shields.io/badge/C%2B%2B-11-blue.svg)](https://isocpp.org/)
  [![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20macOS%20%7C%20WSL-lightgrey)](https://github.com)
  [![Security](https://img.shields.io/badge/Security-Educational_Only-red)](https://github.com)
  
  <p align="center">
    <strong>⚠️ EDUCATIONAL PURPOSE ONLY | AUTHORIZED USE REQUIRED ⚠️</strong>
  </p>
  
</div>

---

## 📋 Table of Contents
- [⚠️ Legal Disclaimer](#️-legal-disclaimer)
- [🎯 Overview](#-overview)
- [✨ Features](#-features)
- [🏗️ Architecture](#️-architecture)
- [🚀 Quick Start](#-quick-start)
- [💻 Installation & Compilation](#-installation--compilation)
- [🔧 Configuration](#-configuration)
- [📊 Output Examples](#-output-examples)
- [🧪 Testing & Validation](#-testing--validation)
- [🛡️ Security Best Practices](#️-security-best-practices)
- [📚 Educational Use Cases](#-educational-use-cases)
- [🐛 Troubleshooting](#-troubleshooting)
- [🔮 Future Roadmap](#-future-roadmap)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)

---

## ⚠️ Legal Disclaimer

<div align="center">
  <table>
    <tr>
      <td align="center">
        <strong>🔴 FOR AUTHORIZED USE ONLY 🔴</strong><br />
        This software is designed exclusively for:<br />
        • Cybersecurity training in controlled environments<br />
        • Security awareness workshops with explicit consent<br />
        • Penetration testing with written authorization<br />
        • Academic research in information security<br /><br />
        <strong>UNAUTHORIZED USE IS ILLEGAL AND UNETHICAL</strong><br />
        The developer assumes NO liability for misuse of this tool.
      </td>
    </tr>
  </table>
</div>

---

## 🎯 Overview

The **Instagram Phishing Simulator** is a sophisticated educational tool that demonstrates the technical mechanisms behind phishing attacks. Built with **C++** and **POSIX sockets**, it provides a realistic simulation environment for cybersecurity professionals, educators, and students to understand how threat actors create convincing phishing pages and capture credentials.

### 🎓 Learning Objectives
- Understand HTTP request/response mechanics
- Learn how phishing pages mimic legitimate services  
- Analyze credential interception techniques
- Develop defensive security strategies
- Identify red flags in phishing attempts

---

## ✨ Features

| Category | Features |
|----------|----------|
| **🖥️ Frontend** | • Pixel-perfect Instagram UI clone<br>• Responsive mobile-friendly design<br>• Font Awesome icons integration<br>• Realistic form validation feedback |
| **🔧 Backend** | • Multi-threaded socket server<br>• HTTP/1.1 protocol compliance<br>• URL decoding for form data<br>• Concurrent connection handling |
| **📊 Logging** | • Real-time colored console output<br>• Persistent JSON-like log files<br>• IP address tracking<br>• User-Agent capture<br>• Timestamp recording |
| **🛡️ Security** | • Error page redirection after capture<br>• No external data transmission<br>• Local storage only<br>• Proper socket cleanup |

---

## 🏗️ Architecture

```mermaid
graph LR
    A[Client Browser] -->|HTTP GET| B[Port 8080]
    B -->|Serves| C[Phishing Page]
    C -->|User submits| D[POST /login]
    D -->|Extract credentials| E[Parser]
    E -->|Log to file| F[Console & Log]
    E -->|Return error page| A
