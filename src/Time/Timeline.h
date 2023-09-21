#ifndef TIMELINE_H
#define TIMELINE_H

class Timeline
{
    private:
        Timeline();
        static Timeline* instancePtr;
        float dt;
        float scale;
        sf::Clock dt_clock;

    public:
        enum SCALE { SCALE_HALF, SCALE_REAL, SCALE_DOUBLE };
        static Timeline *getInstance();
        void pause();
        bool isPaused();
        float getDt();
        float getScale();
        void changeScale(SCALE s);
        sf::Time elapsedTime();
        void updateDeltaTime();
};

#endif