

#include "ArPacketUtil.h"

AREXPORT template<> void ArPacketUtil::addField<int>(ArBasePacket& p, const int& value)
{
  p.byte4ToBuf((ArTypes::Byte4) value);
}


AREXPORT template<> void ArPacketUtil::addField<unsigned int>(ArBasePacket& p, const unsigned int& value)
{
  p.uByte4ToBuf((ArTypes::UByte4) value);
}

AREXPORT template<> void ArPacketUtil::addField<short>(ArBasePacket& p, const short& value)
{
  p.byte2ToBuf((ArTypes::Byte2) value);
}

AREXPORT template<> void ArPacketUtil::addField<unsigned short>(ArBasePacket& p, const unsigned short& value)
{
  p.uByte2ToBuf((ArTypes::UByte2) value);
}

AREXPORT template<> void ArPacketUtil::addField<std::string>(ArBasePacket& p, const std::string& value)
{
  p.strToBuf(value.c_str());
}

AREXPORT template<> void ArPacketUtil::addField<float>(ArBasePacket& p, const float& value)
{
  p.byte4ToBuf(value * 10e4);
}

AREXPORT template<> void ArPacketUtil::addField<double>(ArBasePacket& p, const double& value)
{
  p.byte4ToBuf(value * 10e4);
}

AREXPORT template<> void ArPacketUtil::addField<bool>(ArBasePacket& p, const bool& value)
{
  p.uByteToBuf(value?1:0);
}

AREXPORT template<> void ArPacketUtil::addField<char>(ArBasePacket& p, const char& value)
{
  p.byteToBuf((ArTypes::Byte)value);
}

AREXPORT template<> void ArPacketUtil::addField<unsigned char>(ArBasePacket& p, const unsigned char& value)
{
  p.uByteToBuf((ArTypes::UByte)value);
}

/// X, Y and Theta are truncated to integer values only
AREXPORT template<> void ArPacketUtil::addField<ArPose>(ArBasePacket& pkt, const ArPose& pose)
{
  pkt.byte4ToBuf((ArTypes::Byte4)pose.getX());
  pkt.byte4ToBuf((ArTypes::Byte4)pose.getY());
  pkt.byte4ToBuf((ArTypes::Byte4)pose.getTh());
}

/// X, Y are truncated to integer values 
AREXPORT template<> void ArPacketUtil::addField<ArLineSegment>(ArBasePacket& p, const ArLineSegment& l)
{
  p.byte4ToBuf((ArTypes::Byte4)l.getX1());
  p.byte4ToBuf((ArTypes::Byte4)l.getY1());
  p.byte4ToBuf((ArTypes::Byte4)l.getX2());
  p.byte4ToBuf((ArTypes::Byte4)l.getY2());
}

/// X, Y are truncated to integer values
AREXPORT template<> void ArPacketUtil::addField<ArPos2D>(ArBasePacket& pkt, const ArPos2D& pos)
{
  pkt.byte4ToBuf((ArTypes::Byte4)pos.getX());
  pkt.byte4ToBuf((ArTypes::Byte4)pos.getY());
}


AREXPORT template<typename T> void ArPacketUtil::addField(ArBasePacket& p, const T& value)
{
  ArLog::log(ArLog::Terse, "INTERNAL ERROR: ArPacketUtil::addField<T> called with unhandled type for T! Type must be one of: int, unsigned int, short, unsigned short, std::string, float, double, bool, char, unsigned char, or vector or list of any of the above.");
  abort();
}


#define PACKET_OUT_OF_RANGE_ERR "No more data fields of this type in the packet."

AREXPORT template<> int ArPacketUtil::getNextField<int>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(sizeof(ArTypes::Byte4))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return (int) p.bufToByte4();
}


AREXPORT template<> unsigned int ArPacketUtil::getNextField<unsigned int>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(sizeof(ArTypes::UByte4))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return (unsigned int) p.bufToUByte4();
}

AREXPORT template<> short ArPacketUtil::getNextField<short>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(sizeof(ArTypes::Byte2))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return (short) p.bufToByte2();
}

