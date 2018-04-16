#include "PacketQueue.h"

namespace Puff {


PacketQueue::PacketQueue() {

}

PacketQueue::~PacketQueue() {

}

bool PacketQueue::checkFull() const
{
    return true;
}

bool PacketQueue::checkEmpty() const {
    return true;
}

bool PacketQueue::checkEnough() const {
    return true;
}

}
