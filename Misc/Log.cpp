#include  "Misc/Log.hpp"

bool  Log::mInitialized = false;
int   Log::mReferences  = 0;

std::ofstream* Log::mFile;
std::ofstream* Log::mFileEvents;
std::ofstream* Log::mFileTrack;
std::ofstream* Log::mFileTrackPieces;
std::ofstream* Log::mFileMenu;
std::ofstream* Log::mFileIA;
std::ofstream* Log::mFileIA_Branch;
std::ofstream* Log::mFileIA_Points;
std::vector<std::ofstream*> Log::mFile_Ball;
std::ofstream* Log::mFileFps;



ProfilerDp*    Log::mProf;
ProfilerDp*    Log::mProfGeom;