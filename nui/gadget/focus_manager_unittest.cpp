#include "focus_manager.h"
#include <nui/gadget/world.h>

namespace nui
{

//测试 焦点使用方向键在同一个GROUP中移动焦点
TEST(FocusManagerTest, TestAdvanceGroup)
{
    GadgetWorld test_root(nullptr);
    Gadget c0, c1, c2;
    Gadget c00, c01, c02, c03, c20, c21;
    Gadget c010, c011, c210, c211;

    c0.SetGroup(0);
    c00.SetGroup(0);
    c01.SetGroup(0);
    c010.SetGroup(0);

    test_root.AddChild(ScopedGadget(&c0));
    test_root.AddChild(ScopedGadget(&c1));
    test_root.AddChild(ScopedGadget(&c2));

    c0.AddChild(ScopedGadget(&c00));
    c0.AddChild(ScopedGadget(&c01));
    c0.AddChild(ScopedGadget(&c02));
    c0.AddChild(ScopedGadget(&c03));

    c2.AddChild(ScopedGadget(&c20));
    c2.AddChild(ScopedGadget(&c21));

    c01.AddChild(ScopedGadget(&c010));
    c01.AddChild(ScopedGadget(&c011));

    c21.AddChild(ScopedGadget(&c210));
    c21.AddChild(ScopedGadget(&c211));


    ScopedGadget test_root_gadget;
    test_root_gadget.Reset(&test_root);
    FocusManager * fm = test_root.GetFocusManager();
    //当前焦点为空
    fm->AdvanceFocusInGroup(true);
    ASSERT_EQ(fm->focused_gadget_, nullptr);
    //当前焦点为 默认group 则焦点不变
    fm->focused_gadget_ = &c20;
    fm->AdvanceFocusInGroup(true);
    ASSERT_EQ(fm->focused_gadget_, &c20);

    //当前焦点为group 0时
    //焦点仅在c01 c010中存在
    fm->focused_gadget_ = &c01;
    fm->AdvanceFocusInGroup(false);
    ASSERT_EQ(fm->focused_gadget_, &c010);
    fm->AdvanceFocusInGroup(false);
    ASSERT_EQ(fm->focused_gadget_, &c01);
    fm->AdvanceFocusInGroup(false);
    ASSERT_EQ(fm->focused_gadget_, &c010);

    //测试group中循环移动
    //顺序 c00, c01, c02, c03
    c00.SetGroup(1);
    c01.SetGroup(1);
    c02.SetGroup(1);
    c03.SetGroup(1);

    fm->focused_gadget_ = &c00;
    fm->AdvanceFocusInGroup(true);
    ASSERT_EQ(fm->focused_gadget_, &c03);
    fm->AdvanceFocusInGroup(true);
    ASSERT_EQ(fm->focused_gadget_, &c02);
    fm->AdvanceFocusInGroup(true);
    ASSERT_EQ(fm->focused_gadget_, &c01);
    fm->AdvanceFocusInGroup(true);
    ASSERT_EQ(fm->focused_gadget_, &c00);

    fm->AdvanceFocusInGroup(false);
    ASSERT_EQ(fm->focused_gadget_, &c01);
    fm->AdvanceFocusInGroup(false);
    ASSERT_EQ(fm->focused_gadget_, &c02);
    fm->AdvanceFocusInGroup(false);
    ASSERT_EQ(fm->focused_gadget_, &c03);
    fm->AdvanceFocusInGroup(false);
    ASSERT_EQ(fm->focused_gadget_, &c00);

}
//测试焦点在树上遍历
/*
                           root
       |                    |                     |
       c0                   c1                    c2
  |     |     |    |                      |               |
  c00   c01   c02  c03                    c20             c21
      |     |                                          |        |
     c010  c011                                        c210     c211
*/
TEST(FocusManagerTest, TestAdvance)
{
    GadgetWorld test_root(nullptr);
    Gadget c0, c1, c2;
    Gadget c00, c01, c02, c03, c20, c21;
    Gadget c010, c011, c210, c211;

    c0.SetGroup(0);
    c00.SetGroup(0);
    c01.SetGroup(0);
    c010.SetGroup(0);
    c2.SetFocusable(false);

    test_root.AddChild(ScopedGadget(&c0));
    test_root.AddChild(ScopedGadget(&c1));
    test_root.AddChild(ScopedGadget(&c2));

    c0.AddChild(ScopedGadget(&c00));
    c0.AddChild(ScopedGadget(&c01));
    c0.AddChild(ScopedGadget(&c02));
    c0.AddChild(ScopedGadget(&c03));

    c2.AddChild(ScopedGadget(&c20));
    c2.AddChild(ScopedGadget(&c21));

    c01.AddChild(ScopedGadget(&c010));
    c01.AddChild(ScopedGadget(&c011));

    c21.AddChild(ScopedGadget(&c210));
    c21.AddChild(ScopedGadget(&c211));

    ScopedGadget test_root_gadget;
    test_root_gadget.Reset(&test_root);
    FocusManager * fm = test_root.GetFocusManager();
    //当前焦点为null
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c0);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c00);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c01);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c010);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c011);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c02);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c03);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c1);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c20);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c21);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c210);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c211);
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c0);

    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c211);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c210);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c21);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c20);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c1);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c03);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c02);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c011);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c010);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c01);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c00);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c0);

    fm->focused_gadget_ = &c2;
    fm->AdvanceFocus(false);
    ASSERT_EQ(fm->focused_gadget_, &c20);
    fm->AdvanceFocus(true);
    ASSERT_EQ(fm->focused_gadget_, &c1);
}

}