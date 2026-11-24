# Testing and Validation

## 1. Introduction

This document describes the test cases planned for validating the Smart Fire Escape System. Testing covers sensor inputs, alert outputs, dashboard updates and cloud notifications.

## 2. Test Cases

| Test ID | Test Scenario | Expected Result |
|---|---|---|
| TC01 | Normal operating condition | Green LED ON, red LED OFF and buzzer OFF |
| TC02 | Smoke detected above threshold | Fire alert activated |
| TC03 | Flame detected | Fire alert activated |
| TC04 | PIR motion detected | Motion status updated on dashboard |
| TC05 | Dashboard accessed through local Wi-Fi | Dashboard webpage loads |
| TC06 | Sensor values change | Dashboard receives SSE updates |
| TC07 | Blynk connected during fire detection | Blynk event notification sent |
| TC08 | Blynk unavailable | Local detection and alerts continue |

## 3. Validation Criteria

The system should be evaluated based on:

- Correct sensor input detection.
- Appropriate LED and buzzer responses.
- Accurate dashboard status updates.
- Successful Blynk event delivery when connected.
- Continued local alert operation when cloud connectivity is unavailable.

## 4. Testing Status

The above cases represent the planned validation procedure. Physical hardware testing and measured results are pending.

Firmware compilation and hardware validation should be recorded separately, with actual results added after testing.

## 5. Conclusion

This test plan provides a structured approach to evaluating the prototype's sensor detection, alert mechanisms, web monitoring and cloud notification features.

## 6. Firmware Compilation Results

The firmware was successfully compiled using Arduino IDE with the NodeMCU ESP8266 board configuration.

### Memory Usage

| Memory Type | Used | Available | Usage |
|---|---:|---:|---:|
| RAM | 32,156 bytes | 80,192 bytes | 40% |
| Instruction RAM (IRAM) | 61,427 bytes | 65,536 bytes | 93% |
| Flash | 297,512 bytes | 1,048,576 bytes | 28% |

### Result

- Firmware compilation: Successful
- Compilation errors: None reported
- Hardware upload: Pending
- Physical sensor testing: Pending

**Observation:** IRAM usage is relatively high at 93%. Further firmware modifications should be monitored to avoid exceeding available instruction memory.