AREXPORT template<> unsigned short ArPacketUtil::getNextField<unsigned short>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(sizeof(ArTypes::UByte2))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return (unsigned short) p.bufToUByte2();
}

AREXPORT template<> std::string ArPacketUtil::getNextField<std::string>(ArBasePacket& p)
{
  return p.bufToString();
}

AREXPORT template<> float ArPacketUtil::getNextField<float>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(sizeof(ArTypes::Byte4))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return (float)p.bufToByte4() / 10e4;
}

AREXPORT template<> double ArPacketUtil::getNextField<double>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(sizeof(ArTypes::Byte4))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return (double)p.bufToByte4() / 10e4;
}

AREXPORT template<> bool ArPacketUtil::getNextField<bool>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(sizeof(ArTypes::UByte))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return(p.bufToUByte() != 0);
}

AREXPORT template<> char ArPacketUtil::getNextField<char>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(sizeof(ArTypes::Byte))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return p.bufToByte();
}

AREXPORT template<> unsigned char ArPacketUtil::getNextField<unsigned char>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(sizeof(ArTypes::UByte))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return p.bufToUByte();
}
AREXPORT template<> ArPose ArPacketUtil::getNextField<ArPose>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(2 * sizeof(ArTypes::Byte4))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return ArPose(p.bufToByte4(), p.bufToByte4(), p.bufferContainsBytes(sizeof(ArTypes::Byte4)) ? p.bufToByte4() : 0);

}

AREXPORT template<> ArLineSegment ArPacketUtil::getNextField<ArLineSegment>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(4 * sizeof(ArTypes::Byte4))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return ArLineSegment(p.bufToByte4(), p.bufToByte4(), p.bufToByte4(), p.bufToByte4());

}

AREXPORT template<> ArPos2D ArPacketUtil::getNextField<ArPos2D>(ArBasePacket& p)
{
  if(!p.bufferContainsBytes(2 * sizeof(ArTypes::Byte4))) throw std::out_of_range(PACKET_OUT_OF_RANGE_ERR);
  return ArPos2D(p.bufToByte4(), p.bufToByte4());

}

AREXPORT template<typename VT> std::vector<VT> ArPacketUtil::getNextVectorField(ArBasePacket& p)
{
  std::vector<VT> vec;
  size_t len = p.bufToUByte4();
  vec.reserve(len);
  for(int i = 0; i < len; ++i)
  {
    vec.push_back(getNextField<VT>(p));
  }
  return vec;
}

AREXPORT template<typename LT> std::list<LT> ArPacketUtil::getNextListField(ArBasePacket& p)
{
  std::list<LT> l;
  size_t len = p.bufToUByte4();
  l.reserve(len);
  for(int i = 0; i < len; ++i)
  {
    l.push_back(getNextField<LT>(p));
  }
  return l;
}

AREXPORT template<typename VT> void ArPacketUtil::getNextField(ArBasePacket& p, std::vector<VT> *vec)
{
  vec->clear();
  size_t len = p.bufToUByte4();
  vec->reserve(len);
  for(int i = 0; i < len; ++i)
  {
    vec->push_back(getNextField<VT>(p));
  }
}

AREXPORT template<typename LT> void ArPacketUtil::getNextField(ArBasePacket& p, std::list<LT> *l)
{
  // todo resize and reuse existing list space??
  l->clear();
  size_t len = p.bufToUByte4();
  l->reserve(len);
  for(int i = 0; i < len; ++i)
  {
    l->push_back(getNextField<LT>(p));
  }
  // todo deal with Truncated packet
}

AREXPORT template<typename T> T ArPacketUtil::getNextField(ArBasePacket& p) throw (std::out_of_range)
{
  ArLog::log(ArLog::Terse, "INTERNAL ERROR: ArPacketUtil::getNextField<T>(ArBasePacket& p) called with unhandled type for T! Type must be one of: int, unsigned int, short, unsigned short, std::string, float, double, bool, char, unsigned char, or vector or list of any of the above.");
  abort();
}
