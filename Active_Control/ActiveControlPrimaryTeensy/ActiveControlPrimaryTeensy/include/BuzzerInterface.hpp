#ifndef BUZZER_INTERFACE_H
#define BUZZER_INTERFACE_H

#include <Arduino.h>
#include "MusicalNotes.hpp"
#define BUZZER_PIN 5
#define MAX_SONG_LENGTH 64

namespace Buzzer
{
    enum class BuzzerProgram
    {
        Off,
        On,
        BeepOne,
        BeepTwo,
        BeepThree,
        RapidBeep10s,
        Startup,
        ContinuousBeep,
        SlowContinuousBeep,
        EndSong,
    };

    struct Song
    {
        // in standard time, so 1 = 1 bar, 2 = 1/2 bar etc.
        uint16_t noteLengths[MAX_SONG_LENGTH];
        uint16_t notes[MAX_SONG_LENGTH];
        // in notes
        size_t songLength;

        // in bpm
        float tempo;
    };

    class BuzzerInterface
    {
    public:
        void start(BuzzerProgram defaultProgram_, uint16_t defaultNote_);
        void update();
        void startProgram(BuzzerProgram program);
        void startProgram(BuzzerProgram program, uint16_t note);
        void startProgramDelay(BuzzerProgram program);
        void startProgramDelay(BuzzerProgram program, uint16_t note);
        void endProgram();
        void setDefaultProgram(BuzzerProgram program);

    private:
        bool started = false;
        BuzzerProgram currentProgram = BuzzerProgram::Off;
        BuzzerProgram defaultProgram = BuzzerProgram::Off;
        elapsedMillis elapsedTime;
        uint16_t currentNote = NOTE_A4;
        uint16_t defaultNote = NOTE_A4;
        bool toneOn = false;
        void startTone(uint16_t note);
        void endTone();

        void switchTone();
        // cantina band
        Song endSong{
            {4, 4, 4, 4,
             8, 4, 8, 8, 8, 4,
             8, 8, 8, 8, 8, 8, 8, 8,
             4, 8, 2, 8},
            {NOTE_A4, NOTE_D4, NOTE_A4, NOTE_D4,
             NOTE_A4, NOTE_D4, NOTE_A4, NOTE_REST, NOTE_GS3, NOTE_A4,
             NOTE_A4, NOTE_GS3, NOTE_A4, NOTE_REST, NOTE_G3, NOTE_FS3, NOTE_G3, NOTE_REST,
             NOTE_F3, NOTE_REST, NOTE_D3, NOTE_REST},
            22};

        Song startSong{
            {1, 1, 1},
            {NOTE_A4, NOTE_CS4, NOTE_E4},
            3,
            400};

        void beepProgram(BuzzerProgram next);
        uint16_t currentSongNote;
        Song currentSong;
        unsigned long nextElapsedTime;
        void repeatSongProgram();
        void singleSongProgram();
    };
}

#endif