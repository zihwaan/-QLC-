# 랜덤화 기법 및 해석 가능한 인공지능을 활용한 QLC 낸드 플래시 데이터 신뢰성 개선

## 📌 프로젝트 개요
본 프로젝트는 QLC(Quad-Level Cell) 낸드 플래시 메모리의 데이터 신뢰성 문제를 해결하기 위해 **랜덤화 기법**과 **해석 가능한 인공지능**을 접목한 새로운 접근 방식을 제안합니다. 프로젝트는 데이터 열화 및 셀 간 간섭으로 인한 신뢰성 저하 문제를 해결하고, 인공지능 기반 예측 모델을 통해 열화 현상을 미리 감지 및 보완하는 것을 목표로 합니다.

![image](https://github.com/user-attachments/assets/75d82c7c-fc7b-4652-b87a-99c06646e052)

### 🔍 연구 배경
- **낸드 플래시의 발전**: SLC → MLC → TLC → QLC로 발전하며 데이터 저장 용량이 증가.
- **문제점**: 고밀도 저장 방식으로 인해 셀 간 간섭(Cell-to-Cell Interference)과 데이터 열화(Data Retention Issue)가 심각해짐.
- **기존 해결책의 한계**: PRNG 기반 랜덤화는 특정 패턴에서 성능 저하가 발생.
- **제안 기법**: 해석 가능한 AI(Random Forest, XGBoost)를 활용하여 데이터 패턴을 분석하고 최적의 랜덤화 기법을 적용.

## 🛠️ 기술 스택
- **프로그래밍 언어**: Python, C, Verilog
- **AI 모델**: Random Forest, XGBoost
- **해석 가능성 분석**: SHAP, LIME
- **랜덤화 기법**: LFSR(Linear Feedback Shift Register)
- **시뮬레이션 환경**: Python 기반 데이터 시뮬레이션 및 분석


## 1. 연구 배경 및 필요성
- **낸드 플래시 메모리**는 고집적화 추세에 따라 셀 간 간섭 및 데이터 패턴 의존성 등 다양한 문제를 내포하고 있습니다.
- 특히 **QLC 낸드 플래시**는 셀 당 저장 비트 수가 많아지면서 데이터 열화 현상이 더욱 두드러지며, 기존 하드웨어 기반 해결책의 한계가 존재합니다.
- 이에 본 프로젝트에서는 **랜덤화 기법(LFSR 기반)** 을 통해 데이터 패턴을 무작위화하고, **해석 가능한 인공지능 모델(Random Forest, XGBoost, SHAP, LIME 등)** 을 도입하여 열화 예측 및 오류 정정 전략을 수립하였습니다.

### 연구 목표
- **데이터 열화 및 셀 간 간섭 완화**: 랜덤화를 통해 특정 데이터 패턴에 의한 열화 및 간섭 현상을 분산시키고, 셀 마모를 고르게 분포.
- **열화 예측 모델 개발**: 기계학습 기법을 이용해 낸드 플래시 열화 현상을 예측하고, 모델의 의사결정 과정을 해석 가능하게 하여 신뢰성을 확보.
- **시뮬레이션 및 성능 평가**: C, Verilog, Python을 활용한 다양한 시뮬레이션을 통해 알고리즘의 성능을 평가하고, 개선 방향 도출.

---

## 2. 주요 기능 및 구현 내용

### 랜덤화 기법 (Randomizer)
- **LFSR 기반 구현**  
  - **16비트 및 24비트 LFSR**를 활용하여 데이터의 랜덤화를 수행합니다.
  - 주요 함수: `generate_page_seed`, `lfsr_process_page` 등
  - C 언어 및 Verilog로 각각 기본 알고리즘과 하드웨어 시뮬레이션 회로를 구현
- **목적**:  
  - 데이터 패턴 무작위화를 통해 셀 간 간섭 및 특정 패턴 집중에 의한 데이터 열화를 완화  
  - 각 페이지 별 고유 랜덤 시드 생성으로 데이터 변환 수행

### 해석 가능한 인공지능 모델 (Interpretable AI)
- **모델 구성**  
  - **Random Forest** 및 **XGBoost** 기반 모델을 설계하여 낸드 플래시의 열화 예측 수행
  - 모델 해석을 위해 **SHAP**와 **LIME** 기법 적용
- **학습 및 평가**  
  - 딥러닝 모델의 경우 다수의 은닉층과 활성화 함수(ReLU), Batch Normalization, Dropout 등 적용하여 학습 안정성 확보
  - 평가 지표: Loss, Accuracy, AUC 등 (예: AUC 0.9036 등 우수한 성능 확인)

### 시뮬레이션 및 성능 분석
- **시뮬레이션 환경**:  
  - C 및 Verilog 코드로 랜덤화 알고리즘의 동작 검증
  - Python을 통한 기계학습 모델 학습 및 성능 평가
- **주요 분석 내용**:  
  - 총 디자인 면적, 타이밍 경로 지연(예: 0.90 ns) 등 하드웨어 성능 평가
  - 랜덤화 기법 적용 전후 데이터 특성(최대 연속 비트 길이, 평균 연속 길이, 시퀀스 수 등) 비교 분석

---

## 3. 기술 스택

- **프로그래밍 언어**:  
  - C (랜덤화 알고리즘 구현)
  - Verilog (하드웨어 시뮬레이션 및 회로 구현)
  - Python (데이터 전처리, 기계학습 모델 구현 및 시뮬레이션)
- **머신러닝 프레임워크 및 라이브러리**:  
  - scikit-learn, XGBoost, Random Forest
  - SHAP, LIME (모델 해석 도구)
  - TensorFlow/Keras 또는 PyTorch (심층 학습 모델 구현 시)
- **시뮬레이션 툴**:  
  - C 컴파일러, Verilog 시뮬레이터 등
---

## 4. 연구 결과 및 분석

- **랜덤화 기법 효과**:  
  - 데이터 패턴 무작위화를 통해 셀 간 간섭 및 열화 문제를 완화하는 효과를 확인하였습니다.
  - 16비트 및 24비트 LFSR의 성능 차이를 실험을 통해 분석하여, 데이터의 랜덤성이 크게 향상됨을 입증.
  
- **인공지능 모델 평가**:  
  - Random Forest와 XGBoost 모델을 통해 열화 예측을 수행, 높은 AUC와 예측 정확도를 기록.
  - SHAP, LIME 기법을 이용하여 모델의 예측 근거를 시각화하고, 데이터 신뢰성 개선 요인을 분석.
  
- **하드웨어 시뮬레이션 결과**:  
  - 전체 디자인 면적, Critical Path Delay(예: 0.90 ns) 등 하드웨어 성능 지표를 통해 실제 적용 가능성을 평가.
  - 시뮬레이션 결과를 바탕으로 알고리즘의 한계점과 개선 방향을 도출.

---

## 5. 향후 발전 방향

- **실시간 데이터 처리 적용**:  
  - 현재 시뮬레이션 기반의 연구 결과를 실제 낸드 플래시 디바이스에 적용하여 실시간 오류 정정 및 데이터 복원 기술 개발.
  
- **알고리즘 최적화**:  
  - 랜덤화 기법의 한계를 보완하기 위한 더 정교한 난수 생성 알고리즘 연구.
  - 인공지능 모델의 연산 효율성 개선 및 다양한 환경 조건에서의 성능 최적화.
  
- **응용 분야 확대**:  
  - 낸드 플래시 외 DRAM, MRAM 등 다양한 메모리 장치의 신뢰성 향상을 위한 확장 연구 진행.
  - 산업 현장에서의 적용 가능성을 검증하기 위한 실제 디바이스 기반 테스트 진행.

---

## 6. 참고 문헌

[1] Data Pattern & Memory Variation Aware Fine-Grained ECC Optimized by Neural Network for 3D-TLC NAND Flash Memories with 2.0x Data-Retention Time Extension and 30% Parity Overhead Reduction.
(2019). Proceedings of the 2019 IEEE 11th International Memory Workshop (IMW), May 2019. DOI: 10.1109/IMW.2019.8739730.
[2] 이형간, 김성래, 이명규, 공준진, 신동준. (2012). 멀티 레벨 셀 NAND 플래시 메모리의 셀 간 간섭 영향을 감소시키기 위한 문턱 전압 제어 방법. 한국통신학회 종합 학술 발표회 논문집 (추계), 2012, 79-80.
[3] 정우석. (2012). Non-Volatile Memory의 신뢰성 향상을 위한 데이터 Randomization 알고리즘. 충북대학교 정보통신공학전공 통신회로 및 시스템 국내석사 학위논문.
[4] He, R .; Hu, H .; Xiong, C .; Han, G. (2021). "Artificial Neural Network Assisted Error Correction for MLC NAND Flash Memory". Micromachines, 12(8), 879.
