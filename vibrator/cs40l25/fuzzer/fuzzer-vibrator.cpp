/*
 * Copyright (C) 2024 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fuzzbinder/libbinder_ndk_driver.h>
#include <fuzzer/FuzzedDataProvider.h>

#include "Hardware.h"
#include "Vibrator.h"

using ::aidl::android::hardware::vibrator::HwApi;
using ::aidl::android::hardware::vibrator::HwCal;
using ::aidl::android::hardware::vibrator::Vibrator;
using android::fuzzService;
using ndk::SharedRefBase;

// No stats collection.
class FakeStatsApi : public Vibrator::StatsApi {
  public:
    FakeStatsApi() = default;
    ~FakeStatsApi() = default;

    bool logPrimitive(uint16_t) override { return true; }

    bool logWaveform(uint16_t, int32_t) override { return true; }

    bool logError(uint16_t) override { return true; }

    bool logLatencyStart(uint16_t) override { return true; }

    bool logLatencyEnd() { return true; }

    void debug(int32_t) override {}
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    auto vibService = ndk::SharedRefBase::make<Vibrator>(
            std::make_unique<HwApi>(), std::make_unique<HwCal>(), std::make_unique<FakeStatsApi>());

    fuzzService(vibService->asBinder().get(), FuzzedDataProvider(data, size));

    return 0;
}
