// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2004-2020 Darby Johnston
// All rights reserved.

#pragma once

#include <djvUI/Widget.h>

namespace djv
{
    namespace UI
    {
        //! List item.
        struct ListItem
        {
            ListItem();
            ListItem(const std::string& text);
            
            std::string icon;
            std::string rightIcon;
            std::string text;
            std::string rightText;
            ColorRole   colorRole = ColorRole::None;
            std::string tooltip;

            bool operator == (const ListItem&) const;
        };

        //! List widget.
        //!
        //! \todo Keep the current item visible in the scroll widget.
        class ListWidget : public Widget
        {
            DJV_NON_COPYABLE(ListWidget);

        protected:
            void _init(ButtonType, const std::shared_ptr<System::Context>&);
            ListWidget();

        public:
            ~ListWidget() override;

            //! Create a new list widget.
            static std::shared_ptr<ListWidget> create(ButtonType, const std::shared_ptr<System::Context>&);

            //! \name Items
            ///@{

            void setItems(const std::vector<std::string>&);
            void setItems(const std::vector<ListItem>&);
            void clearItems();

            ///@}

            //! \name Checked State
            ///@{

            int getChecked() const;

            void setChecked(int, bool = true);

            ///@}

            //! \name Callbacks
            ///@{

            void setPushCallback(const std::function<void(int)>&);
            void setToggleCallback(const std::function<void(int, bool)>&);
            void setRadioCallback(const std::function<void(int)>&);
            void setExclusiveCallback(const std::function<void(int)>&);

            ///@}

            //! \name Filter
            ///@{

            void setFilter(const std::string&);

            ///@}

            //! \name Options
            ///@{

            void setRowColorRoles(ColorRole even, ColorRole odd);

            ///@}

        protected:
            void _preLayoutEvent(System::Event::PreLayout&) override;
            void _layoutEvent(System::Event::Layout&) override;
            void _keyPressEvent(System::Event::KeyPress&) override;

        private:
            void _itemsUpdate();
            void _filterUpdate();

            DJV_PRIVATE();
        };

    } // namespace UI
} // namespace djv

