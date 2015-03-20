#ifndef NUI_GADGET_TYPES_H_
#define NUI_GADGET_TYPES_H_

#include <nui/nui.h>
#include <nui/base/ref.h>

namespace nui
{

class GadgetEvent;
class GadgetMouseEvent;
class GadgetKeyEvent;
class GadgetFocusEvent;
class GadgetWheelEvent;
class GadgetWorld;
class Gadget;
class Picture;
class Label;
class ScrollView;
class ScrollBar;

typedef ScopedRef<Gadget> ScopedGadget;
typedef ScopedRef<GadgetWorld> ScopedWorld;
typedef ScopedRef<Picture> ScopedPicture;
typedef ScopedRef<Label> ScopedLabel;
typedef ScopedRef<ScrollView> ScopedScrollView;
typedef ScopedRef<ScrollBar> ScopedScrollBar;

}

#endif