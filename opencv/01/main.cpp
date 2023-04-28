#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::VideoCapture cap (0); // abre a câmera
    if (!cap.isOpened()) // verifica se a câmera foi aberta com sucesso
    {
        std::cout << "Erro ao abrir a câmera." << std::endl;
        return -1;
    }

    cv::Mat frame;
    int frames_per_second = 30; // quantidade de frames por segundo
    int video_duration = 600; // duração do vídeo em segundos
    cv::VideoWriter video ("output.avi", cv::VideoWriter::fourcc ('M', 'J', 'P', 'G'), frames_per_second, cv::Size (cap.get (cv::CAP_PROP_FRAME_WIDTH), cap.get (cv::CAP_PROP_FRAME_HEIGHT))); // cria um objeto VideoWriter para gravar o vídeo
    double start_time = cv::getTickCount(); // marca o tempo inicial

    while ((cv::getTickCount() - start_time) / cv::getTickFrequency() < video_duration) // executa o loop até atingir a duração desejada
    {
        cap >> frame; // captura um frame da câmera
        if (frame.empty()) // verifica se o frame foi capturado com sucesso
            break;
        video.write (frame); // grava o frame no vídeo
        cv::imshow ("Webcam", frame); // exibe o frame na janela "Webcam"
        if (cv::waitKey (1) == 27) // espera 1 milissegundo e verifica se a tecla "Esc" foi pressionada
            break;
    }

    cap.release(); // libera a câmera
    video.release(); // libera o vídeo
    cv::destroyAllWindows(); // fecha todas as janelas abertas pelo OpenCV
    return 0;
}
