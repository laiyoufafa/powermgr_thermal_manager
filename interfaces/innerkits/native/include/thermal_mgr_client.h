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

#ifndef THERMAL_SERVICE_CLIENT_H
#define THERMAL_SERVICE_CLIENT_H

#include <string>
#include <singleton.h>
#include "ithermal_srv.h"
#include "thermal_srv_sensor_info.h"

namespace OHOS {
namespace PowerMgr {
class ThermalMgrClient final : public DelayedRefSingleton<ThermalMgrClient> {
    DECLARE_DELAYED_REF_SINGLETON(ThermalMgrClient)

public:
    DISALLOW_COPY_AND_MOVE(ThermalMgrClient);

    void SubscribeThermalTempCallback(const std::vector<std::string> &typeList,
        const sptr<IThermalTempCallback>& callback);
    void UnSubscribeThermalTempCallback(const sptr<IThermalTempCallback>& callback);
    void SubscribeThermalLevelCallback(const sptr<IThermalLevelCallback>& callback);
    void UnSubscribeThermalLevelCallback(const sptr<IThermalLevelCallback>& callback);
    int32_t GetThermalSensorTemp(const SensorType type);
    ThermalLevel GetThermalLevel();
    std::string Dump(const std::vector<std::string>& args);
private:
    class ThermalMgrDeathRecipient : public IRemoteObject::DeathRecipient {
    public:
        ThermalMgrDeathRecipient() = default;
        ~ThermalMgrDeathRecipient() = default;
        void OnRemoteDied(const wptr<IRemoteObject>& remote);
    private:
        DISALLOW_COPY_AND_MOVE(ThermalMgrDeathRecipient);
    };

private:
    ErrCode Connect();
    void GetLevel(ThermalLevel& level);
    bool GetThermalSrvSensorInfo(const SensorType &type, ThermalSrvSensorInfo& sensorInfo);
private:
    sptr<IThermalSrv> thermalSrv_ {nullptr};
    sptr<IRemoteObject::DeathRecipient> deathRecipient_ {nullptr};
    void ResetProxy(const wptr<IRemoteObject>& remote);
    std::mutex mutex_;
};
} // namespace PowerMgr
} // namespace OHOS
#endif // THERMAL_SERVICE_CLIENT_H