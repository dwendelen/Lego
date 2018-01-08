//
// Created by xtrit on 06.01.18.
//

#ifndef LEGO_EVENTBUS_HPP
#define LEGO_EVENTBUS_HPP

#include "SDL_mutex.h"

namespace engine {
    typedef struct {
        char data[64];
    } Block;

    class EventBus {
    public:
        EventBus();

        template<class T>
        T *writeNext() {
            return static_cast<T *>(writeNext_(size<T>()));
        }

        void commitWrites();

        template<class T>
        T *peak() {
            return static_cast<T *>(peak_(size<T>()));
        }

        template<class T>
        T *readNext() {
            return static_cast<T *>(readNext_(size<T>()));
        }

        void commitReads();

        template<class T>
        static constexpr std::size_t size() {
            return (sizeof(T) - 1) / SIZE_ONE_BLOCK + 1;
        }

        ~EventBus();

        static constexpr std::size_t BUFFER_SIZE = 16 * 1024;
        static constexpr std::size_t SIZE_ONE_BLOCK = sizeof(Block);
    private:

        Block *getWrappedReadPointer();

        void *writeNext_(std::size_t size);

        void *peak_(std::size_t size);

        void *readNext_(std::size_t size);

        Block buffer[BUFFER_SIZE];

        Block *endOfBuffer;
        Block *wrappingPoint;

        Block *readPointer;
        Block *writePointer;

        std::size_t written;
        std::size_t peaked;
        std::size_t read;

        SDL_semaphore *full;
        SDL_semaphore *empty;
    };
}
#endif //LEGO_EVENTBUS_HPP
