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

#ifndef THERMAL_ACTION_HUB_TEST_H
#define THERMAL_ACTION_HUB_TEST_H

#include <gtest/gtest.h>
#include "ithermal_action_callback.h"
#include "thermal_action_callback_stub.h"

namespace OHOS {
namespace PowerMgr {
constexpr int SLEEP_WAIT_TIME_LOOP_S = 3;
const uint32_t MAX_PATH = 256;
const uint32_t WAIT_TIME = 10;
std::string batteryCurrentPath = "/data/service/el0/thermal/mitigation/charger/current";
std::string cpuFreqPath = "/data/service/el0/thermal/mitigation/cpu/freq";
std::string batteryPath = "/data/service/el0/thermal/sensor/battery/temp";
std::string shellPath = "/data/service/el0/thermal/sensor/shell/temp";
std::string chargerPath = "/data/service/el0/thermal/sensor/charger/temp";
std::string socPath = "/data/service/el0/thermal/sensor/soc/temp";
std::string ambientPath = "/data/service/el0/thermal/sensor/ambient/temp";
std::string cpuPath = "/data/service/el0/thermal/sensor/cpu/temp";
std::string paPath = "/data/service/el0/thermal/sensor/pa/temp";
std::string apPath = "/data/service/el0/thermal/sensor/ap/temp";

class ThermalActionHubTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    static int32_t WriteFile(std::string path, std::string buf, size_t size);
    static int32_t ReadFile(const char *path, char *buf, size_t size);
    void InitData();

    class ThermalActionTest1Callback : public ThermalActionCallbackStub {
    public:
        ThermalActionTest1Callback() {};
        virtual ~ThermalActionTest1Callback() {};
        virtual void OnThermalActionChanged(ActionCallbackMap &actionCbMap) override;
    };

    class ThermalActionTest2Callback : public ThermalActionCallbackStub {
    public:
        ThermalActionTest2Callback() {};
        virtual ~ThermalActionTest2Callback() {};
        virtual void OnThermalActionChanged(ActionCallbackMap &actionCbMap) override;
    };

    class ThermalActionTest3Callback : public ThermalActionCallbackStub {
    public:
        ThermalActionTest3Callback() {};
        virtual ~ThermalActionTest3Callback() {};
        virtual void OnThermalActionChanged(ActionCallbackMap &actionCbMap) override;
    };

    class ThermalActionTest4Callback : public ThermalActionCallbackStub {
    public:
        ThermalActionTest4Callback() {};
        virtual ~ThermalActionTest4Callback() {};
        virtual void OnThermalActionChanged(ActionCallbackMap &actionCbMap) override;
    };

    class ThermalActionTest5Callback : public ThermalActionCallbackStub {
    public:
        ThermalActionTest5Callback() {};
        virtual ~ThermalActionTest5Callback() {};
        virtual void OnThermalActionChanged(ActionCallbackMap &actionCbMap) override;
    };

    class ThermalActionTest6Callback : public ThermalActionCallbackStub {
    public:
        ThermalActionTest6Callback() {};
        virtual ~ThermalActionTest6Callback() {};
        virtual void OnThermalActionChanged(ActionCallbackMap &actionCbMap) override;
    };
};
} // namespace PowerMgr
} // namespace OHOS
#endif // THERMAL_ACTION_HUB_TEST_H
