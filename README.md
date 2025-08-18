# Particle Sim (CLI, C)

터미널에서 **문자 하나(o)** 를 입자로 사용해 통통 튀는 애니메이션을 그리는 초간단 데모입니다.
`termios`로 **Raw 모드**(ICANON/ECHO off, 논블로킹 설정)를 적용하고, ANSI 이스케이프 시퀀스로 화면을 갱신합니다.

---

## 요구 사항

* POSIX 환경 (Linux, macOS, WSL 등)
* C 컴파일러 (gcc/clang)

> **Windows 네이티브 콘솔**에선 `termios`/ANSI 동작이 제한됩니다. 필요 시 **WSL** 또는 MSYS2/ConEmu 같은 ANSI 호환 터미널을 사용하세요.

---

## 빌드 & 실행

```bash
make
./particle_sim
```

종료는 `Ctrl + C` (강제 종료)로 합니다.
(코드 마지막에 커서 복구를 호출하므로 보통은 정상 복구됩니다.)

---

## 동작 설명

* **화면 렌더링**

  * `\x1b[2J` : 화면 전체 지우기
  * `\x1b[H`  : 커서 홈(1,1)으로 이동
  * `\x1b[?25l` : 커서 숨김
  * `\x1b[?25h` : 커서 보이기(프로그램 종료 시)

* **물리 업데이트**

  * 위치 적분: `x += vx * dt; y += vy * dt;`
  * 중력 효과: `vy += 20.0f * dt;`
  * 테두리(#)에서 반사 + 약간의 감쇠(damping)
  * 프레임레이트: `frame_rate = 120.0f`(고정), `usleep(dt * 1e6)`

* **터미널 입력 모드(`termios`)**

  * `ICANON` 해제 → 엔터 없이 한 글자 단위 입력
  * `ECHO` 해제 → 입력 문자를 화면에 자동 출력하지 않음
  * `VMIN=0`, `VTIME=0` → `read()` 호출 시 입력 없으면 즉시 0 리턴(논블로킹 형태)
  * 현재 예제는 입력을 읽진 않지만, **raw 모드 환경을 준비**해 둔 상태입니다.

---

## 코드 구조(요지)

```c
// 1) 원본 termios 저장
struct termios orig;
tcgetattr(STDIN_FILENO, &orig);

// 2) raw 모드 적용 (복사본 수정 후 tcsetattr)
enable_raw_mode(&orig);

// 3) 커서 숨김 + 화면 초기화
printf("\x1b[?25l\x1b[2J");

// 4) 메인 루프: 위치 업데이트 → 화면 그리기 → usleep
while (1) { /* ... */ }

// 5) 원복 + 커서 보이기
disable_raw_mode(&orig);
printf("\x1b[?25h");
```

`enable_raw_mode(void* orig)` 는 전달받은 `struct termios*`를 **직접 수정**하고 `tcsetattr`로 적용합니다.
`disable_raw_mode(void* orig)` 는 받은 원본 설정으로 복구합니다.

---

## 문제 해결(Troubleshooting)

* **커서가 안 보인다**
  비정상 종료 시 커서가 숨겨질 수 있습니다. 터미널에서 아래를 입력해 복구하세요:

  ```bash
  printf "\x1b[?25h"
  ```

  또는 터미널을 새로 열어도 됩니다.

* **화면이 깜빡임/느림**
  매 프레임 전체 화면을 다시 그려 간단하지만 비효율적입니다.
  최적화를 원하면 더블 버퍼링(문자 배열을 미리 구성 후 `fwrite` 한 번에 쓰기)이나 부분 업데이트를 고려하세요.

* **WSL/터미널에서 색이 안 나옴**
  현재 예제는 흑백 문자만 사용합니다. 컬러가 필요하면 ANSI 컬러 코드를 추가하세요(확장 아이디어 참고).

---

## 확장 아이디어

* **키 입력 처리**: `select()` + `read()`로 화살표 입력 받아 속도 가감, `q` 종료 등.
* **여러 파티클**: 구조체 배열로 `x,y,vx,vy,life` 관리.
* **잔상/트레일**: 프레임마다 약하게 페이드된 버퍼 유지.
* **컬러 그라데이션**: ANSI 256색으로 속도/높이에 따라 색 변경.
* **신호 처리**: `atexit()`/`sigaction(SIGINT, ...)`으로 **비정상 종료 시에도 커서 복구** 보장.

예) 간단한 종료 안전장치:

```c
#include <signal.h>
#include <stdlib.h>

void restore_and_exit(int signo) {
    printf("\x1b[?25h");
    // termios 원복도 여기서 호출 가능
    _exit(128 + signo);
}

int main(void) {
    signal(SIGINT, restore_and_exit);
    // ...
}
```

---

## 라이선스

개인 학습/연구용 예제. 자유롭게 수정/활용하세요.
