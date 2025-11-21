# Code-Sample

## 🌟 핵심 구현 기능

이 프로젝트에서 제가 직접 설계하고 구현한 주요 시스템입니다.

### 1. 인벤토리 & 장비 시스템
**획득한 무기를 관리하고 장착/해제하며, 강화할 수 있는 인벤토리 시스템을 구축했습니다.**

* **장비 슬롯 및 UI 관리 (`PSWidget_MyEquip`, `PSScrollWidget_Equipment`)**:
    * `UniformGridPanel`을 활용하여 보유 장비 리스트를 시각화하고, 장비의 상태(잠금, 장착 중, 레벨)에 따라 슬롯 UI를 동적으로 갱신합니다.
    * 장착 요청 시 빈 슬롯을 우선 탐색하여 할당하며, 슬롯이 꽉 찬 경우 선택한 장비와 교체하는 로직을 구현하여 사용자 편의성을 높였습니다.
* **정렬 알고리즘 적용**:
    * 장비 리스트가 방대해질 경우를 대비하여, STL `std::multimap`을 활용해 장착 여부, 잠금 상태, 등급, 레벨 순으로 자동 정렬되는 알고리즘을 `PSScrollWidget_Equipment`에 구현했습니다.
* **실시간 데이터 동기화**:
    * 무기 강화나 조각(Piece) 획득 시 `WeaponLevelup` 델리게이트를 통해 관련 UI가 즉시 갱신되도록 옵저버 패턴을 적용했습니다.

### 2. 캐릭터 능력치 업그레이드 시스템
**인게임 재화(골드)를 소모하여 캐릭터의 영구적인 능력치를 강화하는 성장 시스템입니다.**

* **영구적 스탯 데이터 관리 (`PSCharacterMgr`)**:
    * 체력, 공격력, 쿨타임 등 다양한 캐릭터 스탯을 `TMap<FString, int32>` 구조로 관리하여 확장이 용이하도록 설계했습니다.
    * 각 스탯의 레벨별 수치 계산 로직(`GetStatValue`)을 데이터 테이블(`RecordMgr`)과 연동하여 기획 데이터 변경에 유연하게 대처했습니다.
* **상점 구매 로직 (`PSWidgetSlot_CharacterStatStore`)**:
    * 현재 보유 골드와 업그레이드 비용을 비교하여 구매 가능 여부를 판단하고, 구매 성공 시 로컬 매니저(`PSCharacterMgr`)와 서버 데이터(`PSDataBaseMgr`)를 동시에 갱신하여 데이터 무결성을 보장했습니다.

### 3. 안드로이드 네이티브 기능 구현 (JNI 활용)
**언리얼 엔진의 기본 기능을 넘어, 안드로이드 플랫폼 고유 기능을 활용하기 위해 JNI(Java Native Interface)를 구축했습니다.**

* **APL(Android Plugin Language) 커스터마이징**:
    * `AndroidAPITemplate_APL.xml`을 수정하여 `GameActivity.java`에 AdMob(광고), Billing(결제), Google Auth(로그인) 관련 Java 코드를 직접 주입했습니다.
* **C++ ↔ Java 통신 구축**:
    * **AdMob 연동 (`PSAdMobMgr`)**: C++에서 JNI를 통해 Java의 광고 로드 메서드를 호출하고, 광고 시청 완료 콜백(`onNativeRewardResult`)을 받아 보상(부활, 재화 등)을 지급하는 양방향 통신을 구현했습니다.
    * **스레드 동기화**: 안드로이드 UI 스레드에서 오는 콜백을 `AsyncTask(ENamedThreads::GameThread, ...)`를 사용하여 언리얼 게임 스레드와 안전하게 동기화했습니다.
    * **인앱 결제 (`PSPurchaseMgr`)**: 구글 플레이 결제 흐름(`LaunchPurchaseFlow`)을 연동하여 다이아 구매 및 광고 제거 기능을 구현했습니다.

### 4. Firebase 백엔드 연동
**서버리스(Serverless) 환경에서 유저 데이터를 안전하게 저장하고 관리하기 위해 Firebase Realtime Database를 연동했습니다.**

* **인증 및 데이터베이스 설계 (`PSDataBaseMgr`)**:
    * Google Play Games 로그인 정보를 Firebase Auth 자격 증명으로 변환하여 로그인 처리하고, UserUID를 키(Key)로 하는 JSON 트리 구조(Stats, Inventory, Stage Info)를 설계했습니다.
* **비동기 데이터 로드 및 초기화**:
    * 게임 시작 시(`PSWidgetPanel_Splash`) 비동기로 데이터 스냅샷을 받아오고, 로드가 완료되면 델리게이트(`LoadCompleteDataBase`)를 호출하여 로비 진입을 허용하는 흐름을 구축했습니다.
* **오프라인 에너지 회복 시스템**:
    * 서버에 저장된 마지막 접속 시간(`LastSavedTime`)과 클라이언트의 현재 시간을 비교하여, 게임이 꺼져 있던 시간만큼 에너지를 자동으로 회복시키는 로직을 구현했습니다.
