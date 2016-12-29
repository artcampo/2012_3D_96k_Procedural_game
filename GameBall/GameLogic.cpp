#include  "GameBall/Game.hpp"
#include  "GameBall/Modes/GameChrono.hpp"
#include  "GameBall/Modes/GameLab.hpp"
#include  "GameBall/Modes/GameChase.hpp"
#include  "GameBall/Modes/GameSpartacus.hpp"

#include  "Project_Dependant/IOinput.hpp"
#include  "GameBall/Entities/Arch.hpp"

#include  "Misc/CountMeanMax.hpp"
#include  "Geometry/DxFustrum.hpp"


#include  "RenderModels/Metropolis/dx_metarm_model.hpp"
#include  "GameBall/Metropolis/MetropolisStructure.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"

bool Game::areControlsInverted() const
{  
  return mInvertCommandsTimed.acting();
}

bool      Game::isGameFinished() const
{
  return mGameFinished;
}

bool      Game::isGameWon() const
{
  return mGameWon;
}

bool      Game::isGoingBackwards() const
{
  return false;
}