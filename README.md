Yep 😄 Here is the **single complete `README.md` file**, with both **Linux and Windows installation/setup** included:

````markdown
# DrishtiDx

## Explainable AI for Diabetic Retinopathy Screening in Rural India

DrishtiDx is a prototype desktop application for diabetic retinopathy screening using fundus retinal images.

The application demonstrates an explainable screening workflow including image quality assessment, preprocessing, diabetic retinopathy severity grading, referable DR assessment, Grad-CAM explainability, and screening report generation.

> **Note:** This is a prototype for demonstration and research purposes. It does not perform clinical diagnosis or real medical inference.

---

## Features

- Fundus image upload
- Image quality assessment
  - Focus
  - Brightness
  - Contrast
  - Field of View (FOV)
  - Illumination
- Preliminary cataract assessment
- Retinal image preprocessing workflow
- Diabetic Retinopathy severity grading
- Referable DR assessment
- Grad-CAM explainability
- Attention heatmap visualization
- Screening report generation
- PDF report generation
- Previously generated reports management

---

## Tech Stack

- C++17
- Qt 6
- CMake
- Ninja
- Git

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
````

### 2. Clone the Repository

```bash
git clone https://github.com/ShafinMirkar/DrishtiDR.git
cd DrishtiDR
```


### 4. Configure the Project

```bash
cmake -S . -B build \
    -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake/Qt6 \
    -DCMAKE_BUILD_TYPE=Release
```

### 5. Build

```bash
cmake --build build -j$(nproc)
```

### 6. Run

```bash
./build/DrishtiDx
```

---

# Windows

The recommended Windows build environment is **MSYS2 UCRT64**.

### 1. Install MSYS2

Download and install MSYS2:

[https://www.msys2.org/](https://www.msys2.org/)

After installation, open the **MSYS2 UCRT64** terminal.

### 2. Update MSYS2

```bash
pacman -Syu --noconfirm
```

If MSYS2 asks you to restart the terminal, close it, reopen **MSYS2 UCRT64**, and run:

```bash
pacman -Syu --noconfirm
```

### 3. Install Dependencies

```bash
pacman -S --needed --noconfirm \
    mingw-w64-ucrt-x86_64-toolchain \
    mingw-w64-ucrt-x86_64-cmake \
    mingw-w64-ucrt-x86_64-ninja \
    mingw-w64-ucrt-x86_64-qt6-base
```

### 4. Clone the Repository

```bash
git clone https://github.com/ShafinMirkar/DrishtiDR.git
cd DrishtiDR
```


### 6. Configure the Project

```bash
cmake -S . -B build \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release
```

### 7. Build

```bash
cmake --build build
```

### 8. Run

```bash
./build/DrishtiDx.exe
```

---

# Application Workflow

```text
                Fundus Image
                     │
                     ▼
          Image Quality Assessment
                     │
                     ▼
              Cataract Check
                     │
                     ▼
           Image Preprocessing
                     │
                     ▼
          DR Severity Analysis
                     │
                     ▼
        Referable DR Assessment
                     │
                     ▼
             Explainability
          ┌──────────┴──────────┐
          │                     │
       Grad-CAM          Attention Map
          │                     │
          └──────────┬──────────┘
                     ▼
             Screening Report
                     │
                     ▼
                PDF Report
```

---

# Project Structure

```text
DrishtiDx/
│
├── CMakeLists.txt
│
├── include/
│   ├── AppData.h
│   ├── MainWindow.h
│   ├── QualityPage.h
│   ├── CataractPage.h
│   ├── PreprocessingPage.h
│   ├── AnalysisPage.h
│   ├── ReferablePage.h
│   ├── ExplainabilityPage.h
│   └── ReportPage.h
│
├── src/
│   ├── main.cpp
│   ├── MainWindow.cpp
│   ├── QualityPage.cpp
│   ├── CataractPage.cpp
│   ├── PreprocessingPage.cpp
│   ├── AnalysisPage.cpp
│   ├── ReferablePage.cpp
│   ├── ExplainabilityPage.cpp
│   └── ReportPage.cpp
│
├── assets/
│   ├── fundus/
│   ├── preprocessing/
│   ├── gradcam/
│   └── attention/
│
├── reports/
│
└── build/
```

---

# Report Generation

Generated screening reports are saved in:

```text
reports/
```

The PDF report contains:

* Patient information
* Image quality assessment
* DR classification
* Class probabilities
* Referable DR probability
* Decision threshold
* Referable / non-referable decision
* Grad-CAM visualization
* Attention heatmap
* Preprocessing summary
* Screening summary
* Disclaimer

---

# Diabetic Retinopathy Grades

The prototype follows the International Clinical Diabetic Retinopathy severity scale:

| Grade | Severity         |
| ----- | ---------------- |
| 0     | No DR            |
| 1     | Mild NPDR        |
| 2     | Moderate NPDR    |
| 3     | Severe NPDR      |
| 4     | Proliferative DR |

---

# Disclaimer

DrishtiDx is a prototype developed for demonstration and research purposes.

The current implementation uses simulated analysis results and prototype visualizations. It is **not a medical device** and must not be used for clinical diagnosis, treatment decisions, or patient management.

```
```
