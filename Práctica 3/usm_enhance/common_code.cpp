/**
 * @file common_code.cpp
 * @author Francisco José Madrid Cuevas (fjmadrid@uco.es)
 * @brief Utility module to do an Unsharp Mask image enhance.
 * @version 0.1
 * @date 2024-09-19
 *
 * @copyright Copyright (c) 2024-
 *
 */
#include "common_code.hpp"
#include <opencv2/imgproc.hpp>

cv::Mat
fsiv_create_box_filter(const int r)
{
    CV_Assert(r > 0);
    cv::Mat ret_v;
    // TODO
    // Hint: use the constructor of cv::Mat to set the proper initial value.
    int n = 2 * r + 1;
    ret_v = cv::Mat::ones(n, n, CV_32FC1) / (n * n);
    //
    CV_Assert(ret_v.type() == CV_32FC1);
    CV_Assert(ret_v.rows == (2 * r + 1) && ret_v.rows == ret_v.cols);
    CV_Assert(std::abs(1.0 - cv::sum(ret_v)[0]) < 1.0e-6);
    return ret_v;
}

cv::Mat
fsiv_create_gaussian_filter(const int r)
{
    CV_Assert(r > 0);
    cv::Mat ret_v;
    // TODO
    // Hint: use cv::getGaussianKernel()
    int n = 2 * r + 1;
    ret_v = cv::getGaussianKernel(n,-1, CV_32F);
    ret_v = ret_v * ret_v.t();
    //
    CV_Assert(ret_v.type() == CV_32FC1);
    CV_Assert(ret_v.rows == (2 * r + 1) && ret_v.rows == ret_v.cols);
    CV_Assert(std::abs(1.0 - cv::sum(ret_v)[0]) < 1.0e-6);
    return ret_v;
}

cv::Mat
fsiv_fill_expansion(cv::Mat const &in, const int r)
{
    CV_Assert(!in.empty());
    CV_Assert(r > 0);
    cv::Mat ret_v;
    //! TODO:
    // Hint: use cv::copyMakeBorder() using the constant value 0 to fill the
    //       expanded area.
    cv::copyMakeBorder(in, ret_v, r, r, r, r, cv::BORDER_CONSTANT);
    //
    CV_Assert(ret_v.type() == in.type());
    CV_Assert(ret_v.rows == in.rows + 2 * r);
    CV_Assert(ret_v.cols == in.cols + 2 * r);
    return ret_v;
}

cv::Mat
fsiv_circular_expansion(cv::Mat const &in, const int r)
{
    CV_Assert(!in.empty());
    CV_Assert(r > 0);
    cv::Mat ret_v;
    //! TODO
    //  Hint: use cv::copyMakeBorder() filling with a wrapper image.
    cv::copyMakeBorder(in, ret_v, r, r, r, r, cv::BORDER_WRAP);
    //
    CV_Assert(ret_v.type() == in.type());
    CV_Assert(ret_v.rows == in.rows + 2 * r);
    CV_Assert(ret_v.cols == in.cols + 2 * r);
    CV_Assert(!(in.type() == CV_8UC1) || ret_v.at<uchar>(0, 0) == in.at<uchar>(in.rows - r, in.cols - r));
    CV_Assert(!(in.type() == CV_8UC1) || ret_v.at<uchar>(0, ret_v.cols / 2) == in.at<uchar>(in.rows - r, in.cols / 2));
    CV_Assert(!(in.type() == CV_8UC1) || ret_v.at<uchar>(0, ret_v.cols - 1) == in.at<uchar>(in.rows - r, r - 1));
    CV_Assert(!(in.type() == CV_8UC1) || ret_v.at<uchar>(ret_v.rows / 2, 0) == in.at<uchar>(in.rows / 2, in.cols - r));
    CV_Assert(!(in.type() == CV_8UC1) || ret_v.at<uchar>(ret_v.rows / 2, ret_v.cols / 2) == in.at<uchar>(in.rows / 2, in.cols / 2));
    CV_Assert(!(in.type() == CV_8UC1) || ret_v.at<uchar>(ret_v.rows - 1, 0) == in.at<uchar>(r - 1, in.cols - r));
    CV_Assert(!(in.type() == CV_8UC1) || ret_v.at<uchar>(ret_v.rows - 1, ret_v.cols / 2) == in.at<uchar>(r - 1, in.cols / 2));
    CV_Assert(!(in.type() == CV_8UC1) || ret_v.at<uchar>(ret_v.rows - 1, ret_v.cols - 1) == in.at<uchar>(r - 1, r - 1));
    return ret_v;
}

