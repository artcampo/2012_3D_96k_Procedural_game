#include	"Timing_windows_x64.hpp"

using namespace std;

ProfilerDp::ProfilerDp( std::string& aTitle )
{
  mTitle = aTitle;
}



void ProfilerDp::start ( std::string& aName )
{
  mBlockNames.push_back( aName );
  mStart = ReadTSC();
}


void ProfilerDp::end ( )
{
  mEnd = ReadTSC();
  Measurement m;
  m.totalTime = time();
  mTimings.push_back ( m );
}

long long ProfilerDp::time()
{
  return mEnd-mStart;
}

void ProfilerDp::dumpToFile(const char* aFileName)
{
  std::ofstream  file( aFileName, std::fstream::out ); 
  file.setf(std::ios::fixed,std::ios::floatfield); 
  file.precision(0);

  double sum = 0;
  for (unsigned int j = 0; j < mTimings.size() ; ++j)
    sum += mTimings[j].totalTime;


  file << "------------------------------------------------------------" << endl;
  file << "Init times: "<< mTitle << endl;
  file << "------------------------------------------------------------" << endl;
  for (unsigned int j = 0; j < mTimings.size() ; ++j)
  {
    file  << (mTimings[j].totalTime*100)/sum
      << " [" << (mTimings[j].totalTime/1000000)<< "]: " 
          << mBlockNames[j] <<endl;
  }
  file << "Total time:" << (sum/1000000)<< endl; 


  file.close();
}

ProfilerLoopDp::ProfilerLoopDp( std::string& aTitle )
{
  mTitle = aTitle;
}



void ProfilerLoopDp::start ( std::string& aName )
{
  if ( mIds.find( aName ) == mIds.end() )
  {
    mCurrentId = mIds.size();
    mIds[aName] = mCurrentId;
    
    mTimings.resize( mCurrentId + 1 );
    mBlockNames.resize( mCurrentId + 1 );
    mBlockNames[mCurrentId] = aName;    
  }
  else
  {
    mCurrentId = mIds[aName];
  }
  mStart = ReadTSC();
}


void ProfilerLoopDp::end ( )
{
  mEnd = ReadTSC();
  Measurement m;
  m.totalTime = time();
  mTimings[mCurrentId].push_back ( m );
}

long long ProfilerLoopDp::time()
{
  return mEnd-mStart;
}

void ProfilerLoopDp::addTag( const int aTag )
{
  mTag.push_back ( aTag );
}

void ProfilerLoopDp::dumpToFile(const char* aFileName)
{
  std::ofstream  file( aFileName, std::fstream::out ); 
  file.setf(std::ios::fixed,std::ios::floatfield); 
  file.precision(0);

  double sum = 0;
  std::vector<double> mesT ( mTimings.size() );
  for (unsigned int j = 0; j < mTimings.size() ; ++j)
  {
    float sumT = 0.0f;
    for (unsigned int t = 0; t < mTimings[j].size() ; ++t)
      sumT += mTimings[j][t].totalTime;
    mesT[j] = sumT / double( mTimings[j].size() );
    sum += mesT[j];
  }


  file << "------------------------------------------------------------" << endl;
  file << "Loop times: "<< mTitle << endl;
  file << "------------------------------------------------------------" << endl;
  for (unsigned int j = 0; j < mTimings.size() ; ++j)
  {
    file  << (mesT[j]*100)/sum
      << " [" << (mesT[j]/10000)<< "]: " 
          << mBlockNames[j] <<endl;
  }
  file << "Total time:" << (sum/1000000)<< endl; 



  file << "------------------------------------------------------------" << endl;
  file << "Time of each loop: "<< mTitle << endl;
  file << "------------------------------------------------------------" << endl;
  for (unsigned int i = 0; i < mTimings[0].size() ; ++i)
  {
    float frameTime = 0;
    for (unsigned int j = 0; j < mTimings.size() ; ++j)
      frameTime += mTimings[j][i].totalTime;
    float timeDivider = 100000.0f;

    file << "Total: " << frameTime/timeDivider << " " ;
    file << " (frame: " << mTag[i] << " )";
    for (unsigned int j = 0; j < mTimings.size() ; ++j)
    {
      file  << "-- " 
            << mTimings[j][i].totalTime/timeDivider <<" ("
            << 100.0f*mTimings[j][i].totalTime/frameTime<< ") " 
            << mBlockNames[j] <<" ";
             
    }
    file <<endl;
  }
  file << "Total time:" << (sum/1000000)<< endl; 


  file.flush();
  file.close();
}

