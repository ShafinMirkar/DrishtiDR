# DrishtiDx

**Explainable AI for Diabetic Retinopathy Screening in Rural India**

DrishtiDx is a prototype desktop application for diabetic retinopathy screening using fundus retinal images. The application demonstrates an explainable screening workflow including image quality assessment, preprocessing, DR severity grading, referable DR assessment, Grad-CAM explainability, and report generation.

> **Note:** This project is currently a prototype and does not perform clinical diagnosis or real AI-based medical inference.

---

## Features

- 🖼️ Fundus image upload
- 🔍 Image quality assessment
  - Focus
  - Brightness
  - Contrast
  - Field of View (FOV)
  - Illumination
- 👁️ Preliminary cataract assessment
- ⚙️ Retinal image preprocessing workflow
- 🩺 Diabetic Retinopathy severity grading
  - Grade 0: No DR
  - Grade 1: Mild NPDR
  - Grade 2: Moderate NPDR
  - Grade 3: Severe NPDR
  - Grade 4: Proliferative DR
- 🚨 Referable DR assessment
- 🔥 Grad-CAM explainability
- 🗺️ Attention heatmap visualization
- 📄 Screening report generation as PDF
- 📚 Previously generated reports
- 💻 Cross-platform desktop application

---

## Tech Stack

- **C++17**
- **Qt 6**
- **CMake**
- **Ninja** (Windows/MSYS2)
- **Git**

---

# Installation

## Linux

### 1. Install Dependencies

```bash
sudo apt update

sudo apt install -y \
    build-essential \
    cmake \
    git \
    pkg-config \
    qt6-base-dev \
    qt6-base-dev-tools
