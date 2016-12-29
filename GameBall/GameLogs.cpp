#include  "GameBall/Game.hpp"
#include  "Project_Dependant/IOinput.hpp"
#include  "GameBall/Entities/Arch.hpp"
#include  "Project_Dependant/ConditionalCompilation.hpp"

void Game::logTrack()
{   
  using namespace std;
  *Global::mFileDebugTrack << "------------------------------------------------------------" << endl;
  *Global::mFileDebugTrack << "Track"<< endl;
  *Global::mFileDebugTrack << "------------------------------------------------------------" << endl;

#ifndef GBP_GFXDEBUG_TRACK
  *Global::mFileDebugTrack << "Num TrackPaths: " << mTrack->mPath.size() << endl;
#endif
  *Global::mFileDebugTrack << "Num Archs: " << mArchs.size() << endl;
  *Global::mFileDebugTrack << "Num Bumpers: " << mBumpers.size() << endl;
/*
  int sumNumTriangles = 0;
  int sumPathTes = 0;
  for ( int i = 0; i < mTrack->mPath.size(); ++i )
  {
    sumNumTriangles += mTrack->mPath[i]->mNumTrianglesMesh;
    sumPathTes      += mTrack->mPath[i]->mPathTes;
  }

  *Global::mFileDebugTrack << "------------------------------------------------------------" << endl;
  *Global::mFileDebugTrack << "Total Num triangles: " << sumNumTriangles << endl;
  *Global::mFileDebugTrack << "Mean path tesellation: " << sumPathTes / mTrack->mPath.size()<< endl;


  *Global::mFileDebugTrack << "------------------------------------------------------------" << endl;
  *Global::mFileDebugTrack << "Track Paths:" << endl;

  for ( int i = 0; i < mTrack->mPath.size(); ++i )
  {
    mTrack->mPath[i]->writeLog( Global::mFileDebugTrack );
    *Global::mFileDebugTrack  << endl;
    sumNumTriangles += mTrack->mPath[i]->mNumTrianglesMesh;
    sumPathTes      += mTrack->mPath[i]->mPathTes;
  }
*/


}