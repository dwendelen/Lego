//
// Created by xtrit on 06.01.18.
//

#include <stdexcept>
#include "EventBus.hpp"

using namespace std;
using namespace engine;

engine::EventBus::EventBus()
        : endOfBuffer(buffer + BUFFER_SIZE),
          wrappingPoint(endOfBuffer),
          readPointer(buffer),
          writePointer(buffer),
          written(0),
          peaked(0),
          read(0),
          full(nullptr),
          empty(nullptr) {
    full = SDL_CreateSemaphore(0);
    empty = SDL_CreateSemaphore(BUFFER_SIZE);
}

void wait(SDL_semaphore *sem, const string &timeoutError, size_t size) {
    for (size_t i = 0; i < size; i++) {
        int result = SDL_SemWaitTimeout(sem, 5000);
        if (result == SDL_MUTEX_TIMEDOUT) {
            throw runtime_error(timeoutError);
        }
        if (result < 0) {
            throw runtime_error(SDL_GetError());
        }
    }
}

void signal(SDL_semaphore *sem, size_t size) {
    for (size_t i = 0; i < size; i++) {
        int result = SDL_SemPost(sem);

        if (result < 0) {
            throw runtime_error(SDL_GetError());
        }
    }
}

void *engine::EventBus::writeNext_(std::size_t size) {
    size_t sizeLeft = endOfBuffer - writePointer;
    if (sizeLeft < size) {
        wait(empty, "Event queue is full", sizeLeft + size);
        wrappingPoint = writePointer;
        writePointer = buffer;
    } else {
        wait(empty, "Event queue is full", size);
    }

    Block *result = writePointer;
    writePointer += size;
    written += size;

    return result;
}

void engine::EventBus::commitWrites() {
    signal(full, written);
    written = 0;
}

Block *engine::EventBus::getWrappedReadPointer() {
    if (readPointer >= wrappingPoint) {
        size_t gab = endOfBuffer - wrappingPoint;
        read += gab;
        readPointer = buffer;
        wrappingPoint = endOfBuffer;
    }

    return readPointer;
}

void *engine::EventBus::peak_(size_t size) {
    if (size > peaked) {
        wait(full, "Event queue is empty", size - peaked);
        peaked = size;
    }

    return getWrappedReadPointer();
}

void *engine::EventBus::readNext_(std::size_t size) {
    wait(full, "Event queue is empty", size - peaked);

    Block *readp = getWrappedReadPointer();
    Block *result = readp;
    readPointer = readp + size;
    read += size;
    peaked = 0;

    return result;
}

void engine::EventBus::commitReads() {
    signal(empty, read);
    read = 0;
}

engine::EventBus::~EventBus() {
    if (full) {
        SDL_DestroySemaphore(full);
    }
    if (empty) {
        SDL_DestroySemaphore(empty);
    }
}