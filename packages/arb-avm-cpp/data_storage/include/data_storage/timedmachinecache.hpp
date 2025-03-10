/*
 * Copyright 2021, Offchain Labs, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ARB_AVM_CPP_TIMEDMACHINECACHE_H
#define ARB_AVM_CPP_TIMEDMACHINECACHE_H

#include <avm/machine.hpp>
#include <avm_values/bigint.hpp>

class TimedMachineCache {
   public:
    struct Record {
        uint256_t timestamp;

        std::unique_ptr<Machine> machine;
    };

    typedef std::map<uint256_t, Record> map_type;

   private:
    map_type cache;

    const uint32_t expiration_seconds;

   public:
    explicit TimedMachineCache(uint32_t expiration_seconds_)
        : expiration_seconds{expiration_seconds_} {}

    size_t size();
    void add(std::unique_ptr<Machine> machine);
    std::optional<map_type::const_iterator> atOrBeforeGas(uint256_t gas_used);
    std::optional<TimedMachineCache::map_type::const_iterator> findMatching(
        const std::function<bool(const MachineOutput&)>& check_output);
    void reorg(uint256_t next_gas_used);
    void deleteExpired();
    [[nodiscard]] uint256_t expiredTimestamp();
    [[nodiscard]] uint256_t currentTimeExpired() const;
};

#endif  // ARB_AVM_CPP_TIMEDMACHINECACHE_H
