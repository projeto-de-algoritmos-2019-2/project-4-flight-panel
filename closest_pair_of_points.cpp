#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

struct point {
    double x, y;
};

using pair = std::pair<point, point>;

double square_distance(const point& lhs, const point& rhs) {
    double adj = std::abs(lhs.x - rhs.x);
    double opp = std::abs(lhs.y - rhs.y);
    return adj*adj + opp*opp;
}

using Iterator = std::vector<point>::iterator;

double minimal_distance_naive(Iterator first, Iterator last) {
    
    pair closest{*first, *std::next(first)};

    for (auto out = first; out != last; ++out) {
        
        pair temp;
        temp.first = *out;

        for (auto in = std::next(out); in != last; ++in) {
            
            temp.second = *in;

            closest = std::min(
                closest, 
                temp, 
                [](const auto& lhs, const auto& rhs) {
                    return square_distance(lhs.first, lhs.second) < square_distance(rhs.first, rhs.second);
                }
            );
        }
    }
    
    return square_distance(closest.first, closest.second);
}

bool is_inside_rectangle(const point& input, const point& up_left, const point& bottom_right) {
    
    return 
        up_left.x <= input.x && input.x <= bottom_right.x && 
        up_left.y >= input.y && input.y >= bottom_right.y;
}

pair candidates_rectangle(const point& p, double square_dist) {
    double dist = std::sqrt(square_dist);

    pair res{
        { p.x, static_cast<double>(p.y + dist) },
        { static_cast<double>(p.x + dist),  static_cast<double>(p.y - dist) }
    };

    return res;
}


double minimal_distance_rec(Iterator first, Iterator last, const std::size_t threshold = 3) {
    
    if (std::distance(first, last) <= threshold) 
        return minimal_distance_naive(first, last);

    auto pivot = std::next(first, std::distance(first, last) / 2);

    auto min_left = minimal_distance_rec(first, pivot);
    auto min_right = minimal_distance_rec(pivot, last);

    auto temp_min = std::min(min_left, min_right);

    // define the band inside which distance can be less than temp_min
    auto not_too_left = std::partition(
        first, 
        pivot, 
        [&](const auto& p) {
            return p.x < (pivot->x - static_cast<double>(std::sqrt(temp_min)));
        }
    );

    auto not_too_right = std::partition(
        pivot, 
        last, 
        [&](const auto& p) {
            return p.x <= (pivot-> x + static_cast<double>(std::sqrt(temp_min)));
        }
    );

    // and look for the closest pair inside
    std::for_each(not_too_left, pivot, [&](const auto& lp) {
        
        auto [up_left, bottom_right] = candidates_rectangle(lp, std::sqrt(temp_min));

        auto outside = std::partition(
            pivot, 
            not_too_right, 
            [=](const auto& rp) {
                return !is_inside_rectangle(rp, up_left, bottom_right);
            }
        );

        auto middle_closest = std::min_element(
            pivot, 
            outside, 
            [=](const auto& lhs, const auto& rhs) {
                return square_distance(lp, lhs) < square_distance(lp, rhs);
            }
        );

        temp_min = std::min(temp_min, square_distance(lp, *middle_closest));
    });

    return temp_min;
}

double minimal_distance(std::vector<point>& points) {
    
    std::sort(
        points.begin(), 
        points.end(), 
        [](const auto& lhs, const auto& rhs) {
            return lhs.x < rhs.x;
        }
    );

    return std::sqrt(minimal_distance_rec(points.begin(), points.end()));
}

int main() {

    int qnt;
    std::cin >> qnt;

    std::vector<point> points(qnt);

    for(auto &[x,y] : points)
        std::cin >> x >> y;

    double dist = minimal_distance(points);
    
    std::cout << "min dist = " << dist << std::endl;
}