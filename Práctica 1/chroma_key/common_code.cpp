#include <iostream>
#include "common_code.hpp"
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

/**
 * @author Carlos David López Hinojosa i22lohic
*/

cv::Mat
fsiv_convert_bgr_to_hsv(const cv::Mat &img)
{
    CV_Assert(img.channels() == 3);
    cv::Mat out;
    //! TODO
    // Hint: use cvtColor.
    // Remember: the input color scheme is assumed to be BGR.
    cv::cvtColor(img,out,cv::COLOR_BGR2HSV,img.channels());
    //
    CV_Assert(out.channels() == 3);
    return out;
}

cv::Mat
fsiv_combine_images(const cv::Mat &img1, const cv::Mat &img2,
                    const cv::Mat &mask)
{
    CV_Assert(img2.size() == img1.size());
    CV_Assert(img2.type() == img1.type());
    CV_Assert(mask.size() == img1.size());
    cv::Mat output;
    //! TODO
    //  HINT: you can use cv::Mat::copyTo().
    output = img2.clone();
    img1.copyTo(output,mask);
    //
    CV_Assert(output.size() == img1.size());
    CV_Assert(output.type() == img1.type());
    return output;
}

cv::Mat
fsiv_create_mask_from_hsv_range(const cv::Mat &hsv_img,
                                const cv::Scalar &lower_bound,
                                const cv::Scalar &upper_bound)
{
    CV_Assert(hsv_img.channels() == 3);
    cv::Mat mask;

    // TODO
    // Hint: use cv::inRange to get the mask.
    cv::inRange(hsv_img, lower_bound, upper_bound, mask);
    //
    CV_Assert(mask.size() == hsv_img.size());
    CV_Assert(mask.depth() == CV_8U);
    return mask;
}

cv::Mat
fsiv_apply_chroma_key(const cv::Mat &foreg, const cv::Mat &backg, int hue,
                      int sensitivity)
{
    cv::Mat out;
    cv::Scalar lower_b, upper_b; // HSV range.

    // TODO
    // Hint: use fsiv_xxx defined functions.
    // Hint: use all ranges [0,255] for channels S and V.
    // Remember: the backg img must have the same dimensions to combine with
    //   foreg img. You can use cv::resize to assure this.

    //Establecer rango de key
    lower_b = cv::Scalar(hue - sensitivity,0,0);
    upper_b = cv::Scalar(hue + sensitivity,255,255);

    cv::Mat hsv_foreg = fsiv_convert_bgr_to_hsv(foreg);
    //Creamos Máscara
    cv::Mat mask = fsiv_create_mask_from_hsv_range(hsv_foreg,lower_b,upper_b);

    //Imagen con tamaño ajustado
    cv::Mat resized_backg;
    cv::resize(backg,resized_backg,foreg.size());

    //Combinación de las imágenes
    out = fsiv_combine_images(resized_backg,foreg,mask);
    //
    CV_Assert(out.size() == foreg.size());
    CV_Assert(out.type() == foreg.type());
    return out;
}
