#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <string>
#include <vector>

#include "ryi.h"
#include "button.h"
#include "popupmenu.h"

#include "tinyfiledialogs.h"
#include "build.h"

// Configuration management structure
struct AppConfig {
    int gridStep = 20;
    int defaultMode = 2; // 1: SCALE, 2: CENTERED
} config;

// Handles configuration file I/O operations
void handleConfig() {
    const char* fileName = "config.txt";
    std::ifstream infile(fileName);
    
    if (!infile.good()) {
        std::ofstream outfile(fileName);
        if (outfile.is_open()) {
            outfile << "gridStep=20\n";
            outfile << "defaultMode=2\n";
            outfile.close();
        }
    } else {
        std::string line;
        while (std::getline(infile, line)) {
            if (line.find("gridStep=") == 0) {
                try { config.gridStep = std::stoi(line.substr(9)); } catch(...) {}
            } else if (line.find("defaultMode=") == 0) {
                try { config.defaultMode = std::stoi(line.substr(12)); } catch(...) {}
            }
        }
        infile.close();
    }
}

void print_usage() {
    printf("ryi [options]\n");
    printf("options:\n");
    printf("\t<dir>       \t- The directory to load images from (Optional)\n");
    printf("\t-h          \t- Print this help information\n");
    printf("\n");
    printf("examples:\n");
    printf("\tryi           \t- Running without args reads images in current folder\n");
    printf("\tryi images    \t- Loads all images in specified dir\n");
}

int main(int argc, char *argv[]) {
    // Load configuration settings before initialization
    handleConfig();

    char* flag = (char*)(argc - 1 == 0 ? "." : argv[argc - 1]);

    if (strcmp(flag, "-h") == 0) {
        print_usage();
        return 1;
    }

    Ryi::init(flag);
    
    // Apply default mode from config
    Ryi::image_mode = (ImageMode)config.defaultMode;
    
    auto images = Ryi::images();

    // Navigation logic
    auto goLeft = [&images]() {
        if (images.size() == 0) return;
        if (Ryi::image_index <= 0)
            Ryi::image_index = images.size() - 1;
        else Ryi::image_index--;
    };

    auto goRight = [&images]() {
        if (images.size() == 0) return;
        Ryi::image_index++;
        Ryi::image_index %= images.size();
    };

    // UI Elements setup using Fluent Interface
    auto seekLeft = (new Button)
        ->text("<<")
        ->font_size(20)
        ->bg_color(GetColor(0x181818ff))
        ->fg_color(RED)
        ->position(30, GetScreenHeight() / 2)
        ->size(30, 35)
        ->on_click(goLeft);

    auto seekRight = (new Button)
        ->text(">>")
        ->font_size(20)
        ->bg_color(GetColor(0x181818ff))
        ->fg_color(RED)
        ->position(GetScreenWidth() - seekLeft->position().x - 30 * 2, GetScreenHeight() / 2)
        ->size(30, 35)
        ->on_click(goRight);

    auto okButton = (new Button)
        ->text("ok")
        ->font_size(20)
        ->bg_color(GetColor(0x181818ff))
        ->fg_color(RED)
        ->on_click([]() {
            Ryi::show_about = false;
        });

    auto popupMenu = (new PopUpMenu)
        ->rect({0,0,20,0});

    // PopUp Menu Items
    popupMenu->menu_item("Open Dir", [&images]() {
        char* selected_path = tinyfd_selectFolderDialog("Open Images Folder", NULL);
        if (selected_path == nullptr) return;

        if (images.size() > 0) {
            for (auto& image: images) {
                UnloadTexture(image.image);
                free(image.path);
            }
        }
        images.clear();
        Ryi::load_images(selected_path);
        images = Ryi::images();
        Ryi::image_index = 0;
    });

    popupMenu->separator();
    popupMenu->menu_item("Scale Image", []() { Ryi::image_mode = ImageMode::SCALE; });
    popupMenu->menu_item("Center Image", []() { Ryi::image_mode = ImageMode::CENTERED; });

    popupMenu->separator();
    popupMenu->menu_item("Go Left (<<)", goLeft);
    popupMenu->menu_item("Go Right (>>)", goRight);

    float rotation_factor = 10.0f; 
    popupMenu->separator();
    popupMenu->menu_item("Rotate +10deg", [rotation_factor]() {
        Ryi::rotation += rotation_factor;
        Ryi::rotation = (float)((int)Ryi::rotation % 360);
    });
    popupMenu->menu_item("Reset Rotation", []() { Ryi::rotation = 0; });

    popupMenu->separator();
    // Integrated Config File Opening
    popupMenu->menu_item("Configure", []() {
        Ryi::open_app_from_url("config.txt");
    });

    popupMenu->separator();
    popupMenu->menu_item("Zoom In (+)", []() { Ryi::scale_factor += 0.1f; });
    popupMenu->menu_item("Zoom Out (-)", []() { Ryi::scale_factor -= 0.1f; });
    popupMenu->menu_item("Reset Zoom", []() { Ryi::scale_factor = 1.0f; });

    popupMenu->separator();
    popupMenu->menu_item("Toggle Grid View", []() { Ryi::grid_view = !Ryi::grid_view; });

    popupMenu->separator();
    popupMenu->menu_item("Help", []() { Ryi::open_app_from_url(__GIT_REPO__); });

    popupMenu->menu_item("About", []() {
        Ryi::show_about = true;
        Ryi::scroll_y = GetScreenHeight();
    });

    popupMenu->separator();
    popupMenu->menu_item("Exit", []() { Ryi::is_running = false; });

    // Main Application Loop
    while (Ryi::is_running && !WindowShouldClose()) {
        auto dt = GetFrameTime();
        auto w = GetScreenWidth();
        auto h = GetScreenHeight();

        // Update UI logic based on current view
        if (!Ryi::grid_view) {
            seekLeft->update();
            seekRight->update();
            *seekRight->x() = (float)w - seekLeft->position().x - 30 * 2;
            *seekRight->y() = (float)h / 2;
            *seekLeft->y() = (float)h / 2;
        }

        if (Ryi::show_about) {
            *okButton->x() = Ryi::dialog_rect.x + Ryi::dialog_rect.width / 2;
            *okButton->y() = Ryi::dialog_rect.y + Ryi::dialog_rect.height - 40;
            okButton->size(30, 20);
            okButton->update();
        }

        if (IsKeyPressed(KEY_LEFT)) goLeft();
        if (IsKeyPressed(KEY_RIGHT)) goRight();

        popupMenu->update();
        Ryi::scale_factor += GetMouseWheelMove() * dt;

        BeginDrawing();
        {
            // Note: Ryi::draw_background should be updated in ryi.cpp 
            // to use config.gridStep for the checkerboard pattern
            Ryi::draw_background(); 

            if (Ryi::show_about) {
                Ryi::draw_about();
                okButton->draw();
            } else if (Ryi::grid_view) {
                Ryi::draw_grid_view();
            } else {
                Ryi::draw_image_slide();
                seekLeft->draw();
                seekRight->draw();
            }
            
            popupMenu->draw();

            // Status Text Overlay
            DrawText(TextFormat("%d/%d", Ryi::image_index + 1, (int)images.size()), 5, 5, 13, BLACK);
            DrawText(TextFormat("%d/%d", Ryi::image_index + 1, (int)images.size()), 6, 6, 13, RED);
        }
        EndDrawing();
    }

    // Cleanup
    for (auto& image : images) {
        UnloadTexture(image.image);
        free(image.path);
    }

    delete seekLeft; delete seekRight; delete okButton; delete popupMenu;
    CloseWindow();

    return 0;
}
