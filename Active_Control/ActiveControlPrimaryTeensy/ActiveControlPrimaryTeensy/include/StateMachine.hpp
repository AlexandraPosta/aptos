#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef void (*FunctionArray)();
#define NUM_STATES 13

namespace StateMachine
{
    // Used for array indexing, make sure the number matches NUM_STATES above
    enum State
    {
        Startup = 0,
        OffLaunchRodTest,
        OffLaunchRodCalibration,
        Idle,
        LaunchRodCalibration,
        LaunchRodTest,
        Arm,
        Armed,
        BelowVmc,
        ActiveControlEnabled,
        Descent,
        Shutdown,
        Failsafe,
    };

    class StateMachine
    {
    private:
        FunctionArray stateFunctions[NUM_STATES];

    public:
        State currentState;
        StateMachine(FunctionArray functions[NUM_STATES])
        {
            for (int i = 0; i < NUM_STATES; i++)
            {
                stateFunctions[i] = functions[i];
            }
        };
        void run()
        {
            stateFunctions[currentState]();
        };
        void changeState(State newState)
        {
            currentState = newState;
        }
    };
}
#endif