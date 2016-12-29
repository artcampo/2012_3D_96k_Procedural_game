#ifndef _TrackOutPieces_IO_HPP_
#define _TrackOutPieces_IO_HPP_

#include  "dx_misc.hpp"
#include  "Geometry/DirGraph/DirGraph.hpp"
#include  "Geometry/DirGraph/Pattern.hpp"
#include  "Geometry/DirGraph/SubRule.hpp"

namespace TrackPiecesIO{

  DirectedGraph::PatternOut* generatePatternOut ( const DirectedGraph::StoredDirGraph& aStoredDirGraph,
                                   const float aSubsMinLength, 
                                   const float aSubsMaxLength,
                                   const int aOutPatternId,
                                   const float aMinAzimut      = StoredPiecesBalancers::General::subsMinAzimuth,
                                   const float aMaxAzimut      = StoredPiecesBalancers::General::subsMaxAzimuth);
};  //end namespace TrackPiecesIO{

#endif  //_TrackOutPieces_IO_HPP_
