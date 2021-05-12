// Fill out your copyright notice in the Description page of Project Settings.


#include "InterActor_LvlSequenceTrigger.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"

void AInterActor_LvlSequenceTrigger::Act() 
{
    ALevelSequenceActor* OutActor = nullptr;
    if (LevelSequence && LevelSequencePlayer == nullptr)
    {
        LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequence, FMovieSceneSequencePlaybackSettings(), OutActor);
    }
    if (LevelSequencePlayer && !isPlayed)
    {
        LevelSequencePlayer->Play();
        isPlayed = true;
    }
}
