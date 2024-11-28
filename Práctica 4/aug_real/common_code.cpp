#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include "common_code.hpp"

std::vector<cv::Point3f>
fsiv_generate_3d_calibration_points(const cv::Size &board_size,
                                    float square_size)
{
    std::vector<cv::Point3f> ret_v;
    // TODO
    // Remember: the first inner point has (1,1) in board coordinates.
    for (int i = 1; i <= board_size.height; i++)
        for (int j = 1; j <= board_size.width; j++)
            ret_v.push_back(cv::Point3f(j * square_size, i * square_size, 0));

    //
    CV_Assert(ret_v.size() ==
              static_cast<size_t>(board_size.width * board_size.height));
    return ret_v;
}

bool fsiv_fast_find_chessboard_corners(const cv::Mat &img, const cv::Size &board_size,
                                       std::vector<cv::Point2f> &corner_points)
{
    CV_Assert(img.type() == CV_8UC3);
    bool was_found = false;
    // TODO
    // Hint: use cv::findChessboardCorners adding fast check to the defaults flags.
    // Remember: do not refine the corner points to get a better computational performance.
    was_found = cv::findChessboardCorners(img, board_size, corner_points, cv::CALIB_CB_FAST_CHECK);
    //  
    return was_found;
}

void fsiv_compute_camera_pose(const std::vector<cv::Point3f> &_3dpoints,
                              const std::vector<cv::Point2f> &_2dpoints,
                              const cv::Mat &camera_matrix,
                              const cv::Mat &dist_coeffs,
                              cv::Mat &rvec,
                              cv::Mat &tvec)
{
    CV_Assert(_3dpoints.size() >= 4 && _3dpoints.size() == _2dpoints.size());
    // TODO
    // Hint: use cv::solvePnP to the pose of a calibrated camera.
    cv::solvePnP(_3dpoints, _2dpoints, camera_matrix, dist_coeffs, rvec, tvec);
    //
    CV_Assert(rvec.rows == 3 && rvec.cols == 1 && rvec.type() == CV_64FC1);
    CV_Assert(tvec.rows == 3 && tvec.cols == 1 && tvec.type() == CV_64FC1);
}

void fsiv_draw_axes(cv::Mat &img,
                    const cv::Mat &camera_matrix, const cv::Mat &dist_coeffs,
                    const cv::Mat &rvec, const cv::Mat &tvec,
                    const float size, const int line_width)
{
    // TODO
    // Hint: use cv::projectPoints to get the image coordinates of the 3D points
    // (0,0,0), (size, 0, 0), (0, size, 0) and (0, 0, -size) and draw a line for
    // each axis: blue for axis OX, green for axis OY and red for axis OZ.
    // Warning: use of cv::drawFrameAxes() is not allowed.
    std::vector<cv::Point3f> object_points = {cv::Point3f(0, 0, 0), 
                                              cv::Point3f(size, 0, 0),
                                              cv::Point3f(0, size, 0), 
                                              cv::Point3f(0, 0, -size)};
    
    std::vector<cv::Point2f> image_points;
    cv::projectPoints(object_points, rvec, tvec, camera_matrix, dist_coeffs, image_points);

    cv::line(img, image_points[0], image_points[1], cv::Scalar(255, 0, 0), line_width);
    cv::line(img, image_points[0], image_points[2], cv::Scalar(0, 255, 0), line_width);
    cv::line(img, image_points[0], image_points[3], cv::Scalar(0, 0, 255), line_width);
    //
}

void fsiv_load_calibration_parameters(cv::FileStorage &fs,
                                      cv::Size &camera_size,
                                      float &error,
                                      cv::Mat &camera_matrix,
                                      cv::Mat &dist_coeffs,
                                      cv::Mat &rvec,
                                      cv::Mat &tvec)
{
    CV_Assert(fs.isOpened());
    // TODO
    //  Hint: use fs["label"] >> var to load data items from the file.
    //  @see cv::FileStorage operators "[]" and ">>"
    fs["image-width"] >> camera_size.width;
    fs["image-height"] >> camera_size.height;
    fs["error"] >> error;
    fs["camera-matrix"] >> camera_matrix;
    fs["distortion-coefficients"] >> dist_coeffs;
    fs["rvec"] >> rvec;
    fs["tvec"] >> tvec;
    //
    CV_Assert(fs.isOpened());
    CV_Assert(camera_matrix.type() == CV_64FC1 && camera_matrix.rows == 3 && camera_matrix.cols == 3);
    CV_Assert(dist_coeffs.type() == CV_64FC1 && dist_coeffs.rows == 1 && dist_coeffs.cols == 5);
    CV_Assert(rvec.type() == CV_64FC1 && rvec.rows == 3 && rvec.cols == 1);
    CV_Assert(tvec.type() == CV_64FC1 && tvec.rows == 3 && tvec.cols == 1);
    return;
}

