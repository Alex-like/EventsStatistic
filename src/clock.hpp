//
// Created by Alex Shchelochkov on 26.12.2022.
//
#pragma once

#include <ctime>

namespace clock_ns {
    class Clock {
    public:
        virtual std::time_t now() const = 0;
        virtual ~Clock() = default;
    };

    class SettableClock : public Clock {
    private:
        std::time_t now_;
    public:
        explicit SettableClock(std::time_t now = time(nullptr)) : now_(now) {}
        SettableClock& operator=(const SettableClock& other) = default;
        void set(std::time_t now) {
            now_ = now;
        }
        std::time_t now() const override {
            return now_;
        }
    };

    class NormalClock : public Clock {
    public:
        NormalClock& operator=(const NormalClock& other) = default;
        std::time_t now() const override {
            return std::time(nullptr);
        }
    };
}