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

#include "thermal_srv_stub.h"

#include "message_parcel.h"
#include "string_ex.h"

#include "ithermal_temp_callback.h"
#include "ithermal_level_callback.h"
#include "thermal_srv_sensor_info.h"
#include "thermal_common.h"
#include "xcollie.h"

namespace OHOS {
namespace PowerMgr {
namespace {
constexpr int PARAM_MAX_NUM = 10;
}
int ThermalSrvStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    THERMAL_HILOGD(MODULE_THERMALMGR_SERVICE,
        "ThermalSrvStub::OnRemoteRequest, cmd = %{public}d, flags = %{public}d",
        code, option.GetFlags());
    std::u16string descriptor = ThermalSrvStub::GetDescriptor();
    std::u16string remoteDescriptor = data.ReadInterfaceToken();
    if (descriptor != remoteDescriptor) {
        THERMAL_HILOGE(MODULE_THERMALMGR_SERVICE, "ThermalSrvStub::OnRemoteRequest failed, descriptor is not matched!");
        return E_GET_THERMAL_SERVICE_FAILED;
    }
    const int DFX_DELAY_MS = 10000;
    int id = HiviewDFX::XCollie::GetInstance().SetTimer("ThermalSrvStub", DFX_DELAY_MS, nullptr, nullptr,
        HiviewDFX::XCOLLIE_FLAG_NOOP);
    int32_t ret = CheckRequestCode(code, data, reply, option);
    HiviewDFX::XCollie::GetInstance().CancelTimer(id);
    return ret;
}

int32_t ThermalSrvStub::CheckRequestCode(const uint32_t code, MessageParcel& data, MessageParcel& reply,
    MessageOption& option)
{
    switch (code) {
        case static_cast<int>(IThermalSrv::REG_THERMAL_TEMP_CALLBACK): {
            return SubscribeThermalTempCallbackStub(data);
        }
        case static_cast<int>(IThermalSrv::UNREG_THERMAL_TEMP_CALLBACK): {
            return UnSubscribeThermalTempCallbackStub(data);
        }
        case static_cast<int>(IThermalSrv::REG_THERMAL_LEVEL_CALLBACK): {
            return SubscribeThermalLevelCallbackStub(data);
        }
        case static_cast<int>(IThermalSrv::UNREG_THERMAL_LEVEL_CALLBACK): {
            return UnSubscribeThermalLevelCallbackStub(data);
        }
        case static_cast<int>(IThermalSrv::GET_SENSOR_INFO): {
            return GetThermalSrvSensorInfoStub(data, reply);
        }
        case static_cast<int>(IThermalSrv::GET_TEMP_LEVEL): {
            return GetThermalevelStub(reply);
        }
        case static_cast<int>(IThermalSrv::SHELL_DUMP): {
            return ShellDumpStub(data, reply);
        }
        default: {
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
        }
    }
}

int32_t ThermalSrvStub::SubscribeThermalTempCallbackStub(MessageParcel& data)
{
    THERMAL_HILOGD(MODULE_THERMALMGR_SERVICE, "SubscribeThermalTempCallbackStub Enter");
    sptr<IRemoteObject> obj = data.ReadRemoteObject();
    THERMAL_RETURN_IF_WITH_RET((obj == nullptr), E_READ_PARCEL_ERROR_THERMAL);
    sptr<IThermalTempCallback> callback = iface_cast<IThermalTempCallback>(obj);
    THERMAL_RETURN_IF_WITH_RET((callback == nullptr), E_READ_PARCEL_ERROR_THERMAL);
    std::vector<std::string> typeList;
    if (!data.ReadStringVector(&typeList)) {
        THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s failed to read type list", __func__);
        return ERR_INVALID_VALUE;
    }
    SubscribeThermalTempCallback(typeList, callback);
    return ERR_OK;
}

int32_t ThermalSrvStub::UnSubscribeThermalTempCallbackStub(MessageParcel& data)
{
    THERMAL_HILOGD(MODULE_THERMALMGR_SERVICE, "UnSubscribeThermalTempCallbackStub Enter");
    sptr<IRemoteObject> obj = data.ReadRemoteObject();
    THERMAL_RETURN_IF_WITH_RET((obj == nullptr), E_READ_PARCEL_ERROR_THERMAL);
    sptr<IThermalTempCallback> callback = iface_cast<IThermalTempCallback>(obj);
    THERMAL_RETURN_IF_WITH_RET((callback == nullptr), E_READ_PARCEL_ERROR_THERMAL);
    UnSubscribeThermalTempCallback(callback);
    return ERR_OK;
}

int32_t ThermalSrvStub::SubscribeThermalLevelCallbackStub(MessageParcel &data)
{
    THERMAL_HILOGD(MODULE_THERMALMGR_SERVICE, "SubscribeThermalLevelCallbackStub Enter");
    sptr<IRemoteObject> obj = data.ReadRemoteObject();
    THERMAL_RETURN_IF_WITH_RET((obj == nullptr), E_READ_PARCEL_ERROR_THERMAL);
    sptr<IThermalLevelCallback> callback = iface_cast<IThermalLevelCallback>(obj);
    THERMAL_RETURN_IF_WITH_RET((callback == nullptr), E_READ_PARCEL_ERROR_THERMAL);
    SubscribeThermalLevelCallback(callback);
    return ERR_OK;
}

int32_t ThermalSrvStub::UnSubscribeThermalLevelCallbackStub(MessageParcel &data)
{
    THERMAL_HILOGD(MODULE_THERMALMGR_SERVICE, "UnSubscribeThermalLevelCallbackStub Enter");
    sptr<IRemoteObject> obj = data.ReadRemoteObject();
    THERMAL_RETURN_IF_WITH_RET((obj == nullptr), E_READ_PARCEL_ERROR_THERMAL);
    sptr<IThermalLevelCallback> callback = iface_cast<IThermalLevelCallback>(obj);
    THERMAL_RETURN_IF_WITH_RET((callback == nullptr), E_READ_PARCEL_ERROR_THERMAL);
    SubscribeThermalLevelCallback(callback);
    return ERR_OK;
}

int32_t ThermalSrvStub::GetThermalSrvSensorInfoStub(MessageParcel &data, MessageParcel &reply)
{
    ThermalSrvSensorInfo sensorInfo;
    uint32_t type = 0;
    THERMAL_HILOGD(MODULE_THERMALMGR_SERVICE, "GetThermalSrvSensorInfoStub: Enter");

    THERMAL_READ_PARCEL_WITH_RET(data, Uint32, type, E_READ_PARCEL_ERROR_THERMAL);
    THERMAL_HILOGD(MODULE_THERMALMGR_SERVICE, "GetThermalSrvSensorInfoStub: Enter %{public}d", type);
    bool ret = GetThermalSrvSensorInfo(static_cast<SensorType>(type), sensorInfo);
    if (ret) {
        if (!reply.WriteParcelable(&sensorInfo)) {
            THERMAL_HILOGE(MODULE_THERMALMGR_SERVICE, "write failed");
            return -1;
        }
    }
    return ERR_OK;
}

int32_t ThermalSrvStub::GetThermalevelStub(MessageParcel& reply)
{
    THERMAL_HILOGD(MODULE_THERMALMGR_SERVICE, "GetThermalevelStub: Enter");
    ThermalLevel level;
    GetThermalLevel(level);
    THERMAL_WRITE_PARCEL_WITH_RET(reply, Uint32, static_cast<uint32_t>(level), ERR_OK);
    return ERR_OK;
}

int32_t ThermalSrvStub::ShellDumpStub(MessageParcel& data, MessageParcel& reply)
{
    uint32_t argc;
    std::vector<std::string> args;

    if (!data.ReadUint32(argc)) {
        THERMAL_HILOGE(MODULE_THERMALMGR_SERVICE, "Readback fail!");
        return E_READ_PARCEL_ERROR_THERMAL;
    }

    if (argc >= PARAM_MAX_NUM) {
        THERMAL_HILOGE(MODULE_THERMALMGR_SERVICE, "params exceed limit");
        return E_EXCEED_PARAM_LIMIT;
    }

    for (uint32_t i = 0; i < argc; i++) {
        std::string arg = data.ReadString();
        if (arg.empty()) {
            THERMAL_HILOGE(MODULE_THERMALMGR_SERVICE, "read value fail:%{public}d", i);
            return E_READ_PARCEL_ERROR_THERMAL;
        }
        args.push_back(arg);
    }

    std::string ret = ShellDump(args, argc);
    if (!reply.WriteString(ret)) {
        THERMAL_HILOGE(MODULE_THERMALMGR_SERVICE, "PowerMgrStub:: Dump Writeback Fail!");
        return E_READ_PARCEL_ERROR_THERMAL;
    }
    return ERR_OK;
}
} // namespace PowerMgr
} // namespace OHOS