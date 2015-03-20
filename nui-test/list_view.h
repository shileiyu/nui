#ifndef TEST_LIST_VIEW_H_
#define TEST_LIST_VIEW_H_


#include <nui/gadget/scroll_view.h>
#include <nui/layout/multi.h>
#include <nui/layout/sizer.h>
#include "label_button.h"
#include "progress_bar.h"

namespace nui
{

class ListItem : public Gadget
{
public:
    ListItem();

    ~ListItem();

    bool init();

    void fini();

    void Layout() override;

    LabelButton * btn(){ return &btn_; }
    Label * name() { return name_.Get(); }
protected:
    void OnDraw(Painter & painter) override;
private:
    ScopedPicture icon_;
    ScopedLabel name_;
    ScopedPicture bg_;
    LabelButton btn_;
    ProgressBar progress_;

    nui::MultiLayout multi_;
    PixelSizer layout_icon_;
    PixelSizer layout_name_;
    PixelSizer layout_bg_;
    PixelSizer layout_btn_;
    PixelSizer layout_progress_;
};

class ListView : public Gadget, public ButtonClient
{
public:
    ListView();

    void AddItem(int id);

    void RemoveItem(int id);

    void Layout() override;

    ScopedScrollView GetScroller();

    void OnClick(Button*  bt) override;
private:
    ScopedScrollView scroller_;
    std::vector<int> ids_;
    std::map<int, ScopedRef<ListItem >> items_;
    ScopedPicture background_;
};

}

#endif