void fsiv_draw_3d_model(cv::Mat &img, const cv::Mat &M, const cv::Mat &dist_coeffs,
                        const cv::Mat &rvec, const cv::Mat &tvec,
                        const float size)
{
    CV_Assert(img.type() == CV_8UC3);


    std::vector<cv::Point3f> object_points = 
    {
        cv::Point3f(0, 0, 0),
        cv::Point3f(size, 0, 0),
        cv::Point3f(size, size, 0),
        cv::Point3f(0, size, 0),
        cv::Point3f(0, 0, -size),
        cv::Point3f(size, 0, -size),
        cv::Point3f(size, size, -size),
        cv::Point3f(0, size, -size)
    };


    spin_and_move(object_points, size);
    

    std::vector<cv::Point2f> image_points;
    cv::projectPoints(object_points, rvec, tvec, M, dist_coeffs, image_points);

    // Definir las líneas del modelo del cubo
    std::vector<std::vector<cv::Point>> lines = {
        // Cara frontal
        {image_points[0], image_points[1], image_points[2], image_points[3]},
        // Cara trasera
        {image_points[4], image_points[5], image_points[6], image_points[7]},
        // Aristas conectivas
        {image_points[0], image_points[4]},
        {image_points[1], image_points[5]},
        {image_points[2], image_points[6]},
        {image_points[3], image_points[7]}
    };

    // Dibujar el wireframe del modelo del cubo

    static cv::Scalar color(0, 0, 0);

    change_color(color);

    cv::polylines(img, lines, true, color, 2);
}

void fsiv_project_image(const cv::Mat &model, cv::Mat &scene,
                        const cv::Size &board_size,
                        const std::vector<cv::Point2f> &chess_board_corners)
{
    CV_Assert(!model.empty() && model.type() == CV_8UC3);
    CV_Assert(!scene.empty() && scene.type() == CV_8UC3);
    CV_Assert(static_cast<size_t>(board_size.area()) ==
              chess_board_corners.size());

    // TODO
    // Hint: get the upper-left, upper-right, bottom-right and bottom-left
    //   chess_board_corners and map to the upper-left, upper-right, bottom-right
    //   and bottom-left model image point coordinates.
    //   Use cv::getPerspectiveTransform compute such mapping.
    // Hint: use cv::wrapPerspective to get a wrap version of the model image
    //   using the computed mapping. Use INTER_LINEAR as interpolation method
    //   and use BORDER_TRANSPARENT as a border extrapolation method
    //   to maintain the underlying image.
    //
    std::vector<cv::Point2f> scene_corners(4);
    scene_corners[0] = chess_board_corners[0]; // Top-left
    scene_corners[1] = chess_board_corners[board_size.width - 1]; // Top-right
    scene_corners[2] = chess_board_corners[chess_board_corners.size() - 1]; // Bottom-right
    scene_corners[3] = chess_board_corners[chess_board_corners.size() - board_size.width]; // Bottom-left

    std::vector<cv::Point2f> model_corners(4);
    model_corners[0] = cv::Point2f(0, 0); // Top-left
    model_corners[1] = cv::Point2f(model.cols - 1, 0); // Top-right
    model_corners[2] = cv::Point2f(model.cols - 1, model.rows - 1); // Bottom-right
    model_corners[3] = cv::Point2f(0, model.rows - 1); // Bottom-left

    cv::Mat H = cv::getPerspectiveTransform(model_corners, scene_corners);

    cv::warpPerspective(model, scene, H, scene.size(), cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
    //
}


void spin_and_move(std::vector<cv::Point3f> &object_points, const float &size)
{

    static const int board_width = 6;
    static const int board_height = 5;

    // Posición y dirección del cubo (inicializada solo la primera vez que se llama la función)
    static cv::Point3f cube_pos(0.0f, 0.0f, 0.0f);
    static cv::Point3f cube_direction(0.001f, 0.002f, 0.0f); // Velocidad en x, y

    // Actualizar la posición del cubo
    cube_pos += cube_direction;

    // Detectar colisión con los bordes del tablero y cambiar de dirección
    if (cube_pos.x < 0 || cube_pos.x > (board_width - 1) * size)
        cube_direction.x = -cube_direction.x;
    if (cube_pos.y < 0 || cube_pos.y > (board_height - 1) * size)
        cube_direction.y = -cube_direction.y;

    // Puntos 3D del cubo en torno a su posición actual


    static float angle_x = 0.0f;
    static float angle_y = 0.0f;
    static float angle_z = 0.0f;
    angle_x += 0.02f; // Incrementar el ángulo de rotación en X
    angle_y += 0.02f; // Incrementar el ángulo de rotación en Y
    angle_z += 0.02f; // Incrementar el ángulo de rotación en Z

    cv::Vec3f rotation_vector(angle_x, angle_y, angle_z);

    // Convert the rotation vector to a rotation matrix using Rodrigues transformation
    cv::Mat rotation_matrix;
    cv::Rodrigues(rotation_vector, rotation_matrix);

    for (auto &point : object_points)
    {
        cv::Mat point_mat = (cv::Mat_<float>(3, 1) << point.x, point.y, point.z);
        cv::Mat result_mat = rotation_matrix * point_mat;
        point = cv::Point3f(result_mat.at<float>(0, 0), result_mat.at<float>(1, 0), result_mat.at<float>(2, 0)) + cube_pos;
    }
}

void change_color(cv::Scalar &color)
{
    static int hue = 0; // Inicializamos el tono a 0 (rojo)
    
    // Incrementa el tono para recorrer el arcoíris
    hue = (hue + 1) % 180; // OpenCV usa un rango de 0-179 para el tono
    
    // Convertimos HSV a BGR
    cv::Mat hsv(1, 1, CV_8UC3, cv::Scalar(hue, 255, 170)); // Saturación y Valor máximos
    cv::Mat bgr;
    cv::cvtColor(hsv, bgr, cv::COLOR_HSV2BGR);

    // Actualizamos el color en formato BGR
    cv::Vec3b bgr_pixel = bgr.at<cv::Vec3b>(0, 0);
    color = cv::Scalar(bgr_pixel[0], bgr_pixel[1], bgr_pixel[2]);
}
