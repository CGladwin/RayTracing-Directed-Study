
#include "include/hittable.hpp"
#include "include/hittable_list.hpp"
#include "include/sphere.hpp"
#include "include/camera.hpp"
#include "include/material.hpp"
#include "include/external/json.hpp"
#include "include/bvhn.hpp"
#include <unordered_map>
#include "include/quad.hpp"
#include "include/triangle.hpp"
using json = nlohmann::json;
using std::unordered_map;
using std::shared_ptr;
using std::vector;

// lambda function
using material_func = std::function<shared_ptr<material>(const json&)>;
using primitive_parser_func = std::function<shared_ptr<hittable>(const json&)>;

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

// Material parsing helper
shared_ptr<material> parse_material(const json& j) {
    string mat_str = j.at("material").get<string>();
    const auto& material_map = get_material_map();
    auto it = material_map.find(mat_str);
    if (it != material_map.end()) {
        return it->second(j);
    } else {
        throw std::runtime_error("Unknown material type: " + mat_str);
    }
}

// Function to get the static primitive parser map
// TODO: change triangles and quads to cubes and pyramids
static const unordered_map<string, primitive_parser_func>& get_primitive_map() {
    static const unordered_map<string, primitive_parser_func> primitive_map = {
        { "sphere", [](const json& j) -> shared_ptr<hittable> {
            point3 center(j.at("center").get<vector<float>>());
            double radius = j.at("radius").get<double>();
            auto mat = parse_material(j);
            return std::make_shared<sphere>(center, radius, mat);
        }},
        { "quad", [](const json& j) -> shared_ptr<hittable> {
            point3 Q(j.at("Q").get<vector<float>>());
            vec3 u(j.at("u").get<vector<float>>());
            vec3 v(j.at("v").get<vector<float>>());
            auto mat = parse_material(j);
            return std::make_shared<quad>(Q, u, v, mat);
        }},
        { "triangle", [](const json& j) -> shared_ptr<hittable> {
            point3 A(j.at("A").get<vector<float>>());
            point3 B(j.at("B").get<vector<float>>());
            point3 C(j.at("C").get<vector<float>>());
            auto mat = parse_material(j);
            return std::make_shared<triangle>(A, B, C, mat);
        }}
    };
    return primitive_map;
}

shared_ptr<sphere> from_json(const json& j) {
    // Deserialize center
    point3 center = point3(j.at("center").get<vector<float>>());

    // Deserialize radius
    double radius = j.at("radius").get<double>();

    // Get material type and lookup in map
    string mat_str = j.at("material").get<string>();
    shared_ptr<material> mat;
    const auto& material_map = get_material_map();
    auto it = material_map.find(mat_str);
    if (it != material_map.end()) {
        mat = it->second(j);
    }

    // Create and return a shared_ptr<sphere>
    return std::make_shared<sphere>(center, radius, mat);
}



