#include "list_view.h"
#include <nui/gadget/picture.h>
#include <nui/gadget/label.h>

namespace nui
{

static const nui::Color kBtnColor[4] = { { 0xff, 0x5b, 0xb5, 0x31 },
{ 0xff, 0x7a, 0xc5, 0x29 },
{ 0xff, 0x36, 0xa4, 0x02 },
{ 0xff, 0x99, 0x99, 0x99 } };

static const nui::Color kBtnTextColor[4] = { { 0xff, 0xff, 0xff, 0xff },
{ 0xff, 0xff, 0xff, 0xff },
{ 0xff, 0xff, 0xff, 0xff },
{ 0xff, 0xff, 0xff, 0xff } };

static const nui::Color kPrepareProgressBarColor[2] = { { 0xff, 0xff, 0xff, 0xff },
{ 0xff, 0x6f, 0xcc, 0x2e } };

ListItem::ListItem()
{

}

bool ListItem::init()
{
    icon_ = Adopt(new Picture);
    name_ = Adopt(new Label);
    bg_ = Adopt(new Picture);
    btn_.init();
    progress_.init();

    AddChild(bg_);
    AddChild(icon_);
    AddChild(name_);
    AddChild(btn_.GetGadget());
    AddChild(progress_.GetGadget());

    name_->SetText(L"正常大冒险");
    bg_->SetBackground(Color(0xff, 0xf1, 0xf1, 0xf1));
    icon_->SetBackground(Color(0xff, 0xff, 0, 0));
    icon_->SetSize(Size::Make(120, 45));
    name_->SetSize(Size::Make(120, 45));
    btn_.GetGadget()->SetSize(Size::Make(30, 50));
    btn_.SetTextColor(kBtnTextColor);
    btn_.SetText(L"我");
    btn_.SetBGColor(kBtnColor);
    progress_.GetGadget()->SetSize(Size::Make(70, 30));
    progress_.SetColor(kPrepareProgressBarColor);
    progress_.Update(0.5f);

    layout_icon_.left(28).top(22);
    layout_icon_.Attach(icon_);
    layout_name_.left(28).top(74);
    layout_name_.Attach(name_);
    layout_bg_.left(0).top(0).right(0).bottom(1);
    layout_bg_.Attach(bg_);
    layout_btn_.left(250).top(20);
    layout_btn_.Attach(btn_.GetGadget());
    layout_progress_.left(170).top(20);
    layout_progress_.Attach(progress_.GetGadget());

    this->SetLayout(&multi_);
    multi_.Attach(&layout_icon_);
    multi_.Attach(&layout_name_);
    multi_.Attach(&layout_bg_);
    multi_.Attach(&layout_btn_);
    multi_.Attach(&layout_progress_);
    return true;
}
void ListItem::fini()
{
    RemoveChildren();
}
ListItem::~ListItem()
{
    SetLayout(nullptr);
}

void ListItem::Layout()
{
    multi_.Arrange(this);
    for (auto iter = children_.begin(); iter != children_.end(); ++iter)
    {
        (*iter)->Layout();
    }
}



void ListItem::OnDraw(Painter & painter)
{
    Point p0, p1;
    p0.Set(0, GetHeight() - 1);
    p1.Set(GetWidth(), GetHeight() - 1);
    Paint paint;
    paint.color = Color(0xff, 0xff, 0, 0);
    painter.DrawLine(p0, p1, paint);
}
static const int kBodyItemHeight = 106;

ListView::ListView()
{
    background_ = Adopt(new Picture);
    AddChild(background_);
    background_->SetBackground(Color(0xff, 0, 0, 0xff));
    scroller_ = Adopt(new ScrollView);
    scroller_->SetContents(this);
}

void ListView::OnClick(Button*  bt)
{
    for (auto iter = items_.begin(); iter != items_.end(); ++iter)
    {
        if (iter->second->btn() == bt)
        {
            if (iter->first == 1)
            {
                iter->second->fini();
                RemoveItem(1);
                break;
            }
        }
    }
}

ScopedScrollView ListView::GetScroller()
{
    return scroller_; 
}

void ListView::AddItem(int id)
{
    ids_.push_back(id);
    auto p = Adopt(new ListItem);
    p->init();
    items_[id] = p;
    AddChild(p);
    p->btn()->SetClient(this);
    wchar_t tmp[100] = { 0 };
    wsprintf(tmp, L"%d", id);
    std::wstring t = L"正常大冒险: ";
    t += tmp;
    p->name()->SetText(t.data());
    SetPreferredSize(Size::Make(preferred_size_.width(), ids_.size() * kBodyItemHeight));

    //
//    RemoveChild(p);
//    p->fini();
//    items_.erase(id);
//
//    ListItem * m = p.Get();
//    if (m != p.Get())
//    {
//        assert(0);
//    }
}

void ListView::RemoveItem(int id)
{
    auto iter = std::find(ids_.begin(), ids_.end(), id);
    if (iter != ids_.end())
    {
        ids_.erase(iter);
        RemoveChild(items_[id]);
        items_[id]->fini();
        items_.erase(id);
    }
        

    
    
    SetPreferredSize(Size::Make(preferred_size_.width(), ids_.size() * kBodyItemHeight));
}


void ListView::Layout()
{
    background_->SetLoc(Point::Make(0, 0));
    background_->SetSize(Size::Make(GetWidth(), GetHeight()));
    if (background_->GetSize().width() != 0)
    {
        if (1 == 2)
            return;
    }
    int top = 0;
    for (auto iter = ids_.begin(); iter != ids_.end(); ++iter)
    {
        auto item = items_.find(*iter);
        item->second->SetLoc(Point::Make(0, top));
        top += kBodyItemHeight;
        item->second->SetSize(Size::Make(GetWidth(), kBodyItemHeight));
        item->second->Layout();
    }
}



}