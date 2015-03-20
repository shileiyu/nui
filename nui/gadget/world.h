#ifndef NUI_GADGET_WORLD_H_
#define NUI_GADGET_WORLD_H_

#include <nui/nui.h>
#include <nui/base/mouse.h>
#include <nui/gadget/types.h>
#include <nui/gadget/gadget.h>
#include <nui/base/cursor_style.h>
#include <nui/gadget/focus_manager.h>


namespace nui
{

class GadgetWorldClient
{
public:
    virtual void PenddingRedraw(ScopedWorld world, const Rect & rect) = 0;

    virtual void SetCursor(ScopedWorld world, CursorStyles cursor) = 0;
};

class GadgetWorld : public Gadget
{
public:
    GadgetWorld(GadgetWorldClient * client);

    ~GadgetWorld();

    void SetMousePos(const Point & pt);

    void MouseButtonDown(MouseButton button);

    void MouseButtonUp(MouseButton button);

    void Wheel(int dx, int dy);

    void SetMouseState(MouseState state);

    void SetFocus(bool focus);
    //临时
    void HandleKeyEvent(GadgetKeyEvent & e);

    void Draw(Painter & painter, const Rect & inval_rect);

    ScopedGadget HitTest(const Point & pt);

    FocusManager * GetFocusManager();

    const FocusManager * GetFocusManager() const;

    bool IsDrawn() const override;

    void Invalidate(const Rect & inval) override;

    ScopedWorld GetRoot() override;

    const ScopedWorld GetRoot() const override;
protected:
    void OnDraw(Painter & painter) override;

    void OnHierarchyChanged(const HierarchyChangedDetails & details) override;

    void OnVisibilityChanged(ScopedGadget starting_from, bool is_visible) override;
private:
    void ShiftFocus(ScopedGadget gadget);

    void ShiftOver(ScopedGadget gadget);

    ScopedGadget GetTargetByMouse();

    GadgetWorldClient & GadgetWorld::Client();
private:
    FocusManager focus_manager_;

    CursorStyles cursor_;
    MouseState mouse_state_;
    Point mouse_pos_;
    ScopedGadget over_;
    ScopedGadget capture_;

    GadgetWorldClient * client_;
};

}
#endif