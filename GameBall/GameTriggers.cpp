#include  "GameBall/Game.hpp"
#include  "GameBall/Modes/GameChrono.hpp"
#include  "GameBall/Modes/GameLab.hpp"

#include  "Project_Dependant/IOinput.hpp"
#include  "GameBall/Entities/Arch.hpp"
#include  "GameBall/Recording/GameRecording.hpp"
#include  "GameBall/Recording/GameReplay.hpp"
#include  "GameBall/Tricks/Tricks.hpp"

void Game::triggerEnd( Ball& aBall )
{
  mReplay->mTime0 = mTrickManager->mMaxComboStartTime;
  mReplay->mTime1 = mTrickManager->mMaxComboEndTime;
  _triggerEnd( aBall);
}