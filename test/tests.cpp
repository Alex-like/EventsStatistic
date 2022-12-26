//
// Created by Alex Shchelochkov on 26.12.2022.
//
#include <gtest/gtest.h>
#include "event_statistics.hpp"

using namespace std;
using namespace clock_ns;
using namespace event;

GTEST_API_ int main(int argc, char ** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

class EventStatistics_test : public testing::Test {
protected:
    SettableClock clock;
    EventsStatisticImpl events;

    void SetUp() override {
        clock = SettableClock(time(nullptr));
        events = EventsStatisticImpl(&clock);
    }

protected:
    void skip_hours(int hours) {
        std::time_t now = clock.now();
        std::tm *tmp = std::localtime(&now);
        tmp->tm_hour += hours;
        std::time_t after_hour = std::mktime(tmp);
        clock.set(after_hour);
    }
    void skip_minutes(int minutes) {
        std::time_t now = clock.now();
        std::tm *tmp = std::localtime(&now);
        tmp->tm_min += minutes;
        std::time_t after_min = std::mktime(tmp);
        clock.set(after_min);
    }
};

TEST_F(EventStatistics_test, empty_events) {
    EXPECT_NEAR(.0, events.getEventStatisticByName("empty"), .0);
}

TEST_F(EventStatistics_test, two_events) {
    events.incEvent("event#1");
    events.incEvent("event#1");
    EXPECT_NEAR(2. / 60., events.getEventStatisticByName("event#1"), 1e-9);
}

TEST_F(EventStatistics_test, different_events) {
    events.incEvent("event#1");
    events.incEvent("event#2");
    events.incEvent("event#1");
    events.incEvent("event#3");
    EXPECT_NEAR(1. / 60., events.getEventStatisticByName("event#2"), 1e-9);
}

TEST_F(EventStatistics_test, after_hour_events) {
    events.incEvent("event#1");
    events.incEvent("event#2");
    events.incEvent("event#1");
    skip_hours(2);
    events.incEvent("event#3");
    EXPECT_NEAR(0. / 60., events.getEventStatisticByName("event#2"), 1e-9);
    EXPECT_NEAR(1. / 60., events.getEventStatisticByName("event#3"), 1e-9);
}

TEST_F(EventStatistics_test, multiple_events) {
    events.incEvent("event#1");
    events.incEvent("event#2");
    events.incEvent("event#1");
    skip_minutes(10);
    events.incEvent("event#3");
    events.incEvent("event#1");
    events.incEvent("event#2");
    skip_minutes(30);
    events.incEvent("event#3");
    map<string, double> expected = {
            {"event#1", 3./60.},
            {"event#2", 2./60.},
            {"event#3", 2./60.}
    };
    EXPECT_EQ(expected, events.getAllEventStatistic());
}