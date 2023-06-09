/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#include "thermal_mgr_dumper.h"

#include "constants.h"
#include "thermal_common.h"
#include "thermal_service.h"

namespace OHOS {
namespace PowerMgr {
namespace {
const std::string ARGS_HELP = "-h";
const std::string ARGS_DIALOG = "-d";
const std::string ARGS_THERMALINFO = "-t";
}

bool ThermalMgrDumper::Dump(const std::vector<std::string>& args, std::string& result)
{
    THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s enter", __func__);
    result.clear();
    auto argc = args.size();
    if ((argc == 0) || (args[0] == ARGS_HELP)) {
        ShowUsage(result);
        return true;
    }

    auto tms = DelayedSpSingleton<ThermalService>::GetInstance();
    if (tms == nullptr) {
        return false;
    }

    if (args[0] == ARGS_DIALOG) {
        tms->GetActionPopup()->ShowDialog(THERMAL_HIGH_TEMP_PARAMS);
        return true;
    }

    for (auto it = args.begin(); it != args.end(); it++) {
        if (*it == ARGS_THERMALINFO) {
            ShowThermalZoneInfo(result);
        } else {
            break;
        }
    }
    return true;
}

void ThermalMgrDumper::ShowThermalZoneInfo(std::string& result)
{
    auto tms = DelayedSpSingleton<ThermalService>::GetInstance();
    if (tms == nullptr) {
        THERMAL_HILOGE(MODULE_THERMALMGR_SERVICE, "%{public}s enter", __func__);
        return;
    }
    auto tzMap = tms->GetSubscriber()->GetSubscriberInfo();
    for (auto& iter : tzMap) {
        result.append("Type: ")
            .append(iter.first)
            .append("\n")
            .append("Temperature: ")
            .append(ToString(iter.second))
            .append("\n");
    }
}

void ThermalMgrDumper::ShowUsage(std::string& result)
{
    result.append("Thermal manager dump options:\n")
        .append("  [-h] \n")
        .append("  description of the cmd option:\n")
        .append("    -h: show this help.\n")
        .append("    -d: show thermal level dialog.\n")
        .append("    -t: show thermal zone data.\n");
}
} // namespace PowerMgr
} // namespace OHOS