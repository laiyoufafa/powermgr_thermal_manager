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

#include "thermal_mgr_client.h"
#include <datetime_ex.h>
#include <if_system_ability_manager.h>
#include <ipc_skeleton.h>
#include <iservice_registry.h>
#include <string_ex.h>
#include <system_ability_definition.h>
#include "thermal_common.h"

namespace OHOS {
namespace PowerMgr {
ThermalMgrClient::ThermalMgrClient() {};
ThermalMgrClient::~ThermalMgrClient()
{
    if (thermalSrv_ != nullptr) {
        auto remoteObject = thermalSrv_->AsObject();
        if (remoteObject != nullptr) {
            remoteObject->RemoveDeathRecipient(deathRecipient_);
        }
    }
}

ErrCode ThermalMgrClient::Connect()
{
    std::lock_guard<std::mutex> lock(mutex_);
    if (thermalSrv_ != nullptr) {
        return ERR_OK;
    }

    sptr<ISystemAbilityManager> sam = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (sam == nullptr) {
        THERMAL_HILOGE(MODULE_THERMAL_INNERKIT, "%{public}s:Failed to get Registry!", __func__);
        return E_GET_SYSTEM_ABILITY_MANAGER_FAILED_THERMAL;
    }

    sptr<IRemoteObject> remoteObject_ = sam->CheckSystemAbility(POWER_MANAGER_THERMAL_SERVICE_ID);
    if (remoteObject_ == nullptr) {
        THERMAL_HILOGE(MODULE_THERMAL_INNERKIT, "GetSystemAbility failed!");
        return E_GET_THERMAL_SERVICE_FAILED;
    }

    deathRecipient_ = sptr<IRemoteObject::DeathRecipient>(new ThermalMgrDeathRecipient());
    if (deathRecipient_ == nullptr) {
        THERMAL_HILOGE(MODULE_THERMAL_INNERKIT, "%{public}s :Failed to create ThermalMgrDeathRecipient!",
            __func__);
        return ERR_NO_MEMORY;
    }

    if ((remoteObject_->IsProxyObject()) && (!remoteObject_->AddDeathRecipient(deathRecipient_))) {
        THERMAL_HILOGE(MODULE_THERMAL_INNERKIT, "%{public}s :Add death recipient to PowerMgr service failed.",
            __func__);
        return E_ADD_DEATH_RECIPIENT_FAILED_THERMAL;
    }

    thermalSrv_ = iface_cast<IThermalSrv>(remoteObject_);
    THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "%{public}s :Connecting ThermalMgrService success.", __func__);
    return ERR_OK;
}

void ThermalMgrClient::ResetProxy(const wptr<IRemoteObject>& remote)
{
    std::lock_guard<std::mutex> lock(mutex_);
    THERMAL_RETURN_IF(thermalSrv_ == nullptr);

    auto serviceRemote = thermalSrv_->AsObject();
    if ((serviceRemote != nullptr) && (serviceRemote == remote.promote())) {
        serviceRemote->RemoveDeathRecipient(deathRecipient_);
        thermalSrv_ = nullptr;
    }
}

void ThermalMgrClient::ThermalMgrDeathRecipient::OnRemoteDied(const wptr<IRemoteObject>& remote)
{
    if (remote == nullptr) {
        THERMAL_HILOGE(MODULE_THERMAL_INNERKIT, "ThermalMgrDeathRecipient::OnRemoteDied failed, remote is nullptr.");
        return;
    }

    ThermalMgrClient::GetInstance().ResetProxy(remote);
    THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "ThermalMgrDeathRecipient::Recv death notice.");
}

void ThermalMgrClient::SubscribeThermalTempCallback(const std::vector<std::string> &typeList,
    const sptr<IThermalTempCallback> &callback)
{
    THERMAL_RETURN_IF((callback == nullptr) || (Connect() != ERR_OK));
    THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "%{public}s.", __func__);
    thermalSrv_->SubscribeThermalTempCallback(typeList, callback);
}

void ThermalMgrClient::UnSubscribeThermalTempCallback(const sptr<IThermalTempCallback> &callback)
{
    THERMAL_RETURN_IF((callback == nullptr) || (Connect() != ERR_OK));
    THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "%{public}s.", __func__);
    thermalSrv_->UnSubscribeThermalTempCallback(callback);
}

void ThermalMgrClient::SubscribeThermalLevelCallback(const sptr<IThermalLevelCallback>& callback)
{
    THERMAL_RETURN_IF((callback == nullptr) || (Connect() != ERR_OK));
    THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "%{public}s.", __func__);
    thermalSrv_->SubscribeThermalLevelCallback(callback);
}

void ThermalMgrClient::UnSubscribeThermalLevelCallback(const sptr<IThermalLevelCallback>& callback)
{
    THERMAL_RETURN_IF((callback == nullptr) || (Connect() != ERR_OK));
    THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "%{public}s.", __func__);
    thermalSrv_->UnSubscribeThermalLevelCallback(callback);
}

bool ThermalMgrClient::GetThermalSrvSensorInfo(const SensorType &type, ThermalSrvSensorInfo& sensorInfo)
{
    if (Connect() != ERR_OK) {
        return false;
    }
    THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "%{public}s.", __func__);
    bool ret = thermalSrv_->GetThermalSrvSensorInfo(type, sensorInfo);
    return ret;
}

int32_t ThermalMgrClient::GetThermalSensorTemp(const SensorType type)
{
    ThermalSrvSensorInfo info;
    bool ret = GetThermalSrvSensorInfo(type, info);
    if (!ret) {
        THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "%{public}s: failed  to Get sensor info", __func__);
    }
    return info.GetTemp();
}

void ThermalMgrClient::GetLevel(ThermalLevel& level)
{
    THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "%{public}s enter", __func__);
    THERMAL_RETURN_IF(Connect() != ERR_OK);
    thermalSrv_->GetThermalLevel(level);
}

ThermalLevel ThermalMgrClient::GetThermalLevel()
{
    THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "%{public}s enter", __func__);
    ThermalLevel level = ThermalLevel::INVALID_LEVEL;
    GetLevel(level);
    return level;
}

std::string ThermalMgrClient::Dump(const std::vector<std::string>& args)
{
    std::string error = "can't connect service";
    THERMAL_RETURN_IF_WITH_RET(Connect() != ERR_OK, error);
    THERMAL_HILOGI(MODULE_THERMAL_INNERKIT, "%{public}s called.", __func__);
    return thermalSrv_->ShellDump(args, args.size());
}
} // namespace PowerMgr
} // namespace OHOS