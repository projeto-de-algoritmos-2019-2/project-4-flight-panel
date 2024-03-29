#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>

#define EPS 1e-9

struct point {
    double x, y;

    bool operator==(point other) const {
        return (
            std::fabs(x-other.x) < EPS && 
            std::fabs(y-other.y) < EPS
        );
    }
};

struct closest_points
{
    double dist;
    point A, B;
};

double min_dist = 90;
double min_square_dist = min_dist * min_dist;

using pair = std::pair<point, point>;

bool equal_pair(pair Pa, pair Pb) {
    return (
        Pa.first == Pb.first && 
        Pa.second == Pb.second
    );
}

double square_distance(const point& lhs, const point& rhs) {
    double adj = std::abs(lhs.x - rhs.x);
    double opp = std::abs(lhs.y - rhs.y);
    return adj*adj + opp*opp;
}

using Iterator = std::vector<point>::iterator;

std::vector<pair> answer;
void check_dist(const point& lhs, const point& rhs) {

    double d = square_distance(lhs, rhs);
    
    if(d <= min_square_dist) {
        answer.push_back({lhs, rhs});
    }
}

closest_points minimal_distance_naive(Iterator first, Iterator last) {
    
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
                    
                    check_dist(lhs.first, lhs.second);
                    check_dist(rhs.first, rhs.second);

                    return square_distance(lhs.first, lhs.second) < square_distance(rhs.first, rhs.second);
                }
            );
        }
    }

    closest_points cp;

    cp.dist = square_distance(closest.first, closest.second);
    cp.A = closest.first;
    cp.B = closest.second;
    
    return cp;
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


closest_points minimal_distance_rec(Iterator first, Iterator last, const std::size_t threshold = 3) {
    
    if (std::distance(first, last) <= threshold)
        return minimal_distance_naive(first, last);

    auto pivot = std::next(first, std::distance(first, last) / 2);

    auto cp_min_left = minimal_distance_rec(first, pivot);
    auto min_left = cp_min_left.dist;

    auto cp_min_right = minimal_distance_rec(pivot, last);
    auto min_right = cp_min_right.dist;

    closest_points cp;
    double temp_min;

    if(min_left < min_right) {
        temp_min = min_left;
        cp = cp_min_left;
    }

    else {
        temp_min = min_right;
        cp = cp_min_right;
    }

    // auto temp_min = std::min(min_left, min_right);

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

                check_dist(lp, lhs);
                check_dist(lp, rhs);
                check_dist(lhs, rhs);

                return square_distance(lp, lhs) < square_distance(lp, rhs);
            }
        );

        double sd = square_distance(lp, *middle_closest);

        if(sd < temp_min) {
            temp_min = sd;
            cp.A = lp;
            cp.B = *middle_closest;
        }
    });

    return cp;
}

closest_points minimal_distance(std::vector<point>& points) {
    
    std::sort(
        points.begin(), 
        points.end(), 
        [](const auto& lhs, const auto& rhs) {
            return lhs.x < rhs.x;
        }
    );

    closest_points cp = minimal_distance_naive(points.begin(), points.end());
    cp.dist = std::sqrt(cp.dist);

    return cp;
}

int main() {

    int qnt;
    std::cin >> qnt;

    std::vector<point> points(qnt);

    for(auto &[x,y] : points)
        std::cin >> x >> y;

    closest_points cp = minimal_distance(points);

    double dist = cp.dist;
    
    // std::cout << dist << std::endl;
    std::cout << answer.size() << std::endl;
    // std::cout << cp.A.x  << " " << cp.A.y << std::endl;
    // std::cout << cp.B.x  << " " << cp.B.y << std::endl;

    for(auto&[p1, p2] : answer) {

        double dd = std::sqrt(square_distance(p1, p2));

        std::cout << p1.x << " " << p1.y << " " << p2.x << " " << p2.y << " " <<  dd << std::endl;
    }

    return 0;
}