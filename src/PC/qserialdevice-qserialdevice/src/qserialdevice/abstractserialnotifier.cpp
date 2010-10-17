#include "abstractserialnotifier.h"
#include "nativeserialnotifier.h"


AbstractSerialNotifier *AbstractSerialNotifier::createSerialNotifier(Serial::DESCRIPTOR descriptor,
                                                                     NativeSerialEngine *parent)
{
    return new NativeSerialNotifier(descriptor, parent);
}
