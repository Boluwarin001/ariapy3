#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>
#include <list>

template<typename T>
class ArBasePacket {
public:
    void byte4ToBuf(int32_t value) {}
    void uByte4ToBuf(uint32_t value) {}
    void byte2ToBuf(int16_t value) {}
    void uByte2ToBuf(uint16_t value) {}
    void strToBuf(const char* value) {}
    int32_t bufToByte4() { return 0; }
    uint32_t bufToUByte4() { return 0; }
    int16_t bufToByte2() { return 0; }
    uint16_t bufToUByte2() { return 0; }
    std::string bufToString() { return ""; }
    bool bufferContainsBytes(size_t size) { return true; }
};

class ArLog {
public:
    enum LogLevel { Terse };
    static void log(LogLevel level, const char* message) {}
};

class ArTypes {
public:
    using Byte4 = int32_t;
    using UByte4 = uint32_t;
    using Byte2 = int16_t;
    using UByte2 = uint16_t;
};

template<typename T>
class ArPacketUtil {
public:
    template<> static void addField<int>(ArBasePacket<T>& p, const int& value) {
        p.byte4ToBuf(value);
    }

    template<> static void addField<unsigned int>(ArBasePacket<T>& p, const unsigned int& value) {
        p.uByte4ToBuf(value);
    }

    template<> static void addField<short>(ArBasePacket<T>& p, const short& value) {
        p.byte2ToBuf(value);
    }

    template<> static void addField<unsigned short>(ArBasePacket<T>& p, const unsigned short& value) {
        p.uByte2ToBuf(value);
    }

    template<> static void addField<std::string>(ArBasePacket<T>& p, const std::string& value) {
        p.strToBuf(value.c_str());
    }

    template<> static void addField<float>(ArBasePacket<T>& p, const float& value) {
        p.byte4ToBuf(value * 10e4);
    }

    template<> static void addField<double>(ArBasePacket<T>& p, const double& value) {
        p.byte4ToBuf(value * 10e4);
    }

    template<> static void addField<bool>(ArBasePacket<T>& p, const bool& value) {
        p.uByteToBuf(value ? 1 : 0);
    }

    template<> static void addField<char>(ArBasePacket<T>& p, const char& value) {
        p.byteToBuf(value);
    }

    template<> static void addField<unsigned char>(ArBasePacket<T>& p, const unsigned char& value) {
        p.uByteToBuf(value);
    }

    template<> static void addField<ArPose>(ArBasePacket<T>& pkt, const ArPose& pose) {
        pkt.byte4ToBuf(static_cast<ArTypes::Byte4>(pose.getX()));
        pkt.byte4ToBuf(static_cast<ArTypes::Byte4>(pose.getY()));
        pkt.byte4ToBuf(static_cast<ArTypes::Byte4>(pose.getTh()));
    }

    template<> static void addField<ArLineSegment>(ArBasePacket<T>& p, const ArLineSegment& l) {
        p.byte4ToBuf(static_cast<ArTypes::Byte4>(l.getX1()));
        p.byte4ToBuf(static_cast<ArTypes::Byte4>(l.getY1()));
        p.byte4ToBuf(static_cast<ArTypes::Byte4>(l.getX2()));
        p.byte4ToBuf(static_cast<ArTypes::Byte4>(l.getY2()));
    }

    template<> static void addField<ArPos2D>(ArBasePacket<T>& pkt, const ArPos2D& pos) {
        pkt.byte4ToBuf(static_cast<ArTypes::Byte4>(pos.getX()));
        pkt.byte4ToBuf(static_cast<ArTypes::Byte4>(pos.getY()));
    }

    template<typename VT> static std::vector<VT> getNextVectorField(ArBasePacket<T>& p);
    template<typename LT> static std::list<LT> getNextListField(ArBasePacket<T>& p);

    template<typename VT> static void getNextField(ArBasePacket<T>& p, std::vector<VT>* vec);
    template<typename LT> static void getNextField(ArBasePacket<T>& p, std::list<LT>* l);
    template<typename T> static T getNextField(ArBasePacket<T>& p) throw(std::out_of_range);
};

template<typename T>
template<typename VT>
std::vector<VT> ArPacketUtil<T>::getNextVectorField(ArBasePacket<T>& p) {
    std::vector<VT> vec;
    size_t len = p.bufToUByte4();
    vec.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        vec.push_back(getNextField<VT>(p));
    }
    return vec;
}

template<typename T>
template<typename LT>
std::list<LT> ArPacketUtil<T>::getNextListField(ArBasePacket<T>& p) {
    std::list<LT> l;
    size_t len = p.bufToUByte4();
    l.reserve(len);
    for (size_t i = 0; i < len; ++i) {
        l.push_back(getNextField<LT>(p));
    }
    return l;
}

template<typename T>
template<typename VT>
void ArPacketUtil<T>::getNextField(ArBasePacket<T>& p, std::vector<VT>* vec) {
    vec->clear();
    size_t len = p.bufToUByte4();
    vec->reserve(len);
    for (size_t i = 0; i < len; ++i) {
        vec->push_back(getNextField<VT>(p));
    }
}

template<typename T>
template<typename LT>
void ArPacketUtil<T>::getNextField(ArBasePacket<T>& p, std::list<LT>* l) {
    l->clear();
    size_t len = p.bufToUByte4();
    l->reserve(len);
    for (size_t i = 0; i < len; ++i) {
        l->push_back(getNextField<LT>(p));
    }
}

template<typename T>
template<typename T>
T ArPacketUtil<T>::getNextField(ArBasePacket<T>& p) throw(std::out_of_range) {
    ArLog::log(ArLog::Terse, "INTERNAL ERROR: ArPacketUtil::getNextField<T>(ArBasePacket<T>& p) called with unhandled type for T! Type must be one of: int unsigned int, short, unsigned short, std::string, float, double, bool, char, unsigned char, or vector or list of any of the above.");
    abort();
}
