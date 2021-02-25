#include<ros/ros.h>
#include<opencv2/opencv.hpp>
#include<sensor_msgs/Image.h>
#include<std_msgs/UInt8MultiArray.h>
#include<std_msgs/Float32MultiArray.h>


using namespace std;


int main(int argc, char **argv)
{
    cout<<"Starting camera_node by Luis Näva..."<<endl;
    ros::init(argc, argv, "camera_node");
    ros::NodeHandle nh;
    
    int camera = 0;
    if(argc == 2)
        camera = atoi(argv[1]);

    ros::Publisher pubImage = nh.advertise<sensor_msgs::Image>("/hardware/camera/image", 1);
    ros::Publisher pubJpeg  = nh.advertise<std_msgs::UInt8MultiArray>("/hardware/camera/img_compressed", 1);
    ros::Rate loop(30);

    cv::Mat       video_frame;
    cv::VideoCapture capture;
    capture.open(camera);

    sensor_msgs::Image msgImage;
    std_msgs::UInt8MultiArray msgCompressed;

    int img_width  = 640;
    int img_height = 480;
    msgImage.header.frame_id = "camera_link";
    msgImage.data.resize(img_width*img_height*3);
    msgImage.height = img_height;
    msgImage.width  = img_width;
    msgImage.encoding = "bgr8";
    msgImage.step = img_width*3;
    std::vector<int> compressionParams(2);
    std::vector<uchar> compressedBuff;
    compressionParams[0] = cv::IMWRITE_JPEG_QUALITY;
    compressionParams[1] = 95;

    while( ros::ok() && cv::waitKey(15)!=27 )
    {
        capture.read(video_frame);

        capture.retrieve(video_frame);
        msgImage.header.stamp = ros::Time::now();
        memcpy(msgImage.data.data(), video_frame.data, img_width*img_height*3);
        cv::imencode(".jpg", video_frame, msgCompressed.data, compressionParams);

		pubImage.publish(msgImage);
        pubJpeg.publish(msgCompressed);

        ros::spinOnce();
        loop.sleep();        
    }



    return 0;
}  
