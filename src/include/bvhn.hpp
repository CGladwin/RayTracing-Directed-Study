#pragma once

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include <algorithm>

/**
 * @brief A node in a Bounding Volume Hierarchy (BVH) used for efficient ray intersection
 * 
 */
class bvh_node : public hittable {
  public:
    /**
     * @brief Constructs a BVH node from a list of hittable objects.
     * @param list The list of hittable objects to build the BVH from.
     */
    bvh_node(hittable_list list) : bvh_node(list.objects, 0, list.objects.size()) {
        // Implicitly copies the hittable list, which is modified during BVH construction.
        // The copied list's lifetime is limited to this constructor.
    }

    /**
     * @brief Constructs a BVH node for a subset of hittable objects.
     * @param objects The list of hittable objects.
     * @param start The starting index of the subset.
     * @param end The ending index of the subset.
     */
    bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end) {
        // Compute the bounding box for the subset of objects.
        bbox = aabb::empty;
        for (size_t object_index = start; object_index < end; object_index++)
            bbox = aabb(bbox, objects[object_index]->bounding_box());

        // Determine the longest axis of the bounding box for sorting.
        int axis = bbox.longest_axis();

        // Choose the appropriate comparator based on the longest axis.
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                                      : box_z_compare;

        size_t object_span = end - start;

        // Handle cases with 1 or 2 objects directly.
        if (object_span == 1) {
            left = right = objects[start];
        } else if (object_span == 2) {
            left = objects[start];
            right = objects[start + 1];
        } else {
            // Sort objects along the longest axis and recursively build the BVH.
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

            auto mid = start + object_span / 2;
            left = make_shared<bvh_node>(objects, start, mid);
            right = make_shared<bvh_node>(objects, mid, end);
        }
    }

    /**
     * @brief Checks if a ray intersects with the BVH node.
     * @param r The ray to test for intersection.
     * @param ray_t The interval of valid ray distances.
     * @param rec The hit record to store intersection details.
     * @return True if the ray intersects the BVH node, false otherwise.
     */
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        if (!bbox.hit(r, ray_t))
            return false;

        // Check intersections with left and right child nodes.
        bool hit_left = left->hit(r, ray_t, rec);
        bool hit_right = right->hit(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

        return hit_left || hit_right;
    }

    /**
     * @brief Returns the bounding box of the BVH node.
     * @return The axis-aligned bounding box (AABB) of the node.
     */
    aabb bounding_box() const override { return bbox; }

  private:
    shared_ptr<hittable> left;  ///< Left child node.
    shared_ptr<hittable> right; ///< Right child node.
    aabb bbox; ///< Bounding box of the node.

    /**
     * @brief Compares two hittable objects along a specified axis.
     * @param a The first hittable object.
     * @param b The second hittable object.
     * @param axis_index The axis to compare along (0 = x, 1 = y, 2 = z).
     * @return True if a's bounding box min is less than b's along the axis, false otherwise.
     */
    static bool box_compare(
        const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index
    ) {
        auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
        auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
        return a_axis_interval.min < b_axis_interval.min;
    }

    /// Comparator for sorting objects along the x-axis.
    static bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 0);
    }

    /// Comparator for sorting objects along the y-axis.
    static bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 1);
    }

    /// Comparator for sorting objects along the z-axis.
    static bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 2);
    }
};