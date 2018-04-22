/*
    This file is part of Helio Workstation.

    Helio is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Helio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Helio. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

//[Headers]
#include "DraggingListBoxComponent.h"

class IconComponent;

struct MenuItem final : public ReferenceCountedObject
{
    enum Alignment
    {
        Left,
        Right
    };

    typedef Function<void()> Callback;
    typedef ReferenceCountedObjectPtr<MenuItem> Ptr;

    Image image;
    String iconName;
    String commandText;
    String subText;
    Colour colour;
    Alignment alignment;
    int commandId;
    bool isToggled;
    bool isDisabled;
    bool hasSubmenu;
    bool hasTimer;
    Callback callback;

    MenuItem();
    MenuItem::Ptr withAction(const Callback &lambda);
    MenuItem::Ptr withAlignment(Alignment alignment);
    MenuItem::Ptr withSubmenu();
    MenuItem::Ptr withTimer();
    MenuItem::Ptr toggled(bool shouldBeToggled);
    MenuItem::Ptr withSubLabel(const String &text);
    MenuItem::Ptr colouredWith(const Colour &colour);
    MenuItem::Ptr disabledIf(bool condition);

    static MenuItem::Ptr empty();
    static MenuItem::Ptr item(const String &icon, const String &text);
    static MenuItem::Ptr item(const String &icon, int commandId, const String &text = {});
    static MenuItem::Ptr item(Image image, int commandId, const String &text = {});
};
//[/Headers]


class MenuItemComponent final : public DraggingListBoxComponent,
                                private Timer
{
public:

    MenuItemComponent(Component *parentCommandReceiver, Viewport *parentViewport, const MenuItem::Ptr desc);
    ~MenuItemComponent();

    //[UserMethods]

    void setSelected(bool shouldBeSelected) override;

    void update(const MenuItem::Ptr description);

    String getIconName() const noexcept
    {
        return this->description->iconName;
    }

    Font getFont() const noexcept
    {
        return this->textLabel->getFont();
    }

    //[/UserMethods]

    void paint (Graphics& g) override;
    void resized() override;
    void mouseMove (const MouseEvent& e) override;
    void mouseEnter (const MouseEvent& e) override;
    void mouseExit (const MouseEvent& e) override;
    void mouseDown (const MouseEvent& e) override;
    void mouseUp (const MouseEvent& e) override;


private:

    //[UserVariables]

    Image icon;
    MenuItem::Ptr description;

    SafePointer<Component> parent;

    ScopedPointer<Component> clickMarker;
    ScopedPointer<Component> toggleMarker;
    ScopedPointer<Component> colourHighlighter;

    Point<int> lastMouseScreenPosition;

    ComponentAnimator animator;

    Component *createHighlighterComponent() override;
    void timerCallback() override;
    inline bool hasText() const noexcept
    {
        return this->description->commandText.isNotEmpty() ||
            this->description->subText.isNotEmpty();
    }

    // workaround странного поведения juce
    // возможна ситуация, когда mousedown'а не было, а mouseup срабатывает
    bool mouseDownWasTriggered;

    void doAction();

    //[/UserVariables]

    ScopedPointer<Label> subLabel;
    ScopedPointer<Label> textLabel;
    ScopedPointer<IconComponent> submenuMarker;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuItemComponent)
};
