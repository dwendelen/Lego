#include <chrono>
#include "gtest/gtest.h"

#include "EventBus.hpp"
#include "Math.hpp"

using namespace engine;

TEST(EventBusTest, WriteFullAndRead) {
    EventBus eventBus;

    for (int i = 0; i < EventBus::BUFFER_SIZE; ++i) {
        int *next = eventBus.writeNext<int>();
        *next = i;
    }

    eventBus.commitWrites();

    for (int i = 0; i < EventBus::BUFFER_SIZE; ++i) {
        int *peak = eventBus.peak<int>();
        int *actual = eventBus.readNext<int>();

        ASSERT_EQ(*peak, i);
        ASSERT_EQ(*actual, i);
    }

    eventBus.commitReads();
}

/*
TEST(EventBusTest, WriteOneTooMany) {
    EventBus eventBus;

    for (int i = 0; i < EventBus::BUFFER_SIZE; ++i) {
        eventBus.writeNext<int>();
    }

    eventBus.writeNext<int>();
}

TEST(EventBusTest, PeakEmpty) {
    EventBus eventBus;

    eventBus.peak<int>();
}

TEST(EventBusTest, ReadEmpty) {
    EventBus eventBus;

    eventBus.readNext<int>();
}
*/

TEST(EventBusTest, WriteAndWriteMultipleTimes) {
    EventBus eventBus;

    int write = 0;
    int read = 0;

    for (int loops = 0; loops < 5; loops++) {
        for (int i = 0; i < EventBus::BUFFER_SIZE; ++i) {
            int *next = eventBus.writeNext<int>();
            *next = write;
            write++;
        }

        eventBus.commitWrites();

        for (int i = 0; i < EventBus::BUFFER_SIZE; ++i) {
            int *peak = eventBus.peak<int>();
            int *actual = eventBus.readNext<int>();

            ASSERT_EQ(*peak, read);
            ASSERT_EQ(*actual, read);
            read++;
        }

        eventBus.commitReads();
    }
}

typedef struct {
    char chars[3 * EventBus::SIZE_ONE_BLOCK];
} Data;


TEST(EventBusTest, Wrap) {
    EventBus eventBus;

    //Leave one block
    for (int i = 0; i < EventBus::BUFFER_SIZE - 1; ++i) {
        int *next = eventBus.writeNext<int>();
        *next = i;
    }

    eventBus.commitWrites();

    //Clear three blocks
    eventBus.readNext<int>();
    eventBus.readNext<int>();
    eventBus.readNext<int>();

    eventBus.commitReads();

    Data *writeBlock = eventBus.writeNext<Data>();
    char *chars = writeBlock->chars;
    chars[0] = '#';
    for (int i = 1; i < 3 * EventBus::SIZE_ONE_BLOCK; i++) {
        int digit = i % 10;
        chars[i] = static_cast<char>(48 + digit);
    }

    eventBus.commitWrites();

    for (int i = 3; i < EventBus::BUFFER_SIZE - 1; ++i) {
        eventBus.readNext<int>();
    }

    char *peak = eventBus.peak<char>();
    ASSERT_EQ(*peak, '#');
    Data *readBlock = eventBus.readNext<Data>();
    chars = readBlock->chars;
    ASSERT_EQ(chars[0], '#');
    for (int i = 1; i < 3 * EventBus::SIZE_ONE_BLOCK; i++) {
        int digit = i % 10;
        ASSERT_EQ(chars[i], static_cast<char>(48 + digit));
    }

    eventBus.commitReads();
}

/*
TEST(EventBusTest, WrapALot) {
    EventBus eventBus;

    for(int j = 0;  j < 2 * EventBus::BUFFER_SIZE; j++) {

        //Leave one block
        for (int i = 0; i <  EventBus::BUFFER_SIZE / 3 - 1; ++i) {
            eventBus.writeNext<Data>();
        }
        eventBus.commitWrites();

        for (int i = 0; i <  EventBus::BUFFER_SIZE / 3 - 1; ++i) {
            eventBus.peak<char>();
            eventBus.readNext<Data>();
            eventBus.commitRead<Data>();
        }
    }
}
*/

TEST(EventBusTest, Timing) {
    EventBus eventBus;

    auto p1 = std::chrono::high_resolution_clock::now();
    eventBus.writeNext<int>();
    auto p2 = std::chrono::high_resolution_clock::now();
    eventBus.commitWrites();
    auto p3 = std::chrono::high_resolution_clock::now();
    eventBus.peak<int>();
    auto p4 = std::chrono::high_resolution_clock::now();
    eventBus.readNext<int>();
    auto p5 = std::chrono::high_resolution_clock::now();
    eventBus.commitReads();
    auto p6 = std::chrono::high_resolution_clock::now();

    std::cout << "writeNext " << std::chrono::duration<double, std::micro>(p2 - p1).count() << std::endl;
    std::cout << "commitWrites " << std::chrono::duration<double, std::micro>(p3 - p2).count() << std::endl;
    std::cout << "peak " << std::chrono::duration<double, std::micro>(p4 - p3).count() << std::endl;
    std::cout << "readNext " << std::chrono::duration<double, std::micro>(p5 - p4).count() << std::endl;
    std::cout << "commitReads " << std::chrono::duration<double, std::micro>(p6 - p5).count() << std::endl;
    std::cout << "total " << std::chrono::duration<double, std::micro>(p6 - p1).count() << std::endl;
}

TEST(EventBusTest, EnumAnIdAndAVector3f_FitsInOneBlock) {
    enum EventType {
        BLA, BLI, BLO
    };
    class EventClass {
        EventType type = BLA;
        uint32_t id = 4;
        OVR::Vector3f vector;
    };

    size_t size = EventBus::SIZE_ONE_BLOCK;
    ASSERT_LE(sizeof(EventClass), size);
}