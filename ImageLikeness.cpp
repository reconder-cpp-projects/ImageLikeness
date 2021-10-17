
#include "opencv2/core.hpp"
#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <opencv2/core/utils/logger.hpp>
using namespace cv;
using namespace cv::xfeatures2d;
#include <iostream>
#include <string>

short compare_images(Mat img1, Mat img2)
{
    //Инициализируем SURF(Speeded Up Robust Features) алгоритм с Hessian threshold = 400. Согласно документации хорошим значением является от 300 до 500, так что берем то, что по середине.
    int hessian_threshold = 400;
    std::shared_ptr<SURF> detector = SURF::create(hessian_threshold);
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    //Вычисляем keypoints и descriptors для каждого изображения при помощи SURF алгоритма
    detector->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
    detector->detectAndCompute(img2, noArray(), keypoints2, descriptors2);
    /*Создаем инстанс класса, который будет сопоставлять дескрипторы по принципу nearest - neighbor, используя библиотеку FLANN(Fast Library for Approximate Nearest Neighbors).
      Так как используется SURF, используется Евклидова (L2) дистанция между точками.*/
    std::shared_ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<DMatch> > matches;
    matcher->knnMatch(descriptors1, descriptors2, matches, 2);
    /*Фильтруем совпадения при помощи теста Лове. Он сравнивает дистанции совпадений и оставляет только те, у который отношение самой маленькой дистанции ко второй
      самой маленькой высоко.*/
    const float ratio_threshold = 0.7f;
    long good_matches = 0;
    for (size_t i = 0; i < matches.size(); i++)
    {
        if (matches[i][0].distance < ratio_threshold * matches[i][1].distance)
        {
            good_matches++;
        }
    }
    //Вычисляем схожесть изображений как отношение хороших совпадений к количеству минимальному количеству углов(features).
    float likeness = (100 * good_matches / std::min(keypoints1.size(), keypoints2.size()));
    return (short)likeness;
}
struct Image
{
    String filename;
    Mat img;
    Image(String fn, Mat i) : filename(fn), img(i) {};
};


struct Comparison
{
    String filename1;
    String filename2;
    short likeness;
    Comparison(String& fn1, String& fn2, short ln) : filename1(fn1), filename2(fn2), likeness(ln){};

    Comparison(Image& img1, Image& img2) : filename1(img1.filename), filename2(img2.filename)
    {
        likeness = compare_images(img1.img, img2.img);
    }

    void print()
    {
        std::cout << filename1 << " " << filename2 << " " << likeness << "%\n";
    }
};



Image get_image(String input)
{
    
    return Image(input, imread(samples::findFile(input), IMREAD_GRAYSCALE));
}

std::vector<String> parse_input()
{
    std::cout << "Input images' filenames:" << std::endl;
    std::vector<String> inputs;
    inputs.reserve(10);
    String line;
    std::getline(std::cin, line);
    line = "00";
    while (std::getline(std::cin, line) && !line.empty())
    {
        inputs.push_back(line);
    }
    return inputs;
}





int main(int argc, char* argv[])
{
    cv::utils::logging::setLogLevel(utils::logging::LogLevel::LOG_LEVEL_SILENT);
    int threshold;
    std::cout << "Enter the threshold:" << std::endl;
    std::cin >> threshold;
    auto inputs = parse_input();

    std::vector<Image> images;
    images.reserve(inputs.size());
    String blank;
    for (auto& input : inputs)
    {
        images.push_back(get_image(inputs.back()));
        inputs.pop_back();
    }
    
    std::vector<Comparison> results;
    results.reserve(images.size() * (images.size() - 1));
    for (int i = 0; i < images.size(); i++)
        for (int j = i + 1; j < images.size(); j++)
            results.push_back(Comparison(images[i], images[j]));

    for (auto& result : results)
        if (result.likeness >= threshold) result.print();


    return 0;
}
#else
int main()
{
    std::cout << "You will need the xfeatures2d contrib module to run this app." << std::endl;
    return 0;
}
#endif