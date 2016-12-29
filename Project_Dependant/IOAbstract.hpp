#ifndef _IO_ABSTRACT_HPP
#define _IO_ABSTRACT_HPP

enum eShipKeyBinding {
PHYS_SHIP_ACC_FORWARDS_ID,
PHYS_SHIP_ACC_BACKWARDS_ID,
PHYS_SHIP_ACC_LEFT_ID,
PHYS_SHIP_ACC_RIGHT_ID
};

class IoAbstract{
public:
  virtual void	Move	( unsigned int key ) = 0;
  virtual void  MouseMove ( D3DXVECTOR3* inc_mouse ) = 0;
  virtual void  initIoReading( float aDeltaT ) = 0;
  virtual void  finishIoReading( D3DXVECTOR3* aMouseIncrement ) = 0;
};

#endif