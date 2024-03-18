#include "BuzzerInterface.hpp"

void Buzzer::BuzzerInterface::start(BuzzerProgram defaultProgram_, uint16_t defaultNote_)
{
    pinMode(BUZZER_PIN, OUTPUT);
    defaultProgram = defaultProgram_;
    currentProgram = defaultProgram_;
    defaultNote = defaultNote_;
    started = true;
}

void Buzzer::BuzzerInterface::setDefaultProgram(BuzzerProgram defaultProgram_)
{
    defaultProgram = defaultProgram_;
}

void Buzzer::BuzzerInterface::startTone(uint16_t note)
{
    if (!toneOn)
    {
        tone(BUZZER_PIN, note);
        toneOn = true;
    }
}

void Buzzer::BuzzerInterface::endTone()
{
    toneOn = false;
    noTone(BUZZER_PIN);
}

void Buzzer::BuzzerInterface::switchTone()
{
    if (toneOn)
    {
        noTone(BUZZER_PIN);
        toneOn = false;
    }
    else
    {
        tone(BUZZER_PIN, currentNote);
        toneOn = true;
    }
}

void Buzzer::BuzzerInterface::update()
{
    return;
    switch (currentProgram)
    {
    case BuzzerProgram::Off:
    {
        noTone(BUZZER_PIN);
        break;
    }
    case BuzzerProgram::On:
    {
        tone(BUZZER_PIN, currentNote);
        break;
    }
    case BuzzerProgram::EndSong:
    {
        currentSong = endSong;
        repeatSongProgram();
        break;
    }
    case BuzzerProgram::Startup:
    {
        currentSong = startSong;
        singleSongProgram();
        break;
    }
    case BuzzerProgram::RapidBeep10s:
    {
        if (elapsedTime > 10000)
        {
            endProgram();
        }
        if (elapsedTime % 400 < 200)
        {
            startTone(currentNote);
        }
        else
        {
            endTone();
        }
        break;
    }
    case BuzzerProgram::ContinuousBeep:
    {
        if (elapsedTime > 200)
        {
            switchTone();
            elapsedTime = 0;
        }

        break;
    }
    case BuzzerProgram::SlowContinuousBeep:
    {
        if (elapsedTime > 500)
        {
            switchTone();
            elapsedTime = 0;
        }
        break;
    }
    case BuzzerProgram::BeepThree:
    {
        beepProgram(BuzzerProgram::BeepTwo);
        break;
    }
    case BuzzerProgram::BeepTwo:
    {
        beepProgram(BuzzerProgram::BeepOne);
        break;
    }
    case BuzzerProgram::BeepOne:
    {
        beepProgram(defaultProgram);
        break;
    }
    default:
        break;
    }
}

void Buzzer::BuzzerInterface::startProgram(BuzzerProgram program)
{
    startProgram(program, defaultNote);
}

void Buzzer::BuzzerInterface::startProgram(BuzzerProgram program, uint16_t note)
{
    elapsedTime = 0;
    currentProgram = program;
    currentNote = note;
    update();
}

void Buzzer::BuzzerInterface::startProgramDelay(BuzzerProgram program)
{
    startProgramDelay(program, defaultNote);
}

void Buzzer::BuzzerInterface::startProgramDelay(BuzzerProgram program, uint16_t note)
{
    while ((currentProgram != defaultProgram) & (elapsedTime < 10000))
    {
        update();
        delay(10);
    }
}

void Buzzer::BuzzerInterface::endProgram()
{
    startProgram(defaultProgram);
}

void Buzzer::BuzzerInterface::beepProgram(BuzzerProgram next)
{
    if (elapsedTime > 1000)
    {
        startProgram(next);
    }
    else if (elapsedTime > 200)
    {
        noTone(BUZZER_PIN);
    }
    else
    {
        tone(BUZZER_PIN, currentNote);
    }
}

void Buzzer::BuzzerInterface::repeatSongProgram()
{
    if (elapsedTime > nextElapsedTime)
    {
        currentSongNote++;
        if (currentSongNote >= currentSong.songLength)
        {
            currentSongNote = 0;
        }
        noTone(BUZZER_PIN);
        tone(BUZZER_PIN, currentSong.notes[currentSongNote]);
        nextElapsedTime = 60 * 1000 * endSong.tempo / endSong.noteLengths[currentSongNote];
        elapsedTime = 0;
    }
}

void Buzzer::BuzzerInterface::singleSongProgram()
{
    if (elapsedTime > nextElapsedTime)
    {
        currentSongNote++;
        if (currentSongNote >= currentSong.songLength)
        {
            endProgram();
        }
        noTone(BUZZER_PIN);
        tone(BUZZER_PIN, currentSong.notes[currentSongNote]);
        nextElapsedTime = 60 * 1000 * endSong.tempo / endSong.noteLengths[currentSongNote];
    }
}
