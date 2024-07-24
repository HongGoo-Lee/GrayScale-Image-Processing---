# GrayScale Image Processing (GA2) - 미니 프로젝트

## 소개

이 프로젝트 "GrayScale Image Processing (GA2)"는 그레이스케일 이미지 처리를 위한 종합 프로그램입니다. 이 프로그램은 화소 기반 처리, 기하학적 변환, 히스토그램 처리, 화소 영역 처리 및 경계선 검출을 포함한 다양한 이미지 처리 기능을 제공합니다.

## 기능

### 1. 파일 작업
- **이미지 열기**: 지정된 파일 경로에서 원본 그레이스케일 이미지를 불러옵니다.
- **이미지 저장**: 처리된 이미지를 지정된 파일 경로에 저장합니다.

### 2. 화소 처리
- **원본 이미지 불러오기**: 원본 이미지를 변경 없이 표시합니다.
- **밝기 조절**: 각 화소에 지정된 값을 더하여 이미지의 밝기를 조절합니다.
- **반전 이미지**: 화소 값을 반전시켜 이미지의 네거티브를 만듭니다.
- **이진화 이미지**:
  - **임계값 128**: 고정된 임계값 128을 사용하여 이미지를 이진화합니다.
  - **평균값**: 화소 평균값을 임계값으로 사용하여 이미지를 이진화합니다.
  - **중위값**: 화소 중위값을 임계값으로 사용하여 이미지를 이진화합니다.
- **논리 연산**:
  - **AND**: 마스크와 AND 연산을 수행합니다.
  - **OR**: 마스크와 OR 연산을 수행합니다.
  - **NOT**: 화소 값을 반전시킵니다.
  - **XOR**: 마스크와 XOR 연산을 수행합니다.
- **포스터라이징 이미지**: 이미지의 명암 단계를 줄입니다.
- **감마 보정**: 이미지의 감마 값을 조정합니다.
- **범위 강조**: 지정된 화소 값 범위를 강조합니다.
- **파라볼라 변환**:
  - **cup형**: cup형 파라볼라 변환을 적용합니다.
  - **cap형**: cap형 파라볼라 변환을 적용합니다.

### 3. 기하학적 변환
- **확대**: 지정된 배율로 이미지를 확대합니다.
- **축소**: 지정된 배율로 이미지를 축소합니다.
- **이동**: 지정된 x 및 y 오프셋으로 이미지를 이동합니다.
- **회전**: 지정된 각도로 이미지를 회전합니다.
- **대칭**:
  - **좌우 대칭**: 이미지를 좌우 대칭시킵니다.
  - **상하 대칭**: 이미지를 상하 대칭시킵니다.

### 4. 히스토그램 처리
- **히스토그램 스트레칭**: 히스토그램을 스트레칭하여 대비를 향상시킵니다.
- **엔드-인 스트레칭**: 히스토그램의 끝부분을 제외하고 대비를 향상시킵니다.
- **히스토그램 평활화**: 히스토그램 평활화를 적용하여 대비를 향상시킵니다.

### 5. 화소 영역 처리
- **엠보싱**: 이미지를 엠보싱 필터로 처리합니다.
- **블러링**: 이미지를 블러링 필터로 처리합니다.
- **샤프닝**: 다양한 샤프닝 필터를 사용하여 이미지를 선명하게 합니다.

### 6. 경계선 검출
- **수직경계선 검출**: 특정 커널을 사용하여 경계선을 검출합니다.
- **수평경계선 검출 2**: 다른 커널을 사용하여 경계선을 검출합니다.
- **유사연산자 경계선 검출 3**: 커널 내에서 최대 차이를 기반으로 경계선을 검출합니다.
- **차연산자 경계선 검출 4**: 또 다른 특정 방법을 사용하여 경계선을 검출합니다.

## 프로젝트 구조

- **메인 함수**: 메뉴 탐색 및 함수 호출을 처리하는 프로그램의 진입점.
- **공통 함수**: 이미지 로드, 저장, 표시 및 메모리 할당/해제를 위한 유틸리티 함수.
- **메뉴 함수**: 메뉴에서 사용자 입력을 처리하는 함수.
- **이미지 처리 함수**: 화소 처리, 기하학적 변환, 히스토그램 처리, 화소 영역 처리 및 경계선 검출을 구현하는 다양한 함수.

## 의존성

- **Windows API**: 이 프로젝트는 이미지 표시를 위해 Windows API(`Windows.h`)를 사용합니다.

# 느낀점 및 향후 계획

이번 프로젝트를 통해 다양한 영상 처리 알고리즘을 알게 되어 매우 유익한 경험이었다. 특히 영상 처리의 다양한 기법들을 직접 구현하면서 이론적으로만 알고 있던 내용을 실제 코드로 작성해보는 좋은 기회가 되었습니다. 그러나 양선형 보간 및 고주파 샤프닝 등의 몇 가지 알고리즘을 구현하지 못한 점은 아쉬움으로 남습니다. 이러한 한계점을 극복하기 위해 자료를 찾아보고 무엇이 틀렸는지 디버깅을 해볼 것 입니다. 또한 여러가지의 다양한 영상처리 알고리즘을 추가할 것 입니다. 이를 통해 프로그램의 기능을 더욱 향상시켜 보다 완벽한 이미지 처리 도구로 발전시키고자 합니다.