cv::Mat
fsiv_filter2D(cv::Mat const &in, cv::Mat const &filter)
{
    CV_Assert(!in.empty() && !filter.empty());
    CV_Assert(in.type() == CV_32FC1 && filter.type() == CV_32FC1);
    cv::Mat ret_v;

    // TODO
    // Remember: Using cv::filter2D/cv::sepFilter2D is not allowed here because
    //           we want you to code the convolution operation for ease of
    //           understanding. In real applications, you should use one of
    //           those functions.

    ret_v = cv::Mat::zeros(in.rows - 2 * (filter.rows / 2), in.cols - 2 * (filter.cols / 2), CV_32FC1);
    int imgRows = in.rows;
    int imgCols = in.cols;

    int filterRows = filter.rows;
    int filterCols = filter.cols;

    for (int i = filterRows / 2; i < in.rows - filterRows/2; i++)
    {
        for (int j = filterCols/2; j < in.cols - filterCols/2; j++)
        {
            cv::Rect ventana(j - filterCols/2, i - filterRows/2, filterCols, filterRows);
            cv::Mat submat = in(ventana);
            cv::Mat submat2 = filter.mul(submat);
            ret_v.at<float>(i - filterRows/2, j - filterCols/2) = cv::sum(submat2)[0];
        }
    }
    //
    CV_Assert(ret_v.type() == CV_32FC1);
    CV_Assert(ret_v.rows == in.rows - 2 * (filter.rows / 2));
    CV_Assert(ret_v.cols == in.cols - 2 * (filter.cols / 2));
    return ret_v;
}

cv::Mat
fsiv_combine_images(const cv::Mat src1, const cv::Mat src2,
                    double a, double b)
{
    CV_Assert(src1.type() == src2.type());
    CV_Assert(src1.rows == src2.rows);
    CV_Assert(src1.cols == src2.cols);
    cv::Mat ret_v;

    // TODO
    // Hint: use cv::addWeighted()
    cv::addWeighted(src1, a, src2, b, 0, ret_v);
    //
    CV_Assert(ret_v.type() == src2.type());
    CV_Assert(ret_v.rows == src2.rows);
    CV_Assert(ret_v.cols == src2.cols);
    return ret_v;
}

cv::Mat
fsiv_usm_enhance(cv::Mat const &in, double g, int r,
                 int filter_type, bool circular, cv::Mat *unsharp_mask)
{
    CV_Assert(!in.empty());
    CV_Assert(in.type() == CV_32FC1);
    CV_Assert(r > 0);
    CV_Assert(filter_type >= 0 && filter_type <= 1);
    CV_Assert(g >= 0.0);
    cv::Mat ret_v;
    // TODO
    // Remember: use your own functions fsiv_xxxx
    // Remember: when unsharp_mask pointer is nullptr, means don't save the
    //           unsharp mask on int.
    
    cv::Mat filter = (filter_type == 0) ? fsiv_create_box_filter(r) : fsiv_create_gaussian_filter(r);

    cv::Mat expanded_in = (circular) ? fsiv_circular_expansion(in, r) : fsiv_fill_expansion(in, r);

    ret_v = fsiv_filter2D(expanded_in, filter);

    cv::Mat mask = in - ret_v;

    if (unsharp_mask)
        *unsharp_mask = mask.clone();

    ret_v = fsiv_combine_images(in, mask, 1.0, g);
    //
    CV_Assert(ret_v.rows == in.rows);
    CV_Assert(ret_v.cols == in.cols);
    CV_Assert(ret_v.type() == CV_32FC1);
    return ret_v;
}
