
#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace __gnu_pbds;

#define sim template < class c
#define ris return * this
#define dor > debug & operator <<
#define all(x) (x).begin(),(x).end()
#define eni(x) sim > typename \
enable_if<sizeof dud<c>(0) x 1, debug&>::type operator<<(c i) {
sim > struct rge { c b, e; };
sim > rge<c> range(c i, c j) { return rge<c>{i, j}; }
sim > auto dud(c* x) -> decltype(cerr << *x, 0);
sim > char dud(...);
struct debug {
#ifdef LOCAL
~debug() { cerr << endl; }
eni(!=) cerr << boolalpha << i; ris; }
eni(==) ris << range(begin(i), end(i)); }
sim, class b dor(pair < b, c > d) {
  ris << "(" << d.first << ", " << d.second << ")";
}
sim dor(rge<c> d) {
  *this << "[";
  for (auto it = d.b; it != d.e; ++it)
    *this << ", " + 2 * (it == d.b) << *it;
  ris << "]";
}
#else
sim dor(const c&) { ris; }
#endif
};
#define imie(...) " [" << #__VA_ARGS__ ": " << (__VA_ARGS__) << "] "

const std::vector<cv::Scalar> rubiks_cube_hsv_color_from_testing = {
        cv::Scalar(175.38255555555554, 190.29855555555554, 173.24533333333332), // Red
        cv::Scalar(78.68033333333334, 254.65977777777778, 196.2613333333333),   // Green
        cv::Scalar(103.82122222222223, 254.6348888888889, 237.01533333333333),  // Blue
        cv::Scalar(45.80347777777777, 159.54211111111113, 228.0294444444445),    // Yellow
        cv::Scalar(4.253168888888889, 171.7713333333333, 250.2893333333333),    // Orange
        cv::Scalar(30.969644444444448, 4.580559666666667, 222.66044444444444),  // White
};

const std::vector<std::pair<cv::Scalar, cv::Scalar>> rubiks_cube_hsv_color_ranges = {
        {cv::Scalar(173, 100, 100), cv::Scalar(177, 255, 255)},   // Red
        {cv::Scalar(75, 100, 100), cv::Scalar(80, 255, 255)},  // Green
        {cv::Scalar(102, 100, 100), cv::Scalar(135, 255, 255)},// Blue
        {cv::Scalar(43, 100, 100), cv::Scalar(50, 255, 255)},  // Yellow
        {cv::Scalar(0, 100, 100), cv::Scalar(6, 255, 255)},   // Orange

        {cv::Scalar(0, 0, 255 - 15), cv::Scalar(255,  255 - 15, 255)}      // White
};

const std::vector<std::string>color_names = {"Red", "Green", "Blue", "Yellow", "Orange", "White"};

double dotProduct(const cv::Scalar& scalar1, const cv::Scalar& scalar2) {
    return  (double)scalar1[0] * (double)scalar2[0] +
            (double)scalar1[1] * (double)scalar2[1] +
            (double)scalar1[2] * (double)scalar2[2];
}

std::string closestRubiksColor(const cv::Scalar& query_color) {
    double min_distance = std::numeric_limits<double>::max();
    std::string closest_color;

    for (size_t i = 0; i < rubiks_cube_hsv_color_ranges.size(); ++i) {
        double distance = 0.0;
        for (size_t j = 0; j < 3; ++j) {
            double min_val = rubiks_cube_hsv_color_ranges[i].first[j];
            double max_val = rubiks_cube_hsv_color_ranges[i].second[j];
            double query_val = query_color[j];

            if (query_val < min_val)
                distance += std::pow(min_val - query_val, 2.0 * (3.00 - (double)j));
            else if (query_val > max_val)
                distance += std::pow(query_val - max_val, 2.0 * (3.00 - (double)j));
        }

        distance = std::sqrt(distance);

        if (distance < min_distance) {
            min_distance = distance;
            closest_color = color_names[i];
        }
    }

    return closest_color;
}

std::string closestRubiksColorv2(const cv::Scalar& query_color) {
    double min_distance = std::numeric_limits<double>::max();
    std::string closest_color;

    for (size_t i = 0; i < rubiks_cube_hsv_color_from_testing.size(); ++i) {
        double distance = 0.0;
        for (size_t j = 0; j < 3; ++j) {
            double test_value = rubiks_cube_hsv_color_from_testing[i][j];
            double query_val = query_color[j];
            distance += pow(test_value - query_val, 2.00 * (3.00 - (double)j));
        }

        if (distance < min_distance) {
            min_distance = distance;
            closest_color = color_names[i];
        }
    }

    return closest_color;
}


const int num_rows = 3;
const int num_cols = 3;



cv::Scalar calculateAverageColorFromHSVImage(const cv::Mat& image, const cv::Rect& rect) {
    cv::Rect intersection = rect & cv::Rect(0, 0, image.cols, image.rows);

    if (intersection.area() == 0) {
        return cv::Scalar(0, 0, 0);
    }

    cv::Mat roi = image(intersection);
    cv::Scalar sum = cv::sum(roi);
    int num_pixels = intersection.area();
    cv::Scalar average_color = sum / num_pixels;
    return average_color;
}


std::vector<cv::Scalar> extractAverageColors(const cv::Mat& image, const std::string& output_path) {
    std::vector<cv::Scalar> average_colors;
    cv::Mat hsv_image;
    cv::cvtColor(image, hsv_image, cv::COLOR_BGR2HSV);

    const double margin_ratio = 0.30;
    const int region_width = image.cols / num_cols;
    const int region_size = region_width * (1.0 - 2.0 * margin_ratio);

    cv::Mat region_image(image.size(), CV_8UC3, cv::Scalar(255, 255, 255));

    for (int i = 0; i < num_rows; ++i) {
        for (int j = 0; j < num_cols; ++j) {
            int x = j * region_width + region_width / 2 - region_size / 2;
            int y = i * region_width + region_width / 2 - region_size / 2;

            cv::Rect cell_region(x, y, region_size, region_size);
            cv::Scalar avg_color = calculateAverageColorFromHSVImage(hsv_image, cell_region);

            average_colors.push_back(avg_color);

            cv::rectangle(region_image, cell_region, avg_color, cv::FILLED);
            cv::rectangle(region_image, cell_region, cv::Scalar(0, 0, 0), 2);

            //cout << "row: " << i << " -- col: " << j << endl;
//            cout << "(" << closestRubiksColor(avg_color) << "," << closestRubiksColorv2(avg_color) << ")";
            cout << closestRubiksColor(avg_color) << " ";
        }
        cout << endl;
    }

    cv::Mat overlay_image;
    cv::addWeighted(image, 1, region_image, 0.5, 0, overlay_image);

    cv::imwrite(output_path, overlay_image);

    return average_colors;
}


signed main(int argc, char **argv, char **envp) {
    if (2 != argc) {
        perror("Wrong number of argumets (Expecting a path to the image with Rubik`s cube face)");
        return 0x1;
    }

    std::string image_path = argv[1];
    cv::Mat original_image = cv::imread(image_path);

    cv::Mat resized_image;
    cv::resize(original_image, resized_image, cv::Size(720, 720));

    vector<cv::Scalar> average_colors = extractAverageColors(resized_image, "avg_colors.bmp");
    debug() << imie(average_colors);
    return 0x0;
}

