#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"

class TurtleController : public rclcpp::Node
{
public:
    TurtleController() : Node("turtle_controller")
    {
        // Подписка на топик cmd_text
        subscription_ = this->create_subscription<std_msgs::msg::String>(
            "cmd_text", 10,
            std::bind(&TurtleController::command_callback, this, std::placeholders::_1));
        
        // Публикация на топик cmd_vel
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
    }

private:
    void command_callback(const std_msgs::msg::String::SharedPtr msg)
    {
        auto command = msg->data; // Получаем текст команды
        auto twist = geometry_msgs::msg::Twist(); // Создаем сообщение скорости

        if (command == "turn_right")
        {
            twist.angular.z = -1.5;  // скорость вращения (рад/с)
            twist.linear.x = 0.0;    // движение вперед
        }
        else if (command == "turn_left")
        {
            twist.angular.z = 1.5;   // скорость вращения (рад/с)
            twist.linear.x = 0.0;    // движение вперед
        }
        else if (command == "move_forward")
        {
            twist.linear.x = 1.0;    // скорость движения вперед (м/с)
            twist.angular.z = 0.0;    // без вращения
        }
        else if (command == "move_backward")
        {
            twist.linear.x = -1.0;   // скорость движения назад (м/с)
            twist.angular.z = 0.0;    // без вращения
        }
        else
        {
            RCLCPP_WARN(this->get_logger(), "Unknown command: '%s'", command.c_str());
            return;
        }

        // Публикация сообщений
        publisher_->publish(twist);
    }

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_; // Подписчик
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_; // Публикация
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv); // Инициализация ROS 2
    rclcpp::spin(std::make_shared<TurtleController>()); // Запуск узла
    rclcpp::shutdown(); // Завершение работы ROS 2
    return 0;
}