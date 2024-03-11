#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <vector>
#include <cassert>
#include <numeric>
#include <cmath>
#include <list>
#include <queue>

struct Point {
    long double x;
    long double y;
    long double LengthSquared() const {
        return x * x + y * y;
    }
};

Point operator-(const Point& p1, const Point& p2) {
    return Point{.x = p1.x - p2.x, .y = p1.y - p2.y};
}


long double EuclidianDistanceSquared(const Point& p1, const Point& p2) {
    return (p1 - p2).LengthSquared();
}

long double EuclidianDistance(const Point& p1, const Point& p2) {
    return std::sqrt(static_cast<long double>(EuclidianDistanceSquared(p1, p2)));
}

long double GetDifference(const Point& path_previous, const Point& path_next, const Point& new_point) {
    auto current_distance = EuclidianDistance(path_previous, path_next);
    return EuclidianDistance(path_previous, new_point) + EuclidianDistanceSquared(new_point, path_next) - current_distance;
} 


double RandDouble(double fMin, double fMax) {
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

std::list<Point> TryGreedy(const std::vector<Point>& points) {
    std::list<Point> result;
    for (const auto& new_point : points) {
        // tmp
        if (result.empty()) {
            result.push_back(new_point);
            continue;
        }
        auto best_change = GetDifference(result.back(), result.front(), new_point);
        auto best_it = result.begin();
        for (auto it = result.begin(); std::next(it) != result.end(); ++it) {
            const auto new_change = GetDifference(*it, *std::next(it), new_point);
            if (new_change < best_change && RandDouble(0.0, 1.0) < 1.0 - 1.0 / result.size()) {
                best_change = new_change;
                best_it = std::next(it);
            }
        }
        result.insert(best_it, new_point);
    }
    return result;
}


template <typename T> 
long double EuclidianDistance(const T& points) {
    if (points.empty()) {
        assert(false);
        return 0;
    }
    auto result_distance = EuclidianDistance(points.back(), points.front());
    for (auto it = points.begin(); std::next(it) != points.end(); ++it) {
        result_distance += EuclidianDistance(*it, *std::next(it));
    }
    return result_distance;
}

size_t next_index(size_t index, const std::vector<Point>& best_result) {
    return index + 1 == best_result.size() ? 0 : index + 1; 
}

size_t previous_index(size_t index, const std::vector<Point>& best_result) {
    return index == 0 ? best_result.size() - 1 : index - 1;  
}

Point& get_previous(size_t index, std::vector<Point>& best_result) {
    return best_result[previous_index(index, best_result)];  
}

Point& get_next(size_t index, std::vector<Point>& best_result) {
    return best_result[next_index(index, best_result)];
}


struct Command {
    size_t i;
    size_t j;
};

struct Vertex {
    // new - old
    long double distance_dt;
    std::vector<Command> commands;
    bool operator<(const Vertex& other) const {
        return distance_dt < other.distance_dt;
    }
};

std::vector<Vertex> FindCommands(std::vector<Point>& best_result) {
    std::vector<Vertex> vertices;
    for (size_t i = 0; i < best_result.size(); ++i) {
        auto tmp_calc_distance = [&]() {
            return EuclidianDistance(get_previous(i, best_result), best_result[i]) + 
            EuclidianDistance(get_next(i, best_result), get_next(next_index(i, best_result), best_result));
        };
        auto current_distance = tmp_calc_distance();
        std::swap(best_result[i], get_next(i, best_result));
        auto new_distance = tmp_calc_distance();
        vertices.push_back(Vertex{.distance_dt=new_distance - current_distance, .commands = {{i, next_index(i, best_result)}}});
        std::swap(best_result[i], get_next(i, best_result));

        auto calc_distance = [&](size_t index1, size_t index2) {
            long double result = 0.0;
            for (auto index : {index1, index2}) {
                result += EuclidianDistance(get_previous(index, best_result), best_result[index]) + 
                    EuclidianDistance(best_result[index], get_next(index, best_result));
            }
            return result;
        };
        for (size_t j = i + 2; j < best_result.size(); ++j) {
            current_distance = calc_distance(i, j);
            std::swap(best_result[i], best_result[j]);
            new_distance = calc_distance(i, j);
            vertices.push_back(Vertex{.distance_dt=new_distance - current_distance, .commands = {{i, j}}});
            std::swap(best_result[i], best_result[j]);
        }
    }
    return vertices;
}

void BeamSearch(std::vector<Point>& best_result, const size_t elements_per_iterations, const size_t iterations_count) {
    std::priority_queue<Vertex> queue;
    queue.push(Vertex{.distance_dt = 0.0, .commands = {}});
    std::priority_queue<Vertex> next_queue;

    assert(elements_per_iterations > 0);
    for (size_t cycle = 0; cycle < iterations_count; ++cycle) {
        assert(queue.size() <= elements_per_iterations);
        while (!queue.empty()) {
            auto vertex = std::move(queue.top());
            queue.pop();
            for (const auto& command : vertex.commands) {
                std::swap(best_result[command.i], best_result[command.j]);
            }
            auto commands = FindCommands(best_result);
            if (commands.size() >= elements_per_iterations) {
                std::nth_element(commands.begin(), commands.begin() + elements_per_iterations - 1, commands.end());
            }
            std::sort(commands.begin(), commands.end());
            for (size_t i = 0; i < std::min(commands.size(), elements_per_iterations); ++i) {
                auto new_commands = vertex.commands;
                assert(commands[i].commands.size() == 1);
                new_commands.push_back(std::move(commands[i].commands.back()));
                next_queue.push(Vertex{.distance_dt = vertex.distance_dt + commands[i].distance_dt, .commands = std::move(new_commands)});
            }

            for (size_t i = vertex.commands.size(); i--;) {
                std::swap(best_result[vertex.commands[i].i], best_result[vertex.commands[i].j]);
            }
            while (next_queue.size() > elements_per_iterations) {
                next_queue.pop();
            }
        }
        next_queue.push(Vertex{.distance_dt = 0.0, .commands = {}});
        while (next_queue.size() > elements_per_iterations) {
            next_queue.pop();
        }
        std::swap(next_queue, queue);
    }
    if (queue.empty()) {
        return;
    }
    while (queue.size() > 1) {
        queue.pop();
    }
    if (queue.top().distance_dt >= 0) {
        // return;
    }
    for (const auto& command : queue.top().commands) {
        std::swap(best_result[command.i], best_result[command.j]);
    }
}





int main() {
    std::ifstream fin("./data/tsp_51_1");
    int n;
    fin >> n;
    // don't forget
    assert (n > 1);
    std::vector<Point> points;
    for (size_t i = 0; i < n; ++i) {
        long double x;
        long double y;
        fin >> x >> y;
        points.push_back(Point{.x = x, .y = y});
    }
    
    auto best_result = [&]() -> std::vector<Point> {
        std::list<Point> best_result = TryGreedy(points);
        long double best_distance = EuclidianDistance(best_result);
        
        for (size_t i = 0; i < 1000; ++i) {
            auto result = TryGreedy(points);
            const long double distance = EuclidianDistance(result);
            if (distance < best_distance) {
                best_distance = distance;
                best_result = std::move(result);
            }
        }
        return {best_result.begin(), best_result.end()};
    }();
    // auto best_result = points;
    std::cout << "iter0 " << EuclidianDistance(best_result) << '\n';
    
    for (size_t i = 0; i < 100; ++i) {
        BeamSearch(best_result, 1, 1);
    }
    std::cout << "iter1 " << EuclidianDistance(best_result) << '\n';
    for (size_t i = 0; i < 100; ++i) {
        BeamSearch(best_result, 5, 2);
    }
    std::cout << "iter2 " << EuclidianDistance(best_result) << '\n';
    // for (size_t i = 0; i < 1000; ++i) {
    //     TrySwapNodes(best_result);
    // }
    const size_t count = 200;
    for (size_t i = 0; i < count; ++i) {
        BeamSearch(best_result, 10, 10);
        if (i % 40 == 0) {
            std::cout << "jump befor " << i << ' ' << EuclidianDistance(best_result) << '\n';
            BeamSearch(best_result, 500, 50);
            std::cout << "jump after " << i << ' ' << EuclidianDistance(best_result) << '\n';
        }
    }
    
    // std::cout << "res " << EuclidianDistance(best_result) << '\n';
    // BeamSearch(best_result, n, 25);
    // for (size_t i = 0; i < count; ++i) {
    //     BeamSearch(best_result, 10, 10);
    //     if (i % 30 == 0) {
    //         BeamSearch(best_result, 30, 30);
    //     }
    // }
    
    
    long double best_distance = EuclidianDistance(best_result);
    
    
    std::cout << best_distance << '\n' << std::endl;
    for (const auto& point : best_result) {
        std::cout << "{x: " << point.x << ", y: " << point.y << "}, ";
    }
    

}
