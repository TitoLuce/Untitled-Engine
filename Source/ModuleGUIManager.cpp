#include "ModuleGuiManager.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"

ModuleGuiManager::ModuleGuiManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

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

    return true;
}

update_status ModuleGuiManager::PreUpdate()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(App->window->window);
    ImGui::NewFrame();

    return UPDATE_CONTINUE;
}

update_status ModuleGuiManager::Update()
{
    /*ImGui::Begin("Quit");
    if (ImGui::Button("Quit", ImVec2(50.0f, 25.0f)))
    {
        status = UPDATE_ERROR;
    }*/
    //ImGui::Begin("Menu");
    //if (ImGui::BeginMenu("Help"))
    //{
    //    if (ImGui::MenuItem("About")) {
    //    }

    //    ImGui::EndMenu();
    //}
    //if (ImGui::BeginMenu("Quit"))
    //{
    //    if (ImGui::Button("Quit", ImVec2(50.0f, 25.0f)))
    //    {
    //        status = UPDATE_ERROR;
    //    }
    //    ImGui::EndMenu();
    //}
    //ImGui::Begin("Config");
    //if (ImGui::Checkbox("Fullscreen", &fullScreen))
    //{
    //    //App->window->fullScreen
    //}
    //ImGui::SameLine();
    //if (ImGui::Checkbox("Resizable", &resizable))
    //{

    //}
    //if (ImGui::Checkbox("Borderless", &borderless))
    //{

    //}
    //ImGui::SameLine();
    //if (ImGui::Checkbox("Full Desktop", &fullDesktop))
    //{

    //}
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

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help"))
        {
            if (ImGui::MenuItem("Gui Demo"))
            {
                showcase = !showcase;
            }

            if (ImGui::MenuItem("Documentation"))
                App->RequestBrowser("https://github.com/TitoLuce/Untitled-Engine/wiki");

            if (ImGui::MenuItem("Download latest"))
                App->RequestBrowser("https://github.com/TitoLuce/Untitled-Engine/releases");

            if (ImGui::MenuItem("Report a bug"))
                App->RequestBrowser("https://github.com/TitoLuce/Untitled-Engine/issues");

            ImGui::EndMenu();
        }
    }
    ImGui::EndMainMenuBar();

    if (showcase)
    {
        ImGui::ShowDemoWindow();
        ImGui::ShowMetricsWindow();
    }

    return status;
}

update_status ModuleGuiManager::PostUpdate()
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