void quads(int argc, char* argv[]) {
    hittable_list world;

    // Materials
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Quads
    world.add(make_shared<quad>(point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.add(make_shared<triangle>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.add(make_shared<quad>(point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.add(make_shared<quad>(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.add(make_shared<quad>(point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 80;
    cam.lookfrom = point3(0,0,9);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world,argc,argv);
}

void spheres_fov_json(int argc, char* argv[]) {
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


    auto spherical = from_json(scene_json);
    // cout << "Sphere after deserialization:\n";
    // cout << "  Center: (" << spherical.center.x() << ", " 
    //                      << spherical.center.y() << ", " 
    //                      << spherical.center.z() << ")\n";
    // cout << "  Radius: " << spherical.radius << "\n";
    // cout << "  Material: ";
    // if (auto mat = std::dynamic_pointer_cast<lambertian>(spherical.mat)) {
    //     cout << "Lambertian with albedo (" << mat->albedo.x() << ", " 
    //                                       << mat->albedo.y() << ", " 
    //                                       << mat->albedo.z() << ")\n";
    // } else if (auto mat = std::dynamic_pointer_cast<metal>(spherical.mat)) {
    //     cout << "Metal with albedo (" << mat->albedo.x() << ", " 
    //                                  << mat->albedo.y() << ", " 
    //                                  << mat->albedo.z() << ") and fuzz " 
    //                                  << mat->fuzz << "\n";
    // } else if (auto mat = std::dynamic_pointer_cast<dielectric>(spherical.mat)) {
    //     cout << "Dielectric with refraction index " << mat->refraction_index << "\n";
    // } else {
    //     cout << "Unknown material type\n";
    // }

    

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

    world.add(spherical);

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
    world = hittable_list(make_shared<bvh_node>(world));
    cam.render(world,argc,argv);
        
}

void pure_json_test(int argc, char* argv[]){
    hittable_list world;

    // Example JSON scene configuration
    json scene_json1 = json::parse(R"(
        {
            "primitives": [
                {
                    "type": "sphere",
                    "center": [0.0, -100.5, -1.0],
                    "radius": 100.0,
                    "material": "lambertian",
                    "color_args": [0.8, 0.8, 0.0]
                },
                {
                    "type": "sphere",
                    "center": [0.0, 0.0, -1.2],
                    "radius": 0.5,
                    "material": "lambertian",
                    "color_args": [0.1, 0.2, 0.5]
                },
                {
                    "type": "sphere",
                    "center": [-1.0, 0.0, -1.0],
                    "radius": 0.5,
                    "material": "dielectric",
                    "dielectric_refraction_index": 1.5
                },
                {
                    "type": "sphere",
                    "center": [1.0, 0.0, -1.0],
                    "radius": 0.5,
                    "material": "metal",
                    "color_args": [0.8, 0.6, 0.2],
                    "metal_fuzz": 1.0
                }
            ],
            "camera": {
                "aspect_ratio": 1.7777777777777777,
                "image_width": 400,
                "samples_per_pixel": 100,
                "max_depth": 50,
                "vfov": 20,
                "lookfrom": [-2, 2, 1],
                "lookat": [0, 0, -1],
                "vup": [0, 1, 0],
                "defocus_angle": 10.0,
                "focus_dist": 3.4
            }
        }
    )");
    std::ifstream ifs(argv[2]);
    json scene_json = json::parse(ifs);

    // Parse primitives
    const auto& primitive_map = get_primitive_map();
    for (const auto& prim_json : scene_json["primitives"]) {
        string type = prim_json["type"].get<string>();
        auto it = primitive_map.find(type);
        if (it != primitive_map.end()) {
            try {
                auto obj = it->second(prim_json);
                world.add(obj);
            } catch (const std::exception& e) {
                std::cerr << "Error parsing primitive: " << e.what() << std::endl;
            }
        } else {
            std::cerr << "Unknown primitive type: " << type << std::endl;
        }
    }

    // Parse camera settings
    camera cam;
    json cam_json = scene_json["camera"];
    cam.aspect_ratio      = cam_json["aspect_ratio"].get<double>();
    cam.image_width       = cam_json["image_width"].get<int>();
    cam.samples_per_pixel = cam_json["samples_per_pixel"].get<int>();
    cam.max_depth         = cam_json["max_depth"].get<int>();
    cam.vfov              = cam_json["vfov"].get<double>();
    cam.lookfrom          = point3(cam_json["lookfrom"].get<vector<float>>());
    cam.lookat            = point3(cam_json["lookat"].get<vector<float>>());
    cam.vup               = vec3(cam_json["vup"].get<vector<float>>());
    cam.defocus_angle     = cam_json["defocus_angle"].get<double>();
    cam.focus_dist        = cam_json["focus_dist"].get<double>();

    // Build BVH
    world = hittable_list(make_shared<bvh_node>(world));

    cam.render(world, argc, argv);
}

int main(int argc, char* argv[]) {
    pure_json_test(argc,argv);
}