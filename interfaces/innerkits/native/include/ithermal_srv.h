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

#ifndef POWERMGR_INNERKITS_ITHERMAL_SRV_H
#define POWERMGR_INNERKITS_ITHERMAL_SRV_H

#include "iremote_broker.h"
#include "iremote_object.h"
#include "ithermal_temp_callback.h"
#include "ithermal_level_callback.h"
#include "thermal_srv_sensor_info.h"

namespace OHOS {
namespace PowerMgr {
class IThermalSrv : public IRemoteBroker {
public:
    enum {
        REG_THERMAL_TEMP_CALLBACK = 0,
        UNREG_THERMAL_TEMP_CALLBACK,
        REG_THERMAL_LEVEL_CALLBACK,
        UNREG_THERMAL_LEVEL_CALLBACK,
        GET_SENSOR_INFO,
        GET_TEMP_LEVEL,
        SHELL_DUMP
    };

    virtual void SubscribeThermalTempCallback(const std::vector<std::string> &typeList,
        const sptr<IThermalTempCallback>& callback) = 0;
    virtual void UnSubscribeThermalTempCallback(const sptr<IThermalTempCallback>& callback) = 0;
    virtual void SubscribeThermalLevelCallback(const sptr<IThermalLevelCallback>& callback) = 0;
    virtual void UnSubscribeThermalLevelCallback(const sptr<IThermalLevelCallback>& callback) = 0;
    virtual bool GetThermalSrvSensorInfo(const SensorType &type, ThermalSrvSensorInfo& sensorInfo) = 0;
    virtual void GetThermalLevel(ThermalLevel& level) = 0;
    virtual std::string ShellDump(const std::vector<std::string>& args, uint32_t argc) = 0;
    DECLARE_INTERFACE_DESCRIPTOR(u"ohos.powermgr.IThermalSrv");
};
} // namespace PowerMgr
} // namespace OHOS
#endif // POWERMGR_INNERKITS_ITHERMAL_SRV_H