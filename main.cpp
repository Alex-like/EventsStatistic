#include <iostream>
#include "event_statistics.hpp"

int main() {
    auto events = event::EventsStatisticImpl(new clock_ns::NormalClock);

    events.incEvent("event#1");
    events.incEvent("event#2");
    events.incEvent("event#1");
    events.incEvent("event#3");

    events.printStatistic();

    return 0;
}
