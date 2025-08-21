---

# Fluid Simulation (CLI Renderer Prototype)

## 📌 개요

이 프로젝트는 **터미널 환경에서 구동되는 Fluid Simulation 렌더러**의 프로토타입입니다.
아직 물리 연산(Solver)은 구현하지 않았고, **CLI 렌더링 시스템 + 프레임 레이트 관리**를 먼저 구축했습니다.

* ANSI 이스케이프 코드를 이용해 화면 제어 (`ansi.c`)
* 간단한 CLI 테두리 및 화면 출력 (`render_cli.c`)
* 프레임레이트 모드 관리 (`frame_rate.c`):

  * **60 FPS 고정**
  * **120 FPS 고정**
  * **무제한 (inf)**

---

## 📂 폴더 구조

```
fluid-simulation/
├── CMakeLists.txt
├── include/                # (향후 solver API 헤더 예정)
├── render/
│   └── cli/
│       ├── ansi.h/.c       # 터미널 초기화, 커서 숨김/복원, clear, home
│       ├── render_cli.c    # CLI 테두리 및 기본 렌더링
│       ├── render_cli.h    # 렌더링 API 선언
│       ├── frame_rate.c    # 프레임레이트 관리
│       └── frame_rate.h
└── app/
    └── cli/
        ├── main_cli.c      # 실행 진입점
        └── CMakeLists.txt
```

---

## ⚙️ 빌드 방법

### 요구 사항

* CMake ≥ 3.20
* gcc / clang (C11 지원)
* 유닉스 계열 환경 (Linux/macOS)

### 빌드

```bash
cmake -S . -B build
cmake --build build -j
```

### 실행

```bash
./build/app/cli/fluid_cli
```

---

## 🖥️ 실행 화면

* 테두리가 `#` 문자로 출력되고, 상단에 FPS와 모드가 표시됩니다.
* 프로그램은 ANSI escape 코드로 커서를 숨기고, 루프 종료 시 복원합니다.
* 기본 모드는 60 FPS입니다.

예시 출력:

```
mode=fixed-60  fps=60.0  dt=0.016 sec
################################################################################
#                                                                              #
#                                                                              #
#                                                                              #
#                                                                              #
################################################################################
```

---

## 🎮 입력 (TODO)

현재는 입력 처리 미구현 상태입니다.
계획:

* `1` → 60 FPS 모드
* `2` → 120 FPS 모드
* `3` → 무제한 모드
* `q` → 종료

---

## 🧩 주요 모듈 설명

### `ansi.[ch]`

* 터미널 제어 유틸리티

  * 원래 터미널 상태 저장
  * Raw 모드 전환 (ICANON, ECHO 해제)
  * 커서 숨기기/보이기
  * 화면 clear/home

### `render_cli.[ch]`

* CLI 렌더링 함수 제공
* 현재는 **테두리 출력**만 지원

### `frame_rate.[ch]`

* FPS 모드 관리
* `fr_begin()` → 프레임 시작, dt 측정
* `fr_end()`   → 목표 프레임까지 절대시간 sleep
* `fr_fps()`   → EMA 기반 FPS 리포트
* 모드:

  * `FR_FIXED_60`
  * `FR_FIXED_120`
  * `FR_INF`

---

## 🚀 향후 계획

* **Solver 추가**: fluid CPU solver 구현, CLI에서 시각화
* **입력 처리**: 키 입력으로 모드 전환 및 종료
* **렌더링 확장**: 파티클/흐름 시각화
* **Vulkan Renderer**: CLI 렌더러 외 GUI 버전으로 확장

---
