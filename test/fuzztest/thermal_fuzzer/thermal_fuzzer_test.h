/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* This files contains faultlog config modules. */

#ifndef THERMAL_FUZZER_TEST_H
#define THERMAL_FUZZER_TEST_H

#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <cstddef>
#include <cstdint>
#include <fcntl.h>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <securec.h>

#include "ithermal_level_callback.h"
#include "ithermal_temp_callback.h"
#include "securec.h"
#include "thermal_mgr_client.h"
#include "thermal_temp_callback_stub.h"
#include "thermal_level_callback_stub.h"

#define FUZZ_PROJECT_NAME "fault_fuzzer"

namespace OHOS {
namespace PowerMgr {
enum class ApiNumber : int32_t {
    NUM_ZERO = 0,
    NUM_ONE,
    NUM_TWO,
    NUM_THREE
};

class ThermalFuzzerTest {
public:
    static bool DoSomethingInterestingWithMyAPI(const uint8_t* data, size_t size);
    static void TestSubscribeTemp(const uint8_t* data);
    static void TestUnSubscribeTemp();
    static void TestSubscribeLevel(const uint8_t* data);
    static void TestUnSubscribeLevel();
    static void TestGetLevel();
    static void TestGetSensorTemp(const uint8_t* data);

    class ThermalTempTestCallback : public ThermalTempCallbackStub {
    public:
        ThermalTempTestCallback() {};
        virtual ~ThermalTempTestCallback() {};
        virtual void OnThermalTempChanged(TempCallbackMap &tempCbMap) override;
    };
    class ThermalLevelTestCallback : public ThermalLevelCallbackStub {
    public:
        ThermalLevelTestCallback() {};
        virtual ~ThermalLevelTestCallback() {};
        virtual void GetThermalLevel(ThermalLevel level) override;
    };
};
}
}
#endif // THERMAL_FUZZER_TEST_H