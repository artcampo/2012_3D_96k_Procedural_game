#ifndef _Tricks_Punctuation_HPP_
#define _Tricks_Punctuation_HPP_

class Trick_AirTurbo;
class Trick_GettingHigh;
class Trick_BigFall;
class Trick_MoodSwing;
class Trick_Ponder;
class Trick_OverBumper;
class Trick_SwingerLife;

#include  "GameBall/Tricks/Tricks/Trick_AirTurbo.hpp"
#include  "GameBall/Tricks/Tricks/Trick_GettingHigh.hpp"
#include  "GameBall/Tricks/Tricks/Trick_BigFall.hpp"
#include  "GameBall/Tricks/Tricks/Trick_MoodSwing.hpp"
#include  "GameBall/Tricks/Tricks/Trick_Ponder.hpp"
#include  "GameBall/Tricks/Tricks/Trick_OverBumper.hpp"
#include  "GameBall/Tricks/Tricks/Trick_SwingerLife.hpp"

namespace TricksScore{
  namespace Score_AirTurbo{ 
    const int mPoints             = 50;
  }; 
  namespace Score_GettingHigh{ 
    const int mPoints             = 100;
    const int mPointsExtra        = 200;
  }; 
  namespace Score_BigFall{ 
    const int mPoints             = 400;
    const int mPointsExtra        = 600;
  }; 
  namespace Score_MoodSwing{ 
    const int mPoints             = 1500;
  }; 
  namespace Score_Ponder{ 
    const int mPoints             = 300;
    const int mPointsExtra        = 600;
  }; 
  namespace Score_OverBumper{ 
    const int mPoints             = 150;
  }; 
  namespace Score_SwingerLife{ 
    const int mPoints             = 150;
  };
  namespace Score_Lane{ 
    const int mPoints             = 500;
  };
  namespace Score_Grind{ 
    const int mPoints             = 700;
  };
  namespace Score_Almost{ 
    const int mPoints             = 50;
  };

  
};

#endif  //_Tricks_Punctuation_HPP_
