#include "EditorLayer.h"
#include "imgui.h"
#include "implot.h"
#include "FileDialog.h"
#include "Navigator/Application.h"
#include "Navigator/SpectrumSerializer.h"

#include "IconsFontAwesome5.h"

namespace Navigator {
    
    EditorLayer::EditorLayer() :
        Layer("EditorLayer"), m_removeHistogram(false), m_removeCut(false)
    {
    }
    
    EditorLayer::~EditorLayer() {}

    void EditorLayer::OnAttach()
    {
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate()
    {
    }

    void EditorLayer::OnEvent(Event& e)
    {
    }

    void EditorLayer::OnImGuiRender()
    {
        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        HistogramMap& histoMap = HistogramMap::GetInstance();
        CutMap& cutMap = CutMap::GetInstance();
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("MyTestSpace", &dockspaceOpen, window_flags);
        ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        style.WindowMinSize.x = minWinSizeX;

        if(ImGui::BeginMenuBar())
        {
            if(ImGui::BeginMenu("File"))
            {
                if(ImGui::MenuItem(ICON_FA_FOLDER_OPEN "\tOpen"))
                {
                    m_fileDialog.SetOpenFileDialog(true);
                }
                if(ImGui::MenuItem(ICON_FA_SAVE "\tSave"))
                {
                    m_fileDialog.SetSaveFileDialog(true);
                }
                if (ImGui::MenuItem(ICON_FA_TIMES_CIRCLE "\tExit"))
                {
                    Application::Get().Close();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Data Source"))
            {
                if (ImGui::MenuItem(ICON_FA_LINK "\tAttach Source"))
                {
                    m_sourceDialog.OpenSourceDialog();
                }
                if (ImGui::MenuItem(ICON_FA_UNLINK "\tDetach Source"))
                {
                    PhysicsStopEvent event;
                    m_callbackFunc(event);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Add"))
            {
                if (ImGui::MenuItem(ICON_FA_CHART_BAR "\tSpectrum"))
                {
                    m_spectrumDialog.SetSpectrumDialog();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Remove"))
            {
                if (ImGui::MenuItem(ICON_FA_CHART_BAR "\tSpectrum"))
                {
                    m_removeHistogram = true;
                }
                if (ImGui::MenuItem(ICON_FA_CUT "\tCut"))
                {
                    m_removeCut = true;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }


        std::string open_file_result = m_fileDialog.ImGuiRenderOpenFile(".nav");
        std::string save_file_result = m_fileDialog.ImGuiRenderSaveFile(".nav");
        if (!open_file_result.empty())
        {
            SpectrumSerializer serializer(open_file_result);
            serializer.DeserializeData();
        }
        else if (!save_file_result.empty())
        {
            NAV_INFO("Found a Save File! {0}", save_file_result);
            SpectrumSerializer serializer(save_file_result);
            serializer.SerializeData();
        }
        
        m_spectrumDialog.ImGuiRenderSpectrumDialog();

        m_sourceDialog.ImGuiRenderSourceDialog();

        RemoveHistogramDialog();

        RemoveCutDialog();

        m_spectrumPanel.OnImGuiRender();

        if (ImGui::Begin(ICON_FA_CHART_BAR " Spectra"))
        {
            for (auto& gram : histoMap)
            {
                auto& params = gram.second->GetParameters();
                if (ImGui::TreeNode(params.name.c_str()))
                {
                    ImGui::BulletText(("X Parameter: "+params.x_par).c_str());
                    ImGui::BulletText("X Bins: %d X Min: %f X Max: %f", params.nbins_x, params.min_x, params.max_x);
                    if (params.y_par != "None")
                    {
                        ImGui::BulletText(("Y Parameter: "+params.y_par).c_str());
                        ImGui::BulletText("Y Bins: %d Y Min: %f Y Max: %f", params.nbins_y, params.min_y, params.max_y);
                    }
                    if(params.cutsDrawnUpon.size() != 0 && ImGui::TreeNode("Cuts Drawn"))
                    {
                        for(auto& cut : params.cutsDrawnUpon)
                            ImGui::BulletText(cut.c_str());
                        ImGui::TreePop();
                    }
                    if(params.cutsAppliedTo.size() != 0 && ImGui::TreeNode("Cuts Applied"))
                    {
                        for(auto& cut : params.cutsAppliedTo)
                            ImGui::BulletText(cut.c_str());
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
            }
            ImGui::End();
        }
        
        if(ImGui::Begin(ICON_FA_CUT " Cuts"))
        {
            for(auto& cut : cutMap)
            {
                auto& params = cut.second->GetCutParams();
                if(ImGui::TreeNode(params.name.c_str()))
                {
                    ImGui::BulletText(("X Parameter: "+params.x_par).c_str());
                    if(params.y_par != "None")
                        ImGui::BulletText(("Y Parameter: "+params.y_par).c_str());
                    ImGui::TreePop();
                }
            }
            ImGui::End();
        }

        ImGui::End();
    }

    void EditorLayer::RemoveHistogramDialog()
    {
        HistogramMap& histMap = HistogramMap::GetInstance();
        static std::string selectedGram = "";
        if (m_removeHistogram)
        {
            selectedGram = "";
            m_removeHistogram = false;
            ImGui::OpenPopup("Remove Histogram");
        }
        if (ImGui::BeginPopupModal("Remove Histogram"))
        {
            if (ImGui::BeginCombo("Histogram", selectedGram.c_str()))
            {
                for (auto& gram : histMap)
                {
                    if (ImGui::Selectable(gram.second->GetName().c_str(), gram.second->GetName() == selectedGram, ImGuiSelectableFlags_DontClosePopups))
                        selectedGram = gram.second->GetName();
                }
                ImGui::EndPopup();
            }
            if (ImGui::Button("Ok"))
            {
                histMap.RemoveHistogram(selectedGram);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }

    void EditorLayer::RemoveCutDialog()
    {
        HistogramMap& histMap = HistogramMap::GetInstance();
        CutMap& cutMap = CutMap::GetInstance();
        static std::string selectedCut = "";
        if (m_removeCut)
        {
            selectedCut = "";
            m_removeCut = false;
            ImGui::OpenPopup("Remove Cut");
        }
        if (ImGui::BeginPopupModal("Remove Cut"))
        {
            if (ImGui::BeginCombo("Cut", selectedCut.c_str()))
            {
                for (auto& cut : cutMap)
                {
                    if (ImGui::Selectable(cut.second->GetName().c_str(), cut.second->GetName() == selectedCut, ImGuiSelectableFlags_DontClosePopups))
                        selectedCut = cut.second->GetName();
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("Ok"))
            {
                histMap.RemoveCutFromHistograms(selectedCut);
                cutMap.RemoveCut(selectedCut);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}
