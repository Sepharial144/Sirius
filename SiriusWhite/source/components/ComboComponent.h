#pragma once

#include "imgui.h"
#include <cstdint>
#include <vector>

namespace ui::component
{
    struct ComboOptions
    {
        std::vector<const char*> comboValues;
        const char* prevValue;
        int32_t selectedItemIdx;
        ImGuiComboFlags comboFlags;
        bool isSelectedItem = false;
    };

    inline void Combo(const char* name, ComboOptions &options) {
        //std::vector<const char*> itemValuesArray = { "Exact value", "Bigger than...", "Smaller than...", "Value between...", "Uknown intial value" };
        //const char* itemsValue[] = { "Exact value", "Bigger than...", "Smaller than...", "Value between...", "Uknown intial value" };
        //static int item_value_selected_idx = 0; // Here we store our selection data as an index.

        // Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
        //const char* combo_preview_value = itemsValue[item_value_selected_idx];
        options.prevValue = options.comboValues[0];

        if (ImGui::BeginCombo(name, options.prevValue, options.comboFlags))
        {
            for (int32_t i = 0; i < options.comboValues.size(); i++)
            {
                options.isSelectedItem = (options.selectedItemIdx == i);
                if (ImGui::Selectable(options.comboValues[i], options.isSelectedItem))
                    options.selectedItemIdx = i;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (options.isSelectedItem)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
            return;
        }
        // throw exception 
    }
    /*
    const char* itemsValue[] = { "Exact value", "Bigger than...", "Smaller than...", "Value between...", "Uknown intial value" };
    static int item_value_selected_idx = 0; // Here we store our selection data as an index.

    // Pass in the preview value visible before opening the combo (it could technically be different contents or not pulled from items[])
    const char* combo_preview_value = itemsValue[item_value_selected_idx];

    ImGuiComboFlags combo_flags = 0;
    if (ImGui::BeginCombo("Scan type", combo_preview_value, combo_flags))
    {
        for (int i = 0; i < IM_ARRAYSIZE(itemsValue); i++)
        {
            const bool is_selected = (item_value_selected_idx == i);
            if (ImGui::Selectable(itemsValue[i], is_selected))
                item_value_selected_idx = i;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    */


} // !namespace ui::component