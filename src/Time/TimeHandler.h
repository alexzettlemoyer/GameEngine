#ifndef TIME_HANDLER_H
#define TIME_HANDLER_H

class TimeHandler
{
    private:
        TimeHandler();
        static TimeHandler* instancePtr;

    public:
        sf::Clock dt_clock;
        float dt;
        static TimeHandler *getInstance();
        sf::Time elapsedTime();
        void updateDeltaTime();
};

#endif