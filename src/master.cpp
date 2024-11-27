#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <crow.h>
#include <crow/json.h>
#include <string>
#include <iostream>
#include <utility>
#include "utils.h"
#include "circle.h"
#include <atomic>
#include <array>
#include <cstdio>

std::atomic<bool> running(true);

std::string exec(const char* cmd) {
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string generate_id(int length) {
    auto randchar = []() -> char {
        const std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (charset.size() - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), 4, randchar );
    return str;
}

struct Blob : Int2 {
    int radius;
    Blob(int X, int Y, int SIZE) :
        Int2(X, Y),
        radius(SIZE)
    {}
    explicit Blob(crow::json::wvalue json) :
        Int2(0, 0),
        radius(0)
    {
        this->x = std::stoi(json["x"].dump());
        this->y = std::stoi(json["y"].dump());
        this->radius = std::stoi(json["radius"].dump());
    }
    Blob(crow::json::wvalue json, const std::string& name) :
        Int2(0, 0),
        radius(0)
    {
        crow::json::wvalue blob_data = json[name];
        this->x = std::stoi(blob_data["x"].dump());
        this->y = std::stoi(blob_data["y"].dump());
        this->radius = std::stoi(blob_data["radius"].dump());
    }
    void draw(SDL_Renderer* renderer) {
        SDL_RenderDrawCircle(renderer, this->x, this->y, this->radius);
    }
    void update(int x, int y, int radius) {
        this->x = x;
        this->y = y;
        this->radius = radius;
    }
};
struct Player : Blob {
    int des_x, des_y;
    std::string user;
    std::string pass;
    explicit Player(int X = 0, int Y = 0, int RADIUS = 0, int DES_X = 0, int DES_Y = 0, std::string USER = "", std::string PASS = "") :
        Blob(X, Y, RADIUS),
        des_x(DES_X),
        des_y(DES_Y),
        user(std::move(USER)),
        pass(std::move(PASS))
    {}
    Player(const crow::json::wvalue& json, std::string USER, std::string PASS) :
        Blob(0, 0, 0),
        des_x(0),
        des_y(0),
        user(std::move(USER)),
        pass(std::move(PASS))
    {

    }
    void draw(SDL_Renderer* renderer) {
        SDL_RenderFillCircle(renderer, this->x, this->y, this->radius);
    }
};

struct Data {
    int x{}, y{}, speed{};
    std::vector<Blob> others;
    Player player;
    Data(crow::json::wvalue json, const std::string& player_name, const std::string& player_pass) : others({}) {
        this->x = std::stoi(json["MAX_X"].dump());
        this->y = std::stoi(json["MAX_Y"].dump());
        this->speed = std::stoi(json["BLOB_SPEED"].dump());
        crow::json::wvalue blobJson = json["BLOBS"];
        auto keys = blobJson.keys();
        for (const std::string& key : keys) {
            if (key == player_name) {
                continue;
            }
            const Blob blob = Blob(blobJson[key], key);
            this->others.push_back(blob);
        }
        this->player = Player(json[player_name], player_name, player_pass);
    }
    void update(crow::json::wvalue json) {
        const std::string& player_name = this->player.user;
        const std::string& player_pass = this->player.pass;
        this->x = std::stoi(json["MAX_X"].dump());
        this->y = std::stoi(json["MAX_Y"].dump());
        this->speed = std::stoi(json["BLOB_SPEED"].dump());
        crow::json::wvalue blobJson = json["BLOBS"];
        auto keys = blobJson.keys();
        for (const std::string& key : keys) {
            if (key == player_name) {
                continue;
            }
            const Blob blob = Blob(blobJson[key], key);
            this->others.push_back(blob);
        }
        this->player = Player(json[player_name], player_name, player_pass);
    }
};

std::string send_url(const std::string& url) {
    return exec(("curl -s \"" + url + "\"").c_str());
}

int main() {
    const std::string base_url = "localhost:8080";
    Int2 screenSize = {0, 0};
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("An overengineered children's game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        screenSize.x, screenSize.y,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    const std::string name = "gamer_" + generate_id(10);
    const std::string pass = generate_id(15);
    std::string url = base_url + "/?name=" + name + "&password=" + pass;
    crow::json::wvalue json = send_url(url);
    Data data = Data(json, name, pass);
    SDL_Event event;
    Blob blob = Blob(40, 40, 20);
    while (running) {
        url = base_url + "/?name=" +
              name.c_str() + "&password=" +
              pass.c_str() +
              "&x=" + std::to_string(data.player.x) +
              "&y=" + std::to_string(data.player.y);
        json = crow::json::wvalue(send_url(url));
        data.update(json);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        blob.draw(renderer);
    }
}
