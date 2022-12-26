//
// Created by Alex Shchelochkov on 26.12.2022.
//
#pragma once

#include <cstdint>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "clock.hpp"

namespace event {
    class EventsStatistic {
    public:
        virtual ~EventsStatistic() = default;
        virtual void incEvent(std::string name) = 0;
        virtual double getEventStatisticByName(std::string name) = 0;
        virtual std::map<std::string, double> getAllEventStatistic() = 0;
        virtual void printStatistic() = 0;
    };

    class EventsStatisticImpl : public EventsStatistic {
    private:
        clock_ns::Clock* clock;
        std::map<std::string, std::vector<std::time_t>> events = {};

        double last_hour_stat(const std::vector<std::time_t>& instants) const {
            std::time_t now = clock->now();
            std::tm *tmp = std::localtime(&now);
            tmp->tm_hour -= 1;
            std::time_t now_minus_hour = std::mktime(tmp);
            size_t events_amount = 0;
            for (const auto &t : instants)
                if (std::difftime(t, now_minus_hour) >= .0)
                    events_amount++;
            return double(events_amount) / 60.;
        }
    public:
        explicit EventsStatisticImpl(clock_ns::Clock *clock = nullptr) : clock(clock) {}
        double getEventStatisticByName(std::string name) override {
            if (!events.contains(name)) return .0;
            return last_hour_stat(events[name]);
        }
        std::map<std::string, double> getAllEventStatistic() override {
            std::map<std::string, double> res = {};
            for (const auto &p : events)
                res[p.first] = last_hour_stat(p.second);
            return res;
        }
        void incEvent(std::string name) override {
            if (!events.contains(name))
                events[name] = {};
            events[name].push_back(clock->now());
        }
        void printStatistic() override {
            for (const auto &p : getAllEventStatistic())
                std::cout << p.first << " : " << p.second << '\n';
        }
    };
}