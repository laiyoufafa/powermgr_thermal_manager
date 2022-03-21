/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "cpu_action.h"
#include <climits>
#include "securec.h"
#include "thermal_common.h"

namespace OHOS {
namespace PowerMgr {
namespace {
const std::string ACTUAL_CPU_FREQ_PATH = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq";
const std::string SIM_CPU_FREQ_PATH = "/data/cooling/cpu/freq";
constexpr int32_t MAX_PATH = 256;
}
bool CpuAction::AddActionValue(uint32_t value)
{
    latestvalue_ = value;
    return true;
}

void CpuAction::Execute()
{
    static uint32_t value;
    if (value != latestvalue_) {
        if (CpuActionRequest(latestvalue_) != ERR_OK) {
            THERMAL_HILOGE(MODULE_THERMAL_PROTECTOR, "failed to set cpu freq");
        }
        value = latestvalue_;
    } else {
        latestvalue_ = 0;
    }
}

int32_t CpuAction::CpuActionRequest(uint32_t freq)
{
    THERMAL_HILOGI(MODULE_THERMAL_PROTECTOR, "%{public}d", freq);
    char cpuBuf[MAX_PATH] = {0};
    if (snprintf_s(cpuBuf, PATH_MAX, sizeof(cpuBuf) - 1, SIM_CPU_FREQ_PATH.c_str()) < ERR_OK) {
        return ERR_INVALID_VALUE;
    }
    std::string freqStr = std::to_string(freq);
    return ThermalProtectorUtil::WriteFile(cpuBuf, freqStr, freqStr.length());
}
} // namespace PowerMgr
} // namespace OHOS