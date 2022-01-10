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

#ifndef THERMAL_KERNEL_CONFIG_FILE_H
#define THERMAL_KERNEL_CONFIG_FILE_H

#include <string>
#include <vector>
#include <memory>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "protector_base_info.h"

namespace OHOS {
namespace PowerMgr {
class ThermalKernelConfigFile {
public:
    ~ThermalKernelConfigFile() = default;
    ThermalKernelConfigFile(const ThermalKernelConfigFile&) = delete;
    ThermalKernelConfigFile& operator=(const ThermalKernelConfigFile&) = delete;
    static ThermalKernelConfigFile &GetInsance()
    {
        static ThermalKernelConfigFile instance;
        return instance;
    }

    bool Init(const std::string &path);
    void ParseThermalKernelXML(const std::string &path);
    void ParserBaseNode(xmlNodePtr node);
    void ParseControlNode(xmlNodePtr node);
private:
    std::unique_ptr<ProtectorBaseInfo> baseInfo_;
    ThermalKernelConfigFile() {};
};
} // namespace PowerMgr
} // namespace OHOS

#endif // THERMAL_KERNEL_CONFIG_FILE_H