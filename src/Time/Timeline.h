#ifndef TIMELINE_H
#define TIMELINE_H

class Timeline
{
    private:
        Timeline();
        static Timeline* instancePtr;
        float dt;

    public:
        sf::Clock dt_clock;
        static Timeline *getInstance();
        void pause();
        bool isPaused();
        float getDt();
        sf::Time elapsedTime();
        void updateDeltaTime();
};

#endif