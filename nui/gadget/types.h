#ifndef NUI_GADGET_TYPES_H_
#define NUI_GADGET_TYPES_H_

#include <nui/nui.h>
#include <nui/base/ref.h>

namespace nui
{

class Event;
class MouseEvent;
class KeyEvent;
class FocusEvent;
class WheelEvent;
class World;
class Gadget;
class Picture;
class Label;
class ScrollView;
class ScrollBar;

typedef ScopedRef<Gadget> ScopedGadget;
typedef ScopedRef<World> ScopedWorld;
typedef ScopedRef<Picture> ScopedPicture;
typedef ScopedRef<Label> ScopedLabel;
typedef ScopedRef<ScrollView> ScopedScrollView;
typedef ScopedRef<ScrollBar> ScopedScrollBar;

}

#endif