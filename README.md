# 턴제 RPG 전투 시스템


| | |
|---|---|
| 개발 기간 | 2025.03 – 2025.08 |
| 엔진 · 언어 | Unreal Engine 5 · C++ |
| 인원 | 1인 |

---

## 구현한 것

- **전투 루프** — 턴 순서 관리, 행동력 소비, 스킬 사용, 명중 · 치명 판정, 피해 계산
- **스킬** — 사거리 · 공격 범위 · 행동력 소모 · 명중 · 치명 · 피해량을 가진 객체. 애니메이션 노티파이 시점에 피해 적용. 사거리와 시전 범위 인디케이터
- **장비 · 버프** — 부위별 장비 슬롯과 교체에 따른 스탯 반영, 버프 적용과 지속 턴 관리를 컴포넌트로 분리
- **적 AI** — 순찰용 / 전투용 행동 트리를 상황에 따라 교체. AIPerception 시야 감지
  
```
Source/PS/Objects/SkillBase.h                            스킬 정의
Source/PS/ActorComponent/                                스킬 · 장비 · 버프 컴포넌트
Source/PS/Controller/AIController/BasicAIController.cpp  시야 탐지 · 트리 교체
Source/PS/BTTask/                                        행동 트리 태스크 · 데코레이터
Source/PS/Controller/CharacterController.cpp             전투 흐름
```
