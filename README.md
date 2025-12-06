# 🖥️ My-Top — Linux System Monitoring Tool

**My-Top**은 Linux의 `/proc` 파일 시스템을 직접 파싱하여  
CPU 사용률, 메모리 정보, 실행 중인 프로세스 목록을  
실시간 TUI(Text User Interface)로 보여주는 시스템 모니터링 도구입니다

`top`, `htop`과 유사한 기능을 제공하지만,  
외부 명령어(ps, top 등)를 호출하지 않고 구현했습니다

---

## 📌 주요 기능 (Features)

### ✔ CPU 사용률 모니터링
- `/proc/stat`의 CPU 누적 시간(user, system, idle 등)을 읽어 CPU% 계산  
- Δtotal 기반의 정확한 계산  
- 1초 단위 실시간 갱신

### ✔ 메모리 정보 출력
- `/proc/meminfo`에서 MemTotal, MemAvailable 파싱  
- 실제 사용 중인 메모리 및 사용률 표시

### ✔ 프로세스 목록 출력
- `/proc/[pid]/stat`, `/proc/[pid]/statm` 파싱  
- PID, 프로세스 이름(comm), 상태(state), CPU 시간, RSS 메모리 출력  
- 300개 이상 프로세스 처리 가능

### ✔ ncurses 기반 TUI
- 깜빡임 없는 UI  
- 선택된 프로세스 반전 표시  
- 스크롤 및 정렬 지원

### ✔ 정렬 기능
- **F1** → PID 기준 정렬  
- **F2** → CPU 사용량 기준 정렬  
- **F3** → 메모리 사용량(RSS) 기준 정렬  

### ✔ 프로세스 제어 기능 (Advanced)
- **k** → SIGKILL  
- **s** → SIGSTOP  
- **c** → SIGCONT  
- 사용자 프로세스에서 정상 작동  
- 커널 스레드는 kill 불가능 → 정상 동작

---

## 📁 프로젝트 구조 (Project Structure)

```
project/
│── main.c          # 메인 루프 / 입력 처리
│── monitor.c       # CPU, 메모리, 프로세스 정보 파싱
│── monitor.h
│── tui.c           # ncurses 기반 UI 출력
│── tui.h
│── Makefile        # 빌드 자동화
└── README.md
```

---

## 📂 /proc 파일 시스템에서 읽은 데이터

### ✓ CPU 정보 – `/proc/stat`
사용한 필드:
- user, nice, system, idle, iowait, irq, softirq, steal

CPU 사용률 계산:

```
CPU% = (Δtotal - Δidle) / Δtotal × 100
```

---

### ✓ 메모리 정보 – `/proc/meminfo`
사용한 필드:
- MemTotal
- MemAvailable

계산식:

```
Used = MemTotal - MemAvailable
```

---

### ✓ 프로세스 정보 – `/proc/[pid]/stat`
파싱한 항목:
- pid  
- comm(프로세스 이름)  
- state(R, S, T 등)  
- utime, stime(CPU 시간)

---

### ✓ 메모리 정보 – `/proc/[pid]/statm`
파싱한 값:
- rss(Resident Set Size)

---

### ✓ PID 검색 – `/proc` 디렉토리 스캔
- 디렉토리 이름이 숫자인 경우만 프로세스로 인식  
- 모든 PID 자동 수집

---

## 🔧 Build & Run

### 1) Build
```
make
```

### 2) Run
```
./mytop
```

---

## 🎮 조작 방법 (Controls)

| Key | 기능 |
|------|------|
| ↑ / ↓ | 프로세스 목록 스크롤 |
| F1 | PID 정렬 |
| F2 | CPU 정렬 |
| F3 | 메모리(RSS) 정렬 |
| k | SIGKILL |
| s | SIGSTOP |
| c | SIGCONT |
| q | 종료 |

---

## 📸 실행 화면 
<img src="https://github.com/user-attachments/assets/4e367f79-207e-4163-8d8f-82f0dd2d080b"  width = "1000"/>


---

## 📝 배운 점 & 프로젝트 의의

- Linux `/proc` 파일 시스템 구조 이해  
- top/htop 내부 동작 원리 학습  
- 운영체제 레벨의 시스템 데이터 파싱 경험  
- ncurses 기반 UI 개발 능력 향상  
- signal을 통한 프로세스 제어 구현 경험 축적  
