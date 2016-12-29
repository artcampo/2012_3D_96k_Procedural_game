#ifndef _LOG_HPP_
#define _LOG_HPP_

#include <iostream>
#include <fstream>
#include "Misc/Timing_windows_x64.hpp"

class Log{

public:
  Log ()
  {
    if ( !mInitialized )
    {
      initLog();   
      mInitialized = true;
    }
    ++mReferences;
  }
  ~Log ()
  {
    --mReferences;
    if ( mReferences == 0 )
    {
      mFile->flush();
      mFile->close();
      mFileEvents->flush();
      mFileEvents->close();
      mFileTrack->flush();
      mFileTrack->close();
      mFileTrackPieces->flush();
      mFileTrackPieces->close();
      mFileMenu->flush();
      mFileMenu->close();
      mFileIA->flush();
      mFileIA->close();   
      mFileIA_Branch->flush();
      mFileIA_Branch->close(); 
      mFileIA_Points->flush();
      mFileIA_Points->close(); 
      for ( int i = 0; i < 10; ++i )
      {
        mFile_Ball[i]->flush();
        mFile_Ball[i]->close();  
      }
      mFileFps->flush();
      mFileFps->close();        
      
      delete mProf;
      delete mProfGeom;
    }
  }
public:
  static std::ofstream* mFile;
  static std::ofstream* mFileEvents;
  static std::ofstream* mFileTrack;
  static std::ofstream* mFileTrackPieces;
  static std::ofstream* mFileMenu;
  static std::ofstream* mFileIA;
  static std::ofstream* mFileIA_Branch;
  static std::ofstream* mFileIA_Points;
  static std::vector<std::ofstream*> mFile_Ball;
  static std::ofstream* mFileFps;  

  static ProfilerDp*    mProf;
  static ProfilerDp*    mProfGeom;
private:
  static bool mInitialized;
  static int  mReferences;
  
private:
  void initLog()
  {
    mProf     = new ProfilerDp( std::string("General") );
    mProfGeom = new ProfilerDp( std::string("Branches") );

    mFile = new std::ofstream ( "./log/log_general.txt", std::fstream::out );  
    mFile->setf(std::ios::fixed,std::ios::floatfield); 
    mFile->precision(8);

    mFileEvents = new std::ofstream ( "./log/log_events.txt", std::fstream::out );  
    mFileEvents->setf(std::ios::fixed,std::ios::floatfield); 
    mFileEvents->precision(8);

    mFileTrack = new std::ofstream ( "./log/track/log_track_edges.txt", std::fstream::out );  
    mFileTrack->setf(std::ios::fixed,std::ios::floatfield); 
    mFileTrack->precision(5);

    mFileTrackPieces = new std::ofstream ( "./log/track/log_track_pieces.txt", std::fstream::out );  
    mFileTrackPieces->setf(std::ios::fixed,std::ios::floatfield); 
    mFileTrackPieces->precision(0);

    mFileMenu = new std::ofstream ( "./log/game/menu.txt", std::fstream::out );  
    mFileMenu->setf(std::ios::fixed,std::ios::floatfield); 
    mFileMenu->precision(0);
    
    mFileIA = new std::ofstream ( "./log/game/log_IA.txt", std::fstream::out );  
    mFileIA->setf(std::ios::fixed,std::ios::floatfield); 
    mFileIA->precision(3);

    mFileIA_Branch = new std::ofstream ( "./log/game/log_IA_branch.txt", std::fstream::out );  
    mFileIA_Branch->setf(std::ios::fixed,std::ios::floatfield); 
    mFileIA_Branch->precision(2);

    mFileIA_Points = new std::ofstream ( "./log/game/log_IA_points.txt", std::fstream::out );  
    mFileIA_Points->setf(std::ios::fixed,std::ios::floatfield); 
    mFileIA_Points->precision(2);
    

    mFile_Ball.resize(10);
    mFile_Ball[0] = new std::ofstream ( "./log/game/log_Ball_0.txt", std::fstream::out );  
    mFile_Ball[0]->setf(std::ios::fixed,std::ios::floatfield); 
    mFile_Ball[0]->precision(2);
    mFile_Ball[1] = new std::ofstream ( "./log/game/log_Ball_1.txt", std::fstream::out );  
    mFile_Ball[1]->setf(std::ios::fixed,std::ios::floatfield); 
    mFile_Ball[1]->precision(2);
    mFile_Ball[2] = new std::ofstream ( "./log/game/log_Ball_2.txt", std::fstream::out );  
    mFile_Ball[2]->setf(std::ios::fixed,std::ios::floatfield); 
    mFile_Ball[2]->precision(2);
    mFile_Ball[3] = new std::ofstream ( "./log/game/log_Ball_3.txt", std::fstream::out );  
    mFile_Ball[3]->setf(std::ios::fixed,std::ios::floatfield); 
    mFile_Ball[3]->precision(2);
    mFile_Ball[4] = new std::ofstream ( "./log/game/log_Ball_4.txt", std::fstream::out );  
    mFile_Ball[4]->setf(std::ios::fixed,std::ios::floatfield); 
    mFile_Ball[4]->precision(2);
    mFile_Ball[5] = new std::ofstream ( "./log/game/log_Ball_5.txt", std::fstream::out );  
    mFile_Ball[5]->setf(std::ios::fixed,std::ios::floatfield); 
    mFile_Ball[5]->precision(2);
    mFile_Ball[6] = new std::ofstream ( "./log/game/log_Ball_6.txt", std::fstream::out );  
    mFile_Ball[6]->setf(std::ios::fixed,std::ios::floatfield); 
    mFile_Ball[6]->precision(2);
    mFile_Ball[7] = new std::ofstream ( "./log/game/log_Ball_7.txt", std::fstream::out );  
    mFile_Ball[7]->setf(std::ios::fixed,std::ios::floatfield); 
    mFile_Ball[7]->precision(2);
    mFile_Ball[8] = new std::ofstream ( "./log/game/log_Ball_8.txt", std::fstream::out );  
    mFile_Ball[8]->setf(std::ios::fixed,std::ios::floatfield); 
    mFile_Ball[8]->precision(2);
    mFile_Ball[9] = new std::ofstream ( "./log/game/log_Ball_9.txt", std::fstream::out );  
    mFile_Ball[9]->setf(std::ios::fixed,std::ios::floatfield); 
    mFile_Ball[9]->precision(2);

    mFileFps = new std::ofstream ( "./log/log_Fps_lights.txt", std::fstream::out );  
    mFileFps->setf(std::ios::fixed,std::ios::floatfield); 
    mFileFps->precision(0);

    
  }

};
   
#endif  //  _STL_HELPERS_HPP_
