   
#include <nui/gadget/focus_manager.h>
#include <nui/gadget/event_dispatch.h>
#include <nui/gadget/events.h>
#include <nui/gadget/world.h>

namespace nui
{

bool FocusManager::arrow_key_traversal_enabled_ = false;

FocusManager::FocusManager(World & root)
:focus_change_reason_(kReasonFocusTraversal), root_(&root)
{
    ;
}

FocusManager::~FocusManager()
{
    ;
}

bool FocusManager::HandleKeyEvent(KeyEvent & key)
{
    auto key_code = key.key_code();
    if (key.type() == EventType::kKeyDown)
    {//按键按下
        if (focused_gadget_ && focused_gadget_->SkipDefaultKeyEventProcessing(key))
            return false;

        if (IsTabTraversalKeyEvent(key))
        {
            AdvanceFocus(key.IsShiftDown());
            return true;
        }
        
        if (arrow_key_traversal_enabled_ && ProcessArrowKeyTraversal(key))
            return true;

        if (focused_gadget_ && focused_gadget_->GetGroup() != -1 && IsArrowKeyEvent(key))
        {
            bool reverse = (key_code == KeyCode::kLeft || key_code == KeyCode::kUp);
            AdvanceFocusInGroup(reverse);
            return true;
        }
    }

    return false;
}

bool FocusManager::IsTabTraversalKeyEvent(KeyEvent & key)
{
    return key.key_code() == KeyCode::kTab && !key.IsControlDown();
}

bool FocusManager::ProcessArrowKeyTraversal(KeyEvent & key)
{
    if (key.IsShiftDown() || key.IsControlDown() || key.IsAltDown())
        return false;

    auto key_code = key.key_code();
    if (key_code == KeyCode::kLeft || key_code == KeyCode::kUp)
    {
        AdvanceFocus(true);
        return true;
    }

    if (key_code == KeyCode::kRight || key_code == KeyCode::kDown)
    {
        AdvanceFocus(false);
        return true;
    }

    return false;
}

bool FocusManager::IsArrowKeyEvent(KeyEvent & key)
{
    auto key_code = key.key_code();
    return key_code == KeyCode::kLeft || key_code == KeyCode::kUp ||
        key_code == KeyCode::kRight || key_code == KeyCode::kDown;
}

bool FocusManager::Contains(ScopedGadget & gadget)
{
    auto root = gadget->GetRoot();
    return root == root_;
}

void FocusManager::AdvanceFocusIfNecessary()
{
    if (focused_gadget_ && !focused_gadget_->IsFocusable())
    {
        AdvanceFocus(false);
        if (focused_gadget_ && !focused_gadget_->IsFocusable())
            SetFocusedGadget(nullptr);
    }
}

void FocusManager::RemoveGadget(ScopedGadget gadget)
{
    if (!gadget)
        return;
    if (focused_gadget_ && gadget->Contains(focused_gadget_))
    {
        AdvanceFocus(false);
        if (focused_gadget_ && !focused_gadget_->IsFocusable())
            SetFocusedGadget(nullptr);
    }     
}

ScopedGadget FocusManager::GetFocusedGadget()
{
    return focused_gadget_;
}

const ScopedGadget FocusManager::GetFocusedGadget() const
{
    return focused_gadget_;
}

void FocusManager::AdvanceFocus(bool reverse)
{
    //确认焦点合法 要么在root上要么为空
    ValidateFocusedGadget();
    ScopedGadget start = focused_gadget_;

    if (!start)
        start = root_;
    auto gadget = FindNextFocusableGadget(start, reverse, false);

    if (!gadget)
    {//未找到 同时 start ！= root 代表已经搜索到树的叶子或者根部 此时再从root开始搜索
        if (start != root_)
        {
            start = root_;
            gadget = FindNextFocusableGadget(start, reverse, true);
        }
    }

    if (gadget)
    {
        if (gadget != focused_gadget_)
            SetFocusedGadgetWithReason(gadget, kReasonFocusTraversal);
    }
}

void FocusManager::AdvanceFocusInGroup(bool reverse)
{
    ValidateFocusedGadget();
    if (!focused_gadget_ || focused_gadget_->GetGroup() == -1)
        return;

    Gadget::ScopedGadgets gadgets;
    focused_gadget_->parent()->GetGadgetsInGroup(focused_gadget_->GetGroup(), gadgets);

    auto iter = std::find(gadgets.begin(), gadgets.end(), focused_gadget_);
    if (iter == gadgets.end())
        return;

    int index = static_cast<int>(iter - gadgets.begin());
    int max_size = static_cast<int>(gadgets.size());

    index += reverse ? -1 : 1;
    if (index < 0)
        index = max_size - 1;
    else if (index >= max_size)
        index = 0;

    SetFocusedGadgetWithReason(gadgets[index], kReasonFocusTraversal);
}

void FocusManager::SetFocusedGadget(ScopedGadget gadget)
{
    SetFocusedGadgetWithReason(gadget, kReasonFocusTraversal);
}

void FocusManager::SetFocusedGadgetWithReason(ScopedGadget gadget, FocusChangeReason reason)
{
    typedef FocusEventDispatch Dispatch;
    if (focused_gadget_ == gadget)
        return;
    
    Dispatch dispatch;
    bool tab = (reason == kReasonFocusTraversal);

    auto old_focused = focused_gadget_;

    if (old_focused)
        dispatch.Run(FocusEvent::FocusOut(old_focused, tab));

    if (gadget)
        dispatch.Run(FocusEvent::FocusIn(gadget, tab));

    focus_change_reason_ = reason;
    focused_gadget_ = gadget;
    //SetStoredFocusGadget(focused_gadget_);

    if (old_focused)
        dispatch.Run(FocusEvent::Blur(old_focused, tab));

    if (gadget)
        dispatch.Run(FocusEvent::Focus(gadget, tab));
}

void FocusManager::RestoreFocusedGadget()
{
    SetFocusedGadgetWithReason(GetStoredFocusGadget(), kReasonFocusRestore);
}

void FocusManager::StoreFocusedGadget()
{
    if (!focused_gadget_)
        return;

    SetStoredFocusGadget(focused_gadget_);
}

void FocusManager::SetStoredFocusGadget(ScopedGadget gadget)
{
    stored_gadget_ = gadget;
}

const ScopedGadget FocusManager::GetStoredFocusGadget() const
{
    return stored_gadget_;
}

void FocusManager::ClearStoredFocusedGadget()
{
    SetStoredFocusGadget(nullptr);
}

void FocusManager::ValidateFocusedGadget()
{
    if (focused_gadget_ && !Contains(focused_gadget_))
        SetFocusedGadget(nullptr);
}


ScopedGadget FocusManager::FindNextFocusableGadget(ScopedGadget & org_start, bool reverse, bool check_start)
{
    if (!org_start)
        return nullptr;
    auto root = org_start->GetRoot();
    if (!root)
        return nullptr;

    auto start = org_start;
    if (start == root)
    {
        if (start->GetChildCount() == 0)
            return nullptr;

        check_start = true;
        if (reverse)
        {
            while (start->GetChildCount() > 0)
                start = start->GetChild(static_cast<int>(start->GetChildCount()) - 1);
            while (start->GetNextFocusable())
                start = start->GetNextFocusable();
        }
        else
            start = root->GetChild(0);
    }

    int start_group = -1;
    if (start)
        start_group = start->GetGroup();

    ScopedGadget gadget = nullptr;
    if (!reverse)
    {//寻找下一个节点
        gadget = FindNextFocusableGadgetImpl(
            start, check_start, true, true, start_group);
    }
    else
    {//退到上一个节点
        //start是可以获取焦点的 则不需要看自己的子 只需要看父或者兄以及兄的子
        bool can_go_down = !IsFocusable(start);
        gadget = FindPrevFocusableGadgetImpl(
            start, check_start, true, can_go_down, start_group);
    }

    if (gadget && gadget == root)
        gadget = nullptr;


    return gadget;
}

bool FocusManager::IsFocusableCandidate(ScopedGadget & gadget, int skip_group_id)
{
    return IsFocusable(gadget) &&
        (gadget->IsGroupFocusTraversable() || skip_group_id == -1 ||
        gadget->GetGroup() != skip_group_id);
}

bool FocusManager::IsFocusable(ScopedGadget & gadget)
{
    return gadget && gadget->IsFocusable();
}

ScopedGadget FocusManager::FindSelectedGadgetForGroup(ScopedGadget & gadget)
{
    if (gadget->IsGroupFocusTraversable() || gadget->GetGroup() == -1)
        return gadget;

    ScopedGadget selected = gadget->GetSelectedForGroup(gadget->GetGroup());
    if (selected)
        return selected;

    return gadget;
}

ScopedGadget FocusManager::FindNextFocusableGadgetImpl(
    ScopedGadget & start,
    bool check_start,
    bool can_go_up,
    bool can_go_down,
    int skip_group_id)
{
    //1.看自己是否可以获取焦点
    if (check_start)
    {
        if (IsFocusableCandidate(start, skip_group_id))
        {
            ScopedGadget gadget = FindSelectedGadgetForGroup(start);
            if (IsFocusable(gadget))
                return gadget;
        }
    }

    //2.看自己的子是否可以获取焦点
    if (can_go_down)
    {
        if (start->GetChildCount() > 0)
        {//对于子 不需要向上搜索
            auto gadget = FindNextFocusableGadgetImpl(
                start->GetChild(0),
                true, false, true, skip_group_id);

            if (gadget)
                return gadget;
        }
    }

    //3.看自己的兄弟是否可以获取焦点
    ScopedGadget sibling = start->GetNextFocusable();
    if (sibling)
    {
        auto gadget = FindNextFocusableGadgetImpl(
            sibling,
            true, false, true, skip_group_id);
        if (gadget)
            return gadget;
    }

    //4.看自己父的兄是否可以获取焦点
    if (can_go_up)
    {
        ScopedGadget parent = start->parent();

        while (parent)
        {
            sibling = parent->GetNextFocusable();
            if (sibling)
                return FindNextFocusableGadgetImpl(sibling, true, true, true, skip_group_id);

            parent = parent->parent();
        }
    }

    return nullptr;
}

ScopedGadget FocusManager::FindPrevFocusableGadgetImpl(
    ScopedGadget & start,
    bool check_start,
    bool can_go_up,
    bool can_go_down,
    int skip_group_id)
{
    if (can_go_down)
    {
        if (start->GetChildCount() > 0)
        {
            int index = static_cast<int>(start->GetChildCount()) - 1;
            auto gadget = FindPrevFocusableGadgetImpl(
                start->GetChild(index),
                true, false, true, skip_group_id);
            if (gadget)
                return gadget;
        }
    }

    if (check_start && IsFocusableCandidate(start, skip_group_id))
    {
        auto gadget = FindSelectedGadgetForGroup(start);
        if (IsFocusable(gadget))
            return gadget;
    }

    //查找兄弟节点
    auto sibling = start->GetPreviousFocusable();
    if (sibling)//maybe true false true
        return FindPrevFocusableGadgetImpl(sibling, true, true, true, skip_group_id);

    //查找父节点时不需要再查子节点
    if (can_go_up)
    {
        auto parent = start->parent();
        if (parent)
            return FindPrevFocusableGadgetImpl(parent, true, true, false, skip_group_id);
    }

    return nullptr;
}


}