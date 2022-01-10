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

#include "device_control_factory.h"

#include "cpu_action.h"
#include "current_action.h"
#include "voltage_action.h"

namespace OHOS {
namespace PowerMgr {
std::shared_ptr<IThermalAction> DeviceControlFactory::Create(uint32_t mode)
{
    switch (mode) {
        case MODE_CPU: {
            return std::make_shared<CpuAction>();
        }
        case MODE_CURRENT: {
            return std::make_shared<CurrentAction>();
        }
        case MODE_VOLTAGE: {
            return std::make_shared<VoltageAction>();
        }
        default:
            return nullptr;
    }
    return nullptr;
}
} // namespace PowerMgr
} // namespae OHOS