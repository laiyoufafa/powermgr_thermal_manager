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

#include "scene_state_collection.h"

#include <algorithm>

#include "constants.h"
#include "file_operation.h"
#include "securec.h"
#include "string_operation.h"
#include "thermal_service.h"
#include "thermal_common.h"

namespace OHOS {
namespace PowerMgr {
namespace {
auto g_service = DelayedSpSingleton<ThermalService>::GetInstance();
const int MAX_PATH = 256;
std::string scenePath = "/data/thermal/state/scene";
}
bool SceneStateCollection::Init()
{
    if (!timer_) {
        timer_ = std::make_unique<ThermalTimerUtils>();
        ThermalTimerUtils::NotifyTask task = std::bind(&SceneStateCollection::SetRandomScene, this);
        timer_->Start(INTERVAL, task);
    }
    return true;
}

void SceneStateCollection::SetRandomScene()
{
    int scene = rand() % (MAX - MIN + 1) + MIN;
    SceneDecision(scene);
}

bool SceneStateCollection::InitParam(std::string &params)
{
    THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s enter", __func__);
    StringOperation::SplitString(params, paramList_, ",");
    return true;
}

std::string SceneStateCollection::GetState()
{
    THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s scene state = %{public}s", __func__, mockState_.c_str());
    if (!g_service->GetFlag()) {
        return mockState_;
    } else {
        return state_;
    }
}

void SceneStateCollection::SceneDecision(uint32_t mode)
{
    THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s enter", __func__);
    switch (mode) {
        case CAMERA: {
            auto iter = find(paramList_.begin(), paramList_.end(), SCENE_CAMERA);
            if (iter != paramList_.end()) {
                state_ = *iter;
            }
            break;
        }
        case CALL: {
            auto iter = find(paramList_.begin(), paramList_.end(), SCENE_CALL);
            if (iter != paramList_.end()) {
                state_ = *iter;
            }
            break;
        }
        case GAME: {
            auto iter = find(paramList_.begin(), paramList_.end(), SCENE_GAME);
            if (iter != paramList_.end()) {
                state_ = *iter;
            }
            break;
        }
        default:
            break;
    }
}

void SceneStateCollection::SetState()
{
    THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s enter", __func__);
    char sceneBuf[MAX_PATH] = {0};
    char sceneValue[MAX_PATH] = {0};
    std::string separator = ",";
    int32_t result = -1;
    if (snprintf_s(sceneBuf, PATH_MAX, sizeof(sceneBuf) - 1, scenePath.c_str()) < ERR_OK) {
        return;
    }
    THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s read scene state", __func__);
    result = FileOperation::ReadFile(sceneBuf, sceneValue, sizeof(sceneValue));
    if (result != ERR_OK) {
        return;
    }
    mockState_ = sceneValue;
    std::string::size_type postion = mockState_.find(separator);
    if (postion != std::string::npos) {
        StringOperation::SplitString(mockState_, sceneList_, separator);
    }
    THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s mockState_=%{public}s", __func__, mockState_.c_str());
}

bool SceneStateCollection::DecideState(const std::string &value)
{
    THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s enter", __func__);
    bool ret = false;
    bool allRet = false;
    std::string separator = ",";

    SetState();

    std::string::size_type postion = value.find(separator);
    if (postion == std::string::npos) {
        THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s no ,", __func__);
        return StringOperation::Compare(value, mockState_);
    } else {
        std::vector<std::string> sceneList;
        StringOperation::SplitString(value, sceneList, separator);
        for (auto iter = sceneList.begin(); iter != sceneList.end(); iter++) {
            if (sceneList_.empty()) return false;
            auto result = find(sceneList_.begin(), sceneList_.end(), *iter);
                THERMAL_HILOGI(MODULE_THERMALMGR_SERVICE, "%{public}s result=%{public}s, iter=%{public}s",
                    __func__, (*result).c_str(), (*iter).c_str());
            if (result != sceneList_.end()) {
                ret = true;
            } else {
                ret = false;
                break;
            }
            allRet |= ret;
        }
    }
    sceneList_.clear();
    return allRet;
}
} // namespace PowerMgr
} // namespace OHOS