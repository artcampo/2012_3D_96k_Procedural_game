#ifndef _GameBp_General_HPP_
#define _GameBp_General_HPP_


namespace GameBp{
  
  class NitroDescription{
  public:
    int   NumStartingNitros;
    float NitroCoefImpulse;
    float NitroCoefMaxVel;
    float NitroTimeRecharge;    
    float NitroTimeVelChange;
  };

  enum eBallIds{
    eBallMetal,
    eBallGlass,
    eBallBouncy,
    eBallMeteor,
    eBallDemo,
    eBallChase,
    eBallSpartacus
  };

  class BallDescription{
  public:    
    int   idBall;
    float fwdModule;
    float bckModule;
    float sidModule;    
    float maxVelocity;
    float maxVelTimeCoef;   // If ball undergoes any change of velocity, at which
                            // rate current velocity is changed
  };

  class VelModifier{
  public:
    float coefficient;
    float LapsedTime;
    float MaxLapsedTime;
  };

};

#endif  //_GameBp_General_HPP_
