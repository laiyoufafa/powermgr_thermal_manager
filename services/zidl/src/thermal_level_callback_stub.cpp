/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include "thermal_level_callback_stub.h"

#include <message_parcel.h>
#include "errors.h"
#include "ipc_object_stub.h"
#include "thermal_common.h"
#include "thermal_level_callback_ipc_interface_code.h"
#include "thermal_log.h"
#include "thermal_mgr_errors.h"
#include "xcollie/xcollie.h"
#include "xcollie/xcollie_define.h"

namespace OHOS {
namespace PowerMgr {
int ThermalLevelCallbackStub::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply,
    MessageOption &option)
{
    THERMAL_HILOGD(COMP_SVC,
        "ThermalLevelCallbackStub::OnRemoteRequest, cmd = %{public}d, flags= %{public}d",
        code, option.GetFlags());
    std::u16string descripter = ThermalLevelCallbackStub::GetDescriptor();
    std::u16string remoteDescripter = data.ReadInterfaceToken();
    if (descripter != remoteDescripter) {
        THERMAL_HILOGE(COMP_SVC,
            "ThermalLevelCallbackStub::OnRemoteRequest failed, descriptor is not matched!");
        return E_GET_THERMAL_SERVICE_FAILED;
    }
    const int DFX_DELAY_MS = 10000;
    int id = HiviewDFX::XCollie::GetInstance().SetTimer("ThermalLevelCallbackStub", DFX_DELAY_MS, nullptr, nullptr,
        HiviewDFX::XCOLLIE_FLAG_NOOP);
    int ret = ERR_OK;
    if (code == static_cast<uint32_t>(PowerMgr::ThermalLevelCallbackInterfaceCode::GET_THERMAL_LEVEL)) {
        ret = GetThermalLevelStub(data);
    } else {
        ret = IPCObjectStub::OnRemoteRequest(code, data, reply, option);
    }
    HiviewDFX::XCollie::GetInstance().CancelTimer(id);
    return ret;
}

int32_t ThermalLevelCallbackStub::GetThermalLevelStub(MessageParcel& data)
{
    int32_t level;
    THERMAL_READ_PARCEL_WITH_RET(data, Int32, level, E_READ_PARCEL_ERROR_THERMAL);
    GetThermalLevel(static_cast<ThermalLevel>(level));
    return ERR_OK;
}
} // namespace PowerMgr
} // namespace OHOS