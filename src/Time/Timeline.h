#ifndef TIMELINE_H
#define TIMELINE_H

class Timeline
{
    private:
        Timeline();
        static Timeline* instancePtr;

    public:
        sf::Clock dt_clock;
        float dt;
        static Timeline *getInstance();
        sf::Time elapsedTime();
        void updateDeltaTime();
};

#endif