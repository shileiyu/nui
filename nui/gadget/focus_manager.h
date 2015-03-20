
#ifndef NUI_GADGET_FOCUS_MANAGER_H_
#define NUI_GADGET_FOCUS_MANAGER_H_

#include <nui/nui.h>
#include <nui/gadget/types.h>
#include "gtest/gtest.h"

namespace nui
{

class FocusManager
{
    FRIEND_TEST(FocusManagerTest, TestAdvance);
    FRIEND_TEST(FocusManagerTest, TestAdvanceGroup);
public:
    enum FocusChangeReason
    {
        kReasonFocusTraversal,
        
        kReasonFocusRestore,

        kReasonDirectFocusChange,
    };

    FocusManager(GadgetWorld & root);

    virtual ~FocusManager();

    bool HandleKeyEvent(GadgetKeyEvent & key);

    void AdvanceFocusIfNecessary();

    void RemoveGadget(ScopedGadget gadget);

    ScopedGadget GetFocusedGadget();

    const ScopedGadget GetFocusedGadget() const;

    void SetFocusedGadgetWithReason(ScopedGadget gadget, FocusChangeReason reason);

    void SetFocusedGadget(ScopedGadget gadget);

    void StoreFocusedGadget();

    void RestoreFocusedGadget();
private:
    bool IsTabTraversalKeyEvent(GadgetKeyEvent & key);

    bool ProcessArrowKeyTraversal(GadgetKeyEvent & key);

    bool IsArrowKeyEvent(GadgetKeyEvent & key);

    void SetStoredFocusGadget(ScopedGadget gadget);

    const ScopedGadget GetStoredFocusGadget() const;

    void ClearStoredFocusedGadget();

    void AdvanceFocus(bool reverse);

    void AdvanceFocusInGroup(bool reverse);

    void ValidateFocusedGadget();

    bool Contains(ScopedGadget & gadget);

    ScopedGadget FindNextFocusableGadget(ScopedGadget & start, bool reverse, bool check_start);

    bool IsFocusableCandidate(ScopedGadget & gadget, int skip_group_id);

    bool IsFocusable(ScopedGadget & gadget);

    ScopedGadget FindSelectedGadgetForGroup(ScopedGadget & gadget);

    ScopedGadget FindNextFocusableGadgetImpl(
                                             ScopedGadget & start,
                                             bool check_start,
                                             bool can_go_up,
                                             bool can_go_down,
                                             int skip_group_id);

    ScopedGadget FindPrevFocusableGadgetImpl(
                                             ScopedGadget & start,
                                             bool check_start,
                                             bool can_go_up,
                                             bool can_go_down,
                                             int skip_group_id);
private:
    static bool arrow_key_traversal_enabled_;
    GadgetWorld * root_;//focus_manager 对root 弱引用
    ScopedGadget focused_gadget_;
    FocusChangeReason focus_change_reason_;
    ScopedGadget stored_gadget_;
};

}
#endif