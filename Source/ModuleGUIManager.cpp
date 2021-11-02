#include "ModuleGuiManager.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "imgui.h"

ModuleGuiManager::ModuleGuiManager(Application* app, bool start_enabled) : Module(app, start_enabled), fpsLog(100), msLog(100)
{
    fpsCounter = 0;
}

// Destructor
ModuleGuiManager::~ModuleGuiManager()
{}

bool ModuleGuiManager::Start()
{
    LOG("Loading Gui Manager");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& ioHandler = ImGui::GetIO(); (void)ioHandler;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();


    // Setup Platform/Renderer bindings
    // window is the SDL_Window*
    // context is the SDL_GLContext
    ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer3D->context);
    ImGui_ImplOpenGL3_Init("#version 460");

    LogConsoleText.appendf(App->buff->initBuff1);
    LogConsoleText.appendf(App->buff->initBuff2);

    return true;
}

update_status ModuleGuiManager::PreUpdate(float dt)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    return UPDATE_CONTINUE;
}

update_status ModuleGuiManager::Update(float dt)
{
    update_status status = UPDATE_CONTINUE;
    status = MenuBar();

    if (showcase)
    {
        ImGui::ShowDemoWindow();
        ImGui::ShowMetricsWindow();
    }
    if (configOn) Config();
    if (consoleOn) Console();
    //if (aboutOn) About();

    return status;
}

update_status ModuleGuiManager::PostUpdate(float dt)
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    return UPDATE_CONTINUE;
}

bool ModuleGuiManager::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    return true;
}

update_status ModuleGuiManager::MenuBar()
{

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Quit", "ESC"))
            {
                return UPDATE_STOP;
            }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Configuration", "F1", &configOn);
            ImGui::MenuItem("Console", "F12", &consoleOn);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            ImGui::MenuItem("Gui Demo", nullptr, &showcase);

            if (ImGui::MenuItem("Documentation"))
            {
                App->RequestBrowser("https://github.com/TitoLuce/Untitled-Engine/wiki");
            }            

            if (ImGui::MenuItem("Download latest"))
            {
                App->RequestBrowser("https://github.com/TitoLuce/Untitled-Engine/releases");
            }

            if (ImGui::MenuItem("Report a bug"))
            {
                App->RequestBrowser("https://github.com/TitoLuce/Untitled-Engine/issues");
            }

            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    return UPDATE_CONTINUE;
}

void ModuleGuiManager::Config()
{
    UpdateFPS();

    ImGui::Begin("Configuration");
    if (ImGui::BeginMenu("Options"))
    {
        if (ImGui::MenuItem("Set Defaults"))
        {

        }
        if (ImGui::MenuItem("Save"))
        {

        }
        if (ImGui::MenuItem("Load"))
        {

        }

        ImGui::EndMenu();
    }

    if (ImGui::CollapsingHeader("Application"))
    {
        static char appName[120];
        strcpy_s(appName, 120, App->GetAppName());
        if (ImGui::InputText("App Name", appName, IM_ARRAYSIZE(appName), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            App->SetAppName(appName);
        }

        static char orgName[120];
        strcpy_s(orgName, 120, App->GetOrgName());
        if (ImGui::InputText("Organization", orgName, IM_ARRAYSIZE(orgName), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            App->SetOrgName(orgName);
        }
        ImGui::Separator();

        // Framerate

        maxFPS = App->GetMaxFps();
        if (ImGui::SliderInt("Max FPS", &maxFPS, 1, 60))
        {
            App->SetMaxFps(maxFPS);
        }

        ImGui::Text("Limit Framerate:");
        ImGui::SameLine();
        std::string t = std::to_string(App->GetMaxFps());
        const char* t2 = t.c_str();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
        ImGui::Text(t2);
        ImGui::PopStyleColor();


        ImGui::Text("Average Framerate:");
        ImGui::SameLine();
        t = std::to_string(averageFps);
        t2 = t.c_str();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
        ImGui::Text(t2);
        ImGui::PopStyleColor();


        std::string tempTitle = "Framerate ";
        tempTitle += std::to_string((int)App->GetFps());
        ImGui::PlotHistogram("##framerate", &fpsLog[0], fpsLog.size(), 0, tempTitle.c_str(), 0, 100, ImVec2(310, 100));

        tempTitle = "Milliseconds ";
        tempTitle += std::to_string((int)App->GetMs());
        ImGui::PlotHistogram("##milliseconds", &msLog[0], msLog.size(), 0, tempTitle.c_str(), 0.0f, 40.0f, ImVec2(310, 100));
    }

    if (ImGui::CollapsingHeader("Window"))
    {
        float bright = App->window->GetBrightness();
        if (ImGui::SliderFloat("Brightness", &bright, 0.05f, 1.0f))
        {
            App->window->SetBrightness(bright);
        }

        //Currently, if they are resized using the window, the values will not update
        int w = App->window->GetWidth();
        int h = App->window->GetHeight();
        int maxW, maxH;
        App->window->GetMaxWindow(maxW, maxH);
        if (ImGui::SliderInt("Width", &w, 640, maxW))
        {
            App->window->SetWidth(w);
        }

        if (ImGui::SliderInt("Height", &h, 480, maxH))
        {
            App->window->SetHeight(h);
        }

        ImGui::Text("Refresh rate:");
        ImGui::SameLine();
        ImGui::TextColored(IMGUI_YELLOW, "%u Hz", App->window->GetRefreshRate());

        bool fullscreen = App->window->GetFullscreen();
        if (ImGui::Checkbox("Fullscreen", &fullscreen))
        {
            App->window->SetFullscreen(fullscreen);
        }

        ImGui::SameLine();

        bool resizable = App->window->GetResizable();
        if (ImGui::Checkbox("Resizable", &resizable))
        {
            //This is useless as long
            App->window->SetResizable(resizable);
        }

        bool borderless = App->window->GetBorderless();
        if (ImGui::Checkbox("Borderless", &borderless))
        {
            App->window->SetBorderless(borderless);
        }

        ImGui::SameLine();

        bool fullscreenDesktop = App->window->GetFullscreenDesktop();
        if (ImGui::Checkbox("Fullscreen Desktop", &fullscreenDesktop))
        {
            App->window->SetFullscreenDesktop(fullscreenDesktop);
        }
    }

    //Currently does nothing
    if (ImGui::CollapsingHeader("Render"))
    {
        bool wireframe = App->renderer3D->GetWireframe();
        if (ImGui::Checkbox("Wireframe", &wireframe))
        {
            App->renderer3D->ToggleWireframe();
        }
    }
    ImGui::End();
}

void ModuleGuiManager::Console()
{
    ImGui::Begin("Console");
    ImGui::TextUnformatted(LogConsoleText.begin());
    ImGui::SetScrollHereY(1.0f);
    ImGui::End();
}

void ModuleGuiManager::LogConsole(const char* buff)
{
    LogConsoleText.appendf(buff);
}


void ModuleGuiManager::UpdateFPS()
{
    static int fpsAdd = 0;

    if (fpsCounter < 100)
    {
        fpsCounter++;
    }
    else
    {
        int i = 0;

        while (i < 99)
        {
            fpsLog[i] = fpsLog[i + 1];
            msLog[i] = msLog[i + 1];
            fpsAdd += fpsLog[i];
            i++;
        }
    }

    fpsLog[fpsCounter - 1] = App->GetFps();
    msLog[fpsCounter - 1] = App->GetMs();

    if (fpsCounter == 100)
    {
        averageFps = fpsAdd / 100;
        fpsAdd = 0;
    }
}