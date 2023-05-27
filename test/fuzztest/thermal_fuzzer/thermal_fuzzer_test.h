/*
 * Copyright (c) 2022-2023 Huawei Device Co., Ltd.
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

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <random>
#include <securec.h>
#include <string>
#include <thread>

#include "ithermal_action_callback.h"
#include "ithermal_level_callback.h"
#include "ithermal_temp_callback.h"
#include "securec.h"
#include "thermal_action_callback_stub.h"
#include "thermal_level_callback_stub.h"
#include "thermal_mgr_client.h"
#include "thermal_temp_callback_stub.h"

#define FUZZ_PROJECT_NAME "fault_fuzzer"

namespace OHOS {
namespace PowerMgr {
class ThermalFuzzerTest {
public:
    class ThermalTempTestCallback : public ThermalTempCallbackStub {
    public:
        ThermalTempTestCallback() {};
        virtual ~ThermalTempTestCallback() {};
        bool OnThermalTempChanged(TempCallbackMap& tempCbMap) override;
    };
    class ThermalLevelTestCallback : public ThermalLevelCallbackStub {
    public:
        ThermalLevelTestCallback() {};
        virtual ~ThermalLevelTestCallback() {};
        bool GetThermalLevel(ThermalLevel level) override;
    };
    class ThermalActionTestCallback : public ThermalActionCallbackStub {
    public:
        ThermalActionTestCallback() {};
        virtual ~ThermalActionTestCallback() {};
        bool OnThermalActionChanged(ActionCallbackMap& actionCbMap) override;
    };
};
} // namespace PowerMgr
} // namespace OHOS
#endif // THERMAL_FUZZER_TEST_H
