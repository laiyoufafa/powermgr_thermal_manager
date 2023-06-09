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
#ifndef DEVICE_CONTROL_FACTORY_H
#define DEVICE_CONTROL_FACTORY_H

#include <memory>
#include <string>
#include "ithermal_action.h"

namespace OHOS {
namespace PowerMgr {
const std::string CPU_ACTION = "cpu";
const std::string CURRENT_ACTION = "current";
const std::string VOLTAGE_ACTION = "voltage";

class DeviceControlFactory {
public:
    DeviceControlFactory() = default;
    ~DeviceControlFactory() = default;
    static std::shared_ptr<IThermalAction> Create(const std::string &name);
};
} // namespace PowerMgr
} // namespace OHOS
#endif // DEVICE_CONTROL_FACTORY_H