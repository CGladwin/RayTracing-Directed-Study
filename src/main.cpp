
#include "include/hittable.hpp"
#include "include/hittable_list.hpp"
#include "include/sphere.hpp"
#include "include/camera.hpp"
#include "include/material.hpp"
#include "include/external/json.hpp"
#include <unordered_map>
using json = nlohmann::json;
using std::unordered_map;
using std::shared_ptr;
using std::vector;

// lambda function
using material_func = std::function<shared_ptr<material>(const json&)>;

// Function to get a reference to the static material map
static const unordered_map<string, material_func>& get_material_map() {
    static const unordered_map<string, material_func> material_map = {
        { "lambertian", [](const json& j) -> shared_ptr<material> {
            auto col = j["color_args"].get<vector<float>>();
            return std::make_shared<lambertian>(color(col[0], col[1], col[2]));
        }},
        { "metal", [](const json& j) -> shared_ptr<material> {
            auto col = j["color_args"].get<vector<float>>();
            double fuzz = j["metal_fuzz"].get<double>();
            return std::make_shared<metal>(color(col[0], col[1], col[2]), fuzz);
        }},
        { "dielectric", [](const json& j) -> shared_ptr<material> {
            double ref_idx = j["dielectric_refraction_index"].get<double>();
            return std::make_shared<dielectric>(ref_idx);
        }}
    };
    return material_map;
}

void from_json(const json& j, sphere& s) {
    
    // Deserialize center
    s.center = point3(j.at("center").get<vector<float>>());

    // Deserialize radius
    j.at("radius").get_to(s.radius);

    // Get material type and lookup in map
    string mat_str = j.at("material").get<string>();
    shared_ptr<material> mat;
    const auto& material_map = get_material_map();
    auto it = material_map.find(mat_str);
    if (it != material_map.end()) {
        mat = it->second(j);
    }
    s.mat = mat;
    
}


int main(int argc, char* argv[]) {
    hittable_list world;

    // Using (raw) string literals and json::parse
    json scene_json = json::parse(R"(
        {
            "center": [1.0, 0.0, -1.0],
            "radius": 0.5,
            "material" : "metal",
            "color_args" : [0.8, 0.6, 0.2],
            "dielectric_refraction_index" : [],
            "metal_fuzz" : 1.0
        }
    )");


    auto spherical = scene_json.template get<sphere>();
    cout << "Sphere after deserialization:\n";
    cout << "  Center: (" << spherical.center.x() << ", " 
                         << spherical.center.y() << ", " 
                         << spherical.center.z() << ")\n";
    cout << "  Radius: " << spherical.radius << "\n";
    cout << "  Material: ";
    if (auto mat = std::dynamic_pointer_cast<lambertian>(spherical.mat)) {
        cout << "Lambertian with albedo (" << mat->albedo.x() << ", " 
                                          << mat->albedo.y() << ", " 
                                          << mat->albedo.z() << ")\n";
    } else if (auto mat = std::dynamic_pointer_cast<metal>(spherical.mat)) {
        cout << "Metal with albedo (" << mat->albedo.x() << ", " 
                                     << mat->albedo.y() << ", " 
                                     << mat->albedo.z() << ") and fuzz " 
                                     << mat->fuzz << "\n";
    } else if (auto mat = std::dynamic_pointer_cast<dielectric>(spherical.mat)) {
        cout << "Dielectric with refraction index " << mat->refraction_index << "\n";
    } else {
        cout << "Unknown material type\n";
    }

    

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    // auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    // world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
    world.add(make_shared<sphere>(spherical.center,spherical.radius,spherical.mat));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;


    cam.vfov     = 20;
    cam.lookfrom = point3(-2,2,1);
    cam.lookat   = point3(0,0,-1);
    cam.vup      = vec3(0,1,0);
    cam.defocus_angle = 10.0;
    cam.focus_dist    = 3.4;
    cam.render(world,argc,argv);
        
}