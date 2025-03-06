
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

namespace nlohmann {
    template <>
    struct adl_serializer<sphere> {
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

        static sphere from_json(const json& j) {
            
            // Deserialize center
            point3 center = point3(j.at("center").get<vector<float>>());

            // Deserialize radius
            double radius = j.at("radius");

            // Get material type and lookup in map
            string mat_str = j.at("material").get<string>();
            shared_ptr<material> mat;
            const auto& material_map = get_material_map();
            auto it = material_map.find(mat_str);
            if (it != material_map.end()) {
                mat = it->second(j);
            }

            return {center, radius, mat};
        }

        static void to_json(json& j, const sphere& s) {
            j["center"] = { s.center.x(), s.center.y(), s.center.z() };
            j["radius"] = s.radius;

            // Determine material type
            json material_json;
            if (auto mat = std::dynamic_pointer_cast<lambertian>(s.mat)) {
                material_json["material"] = "lambertian";
                material_json["color_args"] = { mat->albedo.x(), mat->albedo.y(), mat->albedo.z() };
            } 
            else if (auto mat = std::dynamic_pointer_cast<metal>(s.mat)) {
                material_json["material"] = "metal";
                material_json["color_args"] = { mat->albedo.x(), mat->albedo.y(), mat->albedo.z() };
                material_json["metal_fuzz"] = mat->fuzz;
            } 
            else if (auto mat = std::dynamic_pointer_cast<dielectric>(s.mat)) {
                material_json["material"] = "dielectric";
                material_json["dielectric_refraction_index"] = mat->refraction_index;
            } 
            else {
                throw std::runtime_error("Unknown material type in serialization.");
            }

            j.update(material_json); // Merge material info into main JSON object
        }

        
    };
}


int main(int argc, char* argv[]) {
    hittable_list world;

    // Using (raw) string literals and json::parse
    json scene_json = json::parse(R"(
        {
            "center": [0.0, 0.0, -1.2],
            "radius": 1,
            "material" : "lambertian",
            "color_args" : [0,8, 0,8, 0.0],
            "dielectric_refraction_index" : [],
            "metal_fuzz" : []
        }
    )");


    auto spherical = scene_json.template get<sphere>();


    auto material_json = make_shared<lambertian>(color(scene_json["position"].template get<std::vector<float>>()));

    

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.50);
    // auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    // world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

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
    // cam.render(world,argc,argv);
        
}