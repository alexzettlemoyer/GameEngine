#ifndef IO_HANDLER_H
#define IO_HANDLER_H

#include "../GraphicsObject/Character.h"
#include <memory>

class ioHandler 
{
    private:
        ioHandler(std::shared_ptr<Character> c);
        static ioHandler* instancePtr;

    public:
        static ioHandler *getInstance(std::shared_ptr<Character> c = nullptr);
        void handle(std::atomic<bool>& isGameRunning);
};

#endif