//
// Created by Danny Le on 2022-10-18.
//

#include "rtweekend.hpp"
#include <glm/glm.hpp>
#include "glmutil.hpp"
#include <iomanip>

inline double pdf(const glm::dvec3& p) {
    return 1 / (4*pi);
}

int main() {
    int N = 1000000;
    auto sum = 0.0;
    for (int i = 0; i < N; i++) {
        glm::dvec3 d = random_unit_vector();
        auto cosine_squared = d.z*d.z;
        sum += cosine_squared / pdf(d);
    }
    std::cout << std::fixed << std::setprecision(12);
    std::cout << "I = " << sum/N << std::endl;
}